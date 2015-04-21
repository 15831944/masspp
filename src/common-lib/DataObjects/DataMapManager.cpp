/**
 * @file DataMapManager.cpp
 * @brief implements of DataMapManager class
 *
 * @author S.Tanaka
 * @date 2007.09.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataMapManager.h"

#include <list>
#include <math.h>


using namespace kome::objects;	// @date 2011/06/16 <Add> OKADA


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DataMapManager::DataMapManager() {
}

// destructor
DataMapManager::~DataMapManager() {
	for( std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* >::iterator it = m_dataMap.begin();
			it != m_dataMap.end(); it++ ) {
		delete (*it).second;
	}
	m_dataMap.clear();
}


std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > DataMapManager::getDataMap(){
	return m_dataMap;
}


void DataMapManager::setDataMap2( std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > dataMap ){
	m_dataMap = dataMap;
	return;
}

// on close sample
void DataMapManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// add to array
	std::vector< kome::objects::DataGroupNode* > groups;
	for( std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* >::iterator it = m_dataMap.begin();
			it != m_dataMap.end(); it++ ) {
		kome::objects::DataGroupNode* group = (*it).first;
		if( group->getSample() == sample ) {
			groups.push_back( group );
		}
	}

	// delete
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		kome::objects::DataGroupNode* group = groups[ i ];
		delete m_dataMap[ group ];
		m_dataMap.erase( group );
	}
}

// get instance
DataMapManager& DataMapManager::getInstance() {
	// create object (This is the only object.)
	static DataMapManager mgr;

	return mgr;
}
