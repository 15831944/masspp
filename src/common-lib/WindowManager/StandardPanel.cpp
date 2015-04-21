/**
 * @file StandardPanel.cpp
 * @brief implements of StandardPanel class
 *
 * @author S.Tanaka
 * @date 2007.08.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "StandardPanel.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						3
#define GRID_GAP						3


BEGIN_EVENT_TABLE( StandardPanel, wxPanel )
	EVT_INIT_DIALOG( StandardPanel::onInitDialog )
END_EVENT_TABLE()


// constructor
StandardPanel::StandardPanel( wxWindow* parent, const int id ) : wxPanel( parent, id ) {
	// style
	SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
}

// destructor
StandardPanel::~StandardPanel() {
}

// create controls
void StandardPanel::createControls() {
	// Auto Layout
	SetAutoLayout( true );

	// create top sizer
	wxSizer* mainSizer = createMainSizer();

	// Fit
	SetSizer( mainSizer );
	mainSizer->Fit( this );
	mainSizer->SetSizeHints( this );
}

// create main sizer
wxSizer* StandardPanel::createMainSizer() {
	return new wxBoxSizer( wxVERTICAL );
}

// on init dialog
void StandardPanel::onInitDialog( wxInitDialogEvent& evt ) {
	createControls();

	wxPanel::OnInitDialog( evt );

	wxWindowList& controlList = GetChildren();
	wxWindow* firstNonStaticControl = NULL;
	for ( unsigned int i = 0; (i < controlList.size()) && (!firstNonStaticControl); i++ )
	{
		if ( controlList[i]->IsEnabled() && (controlList[i]->AcceptsFocus()) )
		{
			firstNonStaticControl = controlList[i];
		}
	}
	if ( firstNonStaticControl != NULL )
	{
		firstNonStaticControl->SetFocus();
	}
}
