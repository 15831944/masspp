/**
 * @file ScriptPluginCall.cpp
 * @brief interfaces of ScriptPluginCall class
 *
 * @author M.Izumi
 * @date 2013.08.21 
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ScriptPluginCall.h"
#include "ScriptPluginManager.h"

using namespace kome::script;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SCRIPT_TYPE_NAME			 "SCRIPT"

// constructor
ScriptPluginCall::ScriptPluginCall( kome::plugin::PluginInfo* plugin ) :
	kome::plugin::PluginCall( plugin ){		
}

// destructor
ScriptPluginCall::~ScriptPluginCall(){
}

// invoke
kome::objects::Variant ScriptPluginCall::invoke( kome::objects::Parameters* params ){
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	std::string strShortName = getProperties().getStringValue( "short name", "" );

	std::string strType = getType();

	int find_first = strShortName.find( "<" )+1;
	int find_end = strShortName.find( ">" ) - find_first;
	strShortName = strShortName.substr( find_first, find_end );
	
	// manager
	ScriptPluginManager& mgr = ScriptPluginManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	
	std::string strItemName = getProperties().getStringValue( "item", "" );
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( SCRIPT_TYPE_NAME, strItemName.c_str() );
	if( item == NULL ){
		return ret;
	}
	
	std::string strPath = getDescription();
	if( strPath.empty() ){
		return ret;
	}
	std::string scVal = mgr.getScriptVal( strPath.c_str() );

	// script
	kome::objects::Script* sc = mgr.getScript( item );
	if( sc == NULL ) {
		kome::window::WindowTool::showError( "Script Console cannot be run because the script language engine cannot be found.\nDownload and install the script language." );
		return ret;
	}
		
	std::string result = mgr.scriptRun( sc, scVal.c_str(), params );
	LOG_DEBUG( result );

	return ret;
}
