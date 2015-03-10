/**
 * @file NeighborChromPeaks.cpp
 * @brief implements of NeighborChromPeaks class
 *
 * @author S.Tanaka
 * @date 2009.02.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "NeighborChromPeaks.h"

#include <math.h>


using namespace kome::search::neighbor;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PEAK_PARAM_NAME	  "peak_detector"
#define TYPE_PARAM_NAME	  "type"

#define PEAK_FUNC_TYPE	   "CHROM_PEAK_PICKING"


// constructor
NeighborChromPeaks::NeighborChromPeaks(
	   kome::objects::SettingParameterValues* settings
) :  NeighborPeaks( settings ) {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// peak detector
	if( m_settings == NULL ) {
		return;
	}

	const char* detector = m_settings->getParameterValue( PEAK_PARAM_NAME );
	m_peakDetector = plgMgr.getFunctionItem( PEAK_FUNC_TYPE, detector );
	m_peakSettings = m_settings->getSubParameterValues( PEAK_PARAM_NAME, detector );

	const char* type = m_settings->getParameterValue( TYPE_PARAM_NAME );
	m_bpcFlg = ( type != NULL && strcmp( type, "bpc" ) == 0 );
}

// destructor
NeighborChromPeaks::~NeighborChromPeaks() {
}

// search peaks
void NeighborChromPeaks::searchPeaks(
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

	// spectra
	kome::objects::DataSet spectra;
	getSpectra( sample, spectra, true );

	// peaks
	std::vector< PeakInfo > peakArray;
	peakArray.resize( peakPos.getNumberOfPeaks() );

	for( int i = 0; i < peakPos.getNumberOfPeaks(); i++ ) {
		peakArray[ i ].idx = i;
		peakArray[ i ].rt = peakPos.getPeakPositionRt( i );
		peakArray[ i ].mz = peakPos.getPeakPositionMz( i );
	}

	std::sort( peakArray.begin(), peakArray.end(), NeighborPeaks::lessMz );

	// progress
	if( peakArray.empty() ) {
		progress.fill();
		return;
	}
	progress.setRange( 0, peakArray.size() );
	progress.setPosition( 0 );

	// chromatogram
	kome::objects::SpectraChromatogram chrom( spectra );
	kome::objects::Peaks* chromPeaks = NULL;
	kome::core::DataPoints* dps = NULL;
	long mzIdx = -1;

	// get function items
	std::vector< kome::plugin::PluginFunctionItem* > items;
	std::vector< kome::objects::SettingParameterValues > settingVals;
	getManipulatFuncInfo( val, items, settingVals );

	// search peaks
	for( unsigned int i = 0; i < peakArray.size() && !progress.isStopped(); i++ ) {
		// peak
		PeakInfo& peak = peakArray[ i ];

		// chromatogram
		long tmpIdx = roundnum( peak.mz / m_mzTol );
		if( tmpIdx > mzIdx ) {
			mzIdx = tmpIdx;
			const double startMz = std::max( m_mzTol * ( (double)mzIdx - 0.5 ), 0.0 );
			const double endMz = m_mzTol * ( (double)mzIdx + 0.5 );

			chrom.setMinMz( startMz );
			chrom.setMaxMz( endMz );

			if( m_bpcFlg ) {
				chrom.setBPC();
			}
			else {
				chrom.setTIC();
			}

			if( dps != NULL ) {
				delete dps;
			}
			if( chromPeaks != NULL ) {
				delete chromPeaks;
			}
			chromPeaks = new kome::objects::Peaks();
			dps = new kome::core::DataPoints();

			getPeaks( chrom, *chromPeaks, *dps, items, settingVals );
		}

		// search peak
		double val = -1.0;
		const double rt = align->invertRt( peak.rt, peak.mz );
		kome::objects::PeakElement* chromPeak = searchPeak( rt, *chromPeaks, &val );

		peakPos.setPeakValueInfo( peak.idx, sampleId, val, *dps, chromPeak, true );

		// progress
		//Åöprogress.setPosition( i + 1 );
	}

	if( dps != NULL ) {
		delete dps;
	}
	if( chromPeaks != NULL ) {
		delete chromPeaks;
	}

	// progress
	if( !progress.isStopped() ) {
		progress.fill();
	}
}

// get peaks
void NeighborChromPeaks::getPeaks( 
	kome::objects::Chromatogram& chrom,
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
	chrom.getXYData( &dps0, false );

	kome::core::DataPoints* org = &dps0;
	kome::core::DataPoints* updated = &dps1;
	kome::core::DataPoints* tmp = NULL;

	// set operation
	std::vector< kome::objects::XYDataOperation* > opts;
	for( unsigned int j=0; j < items.size(); j++ ){
		kome::plugin::PluginFunctionItem* item = items[j];
		if( item != NULL ){
			kome::objects::Parameters params;
			kome::plugin::PluginCallTool::setSettingValues( params, settingVals[j] );
							
			kome::objects::XYDataOperation* opt = NULL;
			kome::plugin::PluginCallTool::setChromatogram( params, chrom );

			opt = (kome::objects::XYDataOperation*)item->getCall()->invoke( &params ).prim.pt;
			if( opt != NULL ){
				opt->update( *org, *updated, chrom );
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
	kome::plugin::PluginCallTool::setChromatogram( params, chrom );
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
kome::objects::PeakElement* NeighborChromPeaks::searchPeak( const double rt, kome::objects::Peaks& peaks, double* val ) {
	// index
	int startIdx = peaks.searchIndex( std::max( rt - m_rtTol, 0.0 ) );
	if( startIdx < 0 ) {
		startIdx = - startIdx - 1;
	}

	int endIdx = peaks.searchIndex( rt + m_rtTol );
	if( endIdx < 0 ) {
		endIdx = - endIdx - 2;
	}

	// max Y
	double maxInt = 1.0;
	for( int i = startIdx; i <= endIdx; i++ ) {
		maxInt = std::max( peaks.getY( i ), maxInt );
	}

	// search
	double score = - FLT_MAX;
	kome::objects::PeakElement* peak = NULL;
	double v = -1.0;
	double sumV = 0.0;
	for( int i = startIdx; i <= endIdx; i++ ) {
		// peak
		kome::objects::PeakElement* tmp = peaks.getPeak( i );
		const double x = tmp->getX();
		const double y = tmp->getY();
		const double y2 = ( m_peakValue == VALUE_INTENSITY ? y : tmp->getArea() );
		sumV += y2;

		// diff
		const double diff = fabs( x - rt ) / m_rtTol;

		// score
		double tmpScore = - FLT_MAX;
		if( m_peakSel == SEL_NEAREST ) {
			tmpScore = - diff;
		}
		else if( m_peakSel == SEL_MAX || m_peakSel == SEL_SUM || m_peakSel == SEL_AVERAGE ) {
			tmpScore = y2;
		}
		else if( m_peakSel == SEL_SCORE ) {
			tmpScore = ( y / maxInt ) / ( diff * diff );
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
		v = sumV / (double)( endIdx - startIdx + 1 );
	}

	if( val != NULL ) {
		*val = v;
	}

	return peak;
}

// compare peaks
bool NeighborChromPeaks::lessPeak( kome::objects::Peak2DElement* p0, kome::objects::Peak2DElement* p1 ) {
	return ( p0->getMz() < p1->getMz() );
}

// get manipulat func info
void NeighborChromPeaks::getManipulatFuncInfo(
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

		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "chrom_manipulation", strItem.c_str() );
	
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
