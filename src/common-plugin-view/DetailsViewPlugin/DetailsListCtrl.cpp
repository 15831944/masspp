/**
 * @file DetailsListCtrl.cpp
 * @brief implements of DetailsListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.02.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DetailsListCtrl.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( DetailsListCtrl, kome::window::GridListCtrl )
	EVT_LIST_COL_CLICK( wxID_ANY, DetailsListCtrl::onClickCol )
END_EVENT_TABLE()


// constructor
DetailsListCtrl::DetailsListCtrl( wxWindow* parent ) 
		: kome::window::GridListCtrl( parent, wxID_ANY, 200, 300, false ) {
	// create list ctrl
	create();

	// Context Menu
	setPopupMenuFlag( POPUP_SELECT | POPUP_DESELECT | POPUP_COPY | POPUP_FILTER | POPUP_SAVE );
}

// destructor
DetailsListCtrl::~DetailsListCtrl() {
}

// update 
void DetailsListCtrl::onCreate() {
	// insert column
	addColumn( "Key", TYPE_STRING );
	addColumn( "Value", TYPE_STRING );

	// add data
	for( unsigned int i = 0; i < m_properties.size(); i++ ) {
		addData( i, false );
	}
}

// set properties
void DetailsListCtrl::setProperties( kome::core::Properties& props, kome::core::Properties& userProps ) {
	// clear
	m_properties.clear();

	// get properties
	m_properties.reserve( props.getNumberOfProperties() + userProps.getNumberOfProperties() );
	for( unsigned int i = 0; i < props.getNumberOfProperties(); i++ ) {
		m_properties.push_back( std::make_pair( props.getKey( i ), props.getValue( i ) ) );
	}
	for( unsigned int j = 0; j < userProps.getNumberOfProperties(); j++ ) {
		char sModifiedKey[200];
		_snprintf(sModifiedKey, 200, "*%s", userProps.getKey( j ));

		m_properties.push_back( std::make_pair( sModifiedKey, userProps.getValue( j ) ) );
	}

	// update list
	create();
}


// get string
std::string DetailsListCtrl::getString( const long long data, const int column ) const {
	std::string s;
	if( data >= 0 && data < (int)m_properties.size() ) {
		if( column == 0 ) {
			s = m_properties[ data ].first;
		}
		else if( column == 1 ) {
			s = m_properties[ data ].second;
		}
	}

	return s;
}

// Stop the list control from sorting its contents when the second column is clicked.
void DetailsListCtrl::onClickCol( wxListEvent& evt )
{
	evt.Veto();
}
