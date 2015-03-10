/**
 * @file Sample3DCanvasOperation.cpp
 * @brief Sample 3D Canvas Operation class
 *
 * @author M.Izumi
 * @date 2013.07.08
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "Sample3DCanvasOperation.h"
#include "Sample3DViewManager.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
Sample3DCanvasOperation::Sample3DCanvasOperation(){
	m_dataMap = NULL;
	m_parent = NULL;
	m_canvas = NULL;
}

// destructor
Sample3DCanvasOperation::~Sample3DCanvasOperation(){
}

// execute
bool Sample3DCanvasOperation::onExecute(){
	
	// get canvas
	m_canvas = Sample3DCanvas::getCanvas( m_dataMap );
	if( m_canvas == NULL ) {
		m_canvas = new Sample3DCanvas( m_parent, *m_dataMap );
	}else{
		return false;
	}
	return true;
}

// load condition
void Sample3DCanvasOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	
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
void Sample3DCanvasOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun  ){
	
}

// get description
std::string Sample3DCanvasOperation::onGetDescription(){
	std::string s;
	
	kome::objects::DataGroupNode* group = ( m_dataMap == NULL ? NULL : &m_dataMap->getGroup() );
	s.append( "Open 3D Canvas: " );
	s.append( ( group == NULL ? "" : group->getName() ) );
	return s;
}

// get parameters string
std::string Sample3DCanvasOperation::onGetParametersString(){
	std::string s;
	s.append( FMT( "%d,", getTargetSample()->getSampleId() ) );
	s.append( FMT( "%d", m_dataMap->getGroup().getId() ) );
	
	return s;
}

// set parameters string
void Sample3DCanvasOperation::onSetParametersString( const char* strParam ){
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
	
	kome::objects::DataSet* dataSet = group;
	Sample3DViewManager& mgr = Sample3DViewManager::getInstance();
	mgr.open3D( group, dataSet );
	
	kome::objects::DataMapManager& dataMapMgr = kome::objects::DataMapManager::getInstance();
	
	std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > dataMapLocal = dataMapMgr.getDataMap();
	if( dataMapLocal.find( group ) == dataMapLocal.end() ) {	// create
	} else {
		m_dataMap = dataMapLocal[ group ];
	}
}

// get canvas
Sample3DCanvas* Sample3DCanvasOperation::getCanvas(){
	return m_canvas;
}
