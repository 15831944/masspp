/**
 * @file CompositionManager.cpp
 * @brief implements of CompositionManager class
 *
 * @author S.Tanaka
 * @date 2009.02.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CompositionManager.h"
#include "SampleFusionOperation.h"


using namespace kome::composition;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
CompositionManager::CompositionManager() {
}

// destructor
CompositionManager::~CompositionManager() {
}

// add composition sample
void CompositionManager::addData(
		kome::objects::Sample* data,
		kome::objects::DataGroupNode* ctrlGroup,
		kome::objects::DataGroupNode* trmtGroup
) {
	// check parameter
	if( data == NULL ) {
		return;
	}

	// add to map
	m_compositionMap[ data ].first = ( ctrlGroup == NULL ? NULL : ctrlGroup->getSample() );
	m_compositionMap[ data ].second = ( trmtGroup == NULL ? NULL : trmtGroup->getSample() );
}

// on close sample
void CompositionManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// check parameter
	if( sample == NULL ) {
		return;
	}

	// check the map
	if( m_compositionMap.find( sample ) != m_compositionMap.end() ) {
		m_compositionMap.erase( sample );
	}

	std::set< kome::objects::Sample* > sampleSet;
	for( std::map< kome::objects::Sample*, std::pair< kome::objects::Sample*, kome::objects::Sample* > >::iterator it = m_compositionMap.begin();
			it != m_compositionMap.end(); it++ ) {
		if( sample == (*it).second.first || sample == (*it).second.second ) {
			sampleSet.insert( (*it).first );
		}
	}

	// close composition samples
	for( std::set< kome::objects::Sample* >::iterator it = sampleSet.begin();
			it != sampleSet.end(); it++ ) {
		(*it)->closeSample();
	}
}


// get instance
CompositionManager& CompositionManager::getInstance() {
	// create object (This is the only object.)
	static CompositionManager mgr;

	return mgr;
}

