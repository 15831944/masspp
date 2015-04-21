/**
 * @file DataManager.cpp
 * @brief implements of DataManager class
 *
 * @author S.Tanaka
 * @date 2006.10.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataManager.h"

#include "Sample.h"
#include "Spectrum.h"
#include "Chromatogram.h"
#include "DataMapInfo.h"
#include "ActiveObjectsManager.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static object
std::vector< DataManager* > DataManager::m_dataMgrs;
bool DataManager::m_sorted = true;


// constructor
DataManager::DataManager() {
	m_order = 0;

	m_dataMgrs.push_back( this );
	m_sorted = false;
}

// destructor
DataManager::~DataManager() {
	for( int i = (int)m_dataMgrs.size() - 1; i >= 0; i-- ) {
		if( this == m_dataMgrs[ i ] ) {
			m_dataMgrs.erase( m_dataMgrs.begin() + i );
		}
	}
}

// open sample
void DataManager::openSample( Sample* sample ) {
	sortDataManagers();
	for( unsigned int i = 0; i < m_dataMgrs.size(); i++ ) {
		m_dataMgrs[ i ]->onOpenSample( sample );
	}
}

// close sample
void DataManager::closeSample( Sample* sample, const bool deleting ) {
	// active object
	ActiveObjectsManager& aoMgr = ActiveObjectsManager::getInstance();
	if( sample == aoMgr.getActiveSample() ) {
		aoMgr.setActiveSample( NULL );
	}

	// each managers
	sortDataManagers();
	for( unsigned int i = 0; i < m_dataMgrs.size(); i++ ) {
		m_dataMgrs[ i ]->onCloseSample( sample, deleting );
	}
}

// open spectrum
void DataManager::openSpectrum( Spectrum* spec ) {
	sortDataManagers();
	for( unsigned int i = 0; i < m_dataMgrs.size(); i++ ) {
		m_dataMgrs[ i ]->onOpenSpectrum( spec );
	}
}

// close spectrum
void DataManager::closeSpectrum( Spectrum* spec, const bool deleting ) {
	sortDataManagers();
	for( unsigned int i = 0; i < m_dataMgrs.size(); i++ ) {
		m_dataMgrs[ i ]->onCloseSpectrum( spec, deleting );
	}
}

// open chromatogram
void DataManager::openChromatogram( Chromatogram* chrom ) {
	sortDataManagers();
	for( unsigned int i = 0; i < m_dataMgrs.size(); i++ ) {
		m_dataMgrs[ i ]->onOpenChromatogram( chrom );
	}
}

// close chromatogram
void DataManager::closeChromatogram( Chromatogram* chrom, const bool deleting ) {
	sortDataManagers();
	for( unsigned int i = 0; i < m_dataMgrs.size(); i++ ) {
		m_dataMgrs[ i ]->onCloseChromatogram( chrom, deleting );
	}
}

// open data map
void DataManager::openDataMap( DataMapInfo* dataMap ) {
	sortDataManagers();
	for( unsigned int i = 0; i < m_dataMgrs.size(); i++ ) {
		m_dataMgrs[ i ]->onOpenDataMap( dataMap );
	}
}

// close data map
void DataManager::closeDataMap( DataMapInfo* dataMap, const bool deleting ) {
	sortDataManagers();
	for( unsigned int i = 0; i < m_dataMgrs.size(); i++ ) {
		m_dataMgrs[ i ]->onCloseDataMap( dataMap, deleting );
	}
}

// compare data managers
bool DataManager::lessManager( DataManager* dm0, DataManager* dm1 ) {
	return ( dm0->m_order < dm1->m_order );
}

// sort data managers
void DataManager::sortDataManagers() {
	if( m_sorted ) {
		return;
	}

	std::sort( m_dataMgrs.begin(), m_dataMgrs.end(), lessManager );
	m_sorted = true;
}
