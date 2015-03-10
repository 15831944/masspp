/**
 * @file DataMapFilterPluginManager.cpp
 * @brief interfaces of Data Map Filter Manager plug-in functions
 *
 * @author M.Izumi
 * @date 2012.09.06
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "DataMapFilterPluginManager.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
DataMapFilterManager::DataMapFilterManager(){
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini != NULL ) {
		m_name = ini->getString( "Data Map Filter", "FILTER", "ms" );
	}
}

// destructor
DataMapFilterManager::~DataMapFilterManager(){
}

// update datamap
void DataMapFilterManager::updateDataMap(){
	
	unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
	for( unsigned int i = 0; i < num; i++ ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
		if( frame != NULL ){
			// data map
			kome::objects::MsDataVariant obj( frame->getActiveObject() );			
			kome::objects::DataMapInfo* dm = obj.getDataMap();

			if( dm != NULL ){
				kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Getting Data" );
	
				double minMz = dm->getMinMz();
				double maxMz = dm->getMaxMz();
				double minRt = dm->getMinRt();
				double maxRt = dm->getMaxRt();

				dm->setRange( minMz, maxMz, minRt, maxRt, m_name.c_str(), progress );

				// update 
				kome::plugin::PluginCallTool::onUpdateDataMap( *dm );
			}
		}
	}
}

// get filter name
const char* DataMapFilterManager::getFilterName(){
	return m_name.c_str();
}

void DataMapFilterManager::setFilterName( const char* name ){
	m_name = name;
}

// get instance
DataMapFilterManager& DataMapFilterManager::getInstance() {
	// create object (This is the only object)
	static DataMapFilterManager mgr;

	return mgr;
} 
