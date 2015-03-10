/**
 * @file Sample3DViewManager.cpp
 * @brief implements of Sample3DViewManager class
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#define SECTION					"Data Map Filter"
#define KEY						"FILTER"
#define DEFAULT					"ms"

#include "stdafx.h"
#include "Sample3DViewManager.h"
#include "Sample3DCanvas.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
Sample3DViewManager::Sample3DViewManager() {
	createGraphTextureImage();
}

// destructor
Sample3DViewManager::~Sample3DViewManager() {
}

// create graph texture image
void Sample3DViewManager::createGraphTextureImage() {
	// weight
	int weights[] = {
		4, 16, 1, 1, 16, 4
	};
	int totalWeight = 0;
	for( int i = 0; i < sizeof( weights ) / sizeof( int ); i++ ) {
		totalWeight += weights[ i ];
	}
	int w = 0;

	// create texture
	w += weights[ 0 ];
	int aquaIdx = w * GRAPH_TEXTURE_IMAGE_SIZE / totalWeight - 1;
	for( int i = 0; i < aquaIdx; i++ ) {
		m_graphTexture[ i ][ 0 ] = 0;
		m_graphTexture[ i ][ 1 ] = 255;
		m_graphTexture[ i ][ 2 ] = 255;
	}

	w += weights[ 1 ];
	int blueIdx = w * GRAPH_TEXTURE_IMAGE_SIZE / totalWeight - 1;
	for( int i = aquaIdx; i <= blueIdx; i++ ) {
		int green = 255 - i * 255 / blueIdx;
		m_graphTexture[ i ][ 0 ] = 0;
		m_graphTexture[ i ][ 1 ] = green;
		m_graphTexture[ i ][ 2 ] = 255;
	}

	w += weights[ 2 ];
	int grayIdx = w * GRAPH_TEXTURE_IMAGE_SIZE / totalWeight - 1;
	for( int i = blueIdx + 1; i <= grayIdx; i++ ) {
		int red = 64 * ( i - blueIdx ) / ( grayIdx - blueIdx );
		int green = red;
		int blue = 255 - 191 * ( i - blueIdx ) / ( grayIdx - blueIdx );
		m_graphTexture[ i ][ 0 ] = red;
		m_graphTexture[ i ][ 1 ] = green;
		m_graphTexture[ i ][ 2 ] = blue;
	}

	w += weights[ 3 ];
	int redIdx = w * GRAPH_TEXTURE_IMAGE_SIZE / totalWeight - 1;
	for( int i = grayIdx + 1; i <= redIdx; i++ ) {
		int red = 64 + 191 * ( i - grayIdx ) / ( redIdx - grayIdx );
		int green = 64 - 64 * ( i - grayIdx ) / ( redIdx - grayIdx );
		int blue = green;
		m_graphTexture[ i ][ 0 ] = red;
		m_graphTexture[ i ][ 1 ] = green;
		m_graphTexture[ i ][ 2 ] = blue;
	}

	w += weights[ 4 ];
	int yellowIdx = w * GRAPH_TEXTURE_IMAGE_SIZE / totalWeight - 1;
	for( int i = redIdx + 1; i <= yellowIdx; i++ ) {
		int green = 255 * ( i - redIdx ) / ( yellowIdx - redIdx );
		m_graphTexture[ i ][ 0 ] = 255;
		m_graphTexture[ i ][ 1 ] = green;
		m_graphTexture[ i ][ 2 ] = 0;
	}

	w += weights[ 5 ];
	int yellowIdx2 = w * GRAPH_TEXTURE_IMAGE_SIZE / totalWeight - 1;
	for( int i = yellowIdx + 1; i <= yellowIdx2; i++ ) {
		m_graphTexture[ i ][ 0 ] = 255;
		m_graphTexture[ i ][ 1 ] = 255;
		m_graphTexture[ i ][ 2 ] = 0;
	}
}

// get graph texture image
void* Sample3DViewManager::getGraphTextureImage() {
	return m_graphTexture;
}

// get graph texture image size
unsigned int Sample3DViewManager::getGraphTextureImageSize() {
	return GRAPH_TEXTURE_IMAGE_SIZE;
}

// get instance
Sample3DViewManager& Sample3DViewManager::getInstance() {
	// create object (This is the only object)
	static Sample3DViewManager mgr;

	return mgr;
}


// open data map
void Sample3DViewManager::open3D( kome::objects::DataGroupNode* group, kome::objects::DataSet* dataSet ) {
	// check the parameter
	if( group == NULL ) {
		return;
	}

	kome::objects::DataMapManager& mgr = kome::objects::DataMapManager::getInstance();

	std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > dataMapLocal = mgr.getDataMap();

	// data map
	kome::objects::DataMapInfo* dataMap = NULL;
	if( dataMapLocal.find( group ) == dataMapLocal.end() ) {	// create
		// create data map
		dataMap = new kome::objects::DataMapInfo( *group, MAP_ROW_SIZE, MAP_COL_SIZE );

		// on open
		if( !kome::plugin::PluginCallTool::onOpenDataMap( *dataMap ) ) {
			delete dataMap;
			return;
		}

		// create data map
		if( !dataMap->isCompleted() ) {
			// progress
			kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Getting Data" );

			//  @date 2012.09.07 <Mod> M.Izumi ->
			// get ini file
			kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
			kome::core::IniFile* ini = msppMgr.getIniFile();

			std::string filterName = ini->getString( SECTION, KEY, DEFAULT );

			// create
			dataMap->setRange( -1.0, -1.0, -1.0, -1.0, filterName.c_str(), progress );
			//  @date 2012.09.07 <Mod> M.Izumi <-
			if( progress.isStopped() ) {
				delete dataMap;
				return;
			}
		}

		// set to map
		dataMapLocal[ group ] = dataMap;
		mgr.setDataMap2( dataMapLocal );
	}
	else {
		dataMap = dataMapLocal[ group ];
	}

	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// open
	wxBeginBusyCursor();

	const char * pszName = ( *dataMap ).getGroup().getSample()->getName();
	std::string strOrgName = pszName;

	std::string strWindowName = "3D Display - " + strOrgName;

	( *dataMap ).getGroup().getSample()->setName( strWindowName.c_str() );	// 2011/06/15 APPEND OKADA DEBUG

	kome::plugin::PluginCallTool::openDataMap( *dataMap );

	( *dataMap ).getGroup().getSample()->setName( strOrgName.c_str() );	// 2011/06/15 APPEND OKADA DEBUG

	wxEndBusyCursor();

	// refresh
	kome::window::WindowTool::refresh();
}
