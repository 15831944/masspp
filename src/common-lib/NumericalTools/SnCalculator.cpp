/**
 * @file SnCalculator.cpp
 * @brief implements of SnCalculator class
 *
 * @author S.Tanaka
 * @date 2014.01.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SnCalculator.h"


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SnCalculator::SnCalculator() {
	m_xyData = NULL;
}

// destructor
SnCalculator::~SnCalculator() {
}

// set data points
void SnCalculator::setXYData( kome::core::XYData& xyData ) {
	m_xyData = &xyData;
	onSetXYData( xyData );
}

// get S/N ratio
double SnCalculator::getSn( const double x ) {
	_ASSERT( m_xyData != NULL );
	return onGetSn( x );
}
