/**
 * @file DialogSelector.cpp
 * @brief implements of DialogSelector class
 *
 * @author S.Tanaka
 * @date 2008.01.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DialogSelector.h"

#include "WindowTool.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE			  3


// constructor
DialogSelector::DialogSelector( wxWindow* parent, const char* title, const bool multiple, const bool required ) {
	m_parent = parent;
	m_title = NVL( title, "" );
	m_multiple = multiple;
	m_required = required;
}

// destructor
DialogSelector::~DialogSelector() {
}

// on select
bool DialogSelector::onSelect(
		const unsigned int num,
		const char** items,
		const char** values,
		bool* selected
) {
	// dialog
	ComboDialog dlg( m_parent, m_title.c_str(), num, items, m_multiple, m_required );
	if( dlg.ShowModal() == wxID_OK ) {
		for( unsigned int i = 0; i < num; i++ ) {
			selected[ i ] = dlg.isSelected( i );
		}

		return true;
	}
	return false;
}


/////// ComboDialog ///////

// constructor
DialogSelector::ComboDialog::ComboDialog(
		wxWindow* parent,
		const char* title,
		const unsigned int num,
		const char** items,
		const bool multiple,
		const bool required ) : StandardDialog( parent, title ) {
	m_num = num;
	m_items = items;
	m_multiple = multiple;
	m_required = required;

	m_ctrl = NULL;

	m_selected = ( num == 0 ) ? NULL : new bool[ num ];
	for( unsigned int i = 0; i < num; i++ ) {
		m_selected[ i ] = false;
	}
}

// destructor
DialogSelector::ComboDialog::~ComboDialog() {
	if( m_selected != NULL ) {
		delete[] m_selected;
		m_selected = NULL;
	}
}

// judges wheter specified item is selected
bool DialogSelector::ComboDialog::isSelected( const int item ) {
	if( item < 0 || item >= (int)m_num ) {
		return false;
	}
	return m_selected[ item ];
}

// creates main sizer
wxSizer* DialogSelector::ComboDialog::createMainSizer() {
	// items
	std::vector< wxString > items;

	if( !m_multiple && !m_required ) {
		items.push_back( wxT( "(None)" ) );
	}

	for( unsigned int i = 0; i < m_num; i++ ) {
		items.push_back( wxT( m_items[ i ] ) );
	}

	// create control
	if( m_multiple ) {		// list box
		m_ctrl = new wxListBox(
			this,
			wxID_ANY,
			wxDefaultPosition,
			wxDefaultSize,
			(int)items.size(),
			items.size() == 0 ? NULL : &( items[ 0 ] ),
			wxLB_EXTENDED | wxLB_HSCROLL | wxLB_NEEDED_SB
		);
	}
	else {  // combo box
		m_ctrl = new wxComboBox(
			this,
			wxID_ANY,
			wxEmptyString,
			wxDefaultPosition,
			wxDefaultSize,
			(int)items.size(),
			items.size() == 0 ? NULL : &( items[ 0 ] ),
			wxCB_SIMPLE | wxCB_READONLY
		);

		if( m_num > 0 ) {
		  ( (wxComboBox*)m_ctrl )->SetSelection( 0 );
		}
	}

	if( m_num == 0 ) {
		m_ctrl->Enable( false );
	}

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	sizer->Add( m_ctrl, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// transfer data from window
bool DialogSelector::ComboDialog::TransferDataFromWindow() {
	// get slection
	std::vector< int > items;
	if( m_multiple ) {		// list box
		wxListBox* listBox = (wxListBox*)m_ctrl;
		for( unsigned int i = 0; i < listBox->GetCount(); i++ ) {
			if( listBox->IsSelected( i ) ) {
				items.push_back( i );
			}
		}
	}
	else {  // combo box
		int item = ( (wxComboBox*)m_ctrl )->GetSelection();
		if( !m_required ) {
			item--;
		}
		if( item >= 0 ) {
			items.push_back( item );
		}
	}

	// check
	if( m_required && items.size() == 0 ) {
		WindowTool::showInfo( "No items are selected. Select at least one item." );
		return false;
	}

	// store
	for( unsigned int i = 0; i < items.size(); i++ ) {
		m_selected[ items[ i ] ] = true;
	}

	return true;
}
