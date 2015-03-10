/**
 * @file MsmsMerge.cpp
 * @brief implements of MsmsMerge class
 *
 * @author S.Tanaka
 * @date 2013.01.16
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MsmsMerge.h"


using namespace kome::operation::msmsmerge;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MsmsMerge::MsmsMerge() {
}

// destructor
MsmsMerge::~MsmsMerge() {
}

// open parameters dialog
bool MsmsMerge::openParametersDialog( double* rtTol, double* mzTol ) {
	// plug-in manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// page
	kome::plugin::PluginInfo* plugin = plgMgr.getPlugin( "MsmsSpectraMergePlugin" );
	if( plugin == NULL ) {
		return false;
	}
	kome::plugin::SettingsPage* page = plugin->getPage( "merge" );
	if( page == NULL ) {
		return false;
	}

	// settings
	kome::objects::SettingParameterValues settings;
		
	// dialog
	kome::window::SettingsPageDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		page,
		&settings,
		true,
		NULL,
		true,
		"Merge"
	);

	if( dlg.ShowModal() == wxID_OK ) {
		double tmpRtTol = settings.getDoubleValue( "rt tol", -1.0 );
		double tmpMzTol = settings.getDoubleValue( "mz tol", -1.0 );

		if( rtTol != NULL && tmpRtTol >= 0.0 ) {
			*rtTol = tmpRtTol;
		}
		if( mzTol != NULL && tmpMzTol >= 0.0 ) {
			*mzTol = tmpMzTol;
		}

		return true;
	}
	return false;
}

// merge MS/MS spectra
void MsmsMerge::mergeSpectra( kome::objects::Sample* sample, const double rtTol, const double mzTol, kome::core::Progress& progress ) {
	// check parameters
	if( sample == NULL ) {
		return;
	}

	// groups
	kome::objects::DataGroupNode* root = sample->getRootDataGroupNode();
	if( root == NULL ) {
		return;
	}

	kome::objects::DataGroupNode* msGroup = root->createChildGroup( "Original MS" );
	kome::objects::DataGroupNode* parentGroup = root->createChildGroup( "Merged Parents" );
	kome::objects::DataGroupNode* msmsGroup = root->createChildGroup( "Merged MS/MS" );

	// progress
	progress.setStatus( "Getting Spectra..." );

	// array
	kome::objects::DataSet dataSet;
	root->getDataSet( &dataSet );

	std::vector< kome::objects::Spectrum* > spectra;
	int msCnt = 0;
	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
		spectra.push_back( spec );
	}
	std::sort( spectra.begin(), spectra.end(), lessSpectrum );

	// progress
	if( spectra.size() == 0 ) {
		progress.fill();
		return;
	}
	progress.setRange( 0, spectra.size() + 1 );
	progress.setPosition( 0 );

	// group set
	std::set< kome::objects::DataGroupNode* > groupSet;

	// parent map
	std::map< kome::objects::AveragedSpectrum*, kome::objects::AveragedSpectrum* > parentMap;

	// each spectra
	for( unsigned int i = 0; i < spectra.size() && !progress.isStopped(); i++ ) {
		// progress
		progress.setStatus( FMT( "Merging Spectra... [%d/%d]", ( i + 1 ), ( spectra.size() ) ).c_str() );

		// spectrum
		kome::objects::Spectrum* spec = spectra[ i ];

		// group
		kome::objects::DataGroupNode* group = spec->getGroup();
		group->removeSpectrum( spec );

		// add spectrum to group
		if( spec->getMsStage() == 1 ) {    // MS
			if( spec->hasChromatogram() ) {
				msCnt++;
				msGroup->addSpectrum( spec );
			}
			else {
				sample->getHiddenDataSet()->addSpectrum( spec );
			}			
		}
		else {    // MS/MS
			sample->getHiddenDataSet()->addSpectrum( spec );

			// search
			kome::objects::AveragedSpectrum* avgSpec = NULL;
			if( spec->getRt() >= 0.0 ) {
				for( unsigned int j = 0; j < msmsGroup->getNumberOfSpectra() && avgSpec == NULL; j++ ) {
					kome::objects::AveragedSpectrum* tmp = (kome::objects::AveragedSpectrum*)msmsGroup->getSpectrum( j );
					const double rtD = fabs( tmp->getEndRt() - spec->getRt() );
					const double mzD = fabs( tmp->getPrecursor() - spec->getPrecursor() );
					if( rtD <= rtTol && mzD <= mzTol && tmp->getMsStage() == spec->getMsStage() 
							&& ( tmp->getPrecursorCharge() <= 0 || spec->getPrecursorCharge() <= 0 || tmp->getPrecursorCharge() == spec->getPrecursorCharge() ) ) {
						avgSpec = tmp;
						if( spec->getPrecursorCharge() > 0 && tmp->getPrecursorCharge() <= 0 ) {
							avgSpec->setPrecursorCharge( spec->getPrecursorCharge() );
						}
					}
				}
			}

			// add
			if( avgSpec == NULL ) {
				avgSpec = new kome::objects::AveragedSpectrum( msmsGroup );
				msmsGroup->addSpectrum( avgSpec );
				avgSpec->addSpectrum( spec );
				avgSpec->setRt( spec->getRt(), spec->getRt() );
				avgSpec->setPrecursor( spec->getPrecursor() );
				avgSpec->setMsStage( spec->getMsStage() );
				avgSpec->setPrecursorCharge( spec->getPrecursorCharge() );
			}
			else {
				const double precursor = ( (double)avgSpec->getNumberOfSpectra() * avgSpec->getPrecursor() + spec->getPrecursor() ) / (double)( avgSpec->getNumberOfSpectra() + 1 );
				avgSpec->addSpectrum( spec );
				avgSpec->setEndRt( spec->getRt() );
				avgSpec->setPrecursor( precursor );
				avgSpec->setMsStage( spec->getMsStage() );
			}

			// parent
			kome::objects::Spectrum* parent = spec->getParentSpectrum();
			if( parent != NULL ) {
				if( parentMap.find( avgSpec ) == parentMap.end() ) {
					kome::objects::AveragedSpectrum* avgParent = new kome::objects::AveragedSpectrum( parentGroup );
					avgSpec->setParentSpectrum( avgParent );
					parentGroup->addSpectrum( avgParent );
					avgParent->addSpectrum( parent );
					avgParent->setRt( parent->getRt(), parent->getRt() );
					avgParent->setMsStage( spec->getMsStage() - 1 );

					parentMap[ avgSpec ] = avgParent;
				}
				else {
					kome::objects::AveragedSpectrum* avgParent = parentMap[ avgSpec ];
					avgParent->addSpectrum( parent );
					double startRt = std::min( parent->getRt(), avgSpec->getStartRt() );
					double endRt = std::max( parent->getRt(), avgSpec->getEndRt() );
					avgParent->setRt( startRt, endRt );
				}
			}
		}

		// store groups to the set
		while( group != root && group != NULL ) {
			groupSet.insert( group );
			group = group->getParentGroup();
		}

		// progress
		progress.setPosition( i + 1 );
	}

	if( progress.isStopped() ){ // @date 2013.11.22 <Add> M.Izumi
		return;
	}

	// chromatogram
	for( unsigned int i = 0; i < sample->getNumberOfGroups() && !progress.isStopped(); i++ ) {
		kome::objects::DataGroupNode* group = sample->getGroup( i );
		for( unsigned int j = 0; j < group->getNumberOfChromatograms(); j++ ) {
			kome::objects::Chromatogram* chrom = group->getChromatogram( j );
			group->removeChromatogram( chrom );
			sample->getHiddenDataSet()->addChromatogram( chrom );
		}
	}

	// delete group
	for( std::set< kome::objects::DataGroupNode* >::iterator it = groupSet.begin(); it != groupSet.end(); it++ ) {
		kome::objects::DataGroupNode* group = *it;
		if( groupSet.find( group->getParentGroup() ) == groupSet.end() ) {
			group->getParentGroup()->removeChildGroup( group );

// >>>>>>	@Date:2013/12/19	<Add:SPEC93464>	A.Ozaki
//
//  ここでgroupを解放してしまうと、その後の処理で落ちてしまいます。
//  そのため、この時点での領域解放は実施しません。
//			delete group;
//
// <<<<<<	@Date:2013/12/19	<Add:SPEC93464>	A.Ozaki
		}
	}

	// progress
	progress.setStatus( "Reconstructing..." );

	// name
	for( unsigned int i = 0; i < msmsGroup->getNumberOfSpectra() && !progress.isStopped(); i++ ) {
		kome::objects::AveragedSpectrum* spec = (kome::objects::AveragedSpectrum*)msmsGroup->getSpectrum( i );
		std::string name = FMT( "MS/MS Scan %d", i );
		spec->setName( name.c_str() );

		if( parentMap.find( spec ) != parentMap.end() ) {
			kome::objects::AveragedSpectrum* parent = parentMap[ spec ];

			name = FMT( "Averaged Spectrum %d (Precursor=%.2f)", i, spec->getPrecursor() );
			parent->setName( name.c_str() );
		}
	}

	// progress
	progress.fill();
}

// compare
bool MsmsMerge::lessSpectrum( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1 ) {
	return ( spec0->getRt() < spec1->getRt() );
}

// get instance
MsmsMerge& MsmsMerge::getInstance() {
	// object
	static MsmsMerge m;
	return m;
}
