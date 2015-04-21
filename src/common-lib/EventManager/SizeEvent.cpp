/**
 * @file SizeEvent.cpp
 * @brief implements of SizeEvent class
 *
 * @author S.Tanaka
 * @date 2007.09.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SizeEvent.h"


using namespace kome::evt;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// cosntructor
SizeEvent::SizeEvent() {
	m_width =0;
	m_height = 0;
}

SizeEvent::SizeEvent( const unsigned int w, const unsigned int h ) {
	m_width = w;
	m_height = h;
}

// destructor
SizeEvent::~SizeEvent() {
}

// set width
void SizeEvent::width( const unsigned int w ) {
	m_width = w;
}

// get width
unsigned int SizeEvent::width() {
	return m_width;
}

// set height
void SizeEvent::height( const unsigned int h ) {
	m_height = h;
}

// get height
unsigned int SizeEvent::height() {
	return m_height;
}
