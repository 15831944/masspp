/**
 * @file SampleSet.cpp
 * @brief implements of SampleSet class
 *
 * @author S.Tanaka
 * @date 2007.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SampleSet.h"
#include "Sample.h"
#include "DataGroupNode.h"
#include "ActiveObjectsManager.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static member
kome::core::Timer SampleSet::m_loadingTimer;
int SampleSet::m_currentSampleSetId = 0;


// construcotr
SampleSet::SampleSet() {
	m_opened = false;
	// >>>>>>	@Date:2013/06/13	<Add>	A.Ozaki
	m_errorCode = ERR_OK;
	// <<<<<<	@Date:2013/06/13	<Add>	A.Ozaki

	m_sampleSetId = issueSampleSetId();
}

// destructor
SampleSet::~SampleSet() {
	clearSamples();
}

// get sample set ID
int SampleSet::getSampleSetId() {
	return m_sampleSetId;
}

// set file path
void SampleSet::setFilePath( const char* path ) {
	m_filePath = NVL( path, "" );

	for( unsigned int i = 0; i < m_samples.size(); i++ ) {
		kome::objects::Sample* sample = m_samples[ i ];
		kome::objects::DataGroupNode* root = sample->getRootDataGroupNode();
		if( root != NULL ) {
			root->getProperties().setValue( "File Path", m_filePath.c_str() );
		}
	}
}

// get file path
const char* SampleSet::getFilePath() {
	return m_filePath.c_str();
}

// get file name
const char* SampleSet::getFileName() {
	return m_fileName.c_str();
}

// judge whter sample set file is opened
bool SampleSet::isOpened() {
	return m_opened;
}

// clear samples
void SampleSet::clearSamples() {
	// clear samples
	for( unsigned int i = 0; i < m_samples.size(); i++ ) {
		delete m_samples[ i ];
	}
	m_samples.clear();

	// opened flag
	m_opened = false;

	// remove from opened sample sets
	ActiveObjectsManager& aoMgr = ActiveObjectsManager::getInstance();
}

// add sample
void SampleSet::addSample( Sample* sample ) {
	// check the parameter
	if( sample == NULL ) {
		return;
	}
	if( sample->getSampleSet() != this ) {
		LOG_WARN( FMT( "Sample set is different." ) );
		return;
	}

	// add
	sample->setSampleIndex( (int)m_samples.size() );
	m_samples.push_back( sample );
}

// get the number of samples
unsigned int SampleSet::getNumberOfSamples() {
	return m_samples.size();
}

// get sample
Sample* SampleSet::getSample( const unsigned int index ) {
	if( index >= m_samples.size() ) {
		return NULL;
	}
	return m_samples[ index ];
}

// open file
bool SampleSet::openFile( const char* path, kome::core::Progress* progress ) { // add param @date 2014.07.08 <Mod> M.Izumi
	if( m_opened ) {		// already opened
		return true;
	}

	// set file path
	m_filePath = absolutepath( path );
	m_fileName = getfilename( m_filePath.c_str() );

	// log
	LOG_DEBUG( FMT( "Open the raw data. [%s]", m_filePath.c_str() ) );

	// open
	startLoadingTimer();

	bool ret = false;
	try {
		ret = onOpenFile( m_filePath.c_str(), progress );// add param
	}
	catch( ... ) {
		ret = false;
	}

	stopLoadingTimer();

	if( ret ) {
		m_opened = true;
	}
	else {
		clearSamples();
		LOG_ERROR( FMT( "Failed to open the file.[%s]", m_filePath.c_str() ) );
	}

	return ret;
}

// close file
bool SampleSet::closeFile() {
	if( !m_opened ) {
		return true;
	}

	// close
	for( unsigned int i = 0; i < m_samples.size(); i++ ) {
		m_samples[ i ]->closeSample();
	}
	bool ret = onCloseFile();

	// clear
	clearSamples();

	return ret;
}

// start loading timer
void SampleSet::startLoadingTimer() {
	m_loadingTimer.start();
}

// stop loading timer
void SampleSet::stopLoadingTimer() {
	m_loadingTimer.stop();
}

// get total loading time
double SampleSet::getTotalLoadingTime() {
	return m_loadingTimer.getTotalTime();
}

// issue sample set ID
int SampleSet::issueSampleSetId() {
	int sampleSetId = m_currentSampleSetId;
	m_currentSampleSetId++;

	return sampleSetId;
}

// >>>>>>	@Date:2013/06/13	<Add>	A.Ozaki
// get error code of file access.
int	SampleSet::getErrorCode( void )
{
	return	m_errorCode;
}

// set error code of file access.
void	SampleSet::setErrorCode( const int errorCode )
{
	m_errorCode = errorCode;

	return;
}
// <<<<<<	@Date:2013/06/13	<Add>	A.Ozaki
