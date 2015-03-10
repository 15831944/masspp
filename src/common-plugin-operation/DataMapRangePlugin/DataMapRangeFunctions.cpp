/**
 * @file DataMapRangeFunctions.cpp
 * @brief implements of DataMapRange plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 *
 * <補足説明>
 * ・Version 2.0.0での変更点　2011.05.19 <Mod> M.Izumi
   View > Operation > DataMap Range をメニューバーから削除
   statusに関係なく範囲選択処理を行なうように変更

    ※ 変更箇所が多いため変更箇所は削除しています。
   　 必要そうな箇所には、コメントを残しています。
 */


#include "stdafx.h"
#include "DataMapRangeFunctions.h"
#include "DataMapRangeManager.h"
#include "DataMapRangeOperation.h"

using namespace kome::operation::range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// create data map
kome::objects::Variant createDataMap( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get data map
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	// prepare
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
	ret.prim.boolVal = mgr.onOpen( *dataMap );

	return ret;
}

// prepare data map
kome::objects::Variant prepareDataMap( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// prepare
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
	mgr.prepare( *dataMap, *graphics, canvas );

	return ret;
}

// on close data map
kome::objects::Variant closeDataMapRange( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// prepare
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
	mgr.close( dataMap );

	return ret;
}

// on button down (data map)
kome::objects::Variant onDataMapInfoButtonDown( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// on button down
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
	ret.prim.boolVal = mgr.downButton( *dataMap, *mouseEvent, *graphics, canvas );

	return ret;
}

// on mouse move (data map)
kome::objects::Variant onDataMapInfoMouseMove( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// on button down
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
	ret.prim.boolVal = mgr.moveMouse( *dataMap, *mouseEvent, *graphics, canvas );

	return ret;
}

// on button up (data map)
kome::objects::Variant onDataMapInfoButtonUp( kome::objects::Parameters* params ) {
		
	static	bool	bExecute = false;

	if  ( false == bExecute )
	{
		bExecute = true;

		// return value
		kome::objects::Variant ret;
		ret.type = kome::objects::Variant::BOOL;

		// get parameters
		kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
		if( mouseEvent == NULL ) {
			return ret;
		}

		kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
		if( dataMap == NULL ) {
			return ret;
		}

		kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
		if( graphics == NULL ) {
			return ret;
		}

		wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

		// on button down
		DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
		ret.prim.boolVal = mgr.upButton( *dataMap, *mouseEvent, *graphics, canvas );

		bExecute = false;
		return ret;
	}
	else
	{
		// return value
		kome::objects::Variant ret;
		ret.type = kome::objects::Variant::BOOL;
		ret.prim.boolVal = false;
		
		return	ret;
	}
}

// draw data map forreground
kome::objects::Variant onDrawDataMapFg( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// draw
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
	mgr.drawForeground( *dataMap, *graphics, canvas );

	return ret;
}

// set data map range status
kome::objects::Variant setRangeStatus( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// toggles status
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
	mgr.toggleDataMapRangeStatus();

	return ret;
}

// reset range status
kome::objects::Variant resetZoomDatamapFromContextMenu( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// data map
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		kome::objects::MsDataVariant obj( frame->getActiveObject() );
		kome::objects::DataMapInfo* dataMap = obj.getDataMap();

		// canvas
		wxWindow* canvas = frame->getCanvas();

		if( dataMap != NULL ) {
			// Add Operation
			DataMapRangeOperation* opt = new DataMapRangeOperation();
			opt->setShortName( "datamp_range" );
			opt->setOperationName( "DataMap Range" );
			opt->setOperationType(  kome::operation::Operation::TYPE_STANDARD );
			opt->setTargetSample( dataMap->getSample() );
			opt->setCanvas( canvas );
			opt->setDataMapInfo( dataMap );
			opt->setStartMz( dataMap->getMinMz() );
			opt->setEndMz( dataMap->getMaxMz() );
			opt->setStartRt( dataMap->getMinRt() );
			opt->setEndRt( dataMap->getMaxRt() );
			opt->setbReset( true );

			if( opt->execute() ){
				kome::window::WindowTool::refresh();
			}else{
				delete opt;
			}
			
		}
	}

	return ret;
}

// open change displayedRange dialog
kome::objects::Variant openChangeDisplayedRangeDlg( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// data map
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		kome::objects::MsDataVariant obj( frame->getActiveObject() );
		kome::objects::DataMapInfo* dataMap = obj.getDataMap();

		// canvas
		wxWindow* canvas = frame->getCanvas();

		if( dataMap != NULL ) {
			DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
			mgr.openChangeDispRangeDlg( *dataMap, canvas );

		}
	}

	return ret;
}

// on button double click (data map)
kome::objects::Variant onDataMapInfoDblClick( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// on button down
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
	ret.prim.boolVal = mgr.dblClick( *dataMap, *mouseEvent, *graphics, canvas );

	return ret;
}

// get datamap range operation
kome::objects::Variant getDataMapRangeOperation( kome::objects::Parameters* params ){
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new DataMapRangeOperation();
	
	return ret;
}
