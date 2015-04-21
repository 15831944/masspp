/**
 * @file CheckGridListCtrl.cpp
 * @brief implements of CheckGridListCtrl class
 *
 * @author S.Tanaka
 * @date 2011.04.27
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"

#include "CheckGridListCtrl.h"
#include "checked_box_16x16.xpm"
#include "unchecked_box_16x16.xpm"

#include "CheckGridListPanel.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( CheckGridListCtrl, GridListCtrl )
	EVT_LIST_ITEM_SELECTED( wxID_ANY, CheckGridListCtrl::onSelectItem )
	EVT_LIST_ITEM_DESELECTED( wxID_ANY, CheckGridListCtrl::onDeselectItem )
END_EVENT_TABLE()


// constructor
CheckGridListCtrl::CheckGridListCtrl(
		wxWindow* parent,
		const int id,
		const int width,
		const int height,
		const bool editable
) : GridListCtrl( parent, id, width, height, editable ),
        m_checkedIcon( checked_box_16x16_xpm ), m_uncheckedIcon( unchecked_box_16x16_xpm ) {
	// image list
	wxImageList* il = new wxImageList( 16, 16 );
	il->Add( m_uncheckedIcon );
	il->Add( m_checkedIcon );

	AssignImageList( il, wxIMAGE_LIST_SMALL );

	// default attribute
	m_defaultAttr.SetBackgroundColour( *wxWHITE );
	m_defaultAttr.SetTextColour( *wxBLACK );

	// auto column width
	setAutoColumnWidth( true );

	// set check grid list
	CheckGridListPanel* panel = dynamic_cast< CheckGridListPanel* >( parent );
	if( panel != NULL ) {
		panel->setCheckGridListCtrl( this );
	}

	// popup
	setPopupMenuFlag( 0 );
}

// destructor
CheckGridListCtrl::~CheckGridListCtrl() {
}

// set selected
void CheckGridListCtrl::setSelected( const long long data, const bool selected ) {
	// insert or erase data
	if( selected ) {
		m_selectedDataSet.insert( data );
	}
	else {
		m_selectedDataSet.erase( data );
	}

	// number of selected items
	CheckGridListPanel* panel = dynamic_cast< CheckGridListPanel* >( GetParent() );
	if( panel != NULL ) {
		int cnt = 0;
		int num = GetItemCount();
		for( int i = 0; i < num; i++ ) {
			long long tmp = getData( i );
			if( m_selectedDataSet.find( tmp ) != m_selectedDataSet.end() ) {
				cnt++;
			}
		}
		panel->setNumberOfSelectedItems( cnt );
	}
}

// selected or not
bool CheckGridListCtrl::isSelected( const long long data ) {
	return ( m_selectedDataSet.find( data ) != m_selectedDataSet.end() );
}

// get selected data
void CheckGridListCtrl::getSelectedData( std::vector< long long >& selectedData ) {
	for( unsigned int i = 0; i < m_data.size(); i++ ) {
		const long long data = m_data[ i ];
		if( isSelected( data ) ) {
			selectedData.push_back( data );
		}
	}
}

// select all items
void CheckGridListCtrl::selectAll() {
	for( unsigned int i = 0; i < m_data.size(); i++ ) {
		m_selectedDataSet.insert( m_data[ i ] );
	}
	updateList();		// SPEC No.85263 @date 2012.04.25 <Add>M.Izumi

	// update panel
	if( m_selectedDataSet.size() > 0 ) {
		setSelected( *m_selectedDataSet.begin() );
	}
	else {
		setSelected( 0, false );
	}

	// for override
	onSelectAll();
}

// deselect all items
void CheckGridListCtrl::deselectAll() {
	m_selectedDataSet.clear();
	updateList();		// SPEC No.85263 @date 2012.04.25 <Add>M.Izumi

	// update panel
	setSelected( 0, false );

	// for override
	onDeselectAll();
}

// on select all items
void CheckGridListCtrl::onSelectAll(){
	return;
}

// on deselect all items
void CheckGridListCtrl::onDeselectAll(){
	return;
}

// on get item attribute
wxListItemAttr* CheckGridListCtrl::OnGetItemAttr( long item ) const {
	// item
	wxListItemAttr* attr = GridListCtrl::OnGetItemAttr( item );
	if( attr == NULL ) {
		attr = const_cast< wxListItemAttr* >( &m_defaultAttr );
	}
	

	return attr;
}

// on get item image
int CheckGridListCtrl::OnGetItemImage( long item ) const {
	// data
	const long long data = m_data[ item ];

	// image
	return ( m_selectedDataSet.find( data ) == m_selectedDataSet.end() ? 0 : 1 );
}

// on selected item
void CheckGridListCtrl::onSelectItem( wxListEvent& evt ) {
	// item
	const int item = evt.GetIndex();

	// add data
	const long long data = m_data[ item ];
	const bool selected = isSelected( data );
	setSelected( data, !selected );

	// deselect
	SetItemState( item, 0, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED );

	// update button
	CheckGridListPanel* panel = dynamic_cast< CheckGridListPanel* >( GetParent() );

	// for override
	onSelectItem( item );
}

// on selected item
void CheckGridListCtrl::onSelectItem( const int item ){
	return;
}

// on deselected item
void CheckGridListCtrl::onDeselectItem( wxListEvent& evt ) {
	// item
	const int item = evt.GetIndex();
}
