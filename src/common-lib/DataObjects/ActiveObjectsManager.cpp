/**
 * @file ActiveObjectsManager.cpp
 * @brief implements of ActiveObjectsManager class
 *
 * @author S.Tanaka
 * @date 2007.12.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ActiveObjectsManager.h"

#include "SampleSet.h"
#include "Sample.h"
#include "Spectrum.h"
#include "Chromatogram.h"
#include "DataGroupNode.h"
#include "DataMapInfo.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define HISTORY_COUNT    9
#define MRU_SECTION      "MRU"


// constructor
ActiveObjectsManager::ActiveObjectsManager() : m_dataSet( NULL ) {
}

// destructor
ActiveObjectsManager::~ActiveObjectsManager() {
	// delete filtered spectra
	for( std::map< Sample*, DataSet* >::iterator it = m_filteredDataSetMap.begin();
			it != m_filteredDataSetMap.end(); it++ ) {
		delete (*it).second;
	}
	m_filteredDataSetMap.clear();
	m_acSample = NULL;
	m_openSpectra.clear();
}

// get the number of opened samples
unsigned int ActiveObjectsManager::getNumberOfOpenedSamples() {
	return m_samples.size();
}

// get opened sample
Sample* ActiveObjectsManager::getOpenedSample( const unsigned int idx ) {
	if( idx >= m_samples.size() ) {
		return NULL;
	}
	return m_samples[ idx ];
}

// sets the active sample
void ActiveObjectsManager::setActiveSample( Sample* sample ) {
	m_acSample = sample;
}

// gets the active sample
Sample* ActiveObjectsManager::getActiveSample() {
	return m_acSample;
}

// get filtered spectra
DataSet* ActiveObjectsManager::getFilteredDataSet( Sample* sample ) {
	// get spectra
	kome::objects::DataSet* dataSet = NULL;

	if( m_filteredDataSetMap.find( sample ) == m_filteredDataSetMap.end() ) {   // new data set
		dataSet = new DataSet( sample->getRootDataGroupNode() );
		if( sample != NULL ) {
			DataGroupNode* root = sample->getRootDataGroupNode();
			root->getDataSet( dataSet );
		}

		for( int i = (int)dataSet->getNumberOfChromatograms() - 1; i >= 0; i-- ) {
			kome::objects::Chromatogram* chrom = dataSet->getChromatogram( i );
			if( chrom->isAutoCreated() ) {
				dataSet->removeChromatogram( chrom );
			}
		}

		m_filteredDataSetMap[ sample ] = dataSet;
	}
	else {
		dataSet = m_filteredDataSetMap[ sample ];
	}

	return dataSet;
}

// search sample
int ActiveObjectsManager::searchSample( Sample* sample ) {
	for( int i = 0; i < (int)m_samples.size(); i++ ) {
		if( sample == m_samples[ i ] ) {
			return i;
		}
	}
	return -1;
}

// clear history
void ActiveObjectsManager::clearFileHistory() {
	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// clear
	m_fileHistory.clear();

	// delete from ini
	if( ini != NULL ) {
		for( unsigned int i = 0; i < HISTORY_COUNT; i++ ) {
			std::string name = FMT( "File%d", ( i + 1 ) );
			ini->deleteValue( MRU_SECTION, name.c_str() );
		}
	}
}

// add history file
void ActiveObjectsManager::addHistoryFile( const char* path ) {
	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// get history
	if( m_fileHistory.size() == 0 ) {
		readHistoryFromIni();
	}

	// absolutepath
	std::string p = absolutepath( path );

	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_fileHistory.size() && idx < 0; i++ ) {
		if( m_fileHistory[ i ].compare( p ) == 0 ) {
			idx = (int)i;
		}
	}

	if( idx >= 0 ) {
		m_fileHistory.erase( m_fileHistory.begin() + idx );
	}

	// add
	m_fileHistory.insert( m_fileHistory.begin(), p );

	// remove
	while( m_fileHistory.size() > HISTORY_COUNT ) {
		m_fileHistory.erase( m_fileHistory.begin() + (int)( m_fileHistory.size() - 1 ) );
	}

	// ini
	if( ini != NULL ) {
		for( unsigned int i = 0; i < m_fileHistory.size(); i++ ) {
			std::string name = FMT( "File%d", ( i + 1 ) );
			ini->setString( MRU_SECTION, name.c_str(), m_fileHistory[ i ].c_str() );
		}
	}
}

// remove history file
void ActiveObjectsManager::removeHistoryFile( const char* path ) {
	// absolute path
	std::string p = absolutepath( path );

	// copy the array
	std::vector< std::string > history;
	if( m_fileHistory.size() > 0 ) {
		history.resize( m_fileHistory.size() );

		for( unsigned int i = 0; i < m_fileHistory.size(); i++ ) {
			history[ i ] = m_fileHistory[ i ];
		}
	}

	// clear
	clearFileHistory();

	// add
	for( unsigned int i = 0; i < history.size(); i++ ) {
		if( history[ i ].compare( p ) != 0 ) {
			addHistoryFile( history[ i ].c_str() );
		}
	}
}

// get the number of history files
unsigned int ActiveObjectsManager::getNumberOfHistoryFiles() {
	// get history
	if( m_fileHistory.size() == 0 ) {
		readHistoryFromIni();
	}

	// size
	return m_fileHistory.size();
}

// get history file
const char* ActiveObjectsManager::getHistoryFile( const unsigned int idx ) {
	// get history
	if( m_fileHistory.size() == 0 ) {
		readHistoryFromIni();
	}

	// path
	if( idx >= m_fileHistory.size() ) {
		return NULL;
	}
	return m_fileHistory[ idx ].c_str();
}

// read history from ini
void ActiveObjectsManager::readHistoryFromIni() {
	// initialize
	m_fileHistory.clear();

	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return;
	}

	// set
	bool loop = true;
	for( unsigned int i = 0; i < HISTORY_COUNT && loop; i++ ) {
		std::string name = FMT( "File%d", ( i + 1 ) );
		std::string path = ini->getString( MRU_SECTION, name.c_str(), "" );

		if( path.empty() ) {
			loop = false;
		}
		else {
			m_fileHistory.push_back( path );
		}
	}
}

// This method is called when a sample is opened
void ActiveObjectsManager::onOpenSample( Sample* sample ) {
	// check
	if( sample == NULL ) {
		return;
	}
	int idx = searchSample( sample );
	if( idx >= 0 ) {
		return;
	}

	m_samples.push_back( sample );
}

// This method is called when a sample is closed
void ActiveObjectsManager::onCloseSample( Sample* sample, const bool deleting ) {
	// delete filtered spectra
	std::vector< DataGroupNode* > groups;
	// @date 2012.04.23 <Mod> M.Izumi
	for( std::map< Sample*, DataSet* >::iterator it = m_filteredDataSetMap.begin();
			it != m_filteredDataSetMap.end(); it++ ) {
		// @date 2012.04.23 <Mod> M.Izumi
		if( sample != NULL && sample == (*it).first ){
			delete (*it).second;
			
			groups.push_back( sample->getRootDataGroupNode() );	// @date 2012.04.23 <Mod> M.Izumi
		}
	}
	for( int i = 0; i < (int)groups.size(); i++ ) {
		m_filteredDataSetMap.erase( groups[ i ]->getSample() ); // @date 2012.04.23 <Mod> M.Izumi
	}

	// delete sample from array
	int idx = searchSample( sample );
	if( idx >= 0 ) {
		m_samples.erase( m_samples.begin() + idx );
	}
}

// get instance
ActiveObjectsManager& ActiveObjectsManager::getInstance() {
	// create object (This is the only object.)
	static ActiveObjectsManager mgr;

	return mgr;
}

// @date 2011.08.24 <Add> M.Izumi ->
// SPEC 82165:Sample name mismatch (Close Dialog)

// add tab sample
void ActiveObjectsManager::addTabSample( Sample* sample ){
	setTabSample();
	m_tmpSamples.push_back( sample );
}

// get 
std::vector< Sample* > ActiveObjectsManager::getTabSamples(){
	return m_tmpSamples;
}

// set tag name
void ActiveObjectsManager::setSampleTagName( Sample* sample, const char* name ) {
	if( name == NULL ) {
		m_tagNameMap.erase( sample );
	}
	else {
		m_tagNameMap[ sample ] = name;
	}
}

// get tag name
const char* ActiveObjectsManager::getSampleTagName( Sample* sample ) {
	if( m_tagNameMap.find( sample ) == m_tagNameMap.end() ) {
		return NULL;
	}
	return m_tagNameMap[ sample ].c_str();
}

// del tab sample
void ActiveObjectsManager::delTabSample( Sample* sample ){
	int i = m_tmpSamples.size()-1;
	while( i > -1 ){
		if( m_tmpSamples[i] == sample ){
			m_tmpSamples.erase( m_tmpSamples.begin() + i );
		}
		i--;
	}
	setTabSample();
}

// set tab sample
void ActiveObjectsManager::setTabSample(){
	std::vector< Sample* > strTmp;
	strTmp = m_tmpSamples;
	m_tmpSamples.clear();

	for( unsigned int i=0; i < strTmp.size(); i++ ){
		m_tmpSamples.push_back( strTmp[i] );
	}
}
// @date 2011.08.24 <Add> M.Izumi <-

// @date 2011.10.03 <Add> M.Izumi ->
// get open spectra
std::vector< Spectrum* > ActiveObjectsManager::getOpenSpectra(){
	return m_openSpectra;
}

// add 
void ActiveObjectsManager::addOpenSpectra( Spectrum* spec ){
	if( spec != NULL ){
		for( unsigned int i=0; i < m_openSpectra.size(); i++ ){
			if( spec == m_openSpectra[i] ){
				return;
			}
		}
		m_openSpectra.push_back( spec );		

	}
}

// remove
void ActiveObjectsManager::removeOpenSpectra( Spectrum* spec ){
	if( spec != NULL ){
		int iSize = m_openSpectra.size()-1;
		while( iSize >= 0 ){
			if( spec == m_openSpectra[iSize] ){
				m_openSpectra.erase( m_openSpectra.begin() + iSize );
				return;
			}
			iSize--;
		}
	}
}
// @date 2011.10.03 <Add> M.Izumi <-
