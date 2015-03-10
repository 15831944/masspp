/**
 * @file SampleMergeManager.cpp
 * @brief implement of SampleMergeManager class
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SampleMergeManager.h"
#include "MergedSampleSet.h"

using namespace kome::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SampleMergeManager::SampleMergeManager() {
	m_id = 0;
}

// destructor
SampleMergeManager::~SampleMergeManager() {
	// delete all sample set
	while( m_sampleSets.size() > 0 ) {
		delete m_sampleSets.front();
	}
}

// get next ID
unsigned int SampleMergeManager::getNextId() {
	return m_id;
}

// add sample set
void SampleMergeManager::addSampleSet( MergedSampleSet* sampleSet ) {
	if( sampleSet != NULL ) {
		m_sampleSets.push_back( sampleSet );
		m_id++;
	}
}

// remove sample set
void SampleMergeManager::removeSampleSet( MergedSampleSet* sampleSet ) {
	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_sampleSets.size() && idx < 0; i++ ) {
		if( m_sampleSets[ i ] == sampleSet ) {
			idx = (int)i;
		}
	}

	// remove
	if( idx >= 0 ) {
		m_sampleSets.erase( m_sampleSets.begin() + idx );
	}
}

// on close sample
void SampleMergeManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// sample sets to close
	std::vector< MergedSampleSet* > sampleSets;
	for( unsigned int i = 0; i < m_sampleSets.size(); i++ ) {
		// sample set
		MergedSampleSet* ss = m_sampleSets[ i ];

		// check groups
		bool flg = false;
		
		for( unsigned int j=0; j < ss->getNumberOfMeargeSamples() && !flg; j++ ){
			kome::objects::Sample* tmpSample = ss->getMergeSample( j );
			if( tmpSample == sample ){
				flg = true;
			}
		}

		if( flg ) {
			sampleSets.push_back( ss );
		}
	}

	// close sample sets
	for( unsigned int i = 0; i < sampleSets.size(); i++ ) {
		sampleSets[ i ]->closeFile();
		delete sampleSets[ i ];
	}
}

// get manager object
SampleMergeManager& SampleMergeManager::getInstance() {
	// create object (This is the only object.)
	static SampleMergeManager mgr;

	return mgr;
}

