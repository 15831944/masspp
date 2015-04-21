/**
 * @file WindowTool.cpp
 * @brief implements of WindowTool class
 *
 * @author S.Tanaka
 * @date 2006.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "WindowTool.h"

#include "MainFrame.h"
#include "Canvas.h"

#include <wx/wx.h>

#include "StatusBar.h"
#include "MessageDialog.h"
#include "ChildFrame.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static members
wxWindow* WindowTool::m_mainWindow = NULL;

// create main window
MainFrame* WindowTool::createMainWindow(
		const char* title,
		const int px,
		const int py,
		const unsigned int cx,
		const unsigned int cy
) {
	// posigion & size
	wxPoint pos( px, py );
	wxSize size( cx, cy );

	// create window
	MainFrame* frame = new MainFrame(
		title,
		pos,
		size,
		wxCLIP_CHILDREN | wxMINIMIZE_BOX | wxMAXIMIZE_BOX
				| wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxTHICK_FRAME
	);
	m_mainWindow = frame;

	return frame;
}

// set main window
void WindowTool::setMainWindow( wxWindow* wnd ) {
	m_mainWindow = wnd;
}

// get main window
wxWindow* WindowTool::getMainWindow() {
	return m_mainWindow;
}

// refresh
void WindowTool::refresh() {
	// main window
	if( m_mainWindow != NULL ) {
		m_mainWindow->Refresh();
	}

	// children
	for( unsigned int i = 0; i < Canvas::getNumberOfCanvases(); i++ ) {
		Canvas::getCanvas( i )->Refresh();
	}
}

// show information
void WindowTool::showInfo( const char* msg, const char* infoTitle, const char* info ) {
	// string
	std::string s = NVL( msg, "" );

	// log
	LOG_INFO_CODE( FMT( "%s", s.c_str() ), ERR_OTHER );

	// window
	if( wxIsMainThread() ) {
		MessageDialog dlg(
			wxIsMainThread() ? getMainWindow() : NULL,
			"Mass+",
			s.c_str(),
			MessageDialog::ICON_INFO,
			MessageDialog::BUTTON_OK,
			infoTitle,
			info
		);

		dlg.ShowModal();
	}
	else {
		wxMessageBox(
			wxT( s.c_str() ),
			"Mass++",
			wxOK | wxICON_INFORMATION
		);
	}
}

// show error
void WindowTool::showError( const char* msg, const char* infoTitle, const char* info ) {
	// string
	std::string s = NVL( msg, "" );

	// log
	LOG_ERROR_CODE( FMT( "%s", s.c_str() ), ERR_OTHER );

	// window
	if( wxIsMainThread() ) {
		MessageDialog dlg(
			wxIsMainThread() ? getMainWindow() : NULL,
			wxT( "Mass++" ),
			s.c_str(),
			MessageDialog::ICON_ERROR,
			MessageDialog::BUTTON_OK,
			infoTitle,
			info
		);

		dlg.ShowModal();
	}
	else {
		wxMessageBox(
			wxT( s.c_str() ),
			wxT( "Mass++" ),
			wxOK | wxICON_ERROR
		);
	}
}

// show warning
void WindowTool::showWarning( const char* msg, const char* infoTitle, const char* info ) {
	// string
	std::string s = NVL( msg, "" );

	// log
	LOG_WARN_CODE( FMT( "%s", s.c_str() ), ERR_OTHER );

	// window
	if( wxIsMainThread() ) {
		MessageDialog dlg(
			wxIsMainThread() ? getMainWindow() : NULL,
			"Mass++",
			s.c_str(),
			MessageDialog::ICON_WARNING,
			MessageDialog::BUTTON_OK,
			infoTitle,
			info
		);

		dlg.ShowModal();
	}
	else {
		wxMessageBox(
			wxT( s.c_str() ),
			wxT( "Mass++" ),
			wxOK | wxICON_EXCLAMATION
		);
	}
}

// show yes-no dialog
bool WindowTool::showYesNo( const char* msg, const char* yesBtnName, const char* noBtnName, const char* infoTitle, const char* info ) {
	// dialog
	MessageDialog dlg(
		wxIsMainThread() ? getMainWindow() : NULL,
		"",
		msg,
		MessageDialog::ICON_QUESTION,
		MessageDialog::BUTTON_YES | MessageDialog::BUTTON_NO,
		infoTitle,
		info,
		yesBtnName,
		noBtnName
	);

	int ret = dlg.ShowModal();

	return ( ret == wxID_YES );
}

// show yes- no-cancel dialog
int WindowTool::showYesNoCancel( const char* msg, const char* yesBtnName, const char* noBtnName, const char* infoTitle, const char* info ) {
	// dialog
	MessageDialog dlg(
		wxIsMainThread() ? getMainWindow() : NULL,
		"",
		msg,
		MessageDialog::ICON_QUESTION,
		MessageDialog::BUTTON_YES | MessageDialog::BUTTON_NO | MessageDialog::BUTTON_CANCEL,
		infoTitle,
		info,
		yesBtnName,
		noBtnName
	);

	int ret = dlg.ShowModal();

	if( ret == wxID_YES ) {
		return 1;
	}
	else if( ret == wxID_NO ) {
		return 0;
	}
	return -1;
}

// show ok-cancel dialog
bool WindowTool::showOkCancel( const char* msg, const char* infoTitle, const char* info ) {
	// dialog
	MessageDialog dlg(
		wxIsMainThread() ? getMainWindow() : NULL,
		"",
		msg,
		MessageDialog::ICON_INFO,
		MessageDialog::BUTTON_OK | MessageDialog::BUTTON_CANCEL,
		infoTitle,
		info
	);

	int ret = dlg.ShowModal();

	return ( ret == wxID_OK );
}

// show ok-cancel Warning dialog
bool WindowTool::showOkCancelWarning( const char* msg, const char* infoTitle, const char* info ) {
	// dialog
	MessageDialog dlg(
		wxIsMainThread() ? getMainWindow() : NULL,
		"Mass++",
		msg,
		MessageDialog::ICON_WARNING,
		MessageDialog::BUTTON_OK | MessageDialog::BUTTON_CANCEL,
		infoTitle,
		info
	);

	int ret = dlg.ShowModal();

	return ( ret == wxID_OK );
}

// set timer
int WindowTool::setTimer(
		boost::function< void () > fun,
		int milliseconds,
		bool oneShot
) {
	// main frame
	MainFrame* frame = dynamic_cast< MainFrame* >( m_mainWindow );
	if( frame == NULL ) {
		return false;
	}

	// set timer
	return frame->setTimer( fun, milliseconds, oneShot );
}

// kill timer
void WindowTool::killTimer( int timer ) {
	// main frame
	MainFrame* frame = dynamic_cast< MainFrame* >( m_mainWindow );
	if( frame == NULL ) {
		return;
	}

	// kill timer
	frame->killTimer( timer );
}
