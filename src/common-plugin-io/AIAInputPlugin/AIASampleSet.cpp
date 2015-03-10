/**
 * @file AIASampleSet.cpp
 * @brief implementation of AIASampleSet class
 *
 * @author fujita
 * @date 2012.07.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "AIASampleSet.h"
#include "AIASample.h"

#include <netcdf.h>


using namespace kome::io::aia;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
AIASampleSet::AIASampleSet() {
	m_fp = NULL;
}

// destructor
AIASampleSet::~AIASampleSet() {
}

// set net cdf file path
void AIASampleSet::setCdfFilePath( const char* path ) {
	m_cdfFile = NVL( path, "" );
}

// get net cdf file path
const char* AIASampleSet::getCdfFilePath()  {
	return m_cdfFile.c_str();
}

// on open file
bool AIASampleSet::onOpenFile( const char* path, kome::core::Progress* progress ) { // add param @date 2014.0.08 <Mod> M.Izumi 
	
	this->setCdfFilePath( path );

	// create sample
	AIASample* sample = new AIASample( this );
	addSample( sample );

	return true;
}

// on close file
bool AIASampleSet::onCloseFile() {
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
	return true;
}

// on open sample
bool AIASampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress ) {	// add param @date 2014.0.08 <Mod> M.Izumi 
	if( m_fp == NULL ) {
		m_fp = fileopen( getFilePath(), "r" );
	}
	
	return ( m_fp != NULL );

	return true;

}

// on close sample
bool AIASampleSet::onCloseSample( kome::objects::Sample* sample ) {
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
	return true;
}

// error message
void AIASampleSet::error( int status ) {
	LOG_ERROR( FMT( "Net CDF Error: %s", nc_strerror( status ) ) );
}
