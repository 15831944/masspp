/**
 * @file MergedSampleSet.cpp
 * @brief implement of MergedSampleSet class
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MergedSampleSet.h"
#include "MergedSample.h"
#include "MergedSpectrum.h"
#include "SampleMergeManager.h"
#include "AverageSampleOperation.h"

using namespace kome::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MergedSampleSet::MergedSampleSet( const double rtTol, const double mzTol )
		: m_rtTol( rtTol ), m_mzTol( mzTol ) {
	// manager
	SampleMergeManager& mgr = SampleMergeManager::getInstance();

	// initialize
	m_id = mgr.getNextId();

	// add to manager
	mgr.addSampleSet( this );
}

// destructor
MergedSampleSet::~MergedSampleSet() {
	SampleMergeManager& mgr = SampleMergeManager::getInstance();
	mgr.removeSampleSet( this );
}

// add sample
void MergedSampleSet::addMergeSample( kome::objects::Sample* sample ){
	if( sample != NULL ){
		m_mergeSamplse.push_back( sample );
	}
}

// get the number of groups
unsigned int MergedSampleSet::getNumberOfMeargeSamples(){
	return m_mergeSamplse.size();
}

// get sample
kome::objects::Sample* MergedSampleSet::getMergeSample( const unsigned int idx ){
	if( idx >= m_mergeSamplse.size() ){
		return NULL;
	}
	return m_mergeSamplse[ idx ];
}

// get ID
unsigned int MergedSampleSet::getId() {
	return m_id;
}

// get RT tolerance
double MergedSampleSet::getRtTol() {
	return m_rtTol;
}

// get m/z tolerance
double MergedSampleSet::getMzTol() {
	return m_mzTol;
}

// open sample set
void MergedSampleSet::openSampleSet() {
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	std::string name = FMT( "Merged_Sample-%d", m_id );
	std::string path = getpath( msppMgr.getTmpDir(), name.c_str() );
	openFile( path.c_str(), NULL );

	kome::plugin::PluginCallTool::onOpenSampleSet( *this );	
}

// on open file
bool MergedSampleSet::onOpenFile( const char* path, kome::core::Progress* progress ) {	// add param @date 2014.07.08 <Mod> M.Izumi
	// sample
	MergedSample* sample = new MergedSample( this );

	// name
	std::string name = getfilename( path );
	sample->setName( name.c_str() );
	
	addSample( sample );
			
	return true;
}

// on close file
bool MergedSampleSet::onCloseFile() {
	return true;
}

// on open sample
bool MergedSampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress ) {	// @date 2014.07.08 <Mod> M.Izumi
	
	return true;
}

// on close sample
bool MergedSampleSet::onCloseSample( kome::objects::Sample* sample ) {
	return true;
}

