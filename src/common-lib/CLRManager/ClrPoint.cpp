/**
 * @file ClrPoint.cpp
 * @brief implements of ClrPoint class
 *
 * @author S.Tanaka
 * @date 2010.01.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrPoint.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrPoint::ClrPoint() {
	m_x = -1.0;
	m_y = -1.0;
}

// constructor
ClrPoint::ClrPoint( double x, double y ) {
	m_x = x;
	m_y = y;
}

// destructor
ClrPoint::~ClrPoint() {
}

// set point
void ClrPoint::setPoint( double x, double y ) {
	m_x = x;
	m_y = y;
}

// set x
void ClrPoint::x( double x ) {
	m_x = x;
}

// get x
double ClrPoint::x() {
	return m_x;
}

// set y
void ClrPoint::y( double y ) {
	m_y = y;
}

// get y
double ClrPoint::y() {
	return m_y;
}
