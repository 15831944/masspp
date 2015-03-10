/**
 * @file MergedSpectrumPeaks.cpp
 * @brief implements of MergedSpectrumPeaks class
 *
 * @author S.Tanaka
 * @date 2009.07.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MergedSpectrumPeaks.h"

#include <math.h>


using namespace kome::position::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define FILTER_PARAM_NAME			  "filter"
#define PEAK_DETECTOR_PARAM_NAME	  "peak_detector"
#define PEAK_FILTER_PARAM_NAME        "peak_filter"
#define VALUE_TYPE_PARAM_NAME		  "val_type"
#define RT_RANGE_PARAM_NAME			  "rt_range"
#define UNIT_PARAM_NAME				  "unit"
#define MASS_TOL_PARAM_NAME			  "tol"

#define PEAK_FUNC_TYPE				  "SPEC_PEAK_PICKING"


// constructor
MergedSpectrumPeaks::MergedSpectrumPeaks( kome::objects::SettingParameterValues* settings, kome::ident::SampleGetter* getter ) {
	m_settings = settings;
	m_getter = getter;
	setParameters();
}

// destructor
MergedSpectrumPeaks::~MergedSpectrumPeaks() {  
}

// execute
void MergedSpectrumPeaks::execute( kome::objects::Peaks2D* peaks, kome::core::Progress& progress ) {
	// check parameters
	_ASSERT( m_peakDetector != NULL );
	_ASSERT( m_getter != NULL );

	// progress
	const unsigned int num = m_getter->getNumberOfSamples();
	if( num == 0 ) {
		return;
	}
	progress.createSubProgresses( num );

	// peak detection
	kome::objects::Sample* sample = NULL;
	int idx = 0;
	while( ( sample = m_getter->next() ) != NULL && !progress.isStopped() ) {
		// progress
		kome::core::Progress* prgs = progress.getSubProgress( idx );
		progress.setStatus( FMT( "Detecting peaks... [%d/%d]", ( idx + 1 ), num ).c_str() );

		// peaks
		kome::objects::Peaks tmp;
		int profileId = detectPeaks( sample, tmp );

		// peak array
		std::vector< kome::objects::PeakElement* > peakArr;
		for( unsigned int i = 0; i < tmp.getLength(); i++ ) {
			kome::objects::PeakElement* peak = tmp.getPeak( i );

			if( peakArr.empty() ) {    // new peak
				peakArr.push_back( peak );
			}
			else {
				double tol = m_massTol;
				if( m_unit == UNIT_PPM ) {
					tol = m_massTol * peakArr.back()->getX() / 1000000.0;
				}
				double maxX = peakArr.back()->getX() + tol;

				if( peak->getX() > maxX ) {
					peakArr.push_back( peak );
				}
				else if( peak->getY() > peakArr.back()->getY() ) {
					peakArr[ peakArr.size() - 1 ] = peak;
				}
			}
		}

		// progress
		if( tmp.getLength() == 0 ) {
			prgs->fill();
		}
		else {
			prgs->setRange( 0, tmp.getLength() );
			prgs->setPosition( 0 );
		}

		// add peak
		int pos = 0;
		peaks->sortByMz( false );
		for( unsigned int i = 0; i < peakArr.size(); i++ ) {
		   kome::objects::PeakElement* elm = peakArr[i];

			   const double val = ( m_type == VALUE_INT ? elm->getY() : elm->getArea() );
			kome::objects::Peak2DElement* peak = NULL;
			bool loop = true;

			while( peak == NULL && pos < (int)peaks->getNumberOfPeaks() && loop ) {
				kome::objects::Peak2DElement* tmpPeak = peaks->getPeak( pos );
				double tol = m_massTol;
				if( m_unit == UNIT_PPM ) {
					tol = m_massTol * peakArr.back()->getX() / 1000000.0;;
				}

				double diff = fabs( tmpPeak->getMz() - elm->getX() );
				if( diff <= tol ) {
					peak = tmpPeak;
				}
				else if( tmpPeak->getMz() > elm->getX() ) {
					loop = false;
				}
				else {
					pos++;
				}
			}

			// new peak
			if( peak == NULL ) {
				peak = peaks->createPeak( m_startRt, elm->getX(), val );
			}

			// add data
			peak->addData( m_getter->getMatrixSampleId(), profileId, val, elm );
			prgs->setPosition( i + 1 );
		}

		idx++;
		prgs->fill();

	}
	
	progress.fill();
}

// set parameters
void MergedSpectrumPeaks::setParameters() {
	// initialize
	m_peakDetector = NULL;
	m_peakSettings = NULL;
	m_type = VALUE_INT;
	m_startRt = -1.0;
	m_endRt = -1.0;
	m_unit = UNIT_DA;
	m_massTol = 0.25;

	// set parameters
	if( m_settings == NULL ) {
		return;
	}

	const char* peakDetector = m_settings->getParameterValue( PEAK_DETECTOR_PARAM_NAME );
	if( peakDetector != NULL ) {
		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		m_peakDetector = plgMgr.getFunctionItem( PEAK_FUNC_TYPE, peakDetector );

		m_peakSettings = m_settings->getSubParameterValues( PEAK_DETECTOR_PARAM_NAME, peakDetector );
	}

	const char* peakFilter = m_settings->getParameterValue( PEAK_FILTER_PARAM_NAME );
	m_peakFilter = NVL( peakFilter, "" );

	const char* valueType = m_settings->getParameterValue( VALUE_TYPE_PARAM_NAME );
	if( valueType != NULL ) {
		if( strcmp( valueType, "area" ) == 0 ) {
			m_type = VALUE_AREA;
		}
	}

	const char* rtRange = m_settings->getParameterValue( RT_RANGE_PARAM_NAME );
	if( rtRange != NULL ) {
		const unsigned int len = strlen( rtRange );
		char* buff = new char[ len + 1 ];
		sprintf( buff, "%s", rtRange );

		char* start = buff;
		char* end = buff;

		char* sep = strstr( buff, ":" );
		if( sep != NULL ) {
			end = sep + 1;
			*sep = '\0';
		}

		m_startRt = todouble( start, -1.0 );
		m_endRt = todouble( end, -1.0 );

		delete[] buff;
	}

	const char* unit = m_settings->getParameterValue( UNIT_PARAM_NAME );
	if( unit != NULL ) {
		if( strcmp( unit, "ppm" ) == 0 ) {
			m_unit = UNIT_PPM;
		}
	}

	const char* tol = m_settings->getParameterValue( MASS_TOL_PARAM_NAME );
	if( tol != NULL ) {
		m_massTol = std::max( todouble( tol, m_massTol ), 0.001 );
	}
}

// detect peaks
int MergedSpectrumPeaks::detectPeaks( kome::objects::Sample* sample, kome::objects::Peaks& peaks ) {
	// return value
	int profileId = -1;

	// spectra
	std::vector< kome::objects::Spectrum* > spectra;

	for( unsigned int i = 0; i < sample->getNumberOfGroups(); i++ ) {    // each groups
		kome::objects::DataGroupNode* group = sample->getGroup( i );

		for( unsigned int j = 0; j < group->getNumberOfSpectra(); j++ ) {
			kome::objects::Spectrum* tmp = group->getSpectrum( j );

			if( tmp->getMsStage() == 1
					&& ( ( m_startRt < 0.0 && m_endRt < 0.0 )
						|| ( tmp->hasChromatogram() && tmp->getRt() >= 0.0 
							&& ( m_startRt < 0.0 || tmp->getRt() >= m_startRt )
							&& ( m_endRt < 0.0 || tmp->getRt() <= m_endRt ) ) ) ) {
				spectra.push_back( tmp );
			}
		}
	}

	// spectrum
	kome::objects::Spectrum* spec = NULL;
	kome::objects::AveragedSpectrum* avgSpec = NULL;
	if( spectra.size() == 0 ) {
		return profileId;
	}
	else if( spectra.size() == 1 ) {
		spec = spectra.front();
	}
	else {
		avgSpec = new kome::objects::AveragedSpectrum( NULL );
		for( unsigned int i = 0; i < spectra.size(); i++ ) {
			avgSpec->addSpectrum( spectra[ i ] );
		}
	}

	kome::core::DataPoints dps;
	spec->getXYData( &dps, false );
	peaks.setDataPoints( dps );

	// peak detection
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setSpectrum( params, *spec );
	kome::plugin::PluginCallTool::setXYData( params, dps );
	kome::plugin::PluginCallTool::setPeaks( params, peaks );

	if( m_peakSettings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params, *m_peakSettings );
	}

	m_peakDetector->getCall()->invoke( &params );

	// filter
	if( !m_peakFilter.empty() ) {
		kome::objects::Spectrum* specArr = spec;
		kome::objects::Peaks* peakArr = &peaks;
		kome::plugin::PluginCallTool::filterSpecPeaks( &specArr, &peakArr, 1, m_peakFilter.c_str() );
	}

	// profile ID
	int sampleId = m_getter->getMatrixSampleId();
	if( sampleId >= 0 ) {
		kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
		profileId = identMgr.setProfile( dps, false );
	}

	// delete
	if( avgSpec != NULL ) {
		delete avgSpec;
	}

	return profileId;
}
