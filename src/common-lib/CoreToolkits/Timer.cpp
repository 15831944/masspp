/**
 * @file  Timer.cpp
 * @brief implements of Timer class
 *
 * @author S.Tanaka
 * @date 2008.05.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Timer.h"

#include <ctime>


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Timer::Timer() {
	reset();
}

// destrctor
Timer::~Timer() {
}

// reset timer
void Timer::reset() {
	m_total = 0.0;
	m_start = 0;
	m_stopped = true;
}

// start timer
void Timer::start() {
	m_start = std::clock();
	m_stopped = false;
}

// stop timer
double Timer::stop() {
	// total time
	const double t = getTime();
	m_total += getTime();

	// set start time
	m_start = std::clock();

	// stopped
	m_stopped = true;

	return t;
}

// get time
double Timer::getTime() {
	// get time
	double t = 0.0;

	if( !m_stopped ) {
		// current
		clock_t current = std::clock();

		// time
		t = (double)( current - m_start ) / (double)( CLOCKS_PER_SEC );
	}

	return t;
}

// get total time
double Timer::getTotalTime() {
	double t = m_total;
	t += getTime();
	return t;
}
