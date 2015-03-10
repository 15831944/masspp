/**
 * @file MrmPeakPosDetect.cpp
 * @brief interfaces of MrmPeakPosDetector class
 *
 * @author M.Izumi
 * @date 2013.12.13
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "MrmPeakPosDetect.h"


using namespace kome::position::mrm;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




#define PEAK_DETECTOR_PARAM_NAME			"peak"
#define VALUE_TYPE_PARAM_NAME			    "value_type"
#define RT_TOLERANCE_PARAM_NAME			    "rt_tol"
#define MZ_TOLERANCE_PARAM_NAME			    "mz_tol"

#define PEAK_DETECTOR_PLUGIN_TYPE		    "CHROM_PEAK_PICKING"

#define RANGE_NUMBER_PARAM_NAME			    "Range Number"


// constructor
MrmPeakPosDetect::MrmPeakPosDetect()
		: kome::ident::PeakPositionsDetector( true, false ) {
	m_settings = NULL;
}

// destructor
MrmPeakPosDetect::~MrmPeakPosDetect(){
}

// on detect peak positons
int MrmPeakPosDetect::onDetectPeakPositions( kome::ident::SampleGetter& getter, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress ){
	// flag
	int ret = HAS_RT;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// parameters
	if( m_settings == NULL ) {
		return ret;
	}

	const char* tmp = m_settings->getParameterValue( PEAK_DETECTOR_PARAM_NAME );
	kome::plugin::PluginFunctionItem* peakDetector = plgMgr.getFunctionItem( PEAK_DETECTOR_PLUGIN_TYPE, tmp );
	if( peakDetector == NULL ) {
		LOG_ERROR( FMT( "Failed to get the peak detector" ) );
		return ret;
	}

	kome::objects::SettingParameterValues* peakSettings = m_settings->getSubParameterValues( PEAK_DETECTOR_PARAM_NAME, tmp );

	bool areaFlg = false;
	tmp = m_settings->getParameterValue( VALUE_TYPE_PARAM_NAME );
	std::string s = NVL( tmp, "" );
	if( s.compare( "area" ) == 0 ) {
		areaFlg = true;
	}

	const double rtTol = std::max( m_settings->getDoubleValue( RT_TOLERANCE_PARAM_NAME, 0.5 ), 0.01 );
	const double mzTol = std::max( m_settings->getDoubleValue( MZ_TOLERANCE_PARAM_NAME, 0.25 ), 0.001 );

	// progress
	progress.setRange( 0, getter.getNumberOfSamples() );
	progress.setPosition( 0 );

	// map
	std::map< long long, std::map< long, int > > peakMap;

	// each sample
	kome::objects::Sample* sample = NULL;
	int cnt = 0;
	while( ( sample = getter.next() ) != NULL && !progress.isStopped() ) {
		// each group
		for( unsigned int i = 0; i < sample->getNumberOfGroups(); i++) {
			kome::objects::DataGroupNode* group = sample->getGroup( i );
			for( unsigned int j = 0; j < group->getNumberOfChromatograms(); j++ ) {
				kome::objects::Chromatogram* chrom = group->getChromatogram( j );

				if( chrom->getQ1() >= 0.0 && chrom->getQ3() >= 0.0 ) {
					detectPeaks(
						chrom,
						peakMap,
						peakDetector,
						peakSettings,
						areaFlg,
						rtTol,
						mzTol,
						peakPos,
						getter.getMatrixSampleId()
					);
				}
			}
		}
		cnt++;
		progress.setPosition( cnt );
	}

	if( !progress.isStopped() ) {
		progress.fill();
	}

	return ret;
}

// detect peaks
void MrmPeakPosDetect::detectPeaks(
		kome::objects::Chromatogram* chrom,
		std::map< long long, std::map< long, int > >& peakMap,
		kome::plugin::PluginFunctionItem* peakDetector,
		kome::objects::SettingParameterValues* peakSettings,
		const bool areaFlg,
		const double rtTol,
		const double mzTol,
		kome::ident::PeakPositions& peakPos,
		const int sampleId
) {
	// Q1, Q3
	const double q1 = chrom->getQ1();
	const double q3 = chrom->getQ3();

	// key
	long long k0 = roundnum( q1 / mzTol );
	long long k1 = roundnum( q3 / mzTol );
	long long key = ( ( k0 << 32 ) | k1 );

	// chromatogram
	kome::core::DataPoints dps;
	chrom->getXYData( &dps, false );

	// peaks
	kome::objects::Peaks peaks;
	peaks.setDataPoints( dps );

	kome::objects::Parameters params;

	kome::plugin::PluginCallTool::setChromatogram( params, *chrom );
	kome::plugin::PluginCallTool::setXYData( params, dps );
	kome::plugin::PluginCallTool::setPeaks( params, peaks );
	if( peakSettings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params, *peakSettings );
	}

	peakDetector->getCall()->invoke( &params );

	// maps
	std::map< long, int >* idMap = &( peakMap[ key ] );
	std::map< int, kome::objects::PeakElement* > currMap;

	for( unsigned int i = 0; i < peaks.getLength(); i++ ) {
		kome::objects::PeakElement* peak = peaks.getPeak( i );
		long k2 = roundnum( peak->getX() / rtTol );

		int peakId = -1;

		if( idMap->find( k2 ) == idMap->end() ) {
			peakId = peakPos.addPeakPosition( rtTol * (double)k2, -1.0, -1 );
			peakPos.setPeakProperty( peakId, "Q1 Mass", FMT( "%f", q1 ).c_str(), kome::ident::TYPE_UDOUBLE );
			peakPos.setPeakProperty( peakId, "Q3 Mass", FMT( "%f", q3 ).c_str(), kome::ident::TYPE_UDOUBLE );
			(*idMap)[ k2 ] = peakId;
		}
		else {
			peakId = (*idMap)[ k2 ];
		}

		// value
		const double val = ( areaFlg ? peak->getArea() : peak->getY() );
		if( currMap.find( peakId ) == currMap.end() ) {
			currMap[ peakId ] = peak;
		}
		else {
			kome::objects::PeakElement* tmp = currMap[ peakId ];
			const double tmpVal = ( areaFlg ? tmp->getArea() : tmp->getY() );

			if( val > tmpVal ) {
				currMap[ peakId ] = peak;
			}
		}
	}

	// set values
	for( std::map< int, kome::objects::PeakElement* >::iterator it = currMap.begin();
			it != currMap.end(); it++ ) {
		const int peakId = (*it).first;
		kome::objects::PeakElement* peak = (*it).second;
		const double val = ( areaFlg ? peak->getArea() : peak->getY() );

		peakPos.setPeakValueInfo( peakId, sampleId, val, dps, peak, true );
	}
}

void MrmPeakPosDetect::setSettings( kome::objects::SettingParameterValues* settings ){
	m_settings = settings;
}

kome::objects::SettingParameterValues* MrmPeakPosDetect::getSettings(){
	return m_settings;
}
