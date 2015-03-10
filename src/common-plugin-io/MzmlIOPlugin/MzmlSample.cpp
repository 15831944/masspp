/**
 * @file MzmlSample.cpp
 * @brief implements of MzmlSample class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzmlSample.h"

#include "MzmlSampleSet.h"
#include "MzmlHandler.h"


using namespace kome::io::mzml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MzmlSample::MzmlSample( MzmlSampleSet* sampleSet ) : kome::objects::Sample( sampleSet ) {
	// initialize
	m_sampleSet = sampleSet;

	setParallelReadable( true );
}

// destructor
MzmlSample::~MzmlSample() {
}

// add parent file
void MzmlSample::addParentFile( const char* path ) {
	// string object
	std::string p = NVL( path, "" );

	// check
	for( unsigned int i = 0; i < m_parentFiles.size(); i++ ) {
		if( p.compare( m_parentFiles[ i ] ) == 0 ) {
			return;
		}
	}

	// property
	kome::core::Properties& props = getRootDataGroupNode()->getProperties();
   
	std::string key = FMT( "Parent File %d", m_parentFiles.size() );
	props.setValue( key.c_str(), p.c_str() );

	// add
	m_parentFiles.push_back( p );
}

// mzML sample set object
MzmlSampleSet* MzmlSample::getMzmlSampleSet() {
	return m_sampleSet;
}

// get the number of parent files
unsigned int MzmlSample::getNumberOfParentFiles() {
	return m_parentFiles.size();
}

// get parent file path
const char* MzmlSample::getParentFilePath( const unsigned int index ) {
	if( index >= m_parentFiles.size() ) {
		return NULL;
	}
	return m_parentFiles[ index ].c_str();
}

// add array information
void MzmlSample::addArrayInfo(
		const char* name,
		const int bits,
		const bool compressed,
		const double scale,
		const bool isY
) {
	// check the name
	if( name == NULL ) {
		return;
	}

	// get array information
	ArrayInfo* info = getArrayInfo( name );
	if( info == NULL ) {
		m_arrayInfo.resize( m_arrayInfo.size() + 1 );
		info = &( m_arrayInfo.back() );
	}

	// set values
	info->name = name;
	info->bits = bits;
	info->compressed = compressed;
	info->scale = scale;
	info->isY = isY;
}

// get array information
MzmlSample::ArrayInfo* MzmlSample::getArrayInfo( const char* name ) {
	// check the name
	if( name == NULL ) {
		return NULL;
	}

	// search
	ArrayInfo* info = NULL;
	for( unsigned int i = 0; i < m_arrayInfo.size() && info == NULL; i++ ) {
		ArrayInfo* tmp = &( m_arrayInfo[ i ] );
		if( tmp->name.compare( name ) == 0 ) {
			info = tmp;
		}
	}

	return info;
}

// on open
bool MzmlSample::onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress ) {
	// handler
	MzmlHandler handler( *this );
	handler.parse( m_sampleSet->getFilePath() );

	return !handler.isError();
}

// on close sample
bool MzmlSample::onCloseSample() {
	return true;
}
