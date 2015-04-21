/**
 * @file ClrProgress.cpp
 * @brief implements of ClrProgress class
 *
 * @author S.Tanaka
 * @date 2006.11.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrProgress.h"

#include "ClrProgressBase.h"
#include "ClrObjectTool.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrProgress::ClrProgress() {
}

// destructor
ClrProgress::~ClrProgress() {
	m_baseProgress = nullptr;
	ClrObjectPool::getInstance().removeProgress( *this );
}

// set base progress
void ClrProgress::setBaseProgress( ClrProgressBase^ baseProgress ) {
	m_baseProgress = baseProgress;
}

// get base progress
ClrProgressBase^ ClrProgress::getBaseProgress() {
	ClrProgressBase^ baseProgress = m_baseProgress;
	return baseProgress;
}

// on set range
void ClrProgress::onSetRange( const int start, const int end ) {
	ClrProgressBase^ progress = m_baseProgress;
	if( progress != nullptr ) {
		progress->onSetRange( start, end );
	}
}

// on set position
void ClrProgress::onSetPosition( const int pos, const int prevPos ) {
	ClrProgressBase^ progress = m_baseProgress;
	if( progress != nullptr ) {
		progress->onSetPosition( pos, prevPos );
	}
}

// on set status
void ClrProgress::onSetStatus( const char* status, const bool bForced ) {
	ClrProgressBase^ progress = m_baseProgress;
	if( progress != nullptr ) {
		progress->onSetStatus( ClrObjectTool::convertString( status, NULL ), bForced );
	}
}

// on is stopped
bool ClrProgress::onIsStopped() {
	ClrProgressBase^ progress = m_baseProgress;
	if( progress == nullptr ) {
		return false;
	}
	return progress->onIsStopped();
}

// >>>>>>	@Date:2013/11/25	<Add>	A.Ozaki
//
// on fill
void ClrProgress::onFill( void )
{
	ClrProgressBase^ progress = m_baseProgress;
	if  ( nullptr != progress )
	{
		progress->onFill( );
	}

	return;
}
//
// <<<<<<	@Date:2013/11/25	<Add>	A.Ozaki
