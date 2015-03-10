/**
 * @file MergedSpectrumManager.cpp
 * @brief implements of MergedSpectrumManager class
 *
 * @author S.Tanaka
 * @date 2007.03.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MergedSpectrumManager.h"
#include "MergedSpectrum.h"
#include "AveragedSpectrumDialog.h"
#include "AverageSpecOperaton.h"

using namespace kome::operation::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MergedSpectrumManager::MergedSpectrumManager() {
	m_mspectra.clear();
	m_spec = NULL;
}

// destructor
MergedSpectrumManager::~MergedSpectrumManager() {
	// delete objects
	while( m_spectraSet.size() > 0 ) {
		delete *( m_spectraSet.begin() );
	}

	// @date 2011.09.29 <Add> M.Izumi ->
	int i = m_mspectra.size()-1;
	while( i >= 0 ){
		delete m_mspectra[i];
		i--;
	}
	// @date 2011.09.29 <Add> M.Izumi <-
}

// add merged spectrum
void MergedSpectrumManager::addMergedSpectrum( MergedSpectrum* spec ) {
	if( spec != NULL ) {
		m_spectraSet.insert( spec );
	}
}

// remove merged spectrum
void MergedSpectrumManager::removeMergedSpectrum( MergedSpectrum* spec ) {
	m_spectraSet.erase( spec );
}

// open merged spectrum
bool MergedSpectrumManager::openMergedSpectrum(
		kome::objects::Chromatogram* chrom,
		const double startRt,
		const double endRt
) {
	// get spectra
	kome::objects::DataSet dataSet( chrom->getGroup() );
	if( chrom != NULL ) {
		chrom->getSpectra( dataSet, startRt, endRt );
	}
	if( dataSet.getNumberOfSpectra() == 0 ) {
		wxMessageBox(
			wxT( "An averaged spectra cannot be generated because it is impossible to get a spectrum from this chromatogram.\nOpen a data file containing spectra." ),
			wxT( "Mass++" ),
			wxOK | wxICON_HAND
		);
		return false;
	}

	// sample
	kome::objects::Sample* sample = chrom->getSample();
	MergedSpectrum* mergedSpec = NULL;
	
	// @date 2012/05/10 <Add> OKADA	//Å@SPEC 85547
	// progress
	kome::window::DialogProgress progressDialog( kome::window::WindowTool::getMainWindow(), "Averaging Spectra..." );
	
	progressDialog.setRange( 0, dataSet.getNumberOfSpectra()+1 );
	progressDialog.setPosition( 0 );

	// spectrum
	if( dataSet.getNumberOfSpectra() == 1 ) {
		m_spec = dataSet.getSpectrum( 0 );
	}
	else {
		// create merged spectrum
		mergedSpec = new MergedSpectrum( chrom->getGroup() );

		// progress
		progressDialog.setRange( 0, dataSet.getNumberOfSpectra() + 1 );
		progressDialog.setPosition( 0 );

		for( unsigned int i = 0; i < dataSet.getNumberOfSpectra() && !progressDialog.isStopped(); i++ ) {
			
			// status
			std::string msg = FMT( "Merging Spectra [%d/%d]", ( i + 1 ), dataSet.getNumberOfSpectra() );
			progressDialog.setStatus( msg.c_str() );

			mergedSpec->addSpectrum( dataSet.getSpectrum( i ) );
			progressDialog.setPosition( i + 1 );
		}
				
		m_spec = mergedSpec;
	}
	
	if( progressDialog.isStopped() ){
		return false;
	}

	// open
	if( kome::plugin::PluginCallTool::onOpenSpectrum( *m_spec ) ) {

		kome::plugin::PluginCallTool::openSpectrum( *m_spec );
	}

	progressDialog.fill();		//Å@SPEC 85547
	
	return true;
}

// merge all active spectra
bool MergedSpectrumManager::mergeAllSpectra() {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// samples
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );

		kome::objects::DataSet* ds = aoMgr.getFilteredDataSet( sample );

		// create merged spectrum
		kome::objects::Spectrum* spec = NULL;
		if( ds->getNumberOfSpectra() == 1 ) {
			spec = ds->getSpectrum( 0 );
		}
		else if( ds->getNumberOfSpectra() > 1 ) {
			kome::objects::AveragedSpectrum* avgSpec = new kome::objects::AveragedSpectrum( sample->getRootDataGroupNode() );
			for( unsigned int k = 0; k < ds->getNumberOfSpectra(); k++ ) {
				avgSpec->addSpectrum( ds->getSpectrum( k ) );
			}
			spec = avgSpec;
		}

		// update data set
		ds->clearSpectra();
		if( spec != NULL ) {
			ds->addSpectrum( spec );
		}
	}

	return true;
}

// merge all MS/MS spectra
bool MergedSpectrumManager::mergeMs2Spectra() {
	// data set
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// samples
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );

		// data set
		kome::objects::DataSet* ds = aoMgr.getFilteredDataSet( sample );
		
		// merge
		std::vector< kome::objects::Spectrum* > parents;
		std::map< kome::objects::Spectrum*, kome::objects::DataSet > dsMap;
		
		for( unsigned int k = 0; k < ds->getNumberOfSpectra(); k++ ) {
			// spectrum
			kome::objects::Spectrum* spec = ds->getSpectrum( k );
			
			if( spec->getMsStage() > 1 ) {	// MS/MS
				kome::objects::Spectrum* parent = spec->getParentSpectrum();
				kome::objects::DataSet* mgDs = &( dsMap[ parent ] );
				
				if( dsMap.find( parent ) == dsMap.end() ) {
					parents.push_back( parent );
				}
				
				mgDs->addSpectrum( spec );
			}
		}
		
		ds->clearSpectra();
		for( unsigned int i = 0; i < parents.size(); i++ ) {
			kome::objects::DataSet* mgDs = &( dsMap[ parents[ i ] ] );
			kome::objects::Spectrum* spec = NULL;
			
			// spectrum
			if( mgDs->getNumberOfSpectra() == 1 ) {   // single spectrum
				spec = mgDs->getSpectrum( i );
			}
			else if( mgDs->getNumberOfSpectra() > 1 ) {	// merge
				MergedSpectrum* mspec = new MergedSpectrum( mgDs->getGroup() );
				for( unsigned int j = 0; j < mgDs->getNumberOfSpectra(); j++ ) {
					mspec->addSpectrum( mgDs->getSpectrum( j ) );
				}
			}
			
			ds->addSpectrum( spec );
		}
	}

	return true;
}

//2012/02/28 add by t.okuno -> This plugin displays a range choice screen.
// creates Averaged Spectrum dialog
void MergedSpectrumManager::createAveragedSpectrum( 
	kome::objects::Chromatogram* chrom,	
	const double startRT,
	const double endRT 
){
	// @date 2012.04.26 <Mod> M.Izumi ->
	// SPEC No.85858 Average Spectrum / Generate Chromatogram operations create a new window, but it is hidden
	AveragedSpectrumDialog dlg( kome::window::WindowTool::getMainWindow(), chrom );
	
	dlg.setChrom( chrom );
	dlg.setStartPos( startRT );
	dlg.setEndPos( endRT );

	if( dlg.ShowModal() == wxID_OK ){
		// -----------------
		// add operation
		// -----------------
		AverageSpecOperaton* opt = new AverageSpecOperaton();
		opt->setOperationName( "Averaged Spectrum" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setShortName( "average_spec" );
		opt->setTargetSample( chrom->getSample() );
		opt->setChrom( chrom );
		opt->setStartRt( dlg.getStartPos() );
		opt->setEndRt( dlg.getEndPos() );
	
		if( !opt->execute() ){
			delete opt;
		}
	}
	// <-
}
//2012/02/28 add by t.okuno <- This plugin displays a range choice screen.

// on close sample
void MergedSpectrumManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
}

// on close spectrum
void MergedSpectrumManager::onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) {

	// delete
	if  ( true == deleting )
	{
		// cast
		MergedSpectrum* mspec = dynamic_cast< MergedSpectrum* >( spec );

		if( mspec != NULL
				&& m_spectraSet.find( mspec ) != m_spectraSet.end() ) {	// check the set
			m_spectraSet.erase( mspec );
			m_mspectra.push_back( mspec );	// @date 2011.09.29 <Add> M.Izumi
		}
	}
}

// get instance
MergedSpectrumManager& MergedSpectrumManager::getInstance() {
	// create object (This is the only object.)
	static MergedSpectrumManager mgr;

	return mgr;
}
