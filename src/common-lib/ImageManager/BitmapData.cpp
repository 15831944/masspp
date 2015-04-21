/**
 * @file BitmapData.cpp
 * @brief implements of BitmapData class
 *
 * @author S.Tanaka
 * @date 2006.07.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"

#include "BitmapData.h"

using namespace kome::img;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
BitmapData::BitmapData() {
	m_bitmap = NULL;
	m_width = 0;
	m_height = 0;
}

// constructor
BitmapData::BitmapData( unsigned int width, unsigned int height ) {
	m_bitmap = NULL;
	m_width  = width;
	m_height = height;

	createArray();
}

// copy constructor
BitmapData::BitmapData( const BitmapData& bitmapData ) {
	m_bitmap = NULL;
	m_width = bitmapData.m_width;
	m_height = bitmapData.m_height;

	createArray();

	// copy data
	if( m_bitmap != NULL && bitmapData.m_bitmap != NULL ) {
		memcpy( m_bitmap, bitmapData.m_bitmap, sizeof( COLOR ) * m_width * m_height );
	}	
}

// destructor
BitmapData::~BitmapData(void){
	deleteArray();
}

// get width
unsigned int BitmapData::getWidth() {
	return m_width;
}

// get height
unsigned int BitmapData::getHeight() {
	return m_height;
}

// resize
void BitmapData::resize( const unsigned int width, const unsigned int height ) {
	m_width = width;
	m_height = height;
	createArray();
}

// set pixel color
void BitmapData::setPixel( const unsigned int x, const unsigned int y, const COLOR color ) {
	// get the index of the array
	int index = getIndexOfArray( x, y );
	if( index < 0 ) {
		LOG_WARN( FMT( "Coordinates are illegal." ) );
		return;
	}

	// set
	m_bitmap[index] = color;
}

// get pixel color
COLOR BitmapData::getPixel( const unsigned int x, const unsigned int y ) {
	// get the index of the array
	int index = getIndexOfArray( x, y );
	if( index < 0 ) {
		LOG_WARN( FMT( "Coordinates are illegal." ) );
		return ColorTool::BLACK;
	}

	// get
	return m_bitmap[index];
}

// create bitmap data array
void BitmapData::createArray() {
	LOG_TRACE( FMT( "creating the array of the bitmap data." ) );

	// delete array
	if( m_bitmap != NULL ) {
		deleteArray();
	}

	// check size
	if( m_width == 0 || m_height == 0 ) {
		return;
	}

	// create array
	unsigned int size = m_width * m_height;
	m_bitmap = new COLOR[ size ];

	// initialize
	fillzero( m_bitmap, size * sizeof( COLOR ) );
}

// delete bitmap data array
void BitmapData::deleteArray() {
	delete[] m_bitmap;
	m_bitmap = NULL;
}

// get the index of the array
int BitmapData::getIndexOfArray( unsigned int x, unsigned int y ) {
	// check parameter
	if( x >= m_width ) {
		LOG_WARN( FMT( "x must be less than width. [x=%ud, width=%ud]", x, m_width ) );
		return -1;
	}
	if( y >= m_height ) {
		LOG_WARN( FMT( "y must be less than height. [y=%ud, height=%ud]", y, m_height ) );
		return -1;
	}

	return ( y * m_width + x );
}
