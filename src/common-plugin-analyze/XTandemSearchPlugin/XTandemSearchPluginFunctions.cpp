/**
 * @file XTandemSearchPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author N.Tashiro
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "XTandemSearchPluginFunctions.h"

#include "XTandemManager.h"

using namespace kome::xtandem;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// initialise the search engine, i.e. create an instance
kome::objects::Variant initialiseSearchEngine( kome::objects::Parameters* params ) {
	// manager
	XTandemManager& mgr = XTandemManager::getInstance();

	// return value
	kome::objects::Variant ret;
	ret.prim.pt = mgr.getEngine();
	ret.type = kome::objects::Variant::OTHER;
	
	return ret;
}

// check modifications
kome::objects::Variant checkModifications( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::STRING;
	ret.setString( "" );

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		ret.setString( "Failed to get the parameters." );
		return ret;
	}

	// modifications
	std::string msg = TandemSearchEngine::checkParams( settings );
	ret.setString( msg.c_str() );

	return ret;
}

// get residues
kome::objects::Variant getResiduesFileList( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// vector
	kome::objects::Variant var = params->getValue( "enumeration" );
	std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration
		= (std::vector< kome::plugin::SettingsValue::EnumItem >*)var.prim.pt;
	if( enumeration == NULL ) {
		LOG_ERROR( FMT( "Failed to get the enumeration array." ) );
		return ret;
	}

	// manager
	XTandemManager& mgr = XTandemManager::getInstance();

	// residues
	mgr.getResiduesFiles( enumeration );

	return ret;
}
