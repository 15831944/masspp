/**
 * @file BitmapDataWrapper.cpp
 * @brief implements of BitmapDataWrapper class
 *
 * @author S.Tanaka
 * @date 2007.09.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BitmapDataWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
BitmapDataWrapper::BitmapDataWrapper( kome::img::BitmapData& bitmap ) : m_bitmap( bitmap ) {
}

// destructor
BitmapDataWrapper::~BitmapDataWrapper() {
}

// get bitmap data
kome::img::BitmapData& BitmapDataWrapper::getBitmapData() {
	return m_bitmap;
}

// get width
unsigned int BitmapDataWrapper::getWidth() {
	return m_bitmap.getWidth();
}

// get height
unsigned int BitmapDataWrapper::getHeight() {
	return m_bitmap.getHeight();
}

// resize
void BitmapDataWrapper::resize( unsigned int width, unsigned int height ) {
	m_bitmap.resize( width, height );
}

// set pixel
void BitmapDataWrapper::setPixel( unsigned int x, unsigned int y, COLOR color ) {
	m_bitmap.setPixel( x, y, color );
}

// get pixel
COLOR BitmapDataWrapper::getPixel( unsigned int x, unsigned int y ) {
	return m_bitmap.getPixel( x, y );
}
