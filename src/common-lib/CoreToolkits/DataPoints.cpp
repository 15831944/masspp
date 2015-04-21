/**
 * @file DataPoints.cpp
 * @brief implements of DataPoints class
 *
 * @author S.Tanaka
 * @date 2006.09.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataPoints.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define FIRST_ARRAY_SIZE 256


// constructor
DataPoints::DataPoints( ArrayType type ) : m_type( type ) {
	// initialize
	if( type == FLOAT ) {
		m_xArray.fltarry = NULL;
		m_yArray.fltarry = NULL;
	}
	else{
		m_xArray.dblarry = NULL;
		m_yArray.dblarry = NULL;
	}

	// clear
	onClearPoints();
}

// copy constructor
DataPoints::DataPoints( const DataPoints& dps ) : kome::core::XYData( dps ) {
	// initialize
	m_type = dps.m_type;
	m_length = dps.m_length;
	m_arraySize = dps.m_arraySize;

	// create array
	if( m_arraySize == 0 ) {		// empty array
		if( m_type == FLOAT ) {		// float
			m_xArray.fltarry = NULL;
			m_yArray.fltarry = NULL;
		}
		else {		// double
			m_xArray.dblarry = NULL;
			m_yArray.dblarry = NULL;
		}
	}
	else {
		if( m_type == FLOAT ) {		// float
			m_xArray.fltarry = new float[ m_arraySize ];
			memcpy( m_xArray.fltarry, dps.m_xArray.fltarry, sizeof( float ) * m_arraySize );

			m_yArray.fltarry = new float[ m_arraySize ];
			memcpy( m_yArray.fltarry, dps.m_yArray.fltarry, sizeof( float ) * m_arraySize );
		}
		else {		// double
			m_xArray.dblarry = new double[ m_arraySize ];
			memcpy( m_xArray.dblarry, dps.m_xArray.dblarry, sizeof( double ) * m_arraySize );

			m_yArray.dblarry = new double[ m_arraySize ];
			memcpy( m_yArray.dblarry, dps.m_yArray.dblarry, sizeof( double ) * m_arraySize );
		}			
	}
}

// destructor
DataPoints::~DataPoints() {
	onClearPoints();
}

// substitution operator
DataPoints& DataPoints::operator=( const DataPoints& other ) {
	// initialize
	m_type = other.m_type;
	m_length = other.m_length;
	m_arraySize = other.m_arraySize;

	// create array
	if( m_arraySize == 0 ) {		// empty array
		if( m_type == FLOAT ) {		// float
			m_xArray.fltarry = NULL;
			m_yArray.fltarry = NULL;
		}
		else {		// double
			m_xArray.dblarry = NULL;
			m_yArray.dblarry = NULL;
		}
	}
	else {
		if( m_type == FLOAT ) {		// float
			m_xArray.fltarry = new float[ m_arraySize ];
			memcpy( m_xArray.fltarry, other.m_xArray.fltarry, sizeof( float ) * m_arraySize );

			m_yArray.fltarry = new float[ m_arraySize ];
			memcpy( m_yArray.fltarry, other.m_yArray.fltarry, sizeof( float ) * m_arraySize );
		}
		else {		// double
			m_xArray.dblarry = new double[ m_arraySize ];
			memcpy( m_xArray.dblarry, other.m_xArray.dblarry, sizeof( double ) * m_arraySize );

			m_yArray.dblarry = new double[ m_arraySize ];
			memcpy( m_yArray.dblarry, other.m_yArray.dblarry, sizeof( double ) * m_arraySize );
		}			
	}
	return *this;
}

// get x data array
void* DataPoints::getXData() {
	if( m_type == FLOAT ) {
		return m_xArray.fltarry;
	}
	return m_xArray.dblarry;
}

// get y data array
void* DataPoints::getYData() {
	if( m_type == FLOAT ) {
		return m_yArray.fltarry;
	}
	return m_yArray.dblarry;
}

// clear data points
void DataPoints::onClearPoints() {
	// delete array
	if( m_type == FLOAT ) {
		if( m_xArray.fltarry != NULL ) {
			delete[] m_xArray.fltarry;
		}
		if( m_yArray.fltarry != NULL ) {
			delete[] m_yArray.fltarry;
		}
		m_xArray.fltarry = NULL;
		m_yArray.fltarry = NULL;
	}
	else {
		if( m_xArray.dblarry != NULL ) { 
			delete[] m_xArray.dblarry;
		}
		if( m_yArray.dblarry != NULL ) {
			delete[] m_yArray.dblarry;
		}
		m_xArray.dblarry = NULL;
		m_yArray.dblarry = NULL;
	}

	// set size
	m_arraySize = 0;
	m_length = 0;
}

// add point
void DataPoints::onAddPoint( const double x, const double y ) {
	// check array size
	if( m_length >= m_arraySize ) {
		changeSize( MAX( FIRST_ARRAY_SIZE, m_arraySize * 2 ) );
	}

	// store data
	if( m_type == FLOAT ) {	// float
		m_xArray.fltarry[ m_length ] = (float)x;
		m_yArray.fltarry[ m_length ] = (float)y;
	}
	else {	// double
		m_xArray.dblarry[ m_length ] = x;
		m_yArray.dblarry[ m_length ] = y;
	}

	// update length
	m_length++;
}

// insert point
void DataPoints::onInsertPoint( const unsigned int index, const double x, const double y ) {
	// check array size
	if( m_length >= m_arraySize ) {
		changeSize( MAX( FIRST_ARRAY_SIZE, m_arraySize * 2 ) );
	}

	// insert
	int idx = (int)MIN( index, m_length );
	if( m_type == FLOAT ) { // float
		// move
		for( int i = (int)m_length  - 1; i >= idx; i-- ) {
			m_xArray.fltarry[ i + 1 ] = m_xArray.fltarry[ i ];
			m_yArray.fltarry[ i + 1 ] = m_yArray.fltarry[ i ];
		}

		// insert
		m_xArray.fltarry[ idx ] = (float)x;
		m_yArray.fltarry[ idx ] = (float)y;
	}
	else {  // double
		// move
		for( int i = (int)m_length - 1; i >= idx; i-- ) {
			m_xArray.dblarry[ i + 1 ] = m_xArray.dblarry[ i ];
			m_yArray.dblarry[ i + 1 ] = m_yArray.dblarry[ i ];
		}

		// insert
		m_xArray.dblarry[ idx ] = x;
		m_yArray.dblarry[ idx ] = y;
	}

	// update length
	m_length++;
}

// delete point
void DataPoints::onDeletePoint( const unsigned int index ) {
	// length
	m_length--;

	// delete
	if( m_type == FLOAT ) { // float
		for( unsigned int i = index; i < m_length; i++ ) {
			m_xArray.fltarry[ i ] = m_xArray.fltarry[ i + 1 ];
			m_yArray.fltarry[ i ] = m_yArray.fltarry[ i + 1 ];
		}
	}
	else {  // double
		for( unsigned int i = index; i < m_length; i++ ) {
			m_xArray.dblarry[ i ] = m_xArray.dblarry[ i + 1 ];
			m_yArray.dblarry[ i ] = m_yArray.dblarry[ i + 1 ];
		}
	}
}

// get the number of points
unsigned int DataPoints::onGetLength() {
	return m_length;
}

// get x
double DataPoints::onGetX( const unsigned int index ) {
	if( m_type == FLOAT ) {
		return (double)m_xArray.fltarry[ index ];
	}
	return m_xArray.dblarry[ index ];
}

// get y
double DataPoints::onGetY( const unsigned int index ) {
	if( m_type == FLOAT ) {
		return (double)m_yArray.fltarry[ index ];
	}
	return m_yArray.dblarry[ index ];
}

// reserve
void DataPoints::onReserve( const unsigned int num ) {
	// necessary size
	unsigned size = m_length + num;

	// change size
	if( size > m_arraySize ) {
		changeSize( size );
	}
}

// change the size of data array
void DataPoints::changeSize( const unsigned int size ) {
	// check size
	if( m_arraySize == size ) {
		return;
	}
	if( size == 0 ) {
		onClearPoints();
		return;
	}

	// the number of points to be copied
	unsigned copyNum = MIN( size, m_length );

	// create new array and copy data
	if( m_type == FLOAT ) {	// float
		// create array
		float* xArray = new float[ size ];
		float* yArray = new float[ size ];

		// copy
		if( copyNum > 0 ) {
			unsigned int copySize = copyNum * sizeof( float );
			memcpy( xArray, m_xArray.fltarry, copySize );
			memcpy( yArray, m_yArray.fltarry, copySize );
		}

		// delete
		if( m_xArray.fltarry != NULL ) {
			delete[] m_xArray.fltarry;
		}
		m_xArray.fltarry = xArray;

		if( m_yArray.fltarry != NULL ) {
			delete[] m_yArray.fltarry;
		}
		m_yArray.fltarry = yArray;
	}
	else {	// double
		// create array
		double* xArray = new double[ size ];
		double* yArray = new double[ size ];

		// copy
		if( copyNum > 0 ) {
			unsigned int copySize = copyNum * sizeof( double );
			memcpy( xArray, m_xArray.dblarry, copySize );
			memcpy( yArray, m_yArray.dblarry, copySize );
		}

		// delete
		if( m_xArray.dblarry != NULL ) {
			delete[] m_xArray.dblarry;
		}
		m_xArray.dblarry = xArray;

		if( m_yArray.dblarry != NULL ) {
			delete[] m_yArray.dblarry;
		}
		m_yArray.dblarry = yArray;
	}

	// array size
	m_arraySize = size;
}
