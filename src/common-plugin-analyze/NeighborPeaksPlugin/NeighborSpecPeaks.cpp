/**
 * @file NeighborSpecPeaks.cpp
 * @brief implements of NeighborSpecPeaks class
 *
 * @author S.Tanaka
 * @date 2009.02.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "NeighborSpecPeaks.h"

#include <math.h>


using namespace kome::search::neighbor;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PEAK_PARAM_NAME	   "peak_detector"   
#define PEAK_FUNC_TYPE	   "SPEC_PEAK_PICKING"


// constructor
NeighborSpecPeaks::NeighborSpecPeaks(
	   kome::objects::SettingParameterValues* settings
) : NeighborPeaks( settings ) {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// peak detector
	if( m_settings == NULL ) {
		return;
	}

	const char* detector = m_settings->getParameterValue( PEAK_PARAM_NAME );
	m_peakDetector = plgMgr.getFunctionItem( PEAK_FUNC_TYPE, detector );
	m_peakSettings = m_settings->getSubParameterValues( PEAK_PARAM_NAME, detector );
}

// destructor
NeighborSpecPeaks::~NeighborSpecPeaks() {
}

// search spectrum peaks
void NeighborSpecPeaks::searchPeaks(
					kome::objects::Sample* sample,
					kome::ident::PeakPositions& peakPos,
					kome::operation::Alignment* align,
					const int sampleId,
					kome::core::Progress& progress,
					const char* val
) {
	// check parameters
	_ASSERT( m_peakDetector != NULL );
	if( peakPos.getNumberOfPeaks() == 0 ) {
		return;
	}

	// check peaks
	bool hasChrom = false;
	for( int i = 0; i < peakPos.getNumberOfPeaks() && !hasChrom; i++ ) {
		hasChrom =( peakPos.getPeakPositionRt( i ) > 0.0 );
	}

	// spectra
	kome::objects::DataSet spectra;
	getSpectra( sample, spectra, hasChrom );

	// peaks
	std::vector< PeakInfo > peakArray;
	peakArray.resize( peakPos.getNumberOfPeaks() );

	for( int i = 0; i < peakPos.getNumberOfPeaks(); i++ ) {
		peakArray[ i ].idx = i;
		double mz = peakPos.getPeakPositionMz( i );
		peakArray[ i ].rt = align->invertRt( peakPos.getPeakPositionRt( i ), mz );
		peakArray[ i ].mz = mz;
	}

	std::sort( peakArray.begin(), peakArray.end(), NeighborPeaks::lessRt );

	// progress
	if( peakArray.empty() ) {
		progress.fill();
		return;
	}
	progress.setRange( 0, peakArray.size() );
	progress.setPosition( 0 );

	// profile list
	std::list< ProfileInfo > profileList;
	int specIdx = 0;

	// get function items
	std::vector< kome::plugin::PluginFunctionItem* > items;
	std::vector< kome::objects::SettingParameterValues > settingVals;
	getManipulatFuncInfo( val, items, settingVals );


	// search peaks
	for( unsigned int i = 0; i < peakArray.size() && !progress.isStopped(); i++ ) {
		// peak
		PeakInfo& peak = peakArray[ i ];

		// delete profiles
		while( profileList.size() > 0
				&& ( hasChrom && profileList.front().rt < peak.rt - m_rtTol ) ) {
			profileList.pop_front();
		}

		// detect peaks
		while( specIdx < (int)spectra.getNumberOfSpectra()
				&& ( !hasChrom || spectra.getSpectrum( specIdx )->getRt() <= peak.rt + m_rtTol ) ) {
			// spectrum
			kome::objects::Spectrum* spec = spectra.getSpectrum( specIdx );

			// detect peaks
			if( !hasChrom || spec->getRt() >= peak.rt - m_rtTol ) {
				profileList.resize( profileList.size() + 1 );
				profileList.back().rt = spec->getRt();
				getPeaks( *spec, profileList.back().peaks, profileList.back().dps, items, settingVals );
			}

			specIdx++;
		}

		// search peak
		double val = -1.0;
		ProfileInfo* profile = NULL;
		kome::objects::PeakElement* pk = searchPeak( peak.rt, peak.mz, profileList, &val, &profile );
		// @date 2014.05.12 <Mod> M.Izumi ->
		kome::core::DataPoints tmp;
		tmp.clearPoints();
		
		kome::core::DataPoints dps = (profile != NULL ? profile->dps : tmp);

		peakPos.setPeakValueInfo( peak.idx, sampleId, val, dps, pk, false );
		// @date 2014.05.12 <Mod> M.Izumi <-

		// progress
		//progress.setPosition( i + 1 );
	}

	// progress
	if( !progress.isStopped() ) {
		progress.fill();
	}
}

// get peaks
void NeighborSpecPeaks::getPeaks( 
	kome::objects::Spectrum& spec,
	kome::objects::Peaks& peaks, 
	kome::core::DataPoints& dps,
	std::vector< kome::plugin::PluginFunctionItem* > items,
	std::vector< kome::objects::SettingParameterValues > settingVals
) {
	// clear
	peaks.clearPoints();
	
	// data points
	kome::core::DataPoints dps0;
	kome::core::DataPoints dps1;
	spec.getXYData( &dps0, false );

	kome::core::DataPoints* org = &dps0;
	kome::core::DataPoints* updated = &dps1;
	kome::core::DataPoints* tmp = NULL;
	
	// get xy data operation
	std::vector< kome::objects::XYDataOperation* > opts;
	for( unsigned int j=0; j < items.size(); j++ ){
		kome::plugin::PluginFunctionItem* item = items[j];
		if( item != NULL ){
			kome::objects::Parameters params;
			kome::plugin::PluginCallTool::setSettingValues( params, settingVals[j] );
							
			kome::objects::XYDataOperation* opt = NULL;
			kome::plugin::PluginCallTool::setSpectrum( params, spec );

			opt = (kome::objects::XYDataOperation*)item->getCall()->invoke( &params ).prim.pt;
			if( opt != NULL ){
				opt->update( *org, *updated, spec );
				tmp = updated;
				updated = org;
				org = tmp;
				updated->clearPoints();
				opts.push_back( opt );
			}
		}
	}

	// data points
	for( unsigned int i = 0; i < org->getLength(); i++ ) {
		dps.addPoint( org->getX( i ), org->getY( i ) );
	}
	peaks.setDataPoints( dps );

	// peak picking
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setSpectrum( params, spec );
	kome::plugin::PluginCallTool::setXYData( params, dps );
	kome::plugin::PluginCallTool::setPeaks( params, peaks );

	if( m_peakSettings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params, *m_peakSettings );
	}

	m_peakDetector->getCall()->invoke( &params );
	
	// clear xy data operation
	for( unsigned int i=0; i < opts.size(); i++ ){
		delete opts[i];
	}
	opts.clear();
}

// search peak
kome::objects::PeakElement* NeighborSpecPeaks::searchPeak(
		const double rt,
		const double mz,
		std::list< ProfileInfo >& profileList,
		double* val,
		ProfileInfo** profile
) {
	// max Y
	double maxInt = 1.0;
	for( std::list< ProfileInfo >::iterator it = profileList.begin(); it != profileList.end(); it++ ) {
		// peaks
		kome::objects::Peaks* peaks = &( (*it).peaks );

		// index
		int startIdx = 0;
		if( rt >= 0.0 ) {
			startIdx = peaks->searchIndex( std::max( rt - m_rtTol, 0.0 ) );
			if( startIdx < 0 ) {
				startIdx = - startIdx - 1;
			}
		}
		
		int endIdx = (int)peaks->getLength() - 1;
		if( rt >= 0.0 ) {
			endIdx = peaks->searchIndex( rt + m_rtTol );
			if( endIdx < 0 ) {
				endIdx = - endIdx - 2;
			}
		}
			
		for( int i = startIdx; i <= endIdx; i++ ) {
			maxInt = std::max( peaks->getY( i ), maxInt );
		}
	}

	// search
	double score = - FLT_MAX;
	kome::objects::PeakElement* peak = NULL;
	double v = -1.0;
	ProfileInfo* p = NULL;
	double sumV = 0.0;
	int profileId = -1;
	int cnt = 0;
	for( std::list< ProfileInfo >::iterator it = profileList.begin(); it != profileList.end(); it++ ) {
		// peaks
		double tmpRt = (*it).rt;
		kome::objects::Peaks* peaks = &( (*it).peaks );

		// RT diff
		const double rtDiff = fabs( rt - tmpRt ) / m_rtTol;

		// index
		int startIdx = peaks->searchIndex( std::max( mz - m_mzTol, 0.0 ) );
		if( startIdx < 0 ) {
			startIdx = - startIdx - 1;
		}
		
		int endIdx = peaks->searchIndex( mz + m_mzTol );
		if( endIdx < 0 ) {
			endIdx = - endIdx - 2;
		}

		// each peaks
		for( int i = startIdx; i <= endIdx; i++ ) {
			// peak
			kome::objects::PeakElement* tmp = peaks->getPeak( i );
			const double x = tmp->getX();
			const double y = tmp->getY();
			const double y2 = ( m_peakValue == VALUE_INTENSITY ? y : tmp->getArea() );
			sumV += y2;

			// diff
			const double mzDiff = fabs( x - mz ) / m_mzTol;
			const double diff = rtDiff * rtDiff + mzDiff * mzDiff;

			// score
			double tmpScore = - FLT_MAX;
			if( m_peakSel == SEL_NEAREST ) {
				tmpScore = - diff;
			}
			else if( m_peakSel == SEL_MAX || m_peakSel == SEL_SUM || m_peakSel == SEL_AVERAGE ) {
				tmpScore = y2;
			}
			else if( m_peakSel == SEL_SCORE ) {
				tmpScore = ( y / maxInt ) / diff;
			}
			else if( m_peakSel == SEL_SCORE2 ) {
				tmpScore = ( 1.0 - y / maxInt );
				tmpScore = tmpScore * tmpScore;
				tmpScore += diff * diff;
				tmpScore = - tmpScore;
			}

			// update
			if( tmpScore > score ) {
				score = tmpScore;
				peak = tmp;

				v = y2;
				p = &( *it );
			}
			cnt++;
		}
	}

	// unfound
	if( peak == NULL ) {
		return peak;
	}

	// value
	if( m_peakSel == SEL_SUM ) {
		v = sumV;
	}
	else if( m_peakSel == SEL_AVERAGE ) {
		v = sumV / (double)cnt;
	}

	if( val != NULL ) {
		*val = v;
	}
	if( profile != NULL ) {
		*profile = p;
	}

	return peak;
}

// compare peaks
bool NeighborSpecPeaks::lessPeak( kome::objects::Peak2DElement* p0, kome::objects::Peak2DElement* p1 ) {
	return ( p0->getRt() < p1->getRt() );
}

// get manipulat func info
void NeighborSpecPeaks::getManipulatFuncInfo(
	const char* val,
	std::vector< kome::plugin::PluginFunctionItem* > &items, 
	std::vector< kome::objects::SettingParameterValues > &settings 
){
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	std::vector< std::string > arry;
	kome::plugin::SettingsValue::separateListValue( val, arry );

	for( unsigned int j=0; j < arry.size(); j++ ){
		std::string name = ( arry[j].c_str() );

		int ifind = name.find_first_of( "[" );
		std::string strItem = name.substr( 0, ifind );
		std::string strParam = name.substr( ifind );

		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "spec_manipulation", strItem.c_str() );
	
		if( item != NULL ){
			kome::objects::SettingParameterValues manipulatSetting;
			kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );
			if( page != NULL ){
				page->setParameterString( manipulatSetting, strParam.c_str() );	
			}
			
			items.push_back( item );
			settings.push_back( manipulatSetting );
		}
	}
}
		
