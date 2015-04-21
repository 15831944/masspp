/**
 * @file BaselineSubtract.cpp
 * @brief implements of BaselineSubtract class
 *
 * @author S.Tanaka
 * @date 2007.08.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BaselineSubtract.h"


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
BaselineSubtract::BaselineSubtract() {
	m_fun = NULL;
	m_settings.clear();

	setName( "" );
}

// destructor
BaselineSubtract::~BaselineSubtract() {
	init();
}

// initialze
void BaselineSubtract::init() {
	m_fun = NULL;
	m_settings.clear();

	setName( "" );
}

// set baseline function
void BaselineSubtract::setBaselineInfo( kome::plugin::PluginCall* func, kome::objects::SettingParameterValues* settings ) {
	// initialize
	init();
	if( func == NULL ) {
		return;
	}

	// set information
	m_fun = func;
	if( settings != NULL ) {
		m_settings = *settings;
	}

	// function item
	kome::plugin::PluginFunctionItem item;
	item.setInfo( m_fun );

	// set name
	std::string name = FMT( "Baseline Subtract[%s", item.getLongName() );
	std::string paramStr;
	for( unsigned int i = 0; i < m_settings.getNumberOfParameters(); i++ ) {
		// add parameter name
		if( !paramStr.empty() ) {
			paramStr.append( ", " );
		}

		std::string name = m_settings.getParameterName( i );
		std::string value = m_settings.getParameterValue( i );

		paramStr.append( FMT( "%s=%s", name.c_str(), value.c_str() ) );
	}

	if( paramStr.empty() ) {
		name.append( "]" );
	}
	else {
		name.append( FMT( "(%s)]", paramStr.c_str() ) );
	}

	setName( name.c_str() );
}

// subtract baseline
void BaselineSubtract::subtractBaseline(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		kome::core::XYData& baseline
) {

	// linear interporeration
	unsigned int len = src.getLength();
	double* xArray = ( len == 0 ? NULL : new double[ len ] );
	for( unsigned int i = 0; i < src.getLength(); i++ ) {
		xArray[ i ] = src.getX( i );
	}

	kome::core::DataPoints baseline2;
	kome::numeric::Interpolation::linear( baseline, baseline2, xArray, src.getLength() );

	// delete array
	if( xArray != NULL ) {
		delete[] xArray;
		xArray = NULL;
	}

	// subtract
	dst.reserve( len );
	for( unsigned int i = 0; i < len; i++ ) {
		double x = src.getX( i );
		double y = src.getY( i ) - baseline2.getY( i );
		y = MAX( y, 0.0 );

		dst.addPoint( x, y );
	}
}

// update data points
void BaselineSubtract::getUpdatedData(
		kome::core::XYData& src,
		kome::core::XYData& dst
) {
	// check the member
	if( m_fun == NULL ) {
		LOG_WARN( FMT( "The baseline function is not assigned." ) );
		return;
	}

	// check the parameter
	if( src.getLength() == 0 ) {
		return;
	}

	// baseline points
	kome::core::DataPoints baseline;

	// parameters
	kome::objects::Parameters params;

	kome::plugin::PluginCallTool::setXYData( params, src );
	kome::plugin::PluginCallTool::setBaseline( params, baseline );
	kome::plugin::PluginCallTool::setSettingValues( params, m_settings );

	m_fun->invoke( &params );

	// subtract baseline
	subtractBaseline( src, dst, baseline );

}

// on update (chromatogram)
void BaselineSubtract::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Chromatogram& chrom
) {
	getUpdatedData( src, dst );
}

// on update (spectrum)
void BaselineSubtract::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Spectrum& spec
) {
	getUpdatedData( src, dst );
}

// on update (sample)
void BaselineSubtract::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Sample& sample
) {
	getUpdatedData( src, dst );
}
