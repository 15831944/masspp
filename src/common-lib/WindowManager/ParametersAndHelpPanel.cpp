/**
 * @file ParametersAndHelpPanel.cpp
 * @brief implements of ParametersAndHelpPanel class
 *
 * @author S.Tanaka
 * @date 2007.08.15
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ParametersAndHelpPanel.h"

#include "BitmapCanvas.h"

#include <wx/splitter.h>
#include <wx/statline.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						3
#define GRID_GAP						3


BEGIN_EVENT_TABLE( ParametersAndHelpPanel, StandardPanel )
END_EVENT_TABLE()


// constructor
ParametersAndHelpPanel::ParametersAndHelpPanel( wxWindow* parent, const int id ) : StandardPanel( parent, id ) {
	// initialize
	m_text = NULL;
	m_bitmap = NULL;
}

// destructor
ParametersAndHelpPanel::~ParametersAndHelpPanel() {
}

// create controls
void ParametersAndHelpPanel::createControls() {
	// Auto Layout
	SetAutoLayout( true );

	// create top sizer
	wxSizer* topSizer = new wxBoxSizer( wxVERTICAL );

	// main sizer
	wxSizer* mainSizer = createMainSizer();
	topSizer->Add( mainSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	// line
	topSizer->Add(
		new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL ),
		0,
		wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxGROW,
		BORDER_SIZE
	);

	// help sizer
	wxSizer* helpSizer = createHelpSizer();
	topSizer->Add( helpSizer, 0, wxALL | wxGROW, BORDER_SIZE );

	// Fit
	SetSizer( topSizer );
	topSizer->Fit( this );
	topSizer->SetSizeHints( this );
}

// create help sizer
wxSizer* ParametersAndHelpPanel::createHelpSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// splitter
	wxSplitterWindow* splitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 350, 175 ), wxSP_3D );
	sizer->Add( splitter, 1, wxALL | wxGROW );

	// text
	m_text = new wxTextCtrl(
		splitter,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( 175, 175 ),
		wxTE_MULTILINE | wxTE_READONLY
	);

	// bitmap
	m_bitmap = new BitmapCanvas( splitter, 175, 175 );

	// splitter
	splitter->SetSashGravity( 0.5 );
	splitter->SplitVertically( m_text, m_bitmap );

	return sizer;
}

// set the description
void ParametersAndHelpPanel::setDescription( const char* desc ) {
	m_text->Clear();
	m_text->WriteText( wxT( desc ) );
	m_text->ShowPosition( 0 );
}

// set the image file path
void ParametersAndHelpPanel::setImageFilePath( const char* path ) {
	// clear
	m_bitmap->setImageFile( path );
}
