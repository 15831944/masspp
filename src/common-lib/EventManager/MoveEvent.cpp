/**
 * @file MoveEvent.cpp
 * @brief implements of MoveEvent class
 *
 * @author S.Tanaka
 * @date 2012.02.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MoveEvent.h"


using namespace kome::evt;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// cosntructor
MoveEvent::MoveEvent() {
	m_px =0;
	m_py = 0;
}

MoveEvent::MoveEvent( const int px, const int py ) {
	m_px = px;
	m_py = py;
}

// destructor
MoveEvent::~MoveEvent() {
}

// set x-coordinate
void MoveEvent::x( const int px ) {
	m_px = px;
}

// get x-coordinate
int MoveEvent::x() {
	return m_px;
}

// set y-coordinate
void MoveEvent::y( const int py ) {
	m_py = py;
}

// get y-coordinate
int MoveEvent::y() {
	return m_py;
}
