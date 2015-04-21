/**
 * @file MessageDialog.cpp
 * @brief implements of MessageDialog class
 *
 * @author S.Tanaka
 * @date 2011.05.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "MessageDialog.h"

#include "CollapsiblePane.h"

#include <wx/artprov.h>
		

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define MESSAGE_MARGIN                    12
#define BORDER_SIZE						   3
#define GRID_GAP						   3

#define MESSAGE_WIDTH                    300

#define INFO_WIDTH                       400
#define INFO_HEIGHT                      250


BEGIN_EVENT_TABLE( MessageDialog, StandardDialog )
    EVT_BUTTON( wxID_YES, MessageDialog::onYes )
    EVT_BUTTON( wxID_NO, MessageDialog::onNo )
END_EVENT_TABLE()



// constructor
MessageDialog::MessageDialog(
		wxWindow* parent,
		const char* title,
		const char* message,
		const IconType icon,
		int buttonStyle,
		const char* infoTitle,
		const char* info,
		const char* yesBtnName,
		const char* noBtnName) : StandardDialog( parent, title ) {
    // initialize
	m_msg = NVL( message, "" );
	m_iconType = icon;
	m_buttonStyle = buttonStyle;
	m_infoTitle = NVL( infoTitle, "" );
	m_info = NVL( info, "" );
	m_yesBtnName = (yesBtnName != NULL) ? yesBtnName : "Yes";
	m_noBtnName = (noBtnName != NULL) ? noBtnName : "No";
}

// destructor
MessageDialog::~MessageDialog() {
}

// create main sizer
wxSizer* MessageDialog::createMainSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* innerSizer = new wxBoxSizer( wxHORIZONTAL );

	// icon
	wxStaticBitmap* bitmap = NULL;
	if( m_iconType == ICON_INFO ) {
		bitmap = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_INFORMATION ) );
	}
	else if( m_iconType == ICON_ERROR ) {
		bitmap = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_ERROR ) );
	}
	else if( m_iconType == ICON_WARNING ) {   
		bitmap = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_WARNING ) );
	}
	else if( m_iconType == ICON_QUESTION ) {
		bitmap = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_QUESTION ) );
	}

	if( bitmap != NULL ) {
		innerSizer->AddSpacer( MESSAGE_MARGIN );
		innerSizer->Add( bitmap, 0, wxLEFT, BORDER_SIZE );
	}

	// message
	wxSizer* txtSizer = CreateTextSizer( wxT( m_msg.c_str() ) );
	innerSizer->AddSpacer( MESSAGE_MARGIN );
	innerSizer->Add( txtSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	sizer->AddSpacer( MESSAGE_MARGIN );
	sizer->Add( innerSizer, 1, wxALL | wxGROW, BORDER_SIZE );
	sizer->AddSpacer( MESSAGE_MARGIN );

	// information
	if( !m_info.empty() ) {
		// collapsible pane
		CollapsiblePane* pane = new CollapsiblePane(
			this,
			m_infoTitle.empty() ? "Information" : m_infoTitle.c_str()
		);
		sizer->Add( pane, 0, wxALL | wxGROW, BORDER_SIZE );

		// info
		wxWindow* win = pane->GetPane();
		wxTextCtrl* infoText = new wxTextCtrl(
			win,
			wxID_ANY,
			wxT( m_info.c_str() ),
			wxDefaultPosition,
			wxSize( INFO_WIDTH, INFO_HEIGHT ),
			wxTE_DONTWRAP | wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL
		);

		// add to pane
		wxSizer* paneSizer = new wxBoxSizer( wxVERTICAL );
		paneSizer->Add( infoText, 1, wxALL | wxGROW, BORDER_SIZE );
		win->SetSizer( paneSizer );
		paneSizer->SetSizeHints( win );
	}

	return sizer;
}

// create button sizer
wxSizer* MessageDialog::createButtonSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	wxSizer* innerSizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( innerSizer, 0, wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );

	// buttons
	if( m_buttonStyle & BUTTON_YES ) {
		wxButton* button = new wxButton( this, wxID_YES, wxT( m_yesBtnName.c_str() ) );
		innerSizer->Add( button, 0, wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	if( m_buttonStyle & BUTTON_NO ) {
		wxButton* button = new wxButton( this, wxID_NO, wxT( m_noBtnName.c_str() ) );
		innerSizer->Add( button, 0, wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	if( m_buttonStyle & BUTTON_OK ) {
		wxButton* button = new wxButton( this, wxID_OK, wxT( "OK" ) );
		innerSizer->Add( button, 0, wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	if( m_buttonStyle & BUTTON_CANCEL ) {
		wxButton* button = new wxButton( this, wxID_CANCEL, wxT( "Cancel" ) );
		innerSizer->Add( button, 0, wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	return sizer;
}

// on yes
void MessageDialog::onYes( wxCommandEvent& evt ) {
	EndDialog( wxID_YES );
}

// on no
void MessageDialog::onNo( wxCommandEvent& evt ) {
	EndDialog( wxID_NO );
}
