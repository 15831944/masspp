/**
 * @file ManualSelector.cpp
 * @brief implements of ManualSelector class
 *
 * @author S.Tanaka
 * @date 2008.01.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ManualSelector.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ManualSelector::ManualSelector() {
}

// destructor
ManualSelector::~ManualSelector() {
}

// set selection status
void ManualSelector::setSelected( const int item, const bool selected ) {
	// check the item
	if( item < 0 ) {
		return;
	}

	// set status
	if( selected ) {
		m_selectedItemSet.insert( item );
	}
	else {
		m_selectedItemSet.erase( item );
	}
}

// on select
bool ManualSelector::onSelect( const unsigned int num, const char** items, const char** values, bool* selected ) {
	// initialize
	for( unsigned int i = 0; i < num; i++ ) {
		selected[ i ] = false;
	}

	// get selection
	for( std::set< int >::iterator it = m_selectedItemSet.begin();
			it != m_selectedItemSet.end(); it++ ) {
		int item = *it;
		if( item >= 0 && item < (int)num ) {
			selected[ item ] = true;
		}
	}

	return true;
}
