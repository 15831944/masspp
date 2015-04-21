/**
 * @file CollapsiblePane.cpp
 * @brief implements of CollapsiblePane class
 *
 * @author S.Tanaka
 * @date 2011.04.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "CollapsiblePane.h"

#include "switch_button_on_16x16.xpm"
#include "switch_button_off_16x16.xpm"


#include <wx/statline.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						3
#define GRID_GAP                        3

#define ID_COLLAPSE_BUTTON             456


BEGIN_EVENT_TABLE( CollapsiblePane, wxGenericCollapsiblePane )
END_EVENT_TABLE()


// constructor
CollapsiblePane::CollapsiblePane( wxWindow* parent, const char* title, const int id ) 
		: wxGenericCollapsiblePane( parent, id, wxT( title ) ) {
    // initialize
	m_text = NULL;

	// delete
	m_sz->Clear( false );
	delete m_pButton;

	// sizer
	wxBitmap bitmap( switch_button_on_16x16_xpm );
	m_pButton = new wxBitmapButton(
			this,
			wxID_ANY,
			bitmap,
			wxDefaultPosition,
			wxDefaultSize,
			wxNO_BORDER | wxBU_EXACTFIT
	);
	m_sz->Add( m_pButton, 0, wxLEFT | wxTOP | wxBOTTOM, GetBorder() );
	m_sz->AddSpacer( 3 );

	m_text = new wxStaticText( this, wxID_ANY, GetLabel() );
	m_sz->Add( m_text, 0, wxALIGN_CENTER, GetBorder() );
	m_sz->AddSpacer( 3 );

	m_sz->Add( m_pStaticLine, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT, GetBorder() );
}

// destructor
CollapsiblePane::~CollapsiblePane() {
	if( m_text != NULL ) {
		m_text->SetContainingSizer( NULL );
	}
}

// Collapse
void CollapsiblePane::Collapse( bool collapse ) {
	// default
	wxGenericCollapsiblePane::Collapse( collapse );

	// button
	wxBitmap bitmap( collapse ? switch_button_on_16x16_xpm : switch_button_off_16x16_xpm );
	wxBitmapButton* button = (wxBitmapButton*)m_pButton;

	button->SetBitmapDisabled( bitmap );
	button->SetBitmapFocus( bitmap );
	button->SetBitmapHover( bitmap );
	button->SetBitmapLabel( bitmap );
	button->SetBitmapSelected( bitmap );
}
