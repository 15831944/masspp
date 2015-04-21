/**
 * @file Progress.cpp
 * @brief implements of Progress class
 *
 * @author S.Tanaka
 * @date 2006.10.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Progress.h"

// >>>>>>	@Date:2013/11/18	<Add>	A.Ozaki
//
#define	_USE_INTERVAL_

#define	DRAW_INTERVAL		( 100 )			// unit:msec
#define	FILL_INTERVAL		( 500 )			// unit:msec
#include <boost/thread.hpp>
//
// <<<<<<	@Date:2013/11/18	<Add>	A.Ozaki

using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Progress::Progress( const char* title ) {
	m_start = 0;
	m_end = 0;
	m_pos = 0;
	m_prevPos = 0;
	m_relPos = 0;
	m_title = NVL( title, "" );

// >>>>>>	@Date:2013/11/18	<Add>	A.Ozaki
//
	m_llPrevTime = 0;
	m_strStatus.clear( );
//
// <<<<<<	@Date:2013/11/18	<Add>	A.Ozaki

	m_timers = new std::vector< std::pair< std::string, Timer* > >();
}

// destructor
Progress::~Progress() {
	// delete sub progresses
	for( unsigned int i = 0; i < m_subProgresses.size(); i++ ) {
		delete m_subProgresses[ i ];
	}
	m_subProgresses.clear();

	// timer info
	if( !m_title.empty() && m_start != m_end ) {
		m_timer.stop();
		LOG_INFO( FMT( "%s Time: %f sec", m_title.c_str(), m_timer.getTotalTime() ) );
	}

	// delete timers
	if( m_timers != NULL ) {
		for( unsigned int i = 0; i < m_timers->size(); i++ ) {
			delete ( *m_timers )[ i ].second;
		}
		delete m_timers;
	}
}

// set range
void Progress::setRange( const int start, const int end ) {
	
	// set members
	m_start = start;
	m_end = end;

	// start timer
	m_timer.start();
		
	// set range
	onSetRange( start, end );	
	setPosition( start );
	
	if( isStopped() ){ // @date 2013.09.26 <Mod> M.Izumi 
		return;
	}	
}

// set position
void Progress::setPosition( const int pos, const bool bForced ) {
	
	// set members
	m_prevPos = m_pos;
	m_pos = pos;

// >>>>>>	@Date:2013/11/18	<Add>	A.Ozaki
//
#ifdef	_USE_INTERVAL_
	if  ( false == bForced )
	{
		long long	llPassed = getcurrenttime( );
		llPassed = llPassed - m_llPrevTime;
		if  ( DRAW_INTERVAL > llPassed )
		{
			return;
		}
	}
	m_llPrevTime = getcurrenttime( );
#endif
//
// <<<<<<	@Date:2013/11/18	<Add>	A.Ozaki


	if( m_start == m_end ) {
		m_relPos = 0;
	}
	else {
		double d1 = (double)abs( m_pos - m_start );
		double d2 = (double)abs( m_end - m_pos );
		m_relPos = roundnum( d1 * 100.0 / ( d1 + d2 ) );
	}

// >>>>>>	@Date:2013/11/18	<Add>	A.Ozaki
//
#ifndef	_USE_INTERVAL_
	if( isStopped() ){ // @date 2013.09.26 <Mod> M.Izumi 
		return;
	}
	// set position
	onSetPosition( m_pos, m_prevPos );
#else
	if  ( false == m_strStatus.empty( ) )
	{
		onSetStatus( m_strStatus.c_str( ), bForced );
		m_strStatus.clear( );
	}

	// set position
	onSetPosition( m_pos, m_prevPos );
	
	if  ( isStopped( ) )
	{
		return;
	}
#endif
	return;
//
// <<<<<<	@Date:2013/11/18	<Add>	A.Ozaki
}

// set status
void Progress::setStatus( const char* status, const bool bForced ) {
// >>>>>>	@Date:2013/11/18	<Add>	A.Ozaki
//
#ifndef	_USE_INTERVAL_
	if( isStopped() ){ // @date 2013.09.26 <Mod> M.Izumi 
		return;
	}
	onSetStatus( status );
#else
	if  ( (const char *)NULL == status )
	{
		if  ( false == m_strStatus.empty( ) )
		{
			onSetStatus( m_strStatus.c_str( ), bForced );
		}
	}
	else
	{
		if  ( false == bForced )
		{
			m_strStatus = NVL( status, "" );
		}
		else
		{
			onSetStatus( status, bForced );
		}

		if  ( isStopped( ) )
		{
			return;
		}
	}
#endif
	return;
//
// <<<<<<	@Date:2013/11/18	<Add>	A.Ozaki
}

// is stopped
bool Progress::isStopped() {
	bool ret = onIsStopped();

	return ret;
}

// fill
void Progress::fill() {
	// set range
	if( m_start == 0 && m_end == 0 ) {
		setRange( 0, 1 );
	}

// >>>>>>	@Date:2013/11/18	<Add>	A.Ozaki
//
#ifdef	_USE_INTERVAL_
	onFill( );

#endif
	// set end position
	if  ( m_start == 0 && m_end == 0 )
	{
		setPosition( 1 );
	}
	else
	{
		setPosition( m_end );
	}
//
// <<<<<<	@Date:2013/11/18	<Add>	A.Ozaki
	m_relPos = 100;

// >>>>>>	@Date:2013/11/18	<Add>	A.Ozaki
//
#ifdef	_USE_INTERVAL_
	boost::this_thread::sleep( boost::posix_time::milliseconds( FILL_INTERVAL ) );
#endif
//
// <<<<<<	@Date:2013/11/18	<Add>	A.Ozaki

	// delete sub progresses
	for( unsigned int i = 0; i < m_subProgresses.size(); i++ ) {
		delete m_subProgresses[ i ];
	}
	m_subProgresses.clear();
}

// get relative position
int Progress::getRelativePosition() {
	return m_relPos;
}

// create timer
Timer* Progress::createTimer( const char* name ) {
	// check the name
	if( name == NULL || m_timers == NULL ) {
		return NULL;
	}

	// search
	for( unsigned int i = 0; i < m_timers->size(); i++ ) {
		if( ( *m_timers )[ i ].first.compare( name ) == 0 ) {
			return ( *m_timers )[ i ].second;
		}
	}

	// new timer
	m_timers->push_back( std::make_pair( name, new Timer() ) );
	
	return m_timers->back().second;
}

// get the number of timers
unsigned int Progress::getNumberOfTimers() {
	if( m_timers == NULL ) {
		return 0;
	}

	return m_timers->size();
}

// get timer name
const char* Progress::getTimerName( const unsigned int index ) {
	if( m_timers == NULL || index >= m_timers->size() ) {
		return NULL;
	}

	return ( *m_timers )[ index ].first.c_str();
}

// get timer
Timer* Progress::getTimer( const unsigned int index ) {
	if( m_timers == NULL || index >= m_timers->size() ) {
		return NULL;
	}

	return ( *m_timers )[ index ].second;
}

// creates sub progresses
void Progress::createSubProgresses( const unsigned int num ) {
	// check the number
	if( num == 0 ) {
		return;
	}
	if( this == &( getIgnoringProgress() ) ) {
		return;
	}

	// create progresses
	m_subProgresses.reserve( num );
	for( unsigned int i = 0; i < num; i++ ) {
		Progress* subProgress = new SubProgress( *this );
		delete subProgress->m_timers;
		subProgress->m_timers = m_timers;

		m_subProgresses.push_back( subProgress );
	}

	// set range
	setRange( 0, (int)m_subProgresses.size() * 100 );
}

// get the number of sub progresses
unsigned int Progress::getNumberOfSubProgresses() {
	if( this == &( getIgnoringProgress() ) ) {
		return 0;
	}
	return m_subProgresses.size();
}

// get sub progress
Progress* Progress::getSubProgress( const unsigned int idx ) {
	if( this == &( getIgnoringProgress() ) ) {
		return this;
	}
	if( idx >= m_subProgresses.size() ) {
		return NULL;
	}

	return m_subProgresses[ idx ];
}

// get ignoring progress
Progress& Progress::getIgnoringProgress() {
	static IgnoringProgress progress;

	return progress;
}


//
// IgnoringProgress
//
 
// constructor
IgnoringProgress::IgnoringProgress() : Progress( NULL ) {
}

// destructor
IgnoringProgress::~IgnoringProgress() {
}

// on set range
void IgnoringProgress::onSetRange( const int, const int ) {
}

// on set position
void IgnoringProgress::onSetPosition( const int, const int  ) {
}

// on set statuc
void IgnoringProgress::onSetStatus( const char*, const bool bForced ) {
}

// on is stopped
bool IgnoringProgress::onIsStopped() {
	return false;
}

// >>>>>>	@Date:2013/11/25	<Add>	A.Ozaki
//
// on fill
void IgnoringProgress::onFill( void )
{
	return;
}
//
// <<<<<<	@Date:2013/11/25	<Add>	A.Ozaki

//
// Sub Progress
//

// constructor
SubProgress::SubProgress( Progress& parent ) : Progress( NULL ) {
	m_parent = &parent;
}

// destructor
SubProgress::~SubProgress() {
	m_timers = NULL;
}

// on set range
void SubProgress::onSetRange( const int start, const int end ) {
}

// on set position
void SubProgress::onSetPosition( const int pos, const int prevPos ) {
	// set parent position
	int parentPos = 0;
	for( unsigned int i = 0; i < m_parent->getNumberOfSubProgresses(); i++ ) {
		// sub progress
		SubProgress* progress = (SubProgress*)( m_parent->getSubProgress( i ) );
		
		parentPos += progress->getRelativePosition();
	}
	m_parent->setPosition( parentPos );
}

// on set status
void SubProgress::onSetStatus( const char* status, const bool bForced ) {
	m_parent->setStatus( status, bForced );
}

// on is stopped
bool SubProgress::onIsStopped() {
	return m_parent->isStopped();
}

// >>>>>>	@Date:2013/11/25	<Add>	A.Ozaki
//
// on fill
void SubProgress::onFill( void )
{
	m_parent->setStatus( (char *)NULL, true );
	return;
}
//
// <<<<<<	@Date:2013/11/25	<Add>	A.Ozaki
