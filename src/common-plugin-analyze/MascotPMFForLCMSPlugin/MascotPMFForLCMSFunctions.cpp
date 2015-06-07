/**
 * @file MascotPMFForLCMSFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author s.Tanaka
 * @date 2012.10.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MascotPMFForLCMSFunctions.h" 
#include "MascotPmfManager.h"


using namespace kome::mascot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// get PMF search engine
kome::objects::Variant getPmfSearchEngine( kome::objects::Parameters* params ) {
	// manager
	MascotPmfManager& mgr = MascotPmfManager::getInstance();

	// return value
	kome::objects::Variant ret;
	ret.prim.pt = mgr.getPmfEngine();
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
	std::string msg = MascotSearchEngineBase::checkMods( settings );
	ret.setString( msg.c_str() );

	return ret;
}
