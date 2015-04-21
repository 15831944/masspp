/**
 * @file Alignment.cpp
 * @brief implements of Alignment class
 *
 * @author S.Tanaka
 * @date 2009.02.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Alignment.h"


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Alignment::Alignment() {
	// initialize
	m_standard = NULL;
	m_treatment = NULL;

	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();
	mgr.addAlignment( this );
}

// destructor
Alignment::~Alignment() {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();
	mgr.removeAlignment( this );
}

// get standard sample
kome::objects::Sample* Alignment::getStandardSample() {
	return m_standard;
}

// get treatment sample
kome::objects::Sample* Alignment::getTreatmentSample() {
	return m_treatment;
}

// prepare
void Alignment::prepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress ) {
	// set
	m_standard = standard;
	m_treatment = treatment;

	// progress
	if( progress == NULL ) {
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}

	// prepare
	onPrepare( standard, treatment, progress );
}

// convert RT
double Alignment::convertRt( const double rt, const double mz ) {
	// samples
	kome::objects::Sample* standard = getStandardSample();
	kome::objects::Sample* treatment = getTreatmentSample();
	if( standard == treatment || standard == NULL || treatment == NULL ) {
		return rt;
	}

	return onConvertRt( rt, mz );
}

// convert m/z
double Alignment::convertMz( const double rt, const double mz ) {
	// samples
	kome::objects::Sample* standard = getStandardSample();
	kome::objects::Sample* treatment = getTreatmentSample();
	if( standard == treatment || standard == NULL || treatment == NULL ) {
		return mz;
	}

	return onConvertMz( rt, mz );
}

// invert RT
double Alignment::invertRt( const double rt, const double mz ) {
	// samples
	kome::objects::Sample* standard = getStandardSample();
	kome::objects::Sample* treatment = getTreatmentSample();
	if( standard == treatment || standard == NULL || treatment == NULL ) {
		return rt;
	}

	return onInvertRt( rt, mz );
}

// invert m/z
double Alignment::invertMz( const double rt, const double mz ) {
	// samples
	kome::objects::Sample* standard = getStandardSample();
	kome::objects::Sample* treatment = getTreatmentSample();
	if( standard == treatment || standard == NULL || treatment == NULL ) {
		return mz;
	}

	return onInvertMz( rt, mz );
}

// get default alignment
Alignment& Alignment::getDefaultAlignment() {
	// create object
	static DefaultAlignment align;

	// remove from set
	AlignmentManager& mgr = AlignmentManager::getInstance();
	mgr.removeAlignment( &align );

	return align;
}


//
// Default Alignment
//

// constructor
DefaultAlignment::DefaultAlignment() {
}

// destructor
DefaultAlignment::~DefaultAlignment() {
}

// on prepare
void DefaultAlignment::onPrepare( kome::objects::Sample* standard, kome::objects::Sample* treatment, kome::core::Progress* progress ) {
	// nothing to do
}

// on convert RT
double DefaultAlignment::onConvertRt( const double rt, const double mz ) {
	return rt;
}

// on convert m/z
double DefaultAlignment::onConvertMz( const double rt, const double mz ) {
	return mz;
}

// on invert RT
double DefaultAlignment::onInvertRt( const double rt, const double mz ) {
	return rt;
}

// on invert m/z
double DefaultAlignment::onInvertMz( const double rt, const double mz ) {
	return mz;
}


//
// AlignmentManager
//

// constructor
AlignmentManager::AlignmentManager() {
}

// destructor
AlignmentManager::~AlignmentManager() {
	while( m_alignSet.size() > 0 ) {
		delete *( m_alignSet.begin() );
	}
}

// add alignment
void AlignmentManager::addAlignment( Alignment* align ) {
	if( align != NULL ) {
		m_alignSet.insert( align );
	}
}

// remove alignment
void AlignmentManager::removeAlignment( Alignment* align ) {
	if( align != NULL ) {
		m_alignSet.erase( align );
	}
}

// get instance
AlignmentManager& AlignmentManager::getInstance() {
	// create object (This is the only object.)
	static AlignmentManager mgr;

	return mgr;
}
