/**
 * @file PointArray.cpp
 * @brief implements of PointArray class
 *
 * @author S.Tanaka
 * @date 2008.07.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PointArray.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PointArray::PointArray() {
}

// destructor
PointArray::~PointArray() {
}

// sort by x
void PointArray::sortByX( const bool desc ) {
	// check the array size
	if( m_points.size() <= 1 ) {
		return;
	}

	// sort
	std::sort(
		m_points.begin(),
		m_points.end(),
		desc ? Point< double >::greaterX : Point< double >::lessX
	);
}

// sort by y
void PointArray::sortByY( const bool desc ) {
	// check the array size
	if( m_points.size() <= 1 ) {
		return;
	}

	// sort
	std::sort(
		m_points.begin(),
		m_points.end(),
		desc ? Point< double >::greaterY : Point< double >::lessY
	);	
}

// on clear points
void PointArray::onClearPoints() {
	m_points.clear();
}

// on add point
void PointArray::onAddPoint( const double x, const double y ) {
	Point< double > pt( x, y );
	m_points.push_back( pt );
}

// on insert point
void PointArray::onInsertPoint( const unsigned int index, const double x, const double y ) {
	// check the parameter
	if( index >= m_points.size() ) {
		onAddPoint( x, y );
		return;
	}

	// position
	std::vector< Point< double > >::iterator it = m_points.begin() + index;

	// insert
	Point< double > pt( x, y );
	m_points.insert( it, pt );
}

// delete point
void PointArray::onDeletePoint( const unsigned int index ) {
	// check the parameter
	if( index >= m_points.size() ) {
		return;
	}

	// delete
	std::vector< Point< double > >::iterator it = m_points.begin() + index;
	m_points.erase( it );
}

// on get length
unsigned int PointArray::onGetLength() {
	return m_points.size();
}

// on get x
double PointArray::onGetX( const unsigned int index ) {
	return m_points[ index ].px;
}

// on get y
double PointArray::onGetY( const unsigned int index ) {
	return m_points[ index ].py;
}

// on reserve
void PointArray::onReserve( const unsigned int num ) {
	if( num > 0 ) {
		m_points.reserve( num );
	}
}
