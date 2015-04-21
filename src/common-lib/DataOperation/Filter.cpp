/**
 * @file Filter.cpp
 * @brief implements of Filter class
 *
 * @author S.Tanaka
 * @date 2007.08.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Filter.h"

using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Filter::Filter() {
	m_fun = NULL;
	m_settings.clear();

	setName( "" );
}

// destructor
Filter::~Filter() {
	init();
}

// initialze
void Filter::init() {
	m_fun = NULL;
	m_settings.clear();

	setName( "" );
}

// set filter function
void Filter::setFilterInfo( kome::plugin::PluginCall* func, kome::objects::SettingParameterValues* settings ) {
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

	// item
	kome::plugin::PluginFunctionItem item;
	item.setInfo( func );

	// set name
	std::string name = FMT( "Filter [%s", item.getLongName() );
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

// update data points
void Filter::getUpdatedData(
		kome::core::XYData& src,
		kome::core::XYData& dst
) {
	// check the member
	if( m_fun == NULL ) {
		LOG_WARN( FMT( "The filter function is not assigned." ) );
		return;
	}

	// check the parameter
	if( src.getLength() == 0 ) {
		return;
	}

	// parameters
	kome::objects::Parameters params;

	kome::plugin::PluginCallTool::setXYData( params, src );
	kome::plugin::PluginCallTool::setUpdatedXYData( params, dst );
	kome::plugin::PluginCallTool::setSettingValues( params, m_settings );

	try {
		m_fun->invoke( &params );
	}
	catch( ... ) {
	}
}

// on update (chromatogram)
void Filter::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Chromatogram& chrom
) {
	getUpdatedData( src, dst );
}

// on update (spectrum)
void Filter::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Spectrum& spec
) {
	getUpdatedData( src, dst );
}

// on update (sample)
void Filter::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Sample& sample
) {
	getUpdatedData( src, dst );
}

//---------------------------------------------------------------
// MzRangeFilter class
//---------------------------------------------------------------
// constructor
MzRangeFilter::MzRangeFilter( double startMz, double endMz ){
	m_startMz = startMz;
	m_endMz = endMz;
}

// destructor
MzRangeFilter::~MzRangeFilter(){
}

// on update (chromatogram)
void MzRangeFilter::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Chromatogram& chrom
) {
}

// on update (spectrum)
void MzRangeFilter::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Spectrum& spec
) {
	
	// check the parameter
	if( src.getLength() == 0 ) {
		return;
	}

	dst.clearPoints();

	for( unsigned int i=0; i < src.getLength(); i++ ){
		
		if( m_startMz < src.getX(i) && src.getX(i) <= m_endMz ){
			
			dst.addPoint( src.getX(i), src.getY(i) );
		}
	}

}

// on update (sample)
void MzRangeFilter::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Sample& sample
) {
}
