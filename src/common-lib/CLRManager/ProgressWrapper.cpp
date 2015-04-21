/**
 * @file ProgressWrapper.cpp
 * @brief implements of ProgressWrapper class
 *
 * @author S.Tanaka
 * @date 2006.11.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ProgressWrapper.h"

#include "TimerWrapper.h"

#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ProgressWrapper::ProgressWrapper( kome::core::Progress& progress )
		: m_progress( progress ) {
}

// destructor
ProgressWrapper::~ProgressWrapper() {
}

// get progress object
kome::core::Progress& ProgressWrapper::getProgress() {
	return m_progress;
}

// set range
void ProgressWrapper::setRange( int start, int end ) {
	m_progress.setRange( start, end );
}

// set position
void ProgressWrapper::setPosition( int pos ) {
	m_progress.setPosition( pos );
}

// set status
void ProgressWrapper::setStatus( System::String^ status ) {
	m_progress.setStatus( ClrObjectTool::convertString( status ).c_str() );
}

// is stopped
bool ProgressWrapper::isStopped() {
	return m_progress.isStopped();
}
  
// set end position
void ProgressWrapper::fill() {
	m_progress.fill();
}

// get relative position
int ProgressWrapper::getRelativePosition() {
	return m_progress.getRelativePosition();
}

// create timer
TimerWrapper^ ProgressWrapper::createTimer( System::String^ name ) {
	// create timer
	kome::core::Timer* timer = m_progress.createTimer( ClrObjectTool::convertString( name ).c_str() );
	if( timer == NULL ) {
		return nullptr;
	}

	return gcnew TimerWrapper( *timer );
}

// get the number of timers
unsigned int ProgressWrapper::getNumberOfTimers() {
	return m_progress.getNumberOfTimers();
}

// get timer name
System::String^ ProgressWrapper::getTimerName( unsigned int index ) {
	return ClrObjectTool::convertString( m_progress.getTimerName( index ), NULL );
}

// get timer
TimerWrapper^ ProgressWrapper::getTimer( unsigned int index ) {
	// get timer
	kome::core::Timer* timer = m_progress.getTimer( index );
	if( timer == NULL ) {
		return nullptr;
	}

	return gcnew TimerWrapper( *timer );
}

// create sub progress
void ProgressWrapper::createSubProgresses( unsigned int num ) {
	m_progress.createSubProgresses( num );
}

// get the number of sub progresses
unsigned int ProgressWrapper::getNumberOfSubProgresses() {
	return m_progress.getNumberOfSubProgresses();
}

// get sub progress
ProgressWrapper^ ProgressWrapper::getSubProgress( unsigned int idx ) {
	// sub progress
	kome::core::Progress* subProgress = m_progress.getSubProgress( idx );
	if( subProgress == NULL ) {
		return nullptr;
	}
	return gcnew ProgressWrapper( *subProgress );
}

// get ignoring progress
ProgressWrapper^ ProgressWrapper::getIgnoringProgress() {
	return gcnew ProgressWrapper( kome::core::Progress::getIgnoringProgress() );
}
