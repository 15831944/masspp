/**
 * @file Selector.cpp
 * @brief implements of Selector class
 *
 * @author S.Tanaka
 * @date 2007.12.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "Selector.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Selector::Selector() {
	m_sel = -1;
}

// destructor
Selector::~Selector() {
}

// clear items
void Selector::clearItems() {
	m_items.clear();
	m_selected.clear();
	m_sel = -1;
}

// add item
void Selector::addItem( const char* name ) {
	addItem( name, name );
}

// add item
void Selector::addItem( const char* name, const char* value ) {
	m_items.push_back( std::make_pair( NVL( name, "" ), NVL( value, "" ) ) );
	m_selected.push_back( false );
}

// get the number of items
unsigned int Selector::getNumberOfItems() {
	return m_items.size();
}

// get item name
const char* Selector::getItemName( const unsigned int index ) {
	if( index >= m_items.size() ) {
		return NULL;
	}
	return m_items[ index ].first.c_str();
}

// get item value
const char* Selector::getItemValue( const unsigned index ) {
	if( index >= m_items.size() ) {
		return NULL;
	}
	return m_items[ index ].second.c_str();
}

// get selected flag value
bool Selector::isSelected( const unsigned int index ) {
	if( index >= m_selected.size() ) {
		return false;
	}
	return m_selected[ index ];
}

// get selected item
int Selector::getSelection() {
	if( m_sel < 0 ) {
		for( unsigned int i = 0; i < m_selected.size() && m_sel < 0; i++ ) {
			if( m_selected[ i ] ) {
				m_sel = i;
			}
		}
	}

	return m_sel;
}

// select item
bool Selector::select() {
	// initialize
	m_sel = -1;

	// create array
	const char** items = NULL;
	const char** values = NULL;
	bool* selected = NULL;
	unsigned int num = m_items.size();
	if( num > 0 ) {
		items = new const char*[ num ];
		values = new const char*[ num ];
		selected = new bool[ num ];

		for( unsigned int i = 0; i < num; i++ ) {
			items[ i ] = m_items[ i ].first.c_str();
			values[ i ] = m_items[ i ].second.c_str();
			selected[ i ] = m_selected[ i ];
		}
	}

	// select
	bool ret = onSelect( num, items, values, selected );
	for( unsigned int i = 0; i < num; i++ ) {
		m_selected[ i ] = selected[ i ];
	}

	// delete
	if( items != NULL ) {
		delete[] items;
	}
	if( values != NULL ) {
		delete[]  values;
	}
	if( selected != NULL ) {
		delete[] selected;
	}

	return ret;
}
