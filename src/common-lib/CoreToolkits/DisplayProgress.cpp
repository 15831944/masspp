/**
 * @file DisplayProgress.cpp
 * @brief implements of DisplayProgress class
 *
 * @author S.Tanaka
 * @date 2007.02.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "DisplayProgress.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DisplayProgress::DisplayProgress( const char* title ) : Progress( title ) {
	m_progress = NULL;
}

// destructor
DisplayProgress::~DisplayProgress() {
	if( m_progress != NULL ) {
		delete m_progress;
	}
}

// set range
void DisplayProgress::onSetRange( const int start, const int end ) {
	// range
	int range = end - start;
	range = ABS( range );

	// create object
	if( m_progress != NULL ) {
		delete m_progress;
	}
	m_progress = new boost::progress_display( range );
}

// set position
void DisplayProgress::onSetPosition( const int pos, const int prevPos ) {
	// check the member
	if( m_progress == NULL ) {
		return;
	}

	// add count
	int count = pos - prevPos;
	if( m_start > m_end ) {
		count = - count;
	}
	if( count > 0 ) {
		( *m_progress ) += count;
	}
}

// set status
void DisplayProgress::onSetStatus( const char* status, const bool bForced ) {
}

// is stopped
bool DisplayProgress::onIsStopped() {
	return false;
}

// >>>>>>	@Date:2013/11/25	<Add>	A.Ozaki
//
void	DisplayProgress::onFill( void )
{
	return;
}
//
// <<<<<<	@Date:2013/11/25	<Add>	A.Ozaki
