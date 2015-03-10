/**
 * @file DataMapCanvasOperation.cpp
 * @brief DataMap Canvas Operation class
 *
 * @author M.Izumi
 * @date 2013.07.08
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "DataMapCanvasOperation.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define SECTION						"Data Map Filter"
#define KEY							"FILTER"
#define DEFAULT						"ms"

// constructor
DataMapCanvasOperation::DataMapCanvasOperation(){
	m_canvas = NULL;
	m_dataMap = NULL;
	m_parent = NULL;
}

// destructor 
DataMapCanvasOperation::~DataMapCanvasOperation(){
}

// execute
bool DataMapCanvasOperation::onExecute(){
	// create canvas
	m_canvas = DataMapCanvas::getCanvas( m_dataMap );
	if( m_canvas == NULL ) {
		m_canvas = new DataMapCanvas( m_parent, *m_dataMap );
	}else{
		return false;
	}
		
	return true;
}

// load condition
void DataMapCanvasOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	if( m_parent == NULL ){
		return;
	}
	if( m_parent->Hide() ){
		m_parent->Show( false );
	}else{
		m_parent->Show( true );	
	}
}

// save condition
void DataMapCanvasOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun  ){
}

// get description
std::string DataMapCanvasOperation::onGetDescription(){
	std::string s;
	
	kome::objects::DataGroupNode* group = &m_dataMap->getGroup();
	s.append( "Opean HeatMap Canvas: " );
	s.append( ( group == NULL ? "" : group->getName() ) );
	
	return s;
}

// get parameters string
std::string DataMapCanvasOperation::onGetParametersString(){
	std::string s;

	// sample id, group id, parent width, parent height
	s.append( FMT( "%d,", getTargetSample()->getSampleId() ) );
	s.append( FMT( "%d,", m_dataMap->getGroup().getId() ) );
	//s.append( FMT( "%s", m_path.c_str() ) );
		
	return s;
}

// set parameters string
void DataMapCanvasOperation::onSetParametersString( const char* strParam ){
	std::vector< std::string > tokens;
	stringtoken( strParam, "," , tokens );

	if( tokens.size() < 2 ){
		return;
	}
		
	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	kome::objects::DataGroupNode* group = sample->getGroupById( toint( tokens[ 1 ].c_str(), 10, -1 ) );
	if( m_parent == NULL ){
		m_parent = new kome::window::ChildFrame( -1, -1, -1, -1, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxTHICK_FRAME | wxSYSTEM_MENU | wxCAPTION );
	}
	// manager
	kome::objects::DataMapManager& mgr = kome::objects::DataMapManager::getInstance();

	std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > dataMapLocal = mgr.getDataMap();

	// data map	
	if( dataMapLocal.find( group ) == dataMapLocal.end() ) {	// create
		// create data map
		m_dataMap = new kome::objects::DataMapInfo( *group, MAP_ROW_SIZE, MAP_COL_SIZE );

		// on open
		if( !kome::plugin::PluginCallTool::onOpenDataMap( *m_dataMap ) ) {
			delete m_dataMap;
			m_dataMap = NULL;
			//return ret;
		}

		// create data map
		if( !m_dataMap->isCompleted() ) {
			// progress
			kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Getting Data" );
			// @date 2012.09.07 <Mod> M.Izumi ->
			// get ini file 
			kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
			kome::core::IniFile* ini = msppMgr.getIniFile();

			std::string filterName = ini->getString( SECTION, KEY, DEFAULT );

			// create
			m_dataMap->setRange( -1.0, -1.0, -1.0, -1.0, filterName.c_str(), progress );
			// @date 2012.09.07 <Mod> M.Izumi <-
			if( progress.isStopped() ) {
				delete m_dataMap;
				m_dataMap = NULL;
				//return ret;
			}
		}

		// set to map
		dataMapLocal[ group ] = m_dataMap;
		mgr.setDataMap2( dataMapLocal );
	}
	else {
		m_dataMap = dataMapLocal[ group ];
	}
	
	const char * pszName = m_dataMap->getGroup().getSample()->getName();
	std::string strOrgName = pszName;

	std::string strWindowName = "Heatmap - " + strOrgName;

	m_dataMap->getGroup().getSample()->setName( strWindowName.c_str() );

	m_dataMap->getGroup().getSample()->setName( strOrgName.c_str() );
	
	onExecute();

	// @date 2013.09.03 <Add> M.Izumi ->
	if( m_parent != NULL ){
		kome::window::ChildFrame* frame = dynamic_cast<kome::window::ChildFrame*>(m_parent);
		if( frame->getNumberOfChromatograms() == 0 && 
			frame->getNumberOfSpectra() == 0 &&
			frame->getNumberOfDataMaps() == 0 ){
				delete m_parent;
				m_parent = NULL;
		}
	}
	// @date 2013.09.03 <Add> M.Izumi <-
}

// get canvas
DataMapCanvas* DataMapCanvasOperation::getCanvas(){
	return m_canvas;
}
