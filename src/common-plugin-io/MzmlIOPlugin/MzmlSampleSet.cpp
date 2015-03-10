/**
 * @file MzmlSampleSet.cpp
 * @brief implements of MzmlSampleSet class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzmlSampleSet.h"

#include "MzmlSample.h"


using namespace kome::io::mzml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MzmlSampleSet::MzmlSampleSet() {
	m_fp = NULL;
}

// destructor
MzmlSampleSet::~MzmlSampleSet() {
}

// get file
FILE* MzmlSampleSet::getFile() {
	return m_fp;
}

// on open file
bool MzmlSampleSet::onOpenFile( const char* path, kome::core::Progress* progress ) { // add param @date 2014.07.08 <Mod> M.Izumi
	// create sample
	MzmlSample* sample = new MzmlSample( this );
	addSample( sample );

	return true;
}

// on close file
bool MzmlSampleSet::onCloseFile() {
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
	return true;
}

// on open sample
bool MzmlSampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress ) {		// add param @date 2014.07.08 <Mod> M.Izumi
	if( m_fp == NULL ) {
		m_fp = fileopen( getFilePath(), "r" );
	}
	
	return ( m_fp != NULL );
}

// on close sample
bool MzmlSampleSet::onCloseSample( kome::objects::Sample* sample ) {
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
	return true;
}
