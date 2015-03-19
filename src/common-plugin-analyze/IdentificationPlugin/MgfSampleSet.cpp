/**
 * @file MgfSampleSet.cpp
 * @brief interfaces of MgfSampleSet class
 *
 * @author M.Izumi
 * @date 2013.07.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "MgfSampleSet.h"
#include "MgfSample.h"
#include "MgfSpectrum.h"
   
using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MgfSampleSet::MgfSampleSet(){
	m_fp = NULL;
}

// destructor
MgfSampleSet::~MgfSampleSet(){
}

// get file
FILE* MgfSampleSet::getFile() {
	return m_fp;
}

// opne file
bool MgfSampleSet::openFile( const char* path ){
	// open
	m_fp = fileopen( path, "rb" );
	if( m_fp == NULL ) {
		return false;
	}

	return true;
}


// open file
bool MgfSampleSet::onOpenFile( const char* path, kome::core::Progress* progress ){ // add param @date 2014.07.08 <Mod> M.Izumi
	if( !openFile( path ) ){
		return false;
	}

	// create sample
	MgfSample* sample = new MgfSample( this );
	addSample( sample );
	
	return true;
}


// on close file
bool MgfSampleSet::onCloseFile(){
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
	return true;
}

// on open sample
bool MgfSampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress ){	// add param @date 2014.0.08 <Mod> M.Izumi 
	return true;
}

// on close sample
bool MgfSampleSet::onCloseSample( kome::objects::Sample* sample ){
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
	
	return true;
}
