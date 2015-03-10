/**
 * @file OperationLogDialog.cpp
 * @brief OperationLog Dialog class
 *
 * @author M.Izumi
 * @date 2011.12.08
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "OperationLogDialog.h"
#include "OperationLogManager.h"
#include "LogGridList.h"

using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE			 3
#define GRID_GAP			 3

#define ID_LIST				 1
#define ID_SAVE_BUTTON		 2

#define LISTBOX_HEIGHT		 200

#define FILE_SAVE_CAPTION	 "Save As"


BEGIN_EVENT_TABLE( OperationLogDialog, kome::window::StandardPanel )
	EVT_BUTTON( ID_SAVE_BUTTON, OperationLogDialog::onSave )
END_EVENT_TABLE()

// constructor
OperationLogDialog::OperationLogDialog( wxWindow* parent )
		: StandardPanel( parent ) {
	
	m_listCtrl = NULL;
	createControls();
}

// destructor
OperationLogDialog::~OperationLogDialog(){
}

// create main sizer
wxSizer* OperationLogDialog::createMainSizer(){
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableCol( 0 );
 
	// list box
	m_listCtrl = new LogGridList( this );
	sizer->Add( m_listCtrl, 1, wxALL | wxGROW, BORDER_SIZE );
	
	// button
	wxSizer* btnSizer = createBtnSizer();
	sizer->Add( btnSizer, 1, wxALL | wxGROW, BORDER_SIZE );
	
	return sizer;
}

// create Button sizer
wxSizer* OperationLogDialog::createBtnSizer(){
	// sizer
	wxGridSizer* sizer = new wxGridSizer( 1, 3, GRID_GAP, GRID_GAP );

	// save button
	m_saveButton = new wxButton( this, ID_SAVE_BUTTON, wxT( "Export for batch command..." ) ); // @date 2013/08/19 <Add> OKADA

//	m_saveButton = new wxButton( this, ID_SAVE_BUTTON, wxT( "Create Batch File..." ) ); // @date 2013/08/19 <Del> OKADA
	// Spec No.86192 Operation Log pane - the "Create Batch File" button is resized and unreadable, e.g. "e Batch F"
	// sizer->Add( m_saveButton , 1, wxALL | wxGROW, BORDER_SIZE ); // @date 2012.06.26 <Del> M.Izumi
	sizer->Add( m_saveButton );

	return sizer;
}

// update operation log list
void OperationLogDialog::updateLogList(){
	m_listCtrl->UpdateLogList();
}

// @date 2013/08/19 <Add> OKADA ------->
// Batch Button Click
void OperationLogDialog::onSave( wxCommandEvent& evt ){

	// SaveAsMspXmlFile()Ç∆ìØìôÇÃèàóù

	// manager
	OperationLOGManager& mgr = OperationLOGManager::getInstance();

	mgr.openFileSaveDialog();
}
// @date 2013/08/19 <Add> OKADA <-------
