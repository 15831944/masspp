/**
 * @file EditBatchMethodNameDlg.cpp
 * @brief implementsation of EditBatchMethodNameDlg class
 *
 * @author OKADA, H
 * @date 2012-03-05
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "EditBatchMethodNameDlg.h"
#include "BatchButtonDef.h"

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP	3
#define BORDER_SIZE		3


BEGIN_EVENT_TABLE( EditBatchMethodNameDlg,			kome::window::StandardDialog )
	
	EVT_BUTTON( wxOK, EditBatchMethodNameDlg::onOkButton )

END_EVENT_TABLE()



// constructor
EditBatchMethodNameDlg::EditBatchMethodNameDlg( wxWindow* parent )
: kome::window::StandardDialog(	parent,	"Edit Batch Method Name")
//EditBatchMethodNameDlg::EditBatchMethodNameDlg( wxWindow* parent )
//: kome::window::StandardDialog(	parent,	"New Batch Method"), m_parameters( NULL, false )
{

	m_isDestructed = 0;	// initialize


}


// destructor
EditBatchMethodNameDlg::~EditBatchMethodNameDlg() {


	m_isDestructed = 1;	// destruct

	_ASSERTE( _CrtCheckMemory( ) );
}

// create main sizer
wxSizer* EditBatchMethodNameDlg::createMainSizer() {

	wxFlexGridSizer* fieldSizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	fieldSizer->AddGrowableCol( 1 );
	fieldSizer->AddGrowableRow( 0 );


	wxStaticText* descrName = new wxStaticText( this, wxID_STATIC, wxT( "Batch Method Name" ), wxDefaultPosition, wxDefaultSize, 0 );
	fieldSizer->Add(descrName, 0, wxALIGN_LEFT|wxALL, 5);

	// A text control for the userfs name
	wxTextCtrl* nameCtrl = new wxTextCtrl ( this, ID_BATCH_METHOD_NAME_TEXT, wxT( "Default-text" ), wxDefaultPosition, wxDefaultSize, 0 );
	fieldSizer->Add(nameCtrl, 0, wxGROW|wxALL, 5);

	return fieldSizer;
}

// create Button sizer
wxSizer* EditBatchMethodNameDlg::createButtonSizer() {

	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );

	// OK, Cancel
	wxSizer* buttonSizer = kome::window::StandardDialog::createButtonSizer();
	sizer->Add( buttonSizer, 2, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );

	return sizer;
}
