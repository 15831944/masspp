/**
 * @file Sample3DViewFunctions.cpp
 * @brief implements of Sample3DView plug-in functions
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Sample3DViewFunctions.h"
#include "Sample3DCanvas.h"
#include "Sample3DViewManager.h"
#include "Sample3DCanvasOperation.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define STATUS_KEY					"DATA_MAP_WINDOW"
#define STATUS_VALUE				"3D"


// creates 3D view canvas
kome::objects::Variant create3DCanvas( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::WINDOW;
	ret.prim.pt = NULL;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	// parent
	wxWindow* parent = kome::plugin::PluginCallTool::getParentWindow( params );
	if( parent == NULL ) {
		return ret;
	}
	// add operation
	Sample3DCanvasOperation* opt = new Sample3DCanvasOperation();
	opt->setShortName( "3d_canvas" );
	opt->setOperationName( "3D Canvas" );
	opt->setOperationType( kome::operation::Operation::TYPE_GUI );
	opt->setTargetSample( dataMap->getSample() );
	opt->setDataMapInfo( dataMap );
	opt->setParentWindow( parent );

	if( opt->execute() ){
		ret.prim.pt = opt->getCanvas();
	}else{
		delete opt;
	}
	
	return ret;
}

// updates graph information
kome::objects::Variant updateGraphInfo( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	// canvas
	Sample3DCanvas* canvas = Sample3DCanvas::getCanvas( dataMap );

	// update
	if( canvas != NULL ) {
		canvas->getGraphInfo().update();
		canvas->Refresh();
	}
	
	return ret;
}

// updates settings
kome::objects::Variant updateSettings( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// sync view
	kome::gl::GLDataMapInfo::setSyncView();

	// axis drawing
	kome::gl::GLDataMapInfo::setAxisDrawing();

	// update
	unsigned int num = Sample3DCanvas::getNumberOfCanvases();
	for( unsigned int i = 0; i < num; i++ ) {
		// canvas
		Sample3DCanvas* canvas = Sample3DCanvas::getCanvas( i );

		// update
		if( canvas != NULL ) {
			canvas->getGraphInfo().update();
		}
	}

	return ret;
}

// sets data map status
kome::objects::Variant setDataMapStatus( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// set status
	statusMgr.setValue( STATUS_KEY, STATUS_VALUE );

	return ret;
}

// set data map status and open data map info
kome::objects::Variant setDataMapStatusOpen( kome::objects::Parameters* params) {
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;
	
	setDataMapStatus( params );
	openDataMapInfo( params );

	return ret;
}

// open DataMap Info
kome::objects::Variant openDataMapInfo( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	
	// active group 
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Sample* sample = obj.getSample();
	kome::objects::DataGroupNode* group = ( sample == NULL ? NULL : sample->getRootDataGroupNode() );
		
	if( group == NULL ) {
		LOG_ERROR( FMT( "Failed to get the active spectrum group." ) );
		return ret;
	}

	// open
	Sample3DViewManager& mgr = Sample3DViewManager::getInstance();
	mgr.open3D( group, group );

	ret.prim.boolVal = true;
	return ret;
}

// on DataMap mouse move for Change Mouse cursor
kome::objects::Variant onDataMapMouseMoveMouseCursor( kome::objects::Parameters* params) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;
	
	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}


	// @date 2011/06/29 <Add> OKADA ------->
	// マウスカーソルの設定
	{
		// graphics
		kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
		if( g == NULL ) {
			return ret;
		}
	}
	// @date 2011/06/29 <Add> OKADA <-------


	return ret;
}

kome::objects::Variant get3DCanvasOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new Sample3DCanvasOperation();
	
	return ret;
}

// kill timers
kome::objects::Variant killTimers( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// timers
	const unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
	for( unsigned int i = 0; i < num; i++ ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		Sample3DCanvas* canvas = dynamic_cast< Sample3DCanvas* >( frame->getCanvas() );

		if( canvas != NULL ) {
			canvas->killTimer();
		}
	}

	return ret;
}
