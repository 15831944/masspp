/**
 * @file Ab3dManager.cpp
 * @brief implement of Ab3dManager class
 *
 * @author S.Tanaka
 * @date 2007.09.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dManager.h"

#include "Ab3dSampleSet.h"
#include "Ab3dSample.h"
#include "Ab3dCompositionCluster.h"


using namespace kome::ab3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Ab3dManager::Ab3dManager() {
	m_peaksDlg = NULL;
}

// destructor
Ab3dManager::~Ab3dManager() {
	// delete data
	while( m_sampleSets.size() > 0 ) {
		delete *( m_sampleSets.begin() );
	}
}

// add sample set
void Ab3dManager::addSampleSet( Ab3dSampleSet* sampleSet ) {
	// delete closed sample sets
	for( int i = (int)m_sampleSets.size() - 1; i >= 0; i-- ) {
		// sample
		kome::objects::SampleSet* ss = m_sampleSets[ i ];
		kome::objects::Sample* s = ss->getSample( 0 );

		// delete sample set
		if( s == NULL || !s->isOpened() ) {
			delete ss;
		}
	}

	// add sample set
	m_sampleSets.push_back( sampleSet );
}

// delete data
void Ab3dManager::deleteSampleSet( Ab3dSampleSet* sampleSet ) {
	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_sampleSets.size() && idx < 0; i++ ) {
		if( sampleSet == m_sampleSets[ i ] ) {
			idx = i;
		}
	}

	// delete
	if( idx >= 0 ) {
		m_sampleSets.erase( m_sampleSets.begin() + idx );
	}
}

// count
unsigned int Ab3dManager::getCount() {
	return m_sampleSets.size();
}

// get data
Ab3dSample* Ab3dManager::getSample( const unsigned int idx ) {
	if( idx >= m_sampleSets.size() ) {
		return NULL;
	}
	return (Ab3dSample*)m_sampleSets[ idx ]->getSample( 0 );
}

// on close sample
void Ab3dManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// sample set
	if( sample == NULL ) {
		return;
	}
	kome::objects::SampleSet* sampleSet = sample->getSampleSet();

	// search
	int idx = -1;
	for( int i = 0; i < (int)m_sampleSets.size() && idx < 0; i++ ) {
		if( sampleSet == m_sampleSets[ i ] ) {
			idx = i;
		}
	}
	if( idx < 0 ) {
		return;
	}

	// selected clusters
	for( int i = (int)m_selectedClusters.size() - 1; i >= 0; i-- ) {
		if( m_selectedClusters[ i ].first == sample ) {
			m_selectedClusters.erase( m_selectedClusters.begin() + i );
		}
	}
}

// compare clusters
bool Ab3dManager::lessCluster(
		std::pair< kome::objects::DataGroupNode*, kome::objects::PeaksCluster2D* >& c0,
		std::pair< kome::objects::DataGroupNode*, kome::objects::PeaksCluster2D* >& c1
) {
	return ( c0.second->getIntensity() < c1.second->getIntensity() );
}

// get manager object
Ab3dManager& Ab3dManager::getInstance() {
	// create object (This is the only object.)
	static Ab3dManager mgr;

	return mgr;
}
