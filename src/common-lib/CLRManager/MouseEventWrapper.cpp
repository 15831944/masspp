/**
 * @file MouseEventWrapper.cpp
 * @brief implements of MouseEventWrapper class
 *
 * @author S.Tanaka
 * @date 2007.01.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "MouseEventWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MouseEventWrapper::MouseEventWrapper( kome::evt::MouseEvent& evt )
		: m_evt( evt ){
}

// destructor
MouseEventWrapper::~MouseEventWrapper() {
}

// get mouse event object
kome::evt::MouseEvent& MouseEventWrapper::getMouseEvent() {
	return m_evt;
}

// set position
void MouseEventWrapper::setPosition( int x, int y ) {
	m_evt.setPosition( x, y );
}

// set x
void MouseEventWrapper::x( int px ) {
	m_evt.x( px );
}

// get x
int MouseEventWrapper::x() {
	return m_evt.x();
}

// set y
void MouseEventWrapper::y( int py ) {
	m_evt.y( py );
}

// get y 
int MouseEventWrapper::y() {
	return m_evt.y();
}

// set L button
void MouseEventWrapper::lbutton( bool lbtn ) {
	m_evt.lbutton( lbtn );
}

// get L button
bool MouseEventWrapper::lbutton() {
	return m_evt.lbutton();
}

// set M button
void MouseEventWrapper::mbutton( bool mbtn ) {
	m_evt.mbutton( mbtn );
}

// get M button
bool MouseEventWrapper::mbutton() {
	return m_evt.mbutton();
}

// set R button
void MouseEventWrapper::rbutton( bool rbtn ) {
	m_evt.rbutton( rbtn );
}

// get R button
bool MouseEventWrapper::rbutton() {
	return m_evt.rbutton();
}

// set shift
void MouseEventWrapper::shift( bool s ) {
	m_evt.shift( s );
}

// get shift
bool MouseEventWrapper::shift() {
	return m_evt.shift();
}

// set ctrl
void MouseEventWrapper::ctrl( bool c ) {
	m_evt.ctrl( c );
}

// get ctrl
bool MouseEventWrapper::ctrl() {
	return m_evt.ctrl();
}

// set alt
void MouseEventWrapper::alt( bool a ) {
	m_evt.alt( a );
}

// get alt
bool MouseEventWrapper::alt() {
	return m_evt.alt();
}
