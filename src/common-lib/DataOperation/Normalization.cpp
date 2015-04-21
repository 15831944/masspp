/**
 * @file Normalization.cpp
 * @brief Normalization class
 *
 * @author M.Izumi
 * @date 2012.07.20
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "Normalization.h"

using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
Normalization::Normalization(){
	m_fun = NULL;
	m_settings.clear();

	m_stdGroup = NULL;
	m_trmtGroup = NULL;

	setName( "" );
}

// destructor
Normalization::~Normalization(){
	init();
}

// initialze
void Normalization::init(){
	m_fun = NULL;
	m_settings.clear();

	setName( "" );
}

// set normalization function
void Normalization::setNormalizInfo( 
	kome::plugin::PluginCall* func,
	kome::objects::SettingParameterValues* settings,
	kome::objects::DataGroupNode* stdGroup 
){
	// initialize
	init();
	if( func == NULL ){
		return;
	}
	
	if( stdGroup != NULL ){
		m_stdGroup = stdGroup;
	}

	// set information
	m_fun = func;
	if( settings != NULL ){
		m_settings = *settings;
	}

	// item
	kome::plugin::PluginFunctionItem item;
	item.setInfo( func );

	// set name
	std::string name = FMT( "Normalization [%s", item.getLongName() );
	std::string paramStr;
	for( unsigned int i=0; i < m_settings.getNumberOfParameters(); i++ ){
		// add parameter name
		if( !paramStr.empty() ){
			paramStr.append( ", " );
		}

		std::string name = m_settings.getParameterName( i );
		std::string value = m_settings.getParameterValue( i );

		paramStr.append( FMT( "%s=%s", name.c_str(), value.c_str() ) );
	}

	if( paramStr.empty() ){
		name.append( "]" );
	}else{
		name.append( FMT( "(%s)]", paramStr.c_str() ) );
	}

	setName( name.c_str() );
}

// update data point
void Normalization::getUpdatedData(
	kome::core::XYData& src,
	kome::core::XYData& dst,
	kome::objects::Spectrum* spec
){
	// check the member
	if( m_fun == NULL ){
		LOG_WARN_CODE( FMT( "The filter function is not assigned." ), ERR_NULL_POINTER );
		return;
	}

	// parameters
	kome::objects::Parameters params;

	kome::plugin::PluginCallTool::setXYData( params, src );
	kome::plugin::PluginCallTool::setUpdatedXYData( params, dst );
	kome::plugin::PluginCallTool::setSettingValues( params, m_settings );
	kome::plugin::PluginCallTool::setSpectrum( params, *spec );
	
	kome::plugin::PluginCallTool::setControlGroup( params, *m_stdGroup );
	kome::plugin::PluginCallTool::setGroup( params, *m_trmtGroup );
	
	// normalization methods
	m_fun->invoke( &params );	
}

//on update (chromatogram)
void Normalization::onUpdate(
	kome::core::XYData& src,
	kome::core::XYData& dst,
	kome::objects::Chromatogram& chrom
){
	m_trmtGroup = chrom.getOrgChromatogram()->getGroup(); // @date 2014.03.06 <Mod> M.Izumi
	getUpdatedData( src, dst, NULL );
}

// on update (spectrum)
void Normalization::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Spectrum& spec
){
	m_trmtGroup = spec.getOrgSpectrum()->getGroup();	// @date 2014.03.06 <Mod> M.Izumi
	getUpdatedData( src, dst, &spec );
}

// on update (sample)
void Normalization::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst, 
		kome::objects::Sample& sample
) {
	m_trmtGroup = sample.getRootDataGroupNode();
	getUpdatedData( src, dst, NULL );
}
