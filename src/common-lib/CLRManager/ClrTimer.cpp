/**
 * @file  ClrTimer.cpp
 * @brief implements of ClrTimer class
 *
 * @author S.Tanaka
 * @date 2008.05.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrTimer.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrTimer::ClrTimer() : TimerWrapper( *( new kome::core::Timer() ) ) {
}

// destrctor
ClrTimer::~ClrTimer() {
	delete &m_timer;
}
