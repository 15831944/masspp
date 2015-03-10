/**
 * @file NeighborPeaks.cpp
 * @brief implements of NeighborPeaks class
 *
 * @author S.Tanaka
 * @date 2008.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "NeighborPeaks.h"


using namespace kome::search::neighbor;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define FILTER_PARAM_NAME	    "filter"
#define RT_RANGE_PARAM_NAME	    "rt_range"
#define VALUE_PARAM_NAME		"val_type"
#define SELECTION_PARAM_NAME	"sel_method"
#define RT_TOL_PARAM_NAME       "rt_tol"
#define MZ_TOL_PARAM_NAME       "mz_tol"

#define ALIGN_FUNC_TYPE		    "ALIGNMENT"



// constructor
NeighborPeaks::NeighborPeaks(
		kome::objects::SettingParameterValues* settings
) {
	// initialize
	m_settings = settings;

	m_filter = 0xff;
	m_peakDetector = NULL;
	m_peakSettings = NULL;
	m_peakValue = VALUE_INTENSITY;
	m_peakSel = SEL_NEAREST;
	m_rtTol = - 1.0;
	m_mzTol = -1.0;

	// set parameters
	setParameters();
}

// destructor
NeighborPeaks::~NeighborPeaks() {
}

// set parameters
void NeighborPeaks::setParameters() {
	// check the member
	if( m_settings == NULL ) {
		return;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// filter
	const char* filter = m_settings->getParameterValue( FILTER_PARAM_NAME );
	if( filter != NULL ) {
		if( strcmp( filter, "ms" ) == 0 ) {
			m_filter = 0x01;
		}
		else if( strcmp( filter, "ms2" ) == 0 ) {
			m_filter = 0xfe;
		}
	}

	// peak value type
	const char* valType = m_settings->getParameterValue( VALUE_PARAM_NAME );
	if( valType != NULL && strcmp( valType, "area" ) == 0 ) {
		m_peakValue = VALUE_AREA;
	}

	// selection
	const char* selection = m_settings->getParameterValue( SELECTION_PARAM_NAME );
	m_peakSel = getPeakSelection( selection );

	// tolerance
	m_rtTol = std::max( m_settings->getDoubleValue( RT_TOL_PARAM_NAME, 0.5 ), 0.01 );
	m_mzTol = std::max( m_settings->getDoubleValue( MZ_TOL_PARAM_NAME, 0.25 ), 0.001 );
}

// get spectra
void NeighborPeaks::getSpectra( kome::objects::Sample* sample, kome::objects::DataSet& spectra, const bool hasChrom ) {
	// each groups
	for( unsigned int i = 0; i < sample->getNumberOfGroups(); i++ ) {
		kome::objects::DataGroupNode* group = sample->getGroup( i );

		// each spectra
		for( unsigned int j = 0; j < group->getNumberOfSpectra(); j++ ) {
			kome::objects::Spectrum* spec = group->getSpectrum( j );

			if( spec->getMsStage() == 1
					&& ( spec->hasChromatogram() || !hasChrom ) ) {
				spectra.addSpectrum( spec );
			}
		}
	}

	// sort
	spectra.sortSpectra();
}

// get peak selection type
NeighborPeaks::PeakSelection NeighborPeaks::getPeakSelection( const char* s ) {
	if( compareignorecase( s, "nearest" ) == 0 ) {
		return SEL_NEAREST;
	}
	if( compareignorecase( s, "score" ) == 0 ) {
		return SEL_SCORE;
	}
	if( compareignorecase( s, "score2" ) == 0 ) {
		return SEL_SCORE2;
	}
	if( compareignorecase( s, "sum" ) == 0 ) {
		return SEL_SUM;
	}
	if( compareignorecase( s, "avg" ) == 0 ) {
		return SEL_AVERAGE;
	}

	return SEL_MAX;
}

// compare RT
bool NeighborPeaks::lessRt( PeakInfo& peak0, PeakInfo& peak1 ) {
	return ( peak0.rt < peak1.rt );
}

// compare m/z
bool NeighborPeaks::lessMz( PeakInfo& peak0, PeakInfo& peak1 ) {
	return ( peak0.mz < peak1.mz );
}
