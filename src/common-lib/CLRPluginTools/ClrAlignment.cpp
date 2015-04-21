/**
 * @file ClrAlignment.cpp
 * @brief implements of ClrAlignment class
 *
 * @author S.Tanaka
 * @date 2013.01.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ClrAlignment.h"

#include "ClrAlignmentBase.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrAlignment::ClrAlignment() : kome::operation::Alignment() {
}

// destructor
ClrAlignment::~ClrAlignment() {
	m_baseAlignment = nullptr;
	ClrAlignmentManager::getInstance().removeAlignment( *this );
}

// set base alignment
void ClrAlignment::setBaseAlignment( ClrAlignmentBase^ baseAlignment ) {
	m_baseAlignment = baseAlignment;
}

// get base alignment
ClrAlignmentBase^ ClrAlignment::getBaseAlignment() {
	ClrAlignmentBase^ baseAlignment = m_baseAlignment;
	return baseAlignment;
}

// on prepare
void ClrAlignment::onPrepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress ) {
	// base object
	ClrAlignmentBase^ base = getBaseAlignment();
	if( base == nullptr ) {
		return;
	}

	// prepare
	base->onPrepare(
		ClrObjectTool::createSampleWrapper( standard ),
		ClrObjectTool::createSampleWrapper( treatment ),
		ClrObjectTool::createProgressWrapper( progress )
	);
}

// on convert RT
double ClrAlignment::onConvertRt( const double rt, const double mz ) {
	// base object
	ClrAlignmentBase^ base = getBaseAlignment();
	if( base == nullptr ) {
		return - 1.0;
	}

	return base->onConvertRt( rt, mz );
}

// on convert m/z
double ClrAlignment::onConvertMz( const double rt, const double mz ) {
	// base object
	ClrAlignmentBase^ base = getBaseAlignment();
	if( base == nullptr ) {
		return - 1.0;
	}

	return base->onConvertMz( rt, mz );
}


// on invert RT
double ClrAlignment::onInvertRt( const double rt, const double mz ) {
	// base object
	ClrAlignmentBase^ base = getBaseAlignment();
	if( base == nullptr ) {
		return - 1.0;
	}

	return base->onInvertRt( rt, mz );
}


// on invert m/z
double ClrAlignment::onInvertMz( const double rt, const double mz ) {
	// base object
	ClrAlignmentBase^ base = getBaseAlignment();
	if( base == nullptr ) {
		return - 1.0;
	}

	return base->onInvertMz( rt, mz );
}


//
// ClrAlignmentManager
//

// constructor
ClrAlignmentManager::ClrAlignmentManager() {
}

// destructor
ClrAlignmentManager::~ClrAlignmentManager() {
	while( m_alignSet.size() > 0 ) {
		delete *( m_alignSet.begin() );
	}
}

// create alignment object
ClrAlignment& ClrAlignmentManager::createAlignment() {
	ClrAlignment* align = new ClrAlignment();
	m_alignSet.insert( align );

	return *align;
}

// remove alignment
void ClrAlignmentManager::removeAlignment( ClrAlignment& align ) {
	m_alignSet.erase( &align );
}

// destroy alignment
void ClrAlignmentManager::destroyAlignment( ClrAlignment& align ) {
	if( m_alignSet.find( &align ) != m_alignSet.end() ) {
		delete &align;
	}
}

// get instance
ClrAlignmentManager& ClrAlignmentManager::getInstance() {
	// create object
	static ClrAlignmentManager mgr;

	return mgr;
}
