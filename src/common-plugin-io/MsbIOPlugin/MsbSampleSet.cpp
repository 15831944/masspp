/**
 * @file MsbSampleSet.cpp
 * @brief implements of MsbSampleSet class
 *
 * @author S.Tanaka
 * @date 2008.01.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MsbSampleSet.h"

#include "MsbSample.h"
#include "MsbManager.h"

using namespace kome::io::msb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MsbSampleSet::MsbSampleSet() {
	m_mgr = NULL;
}

// destructor
MsbSampleSet::~MsbSampleSet() {
	if( m_mgr != NULL ) {
		delete m_mgr;
	}
}

// set source file path
void MsbSampleSet::setSourceFilePath( const char* path ) {
	// set
	m_sourceFile = std::string( NVL( path, "" ) );

	// set properties
	MsbSample* sample = (MsbSample*)getSample( 0 );
	if( sample != NULL ) {
		kome::objects::DataGroupNode* root = sample->getRootDataGroupNode();
		if( root != NULL ) {
			root->getProperties().setValue( "Source File Path", m_sourceFile.c_str() );
		}
	}
}

// get source file path
const char* MsbSampleSet::getSourceFilePath() {
	return m_sourceFile.c_str();
}

// get msb IO manager
MsbManager* MsbSampleSet::getManager() {
	return m_mgr;
}

// on open file
bool MsbSampleSet::onOpenFile( const char* path, kome::core::Progress* progress ) {		// add param @date 2014.07.08 <Mod> M.Izumi
	// open file
	FILE* fp = fileopen( path, "rb" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the sample. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	// get version
	unsigned short version[ 2 ];
	unsigned char endian;
	MsbManager::getFormatVersion( version, version + 1, &endian, fp );

	// get manager
	MsbManager* mgr = MsbManager::getMsbManager( version[ 0 ], version[ 1 ], endian, fp );
	if( mgr == NULL ) {
		LOG_ERROR_CODE(
			FMT(
				"Msb version [%d.%d] is not supported because this version of Mass++ is older.\nDownload and install the latest Mass++.",
				version[ 0 ],
				version[ 1 ]
			),
			ERR_OTHER
		);
		fclose( fp );

		return false;
	}
	else {
		LOG_INFO( FMT( "Reading MSB format sample. [ver %d.%d]", version[ 0 ], version[ 1 ]  ));
	}
	m_mgr = mgr;

	// create sample
	MsbSample* sample = new MsbSample( this );
	sample->setMsbMajorVersion( version[ 0 ] );
	sample->setMsbMinorVersion( version[ 1 ] );

	addSample( sample );

	// read sample information
	mgr->readSampleInfo( *sample );

	return true;
}

// on close file
bool MsbSampleSet::onCloseFile() {
	m_mgr->closeFile();
	return true;
}

// on open sample
bool MsbSampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress ) {
	return true;
}

// on close sample
bool MsbSampleSet::onCloseSample( kome::objects::Sample* sample ) {
	m_mgr->closeFile();
	return true;
}
