/**
 * @file MoveEventWrapper.cpp
 * @brief implements of MoveEventWrapper class
 *
 * @author S.Tanaka
 * @date 2007.09.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MoveEventWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MoveEventWrapper::MoveEventWrapper( kome::evt::MoveEvent& evt )
		: m_evt( evt ){
}

// destructor
MoveEventWrapper::~MoveEventWrapper() {
}

// get move event object
kome::evt::MoveEvent& MoveEventWrapper::getMoveEvent() {
	return m_evt;
}

// set x
void MoveEventWrapper::x( int px ) {
	m_evt.x( px );
}

// get x
int MoveEventWrapper::x() {
	return m_evt.x();
}

// set y
void MoveEventWrapper::y( int py ) {
	m_evt.y( py );
}

// get y
int MoveEventWrapper::y() {
	return m_evt.y();
}
