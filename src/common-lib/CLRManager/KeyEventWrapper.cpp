/**
 * @file KeyEventWrapper.cpp
 * @brief implements of KeyEventWrapper class
 *
 * @author S.Tanaka
 * @date 2007.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "KeyEventWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
KeyEventWrapper::KeyEventWrapper( kome::evt::KeyEvent& evt )
		: m_evt( evt ){
}

// destructor
KeyEventWrapper::~KeyEventWrapper() {
}

// get key event object
kome::evt::KeyEvent& KeyEventWrapper::getKeyEvent() {
	return m_evt;
}

// set key code
void KeyEventWrapper::setCode( int code ) {
	m_evt.setCode( code );
}

// get key code
int KeyEventWrapper::getCode() {
	return m_evt.getCode();
}

// set shift
void KeyEventWrapper::shift( bool s ) {
	m_evt.shift( s );
}

// get shift
bool KeyEventWrapper::shift() {
	return m_evt.shift();
}

// set ctrl
void KeyEventWrapper::ctrl( bool c ) {
	m_evt.ctrl( c );
}

// get ctrl
bool KeyEventWrapper::ctrl() {
	return m_evt.ctrl();
}

// set alt
void KeyEventWrapper::alt( bool a ) {
	m_evt.alt( a );
}

// get alt
bool KeyEventWrapper::alt() {
	return m_evt.alt();
}
