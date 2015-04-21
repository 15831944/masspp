/**
 * @file  TimerWrapper.cpp
 * @brief implements of TimerWrapper class
 *
 * @author S.Tanaka
 * @date 2008.05.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TimerWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
TimerWrapper::TimerWrapper( kome::core::Timer& timer ) : m_timer( timer ) {
}

// destrctor
TimerWrapper::~TimerWrapper() {
}

// get wrapped timer object
kome::core::Timer& TimerWrapper::getTimer() {
	return m_timer;
}

// reset timer
void TimerWrapper::reset() {
	m_timer.reset();
}

// start timer
void TimerWrapper::start() {
	m_timer.start();
}

// stop timer
double TimerWrapper::stop() {
	return m_timer.stop();
}

// get total time
double TimerWrapper::getTotalTime() {
	return m_timer.getTotalTime();
}
