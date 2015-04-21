/**
 * @file ParameterTitlePanel.cpp
 * @brief implements of ParameterTitlePanel class
 *
 * @author S.Tanaka
 * @date 2013.02.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ParameterTitlePanel.h"
#include "ParametersAndHelpPanel.h"

#include "required_mark16x16.xpm"

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE					  3
#define GRID_GAP				      3


BEGIN_EVENT_TABLE( ParameterTitlePanel, StandardPanel )
END_EVENT_TABLE()


// constructor
ParameterTitlePanel::ParameterTitlePanel( wxWindow* parent, const char* title, const bool required )
        : StandardPanel( parent, wxID_ANY ) {
	// members
	m_title = NVL( title, "" );
	m_required = required;

	// create
	createControls();
}

// destructor
ParameterTitlePanel::~ParameterTitlePanel() {
}

// main sizer
wxSizer* ParameterTitlePanel::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// inner sizer
	wxSizer* innerSizer = new wxBoxSizer( wxHORIZONTAL );

	// text
	wxStaticText* text = new wxStaticText( this, wxID_ANY, wxT( m_title.c_str() ) );
	innerSizer->Add( text, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// mark
	if( m_required ) {
		wxBitmap bitmap( required_mark16x16_xpm );
		wxStaticBitmap* bmp = new wxStaticBitmap( this, wxID_ANY, bitmap );
		innerSizer->Add( bmp, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	sizer->Add( innerSizer, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return sizer;
}
