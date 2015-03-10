/**
 * @file MdiChildFrameFunctions.cpp
 * @brief implements of functions of MdiChildFrame plug-in
 *
 * @author S.Tanaka
 * @date 2007.09.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MdiChildFrameFunctions.h"
#include "MdiChildFrameManager.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// open spectrum frame
kome::objects::Variant openSpectrumFrame( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* s = statusMgr.getValue( AUTO_ARRANGE_STATUS_NAME );
	std::string ss = NVL( s, "" );

	// manager
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();

	// open
	mgr.openSpectrum( spec );

	// BackgroundSubtractDialogÇ…ëóÇÈÇΩÇﬂÇÃèàóù @date 2011.07.14 <Add> M.Izumi ->
	kome::objects::Parameters param;
	// set chromatogram
	kome::plugin::PluginCallTool::setSpectrum( param, *spec );
	// managers
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	plgMgr.callAllFunctions( &param, "SPECTRUM_OPEN_BG_DIALOG" );
	// @date 2011.07.14 <Add> M.Izumi <-

	mgr.updateStyle();

	return ret;
}

// open chromatogram frame
kome::objects::Variant openChromatogramFrame( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	// manager
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();

	// open
	mgr.openChromatogram( chrom );
	mgr.updateStyle();

	return ret;
}

// open data map frame
kome::objects::Variant openDataMapFrame( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get chromatogram
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	// manager
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();

	// open
	mgr.openDataMap( dataMap );
	mgr.updateStyle();

	return ret;
}

// close all spectrum
kome::objects::Variant closeAllSpectrumFrames( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// close all spectrum
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();
	mgr.closeAllSpecFrames();

	return ret;
}

// set auto arrange mode
kome::objects::Variant toggleAutoArrangeMode( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// mager
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();

	// status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* s = statusMgr.getValue( AUTO_ARRANGE_STATUS_NAME );
	std::string ss = NVL( s, "" );

	if( ss.compare( AUTO_ARRANGE_STATUS_VALUE ) == 0 ) {    // on -> off
		statusMgr.setValue( AUTO_ARRANGE_STATUS_NAME, "off" );
	}
	else {    // off -> on
		if( !mgr.setAutoArrangeStatus() ) {
			return ret;
		}

		statusMgr.setValue( AUTO_ARRANGE_STATUS_NAME, AUTO_ARRANGE_STATUS_VALUE );
	}

	// set style
	mgr.updateStyle();
	mgr.layoutChildren( NULL, false );

	return ret;
}

// close all chromatogram
kome::objects::Variant closeAllChromatogramFrames( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// close all chromatogram
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();
	mgr.closeAllChromFrames();

	return ret;
}

// close all data map
kome::objects::Variant closeAllDataMapFrames( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// close all chromatogram
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();
	mgr.closeDataMapFrames();

	return ret;
}

// set profile window status
kome::objects::Variant setMdiWindowStatus( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// managers
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// check status
	const char* s = statusMgr.getValue( MDI_WINDOW_STATUS_NAME );
	std::string status = NVL( s, "" );
	if( status.compare( MDI_WINDOW_STATUS_VALUE ) == 0 ) {
		return ret;
	}

	// close all window
	while( kome::window::ChildFrame::getNumberOfFrames() > 0 ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( 0 );
		frame->Close();
		frame->Destroy();
		delete frame;
	}

	// set status
	statusMgr.setValue( MDI_WINDOW_STATUS_NAME, MDI_WINDOW_STATUS_VALUE );

	return ret;
}

// cascade children
kome::objects::Variant cascadeChildren( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// main frame
	kome::window::MainFrame* mainFrame
		= (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();

	// cascade
	mainFrame->Cascade();

	return ret;
}

// tile vertically
kome::objects::Variant tileChildrenVertically( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// tile vertically
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();
	mgr.tileVertically();

	return ret;
}

// tile vertically
kome::objects::Variant tileChildren( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// main frame
	kome::window::MainFrame* mainFrame
		= (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();

	// client window size
	wxSize size;
	wxWindow* client = mainFrame->GetClientWindow();
	if( client != NULL ) {
		size = client->GetSize();
	}

	// tile
	if( 3 * size.GetWidth() > 5 * size.GetHeight() ) {
		mainFrame->Tile( wxVERTICAL );
	}
	else {
		mainFrame->Tile( wxHORIZONTAL );
	}

	return ret;
}

// arrange icons
kome::objects::Variant arrangeIcons( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// main frame
	kome::window::MainFrame* mainFrame
		= (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();

	// arrange
	mainFrame->ArrangeIcons();

	return ret;
}

// on move
kome::objects::Variant onMoveChildFrame( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// child frame
	wxWindow* wnd = kome::plugin::PluginCallTool::getWindow( params );
	kome::window::ChildFrame* child = dynamic_cast< kome::window::ChildFrame* >( wnd );
	if( child == NULL ) {
		return ret;
	}

	// event
	kome::evt::MoveEvent* evt = kome::plugin::PluginCallTool::getMoveEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// set
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();
	mgr.setMovePosition( child, evt->x(), evt->y() );

	return ret;
}

// on moving
kome::objects::Variant onMovingChildFrame( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// child frame
	wxWindow* wnd = kome::plugin::PluginCallTool::getWindow( params );
	kome::window::ChildFrame* child = dynamic_cast< kome::window::ChildFrame* >( wnd );
	if( child == NULL ) {
		return ret;
	}

	// event
	kome::evt::MoveEvent* evt = kome::plugin::PluginCallTool::getMoveEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// set
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();
	mgr.setMovingPosition( child, evt->x(), evt->y() );

	return ret;
}

// on client size
kome::objects::Variant onClientSize( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// event
	kome::evt::SizeEvent* evt = kome::plugin::PluginCallTool::getSizeEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// set
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();
	mgr.setClientSize( evt->width(), evt->height() );

	return ret;
}

// on settings
kome::objects::Variant onChangeSettings( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// set
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();
	mgr.layoutChildren( NULL );

	return ret;
}

// next child
kome::objects::Variant activateNextChild( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// main frame
	kome::window::MainFrame* mainFrame
		= (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();

	// next
	mainFrame->ActivateNext();

	return ret;
}

// previous
kome::objects::Variant activatePreviousChild( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// main frame
	kome::window::MainFrame* mainFrame
		= (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();

	// previous
	mainFrame->ActivatePrevious();

	return ret;
}

// not auto arrange window mode
kome::objects::Variant isNotAutoArrange( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* s = statusMgr.getValue( AUTO_ARRANGE_STATUS_NAME );
	std::string ss = NVL( s, "" );

	ret.prim.boolVal = ( ss.compare( AUTO_ARRANGE_STATUS_VALUE ) != 0 );

	return ret;
}

// layout child frames when a frame is closed
kome::objects::Variant layoutChildrenOnClose( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	MdiChildFrameManager& mgr = MdiChildFrameManager::getInstance();

	// canvas
	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	kome::window::ChildFrame* frame = ( canvas == NULL ? NULL : dynamic_cast< kome::window::ChildFrame* >( canvas->GetParent() ) );

	// layout
	mgr.layoutChildren( frame );

	return ret;
}
