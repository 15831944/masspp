/**
 * @file ClrSampleSet.cpp
 * @brief implements of ClrSampleSet class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrSampleSet.h"

#include "ClrSampleSetBase.h"
#include "SampleWrapper.h"
#include "ClrObjectTool.h"
#include "ClrObjectPool.h"
#include "ClrLogger.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrSampleSet::ClrSampleSet() {
}

// destructor
ClrSampleSet::~ClrSampleSet() {
	ClrObjectPool::getInstance().removeSampleSet( *this );
	m_baseSampleSet = nullptr;
}

// set base sample set
void ClrSampleSet::setBaseSampleSet( ClrSampleSetBase^ baseSampleSet ) {
	m_baseSampleSet = baseSampleSet;
}

// get base sample set
ClrSampleSetBase^ ClrSampleSet::getBaseSampleSet() {
	ClrSampleSetBase^ baseSampleSet = m_baseSampleSet;
	return baseSampleSet;
}

// on open file
bool ClrSampleSet::onOpenFile( const char* path, kome::core::Progress* progress ) {	// add param @date 2014.07.08 <Add> M.Izumi
	// logger
	ClrLogger^ logger = ClrLogger::getInstance();

	// base sample set
	ClrSampleSetBase^ baseSampleSet = m_baseSampleSet;
	if( baseSampleSet == nullptr ) {
		return false;
	}

	// path
	System::String^ p = ClrObjectTool::convertString( path, NULL );

	// open file
	bool ret = false;
		
	try {
		ret = baseSampleSet->onOpenFile( p );
	}
	catch ( System::Exception^ e ) {
		logger->error( e->ToString() );
		ret = false;
	}

	return ret;
}

// on close file
bool ClrSampleSet::onCloseFile() {
	// base sample set
	ClrSampleSetBase^ baseSampleSet = m_baseSampleSet;
	if( baseSampleSet == nullptr ) {
		return false;
	}

	// close file
	return baseSampleSet->onCloseFile();
}

// on open sample
bool ClrSampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress ) {
	// base sample set
	ClrSampleSetBase^ baseSampleSet = m_baseSampleSet;
	if( baseSampleSet == nullptr ) {
		return false;
	}

	// get sample
	SampleWrapper^ s = ClrObjectTool::createSampleWrapper( sample );

	// on open sample
	return baseSampleSet->onOpenSample( s );
}

// on close sample
bool ClrSampleSet::onCloseSample( kome::objects::Sample* sample ) {
	// base sample set
	ClrSampleSetBase^ baseSampleSet = m_baseSampleSet;
	if( baseSampleSet == nullptr ) {
		return false;
	}

	// get sample
	SampleWrapper^ s = ClrObjectTool::createSampleWrapper( sample );

	// on close sample
	return baseSampleSet->onCloseSample( s );
}
