/**
 * @file KeyEvent.cpp
 * @brief implements of KeyEvent class
 *
 * @author S.Tanaka
 * @date 2007.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "KeyEvent.h"


using namespace kome::evt;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
KeyEvent::KeyEvent() {
	m_code = 0;
	m_shift = false;
	m_ctrl = false;
	m_alt = false;
}

// destructor
KeyEvent::~KeyEvent() {
}

// set key code
void KeyEvent::setCode( const int code ) {
	m_code = code;
}

// get code
int KeyEvent::getCode() {
	return m_code;
}

// set shift key flag
void KeyEvent::shift( const bool s ) {
	m_shift = s;
}

// get shift key flag
bool KeyEvent::shift() {
	return m_shift;
}

// set control key flag
void KeyEvent::ctrl( const bool c ) {
	m_ctrl = c;
}

// get control key flag
bool KeyEvent::ctrl() {
	return m_ctrl;
}

// set alt key flag
void KeyEvent::alt( const bool a ) {
	m_alt = a;
}

// get alt key flag
bool KeyEvent::alt() {
	return m_alt;
}
