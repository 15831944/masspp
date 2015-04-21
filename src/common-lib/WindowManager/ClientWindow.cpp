/**
 * @file ClientWindow.cpp
 * @brief implements of ClientWindow class
 *
 * @author S.Tanaka
 * @date 2008.12.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClientWindow.h"

#include "MainFrame.h"
#include "WindowTool.h"
#include "ChildFrame.h"
#include "Canvas.h"
#include "CanvasBase.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( ClientWindow, wxMDIClientWindow )
    EVT_SIZE( ClientWindow::onSize )
END_EVENT_TABLE()


#define CLIENT_SIZE_FUNCTION_TYPE	  "ON_CLIENT_SIZE"


// constructor
ClientWindow::ClientWindow() {

}

// constructor
ClientWindow::ClientWindow( wxMDIParentFrame* parent )
		: wxMDIClientWindow( parent, 0 ), m_prevSize( -1, -1 ) {
}

// destructor
ClientWindow::~ClientWindow() {
}

// do set size
void ClientWindow::DoSetSize(
		int x,
		int y,
		int width,
		int height,
		int sizeFlags
) {
	// default
	wxMDIClientWindow::DoSetSize( x, y, width, height, sizeFlags );

	// 2010.11.10 Add M.Izumi
	MainFrame* main =  dynamic_cast< MainFrame* >( WindowTool::getMainWindow() );	
	if( ( main != NULL ) && !main->IsIconized() && !main->IsMaximized() ){
		main->setRect(main->GetRect());	// @date 2011.08.29 <Mod> M.Izumi
	}
}

// on size
void ClientWindow::onSize( wxSizeEvent& evt ) {
	// size
	wxSize size = evt.GetSize();
	int width = size.GetWidth();
	int height = size.GetHeight();

	// check the size
	if( m_prevSize.x == width && m_prevSize.y == height ) {
		wxWindow* wnd = kome::window::WindowTool::getMainWindow();
		if( wnd != NULL ) {
			wxSize size = wnd->GetSize();
			wnd->SetSize( size.x - 1, size.y );
			wnd->SetSize( size );
			return;
		}
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// parameters
	kome::objects::Parameters params;

	kome::evt::SizeEvent e;
	e.width( width );
	e.height( height );
	kome::plugin::PluginCallTool::setSizeEvent( params, e );

	// size event
	plgMgr.callAllFunctions( &params, CLIENT_SIZE_FUNCTION_TYPE, true );

	m_prevSize.x = width;
	m_prevSize.y = height;
}
