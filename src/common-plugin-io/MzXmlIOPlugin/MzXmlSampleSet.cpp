/**
 * @file MzXmlSampleSet.cpp
 * @brief implements of MzXmlSampleSet class
 *
 * @author S.Tanaka
 * @date 2008.01.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzXmlSampleSet.h"

#include "MzXmlSample.h"


using namespace kome::io::mzxml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MzXmlSampleSet::MzXmlSampleSet() {
	m_fp = NULL;
}

// destructor
MzXmlSampleSet::~MzXmlSampleSet() {
}

// get file
FILE* MzXmlSampleSet::getFile() {
	return m_fp;
}

// on open file
bool MzXmlSampleSet::onOpenFile( const char* path, kome::core::Progress* progress ) {	// add param @date 2014.07.08 <Mod> M.Izumi
	// create sample
	MzXmlSample* sample = new MzXmlSample( this );
	addSample( sample );

	return true;
}

// on close file
bool MzXmlSampleSet::onCloseFile() {
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
	return true;
}

// on open sample
bool MzXmlSampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress ) {	// add param @date 2014.07.08 <Mod> M.Izumi
	if( m_fp == NULL ) {
		m_fp = fileopen( getFilePath(), "r" );
	}
	
	return ( m_fp != NULL );
}

// on close sample
bool MzXmlSampleSet::onCloseSample( kome::objects::Sample* sample ) {
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
	return true;
}
