/**
 * @file ListTool.cpp
 * @brief implements of ListTool class
 *
 * @author S.Tanaka
 * @date 2006.09.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ListTool.h"

#include <wx/listctrl.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// get selected items
void ListTool::getSelectedItems( wxListCtrl* listCtrl, std::vector< int >& items ) {
	// check the list
	if( listCtrl == NULL ) {
		return;
	}

	// get selected item
	long item = -1;
	do {
		item = listCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
		if( item >= 0 && item < listCtrl->GetItemCount() ) {
			items.push_back( item );
		}
	} while( item >= 0 );
}

// select all items
void ListTool::selectAll( wxListCtrl* listCtrl ) {
	// select all
	for( int i = 0; i < listCtrl->GetItemCount(); i++ ) {
		listCtrl->SetItemState( i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
	}
}

// unselect all items
void ListTool::unselectAll( wxListCtrl* listCtrl ) {
	// select all
	for( int i = 0; i < listCtrl->GetItemCount(); i++ ) {
		listCtrl->SetItemState( i, 0, wxLIST_STATE_SELECTED );
	}
}

// judges whether the list item is selected
bool ListTool::isSelected( wxListCtrl* listCtrl, int item ) {
	// check parameters
	if( listCtrl == NULL || item < 0 || item >= listCtrl->GetItemCount() ) {
		return false;
	}

	// get state
	if( listCtrl->GetItemState( item, wxLIST_STATE_SELECTED ) ) {
		return true;
	}
	return false;
}

// select item
void ListTool::selectItem( wxListCtrl* listCtrl, int item ) {
	// check parameters
	if( listCtrl == NULL || item < 0 || item >= listCtrl->GetItemCount() ) {
		return;
	}

	// select
	listCtrl->SetItemState( item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
}

// unselect item
void ListTool::unselectItem( wxListCtrl* listCtrl, int item ) {
	// check parameters
	if( listCtrl == NULL || item < 0 || item >= listCtrl->GetItemCount() ) {
		return;
	}

	// select
	listCtrl->SetItemState( item, 0, wxLIST_STATE_SELECTED );
}
