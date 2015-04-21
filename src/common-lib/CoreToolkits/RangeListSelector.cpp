/**
 * @file RangeListSelector.cpp
 * @brief implements of RangeListSelector class
 *
 * @author S.Tanaka
 * @date 2009.01.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "RangeListSelector.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
RangeListSelector::RangeListSelector( const RangeList::DefaultType dflt, const RangeType type )
		: m_type( type ), m_ranges( dflt ) {
}

// destructor
RangeListSelector::~RangeListSelector() {
}

// get range list object
RangeList& RangeListSelector::getRangeList() {
	return m_ranges;
}

// on select
bool RangeListSelector::onSelect(
		const unsigned int num,
		const char** items,
		const char** values,
		bool* selected
) {
	// each items
	for( unsigned int i = 0; i < num; i++ ) {
		// get selected
		bool sel = false;

		if( m_type == RANGE_INDEX ) {	// index
			sel = m_ranges.checkIntValue( i );
		}
		else if( m_type == RANGE_INT_VALUE ) {	// integer value
			int v = int();
			if( isint( values[ i ], 10, &v ) ) {
				sel = m_ranges.checkIntValue( v );
			}
		}
		else if( m_type == RANGE_DOUBLE_VALUE ) {	// double value
			double v = double();
			if( isdouble( values[ i ], &v ) ) {
				sel = m_ranges.checkDoubleValue( v );
			}
		}

		selected[ i ] = sel;
	}

	return true;
}
