/**
 * @file MsbSample.cpp
 * @brief implements of MsbSample class
 *
 * @author S.Tanaka
 * @date 2008.01.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MsbSample.h"

#include "MsbSampleSet.h"
#include "MsbManager.h"


using namespace kome::io::msb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static member
bool MsbSample::m_commonPropsFlg = false;


// constructor
MsbSample::MsbSample( MsbSampleSet* sampleSet ) : kome::objects::Sample( sampleSet ) {
	m_sampleSet = sampleSet;

	setParallelReadable( true );

	// set common properties
	if( !m_commonPropsFlg ) {
		isCommonProperty( NULL );

		m_commonProps.push_back( "Source File Path" );
		m_commonProps.push_back( "MSB Major Version" );
		m_commonProps.push_back( "MSB Minor Version" );

		m_commonPropsFlg = true;
	}
}

// destructor
MsbSample::~MsbSample() {
}

// get MSB sample set object
MsbSampleSet* MsbSample::getMsbSampleSet() {
	return m_sampleSet;
}

// set MSB major version
void MsbSample::setMsbMajorVersion( const unsigned short ver ) {
	m_majorVer = ver;
}

// get MSB minor version
unsigned short MsbSample::getMsbMajorVersion() {
	return m_majorVer;
}

// set MSB minor version
void MsbSample::setMsbMinorVersion( const unsigned short ver ) {
	m_minorVer = ver;
}

// get MSB major version
unsigned short MsbSample::getMsbMinorVersion() {
	return m_minorVer;
}

// on open sample
bool MsbSample::onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress ) {
	// set properties
	if( rootGroup != NULL ) {
		kome::core::Properties& props = rootGroup->getProperties();

		props.setValue( "Source File Path", m_sampleSet->getSourceFilePath() );
		props.setIntValue( "MSB Major Version", m_majorVer );
		props.setIntValue( "MSB Minor Version", m_minorVer );
	}

	// manager
	MsbManager* mgr = m_sampleSet->getManager();

	// open file
	FILE* fp = mgr->getFile();
	if( fp == NULL ) {
		const char* path = m_sampleSet->getFilePath();

		fp = fileopen( path, "rb" );
		if( fp == NULL ) {
			LOG_ERROR_CODE( FMT( "Failed to open the sample. [%s]", path ), ERR_FILE_OPEN_FAILED );
			return false;
		}

		mgr->setFile( fp );
		mgr->readSampleInfo( *this );
	}

	// read file
	m_sampleSet->getManager()->readMsb( *this );

	return true;
}

// on close sample
bool MsbSample::onCloseSample() {
	return true;
}
