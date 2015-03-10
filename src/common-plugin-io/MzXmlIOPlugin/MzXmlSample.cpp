/**
 * @file MzXmlSample.cpp
 * @brief implements of MzXmlSample class
 *
 * @author S.Tanaka
 * @date 2008.01.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzXmlSample.h"

#include "MzXmlSampleSet.h"
#include "MzXMLHandler.h"


using namespace kome::io::mzxml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static member
unsigned int MzXmlSample::m_parentFileCnt = 0;


// constructor
MzXmlSample::MzXmlSample( MzXmlSampleSet* sampleSet ) : kome::objects::Sample( sampleSet ) {
	m_sampleSet = sampleSet;

	setParallelReadable( true );

	// data file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	std::string mzxmlDir = getpath( msppMgr.getTmpDir(), "mzxml" );
	if( !fileexists( mzxmlDir.c_str() ) ) {
		makedirectory( mzxmlDir.c_str() );
	}

	m_dataFile = getpath( mzxmlDir.c_str(), FMT( "%lld.data", getcurrenttime() ).c_str() );
	m_fp = NULL;
}

// destructor
MzXmlSample::~MzXmlSample() {
	closeFile();
}

// get data directory
const char* MzXmlSample::getDataFile() {
	return m_dataFile.c_str();
}

// close file
void MzXmlSample::closeFile() {
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}
}

// open the read file
FILE* MzXmlSample::openReadFile() {
	closeFile();
	m_fp = fileopen( m_dataFile.c_str(), "rb" );
	return m_fp;
}

// open the write file
FILE* MzXmlSample::openWriteFile() {
	closeFile();
	m_fp = fileopen( m_dataFile.c_str(), "wb" );
	return m_fp;
}

// get file 
FILE* MzXmlSample::getFile() {
	return m_fp;
}

// add parent file
void MzXmlSample::addParentFile( const char* path ) {
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

	// set count
	if( m_parentFiles.size() >= m_parentFileCnt ) {
		isCommonProperty( NULL );

		m_commonProps.push_back( key );
		m_parentFileCnt = m_parentFiles.size() + 1;
	}

	// add
	m_parentFiles.push_back( p );
}

// mzXML sample set object
MzXmlSampleSet* MzXmlSample::getMzXmlSampleSet() {
	return m_sampleSet;
}

// get the number of parent files
unsigned int MzXmlSample::getNumberOfParentFiles() {
	return m_parentFiles.size();
}

// get parent file path
const char* MzXmlSample::getParentFilePath( const unsigned int index ) {
	if( index >= m_parentFiles.size() ) {
		return NULL;
	}
	return m_parentFiles[ index ].c_str();
}

// on open
bool MzXmlSample::onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress ) {
	// handler
	MzXMLHandler handler( *this );
	handler.parse( m_sampleSet->getFilePath() );

	return !handler.isError();
}

// on close sample
bool MzXmlSample::onCloseSample() {
	return true;
}
