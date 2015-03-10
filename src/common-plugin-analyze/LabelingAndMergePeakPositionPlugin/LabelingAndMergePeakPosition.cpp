/**
 * @file LabelingAndMergePeakPosition.cpp
 * @brief implements of LabelingAndMergePeakPosition class
 *
 * @author S.Tanaka
 * @date 2008.11.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LabelingAndMergePeakPosition.h"

#include <math.h>


using namespace kome::position::merge;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PEAK_DETECTOR_PARAM_NAME			"peak"
#define CLUSTER_DETECTOR_PARAM_NAME		    "cluster"
#define RT_TOLERANCE_PARAM_NAME			    "rt_tol"
#define MZ_TOLERANCE_PARAM_NAME			    "mz_tol"
#define CONTINUME_MODE_PARAM_NAME		    "continue"

#define PEAK_DETECTOR_PLUGIN_TYPE		    "2D_PEAK_PICKING"


// constructor
LabelingAndMergePeakPosition::LabelingAndMergePeakPosition( kome::objects::SettingParameterValues* settings, kome::ident::SampleGetter* getter ) {
	m_settings = settings;
	m_getter = getter;
	setParameters();
}

// destructor
LabelingAndMergePeakPosition::~LabelingAndMergePeakPosition() {
}

// execute
void LabelingAndMergePeakPosition::execute( kome::objects::Peaks2D* peaks, kome::core::Progress& progress ) {
	// check parameters
	_ASSERT( m_getter != NULL );
	_ASSERT( m_rtTol > 0.0 );
	_ASSERT( m_mzTol > 0.0 );
//	_ASSERT( m_peakDetector != NULL );

	// progress
	const unsigned int num = m_getter->getNumberOfSamples();
	if( num == 0 ) {
		return;
	}
	progress.createSubProgresses( num + 1 );

	// peak map
	std::map< long long, PeakInfo > peakMap;
	std::set< long long > keySet;

	// each samples
	kome::objects::Sample* sample = NULL;
	int idx = 0;
	while( ( sample = m_getter->next() ) != NULL && !progress.isStopped() ) {
		// progress
		kome::core::Progress* prgs = progress.getSubProgress( idx );
		progress.setStatus( FMT( "Peak Detecting... [%d/%d]", ( idx + 1 ), num ).c_str() );

		// peaks
		kome::objects::Peaks2D tmp;
		detectPeaks( sample, tmp, *prgs );
		std::vector< kome::objects::PeaksCluster2D* > clusters;
		for( unsigned int i = 0; i < tmp.getNumberOfClusters(); i++ ) {
			clusters.push_back( tmp.getCluster( i ) );
		}
		std::sort( clusters.begin(), clusters.end(), lessCluster );

		// set to map
		for( unsigned int i = 0; i < clusters.size(); i++ ) {
			kome::objects::PeaksCluster2D* cluster = clusters[ i ];

			double rt = double();
			double mz = double();
			m_getter->align( cluster->getRt(), cluster->getMz(), &rt, &mz );
			double val = cluster->getIntensity();
			int charge = cluster->getCharge();

			long rtKey = roundnum( rt / m_rtTol );
			long mzKey = roundnum( mz / m_mzTol );
			long long key = ( ( (long long)rtKey << 32 ) | mzKey );

			if( peakMap.find( key ) == peakMap.end() ) {    // new peak
				peakMap[ key ].rt = rt;
				peakMap[ key ].mz = mz;
				peakMap[ key ].intensity = val;
				peakMap[ key ].charge = charge;
				peakMap[ key ].id = m_getter->getMatrixSampleId();
				keySet.insert( key );
			}
			else {
				if( val > peakMap[ key ].intensity ) {    // update peak
					peakMap[ key ].rt = rt;
					peakMap[ key ].mz = mz;
					peakMap[ key ].intensity = val;
					peakMap[ key ].charge = charge;
				}
			}
		}
		prgs->fill();

		idx++;
	}
	
	// merge peaks
	if( progress.isStopped() ) {
		return;
	}
	if( peakMap.empty() ) {
		progress.fill();
		return;
	}

	// create array
	std::vector< std::pair< long long, PeakInfo > > pairs;

	for( std::map< long long, PeakInfo >::iterator it = peakMap.begin(); it != peakMap.end(); it++ ) {
		pairs.push_back( *it );
	}

	std::sort( pairs.begin(), pairs.end(), lessPeak );
	
	progress.setStatus( "Merging Peaks..." );
	kome::core::Progress* prgs = progress.getSubProgress( num );
	prgs->setRange( 0, pairs.size() );
	prgs->setPosition( 0 );

	int poss[] = {
        -1, -1, -1, 0, -1, 1, 0, 1, 1, 1, 1, 0, 1, -1, 0, -1
	};
	unsigned int posNum = sizeof( poss ) / sizeof( int ) / 2;
	
	for( unsigned int i = 0; i < pairs.size() && !progress.isStopped(); i++ ) {
		long long key = pairs[ i ].first;

		if( peakMap.find( key ) != peakMap.end() ) {    // existing peak
			// register
			const double rt = pairs[ i ].second.rt;
			const double mz = pairs[ i ].second.mz;
			kome::objects::Peak2DElement* peak = peaks->createPeak( rt, mz, pairs[ i ].second.intensity );
			peak->setCharge( pairs[ i ].second.charge );
			
			// merge
			long rtKey = (long)( ( key >> 32 ) & 0xffffffff );
			long mzKey = (long)( key & 0xffffffff );

			for( unsigned int j = 0; j < posNum; j++ ) {
				long tmpRtKey = rtKey + poss[ j * 2 ];
				long tmpMzKey = mzKey + poss[ j * 2 + 1 ];

				if( tmpRtKey >= 0 && tmpMzKey >= 0 ) {
					long long tmpKey = ( ( (long long)tmpRtKey << 32 ) | tmpMzKey );
					if( peakMap.find( tmpKey ) != peakMap.end() ) {
						const double rtDiff = fabs( rt - peakMap[ tmpKey ].rt );
						const double mzDiff = fabs( mz - peakMap[ tmpKey ].mz );

						if( rtDiff <= m_rtTol && mzDiff <= m_mzTol ) {
							peakMap.erase( tmpKey );
						}
					}
				}
			}
			
			peaks->setDataId( pairs[ i ].second.id );
		}
		prgs->setPosition( i + 1 );
	}

	

	prgs->fill();

	progress.fill();
}

// set parameters
void LabelingAndMergePeakPosition::setParameters() {
	// initialize
	m_peakDetector = NULL;
	m_peakSettings = NULL;
	m_rtTol = 0.5;
	m_mzTol = 0.25;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// get from settings object
	if( m_settings == NULL ) {
		return;
	}

	const char* peakDetector = m_settings->getParameterValue( PEAK_DETECTOR_PARAM_NAME );
	m_peakDetector = plgMgr.getFunctionItem( PEAK_DETECTOR_PLUGIN_TYPE, peakDetector );

	m_peakSettings = m_settings->getSubParameterValues( PEAK_DETECTOR_PARAM_NAME, peakDetector );

	m_rtTol = std::max( m_settings->getDoubleValue( RT_TOLERANCE_PARAM_NAME, 0.5 ), 0.01 );
	m_mzTol = std::max( m_settings->getDoubleValue( MZ_TOLERANCE_PARAM_NAME, 0.25 ), 0.001 );
}

// detect peaks
void LabelingAndMergePeakPosition::detectPeaks( kome::objects::Sample* sample, kome::objects::Peaks2D& peaks, kome::core::Progress& progress ) {
	// spectra
	kome::objects::DataSet spectra;
	getSpectra( sample, spectra );

	// detect peaks
	progress.createSubProgresses( 2 );
	kome::core::Progress* prgs = progress.getSubProgress( 0 );

	kome::objects::Parameters peakParams;
	kome::plugin::PluginCallTool::setDataSet( peakParams, spectra );
	kome::plugin::PluginCallTool::setPeaks2D( peakParams, peaks );
	kome::plugin::PluginCallTool::setProgress( peakParams, *prgs );
	if( m_peakSettings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( peakParams, *m_peakSettings );
	}

	m_peakDetector->getCall()->invoke( &peakParams );

	if( progress.isStopped() ) {
		return;
	}

	// detect clusters
	prgs = progress.getSubProgress( 1 );
	
	for( unsigned int i = 0; i < peaks.getNumberOfPeaks() && !progress.isStopped(); i++ ) {
		kome::objects::Peak2DElement* peak = peaks.getPeak( i );
		kome::objects::PeaksCluster2D* cluster = peak->getCluster();
		if( cluster == NULL ) {
			cluster = peaks.createCluster();
			cluster->addPeak( peak );
		}
	}

	prgs->fill();

	progress.fill();
}

// get spectra
void LabelingAndMergePeakPosition::getSpectra( kome::objects::Sample* sample, kome::objects::DataSet& spectra ) {
	// check the parameter
	if( sample == NULL ) {
		return;
	}

	// spectra
	for( unsigned int i = 0; i < sample->getNumberOfGroups(); i++ ) {    // each groups
		kome::objects::DataGroupNode* group = sample->getGroup( i );

		for( unsigned int j = 0; j < group->getNumberOfSpectra(); j++ ) {
			kome::objects::Spectrum* spec = group->getSpectrum( j );

			if( spec->getMsStage() == 1 && spec->getRt() >= 0.0 && spec->hasChromatogram() ) {
				spectra.addSpectrum( spec );
			}
		}
	}

	// sort
	spectra.sortSpectra();
}

// compare peaks
bool LabelingAndMergePeakPosition::lessPeak( std::pair< long long, PeakInfo >& p0, std::pair< long long, PeakInfo >& p1 ) {
	return ( p0.second.intensity > p1.second.intensity );
}

// compare clusters
bool LabelingAndMergePeakPosition::lessCluster( kome::objects::PeaksCluster2D* c0, kome::objects::PeaksCluster2D* c1 ) {
	return ( c0->getMz() < c1->getMz() );
}
