/**
 * @file SizeEventWrapper.cpp
 * @brief implements of SizeEventWrapper class
 *
 * @author S.Tanaka
 * @date 2007.09.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SizeEventWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SizeEventWrapper::SizeEventWrapper( kome::evt::SizeEvent& evt )
		: m_evt( evt ){
}

// destructor
SizeEventWrapper::~SizeEventWrapper() {
}

// get size event object
kome::evt::SizeEvent& SizeEventWrapper::getSizeEvent() {
	return m_evt;
}

// set width
void SizeEventWrapper::width( unsigned int w ) {
	m_evt.width( w );
}

// get width
unsigned int SizeEventWrapper::width() {
	return m_evt.width();
}

// set height
void SizeEventWrapper::height( unsigned int h ) {
	m_evt.height( h );
}

// get height
unsigned int SizeEventWrapper::height() {
	return m_evt.height();
}
