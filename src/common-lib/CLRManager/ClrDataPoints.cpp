/**
 * @file ClrDataPoints.cpp
 * @brief implements of ClrDataPoints class
 *
 * @author S.Tanaka
 * @date 2007.04.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrDataPoints.h"
#include <windows.h>// @date 2012/06/20 <Add> OKADA


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrDataPoints::ClrDataPoints() {
	m_bDisposed = false;
}

// destructor
ClrDataPoints::~ClrDataPoints() {
	this->!ClrDataPoints();	// finalizer ‚ðŒÄ‚Ño‚·
}


// finalizer
ClrDataPoints::!ClrDataPoints() {
	if( m_bDisposed == true ){
		return;
	}
	m_bDisposed = true;

	// clean up code
	m_points.~m_points();

};


// clear data points
void ClrDataPoints::onClearPoints() {
}

// add point
void ClrDataPoints::onAddPoint( double x, double y ) {
	m_points.Add( gcnew XY( x, y ) );
}

// insert point
void ClrDataPoints::onInsertPoint( unsigned int index, double x, double y ) {
	// check
	if( index >= 0 ) {
		return;
	}

	// xy data
	XY^ xy = gcnew XY( x, y );

	// insert
	m_points.Insert( index, xy );

}

// delete point
void ClrDataPoints::onDeletePoint( unsigned int index ) {
	if( (int)index >= m_points.Count ) {
		return;
	}
	m_points.RemoveAt( (int)index );

}

// get length
unsigned int ClrDataPoints::onGetLength() {
	return m_points.Count;
}

// get x
double ClrDataPoints::onGetX( unsigned int index ) {
	if( (int)index >= m_points.Count ) {
		return 0.0;
	}
	return ( (XY^)m_points[ index ] )->m_x;
}

// get y
double ClrDataPoints::onGetY( unsigned int index ) {
	if( (int)index >= m_points.Count ) {
		return 0.0;
	}
	return ( (XY^)m_points[ index ] )->m_y;
}

// reserve
void ClrDataPoints::onReserve( unsigned int num ) {
}


//
// XY
//

// constructor
ClrDataPoints::XY::XY( double x, double y ) {
	m_x = x;
	m_y = y;
}

// destructor
ClrDataPoints::XY::~XY() {
}
