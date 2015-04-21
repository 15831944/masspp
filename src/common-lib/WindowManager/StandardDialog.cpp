/**
 * @file StandardDialog.cpp
 * @brief implements of StandardDialog class
 *
 * @author S.Tanaka
 * @date 2007.08.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "StandardDialog.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						3
#define GRID_GAP						3


BEGIN_EVENT_TABLE( StandardDialog, wxDialog )
	EVT_INIT_DIALOG( StandardDialog::onInitDialog )
	EVT_BUTTON( wxID_OK, StandardDialog::onOkButton )
	EVT_BUTTON( wxID_CANCEL, StandardDialog::onCancelButton )
END_EVENT_TABLE()


// constructor
StandardDialog::StandardDialog(
	   wxWindow* parent,
	   const char* title,
	   const char* commitBtnName,
	   const int px,
	   const int py,
	   const int width,
	   const int height
) : wxDialog(
		parent,
		wxID_ANY,
		wxT( NVL( title, "" ) ),
		wxPoint::wxPoint( px, py ),
		wxSize::wxSize( width, height ),
		wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX ) {
	// style
	SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );

	m_commitBtnName = (commitBtnName != NULL) ? commitBtnName : "OK";
}

// destructor
StandardDialog::~StandardDialog() {
}

// close child dialogs
void StandardDialog::closeChildDialogs() {
	closeChildDialogs( this );
}

// create controls
void StandardDialog::createControls() {
	// Auto Layout
	SetAutoLayout( true );

	// create top sizer
	wxSizer* topSizer = createTopSizer();

	// Fit
	SetSizer( topSizer );
	topSizer->Fit( this );
	topSizer->SetSizeHints( this );
}

// create top sizer
wxSizer* StandardDialog::createTopSizer() {
	// top sizer
	wxFlexGridSizer* topSizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	topSizer->AddGrowableCol( 0 );
	topSizer->AddGrowableRow( 0 );

	// main sizer
	wxSizer* fieldSizer = createMainSizer();
	topSizer->Add( fieldSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	// button sizer
	wxSizer* buttonSizer = createButtonSizer();
	topSizer->Add( buttonSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	return topSizer;
}

// create main sizer
wxSizer* StandardDialog::createMainSizer() {
	return new wxBoxSizer( wxVERTICAL );
}

// create button sizer
wxSizer* StandardDialog::createButtonSizer() {

	if (m_commitBtnName.compare("OK") == 0)
	{
		return CreateButtonSizer( wxOK | wxCANCEL );
	}
	else
	{
		// sizer
		wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

		// button sizer
		wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

		// commit button
		wxButton* button = new wxButton( this, wxID_OK, wxT( m_commitBtnName.c_str() ) );
		buttonSizer->Add( button, 1, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

		button = new wxButton( this, wxID_CANCEL, wxT( "Cancel" ) );
		buttonSizer->Add( button, 1, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

		sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );

		return sizer;
	}
}

// on OK
bool StandardDialog::onOK() {
	return true;
}

// on Cancel
bool StandardDialog::onCancel() {
	return true;
}

// on init dialog
void StandardDialog::onInitDialog( wxInitDialogEvent& evt ) {
	createControls();

	wxDialog::OnInitDialog( evt );

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

// on OK button
void StandardDialog::onOkButton( wxCommandEvent& evt ) {
	if( Validate() && TransferDataFromWindow() ) {	// check values in the window
		// close child dialogs
		closeChildDialogs();

		// end dialog
		if( onOK() ) {
			if( IsModal() ) {	// modal
				EndModal( wxID_OK );
			}
			else {	// modeless
				SetReturnCode( wxID_OK );
				closeChildDialogs();
				Show( false );
			}
		}
	}
}

// on Cancel button
void StandardDialog::onCancelButton( wxCommandEvent& evt ) {
	// cancel
	if( onCancel() ) {
		closeChildDialogs();

		// end dialog
		if( IsModal() ) {	// modal
			EndModal( wxID_CANCEL );
		}
		else {
			SetReturnCode( wxID_CANCEL );
			Show( false );
		}
	}
}

// close child dialogs
void StandardDialog::closeChildDialogs( wxWindow* parent ) {
	// check the parameter
	if( parent == NULL ) {
		return;
	}

	// get children
	wxWindowList& wl = parent->GetChildren();

	for( wxWindowList::iterator it = wl.begin(); it != wl.end(); it++ ) {
		wxWindow* child = *it;
		closeChildDialogs( child );	// recursive

		// check the window type
		wxDialog* dlg = dynamic_cast< wxDialog* >( child );
		if( dlg != NULL ) {	// top level window
			if( dlg->IsShown() && !dlg->IsModal() ) {
				dlg->Destroy();
			}
		}
	}
}
