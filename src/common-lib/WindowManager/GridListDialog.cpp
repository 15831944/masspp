/**
 * @file GridListDialog.cpp
 * @brief implements of GridListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.09.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "GridListDialog.h"

#include "GridListCtrl.h"
#include "ListTool.h"
#include "WindowTool.h"

#include "delete_icon16x16.xpm"
#include "save_icon16x16.xpm"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP						 3
#define BORDER_SIZE						 3

#define ID_SAVE_BUTTON					 10001
#define ID_DELETE_BUTTON                 10002

 
BEGIN_EVENT_TABLE( GridListDialog, StandardDialog )
	EVT_BUTTON( ID_SAVE_BUTTON, GridListDialog::saveData )
	EVT_BUTTON( ID_DELETE_BUTTON, GridListDialog::deleteData )
	EVT_UPDATE_UI( ID_SAVE_BUTTON, GridListDialog::updateUI )
	EVT_UPDATE_UI( ID_DELETE_BUTTON, GridListDialog::updateUI )
END_EVENT_TABLE()


// constructor
GridListDialog::GridListDialog( wxWindow* parent, const char* title, const bool canDelete )
		: StandardDialog( parent, title ) {
	m_list = NULL;
	m_deleteFlg = canDelete;
}

// destructor
GridListDialog::~GridListDialog() {
}

// set grid list control
void GridListDialog::setGridListCtrl( GridListCtrl* listCtrl ) {
	m_list = listCtrl;
}

// update data
void GridListDialog::updateDialog() {
	// update list
	if( m_list != NULL ) {
		m_list->create();
	}

	// refresh
	if( IsShown() ) {
		Refresh();
	}
}

// create main sizer
wxSizer* GridListDialog::createMainSizer() { 
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// list control
	if( m_list != NULL ) {
		m_list->create();
		sizer->Add( m_list, 1, wxALL | wxGROW, BORDER_SIZE );
	}

	// butotn sizer
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBitmapButton* button = new wxBitmapButton( this, ID_SAVE_BUTTON, wxImage( save_icon16x16_xpm ) );
	button->SetToolTip("Save list to text file");
	buttonSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	if( m_deleteFlg ) {
		button = new wxBitmapButton( this, ID_DELETE_BUTTON, wxImage( delete_icon16x16_xpm ) );
		button->SetToolTip("Delete selected item");
		buttonSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, BORDER_SIZE );

	return sizer;
}

// create button sizer
wxSizer* GridListDialog::createButtonSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// button
	wxButton* button = new wxButton( this, wxID_OK, wxT( "Close" ) );
	sizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );

	return sizer;
}

// save data
void GridListDialog::saveData( wxCommandEvent& evt ) {
	m_list->exportData();
}

// delete data
void GridListDialog::deleteData( wxCommandEvent& evt ) {
	// confirm
	if( !WindowTool::showYesNo( "Are you sure you want to delete the selected item?", "Delete", "Don't delete" ) ) {
		return;
	}

	// selected item
	std::vector< int > items;
	ListTool::getSelectedItems( m_list, items );

	int iItemToSelect = 999999;

	// delete
	wxBeginBusyCursor();
	
	// @date 2013.09.27 <Mod> M.Izumi ->
	for( int i=items.size()-1; i >= 0; i-- ){
		if( items[ i ] < iItemToSelect ){
			iItemToSelect = items[ i ];
		}
		m_list->deleteData( iItemToSelect, false );
	}
	// @date 2013.09.27 <Mod> M.Izumi <-
	wxEndBusyCursor();

	m_list->updateList();

	m_list->Refresh();
}

// update UI
void GridListDialog::updateUI( wxUpdateUIEvent& evt ) {
	bool enable = true;

	if( evt.GetId() == ID_DELETE_BUTTON ) {
		std::vector< int > items;
		ListTool::getSelectedItems( m_list, items );

		enable = ( items.size() > 0 );
	}

	evt.Enable( enable );
}
