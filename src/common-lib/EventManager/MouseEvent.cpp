/**
 * @file MouseEvent.cpp
 * @brief implements of MouseEvent class
 *
 * @author S.Tanaka
 * @date 2007.01.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MouseEvent.h"


using namespace kome::evt;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MouseEvent::MouseEvent() {
	m_x = -1;
	m_y = -1;

	m_lbutton = false;
	m_mbutton = false;
	m_rbutton = false;

	m_shift = false;
	m_ctrl  = false;
	m_alt   = false;
}

// destructor
MouseEvent::~MouseEvent() {
}

// set position
void MouseEvent::setPosition( const int px, const int py ) {
	x( px );
	y( py );
}

// set x coordinate
void MouseEvent::x( const int px ) {
	m_x = px;
}

// get x coordinate
int MouseEvent::x() {
	return m_x;
}

// set y coordinate
void MouseEvent::y( const int py ) {
	m_y = py;
}

// get y coordinate
int MouseEvent::y() {
	return m_y;
}

// set L button flag
void MouseEvent::lbutton( const bool lbtn ) {
	m_lbutton = lbtn;
}

// get L button flag
bool MouseEvent::lbutton() {
	return m_lbutton;
}

// set M button flag
void MouseEvent::mbutton( const bool mbtn ) {
	m_mbutton = mbtn;
}

// get M button flag
bool MouseEvent::mbutton() {
	return m_mbutton;
}

// get R button flag
void MouseEvent::rbutton( const bool rbtn ) {
	m_rbutton = rbtn;
}

// get R button flag
bool MouseEvent::rbutton() {
	return m_rbutton;
}

// set shift flag
void MouseEvent::shift( const bool s ) {
	m_shift = s;
}

// get shift flag
bool MouseEvent::shift() {
	return m_shift;
}

// set ctrl flag
void MouseEvent::ctrl( const bool c ) {
	m_ctrl = c;
}

// get ctrl flag
bool MouseEvent::ctrl() {
	return m_ctrl;
}

// set alt flag
void MouseEvent::alt( const bool a ) {
	m_alt = a;
}

// get alt flag
bool MouseEvent::alt() {
	return m_alt;
}
