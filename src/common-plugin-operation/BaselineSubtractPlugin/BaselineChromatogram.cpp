/**
 * @file BaselineChromatogram.cpp
 * @brief implements of BaselineChromatogram class
 *
 * @author S.Tanaka
 * @date 2007.07.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BaselineChromatogram.h"


using namespace kome::baseline;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
BaselineChromatogram::BaselineChromatogram(
		kome::objects::Chromatogram& chrom,
		kome::plugin::PluginCall& fun,
		kome::objects::SettingParameterValues* settings
) : kome::objects::Chromatogram( chrom.getSample() ), m_chrom( chrom ) {
	// set item
	m_item.setInfo( &fun );

	// set parameters
	if( settings != NULL ) {
		m_settings = *settings;
	}
}

// destructor
BaselineChromatogram::~BaselineChromatogram() {
}

// get xy data
void BaselineChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// check the member
	if( xyData == NULL ) {
		return;
	}

	// get data point
	kome::core::XYData* src = m_chrom.getXYData();
	if( src == NULL ) {
		LOG_WARN( FMT( "Failed to get chromatogram points." ) );
		return;
	}

	// set parameters
	kome::objects::Parameters params;

	kome::plugin::PluginCallTool::setChromatogram( params, m_chrom );
	kome::plugin::PluginCallTool::setXYData( params, *src );
	kome::plugin::PluginCallTool::setBaseline( params, *xyData );
	kome::plugin::PluginCallTool::setSettingValues( params, m_settings );

	// get baseline
	m_item.getCall()->invoke( &params );
}

// on get spectra
void BaselineChromatogram::onGetSpectra(
		kome::objects::DataSet& dataSet,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
}

// on get mass
double BaselineChromatogram::onGetMass( const unsigned int index ) {
	return -1.0;
}

// on get MS stage
int BaselineChromatogram::onGetMsStage( const unsigned int index ) {
	return -1;
}

// on get precursor
double BaselineChromatogram::onGetPrecursor( const unsigned int index ) {
	return -1.0;
}
