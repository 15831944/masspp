/**
 * @file DataMapFilterPluginFunction.cpp
 * @brief interfaces of Data Map Filter plug-in functions
 *
 * @author M.Izumi
 * @date 2012.09.06
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "DataMapFilterPluginFunction.h"
#include "DataMapFilterPluginManager.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// update settings
kome::objects::Variant updateSettings( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// get ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return ret;
	}

	// get ini file
	std::string filterName = ini->getString( "Data Map Filter", "FILTER", "ms" );

	// update data map
	DataMapFilterManager& mgr = DataMapFilterManager::getInstance();
	if( filterName.compare( mgr.getFilterName() ) != 0 ) {
		mgr.setFilterName( filterName.c_str() );
		mgr.updateDataMap();
	}

	// refresh
	kome::window::WindowTool::refresh();	

	return ret;
}

