/**
 * @file FilterSpectraOperation.cpp
 * @brief Filter Spectra Operation class
 *
 * @author M.Izumi
 * @date 2012.01.30
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "FilterSpectraOperation.h"
#include "SpectraSearchManager.h"
#include "SpectraSearchFilter.h"

using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
FilterSpectraOperation::FilterSpectraOperation(){
	// èâä˙âª
	m_strOptDescript = "";
	m_type			 = SpectraSearchManager::SEARCH_NEW;

}

// destructor
FilterSpectraOperation::~FilterSpectraOperation(){
}

// on execute
bool FilterSpectraOperation::onExecute(){
	bool ret = true;
	
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::objects::DataSet* dataSet = aoMgr.getFilteredDataSet( m_targetSample );

	SpectraSearchManager& mgr = SpectraSearchManager::getInstance();

	// -------------------------------------------------
	// search	
	// -------------------------------------------------
	mgr.searchSpectra( m_targetSample, m_type, m_filter );

	ret = true;
	return ret;
}

// on save condition
void FilterSpectraOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
	if( m_targetSample == NULL ){
		return;
	}
	
	
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::objects::DataSet* dataSet = aoMgr.getFilteredDataSet( m_targetSample );
	if( dataSet == NULL ){
		return;
	}
    // id èëçû
    int id = ( dataSet->getGroup() != NULL ? dataSet->getGroup()->getId() : 0 );
	writeFun( &id, sizeof( id ) );

		
	int inum = dataSet->getNumberOfSpectra();
	writeFun( &inum, sizeof( inum ) );
			
	for( int i=0; i < inum; i++ ){
		kome::objects::Spectrum* spec = dataSet->getSpectrum(i);
		int id = spec->getId();
		writeFun( &id, sizeof( id ) );
	}	
	
}

// on load condition
void FilterSpectraOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	int id = 0;
	readFun( &id, sizeof( id ) );

	kome::objects::DataGroupNode* group = NULL;
	kome::objects::Sample* sample = getTargetSample();
	if( sample != NULL ){
		group = sample->getGroupById( id );
	}
	if( group == NULL ){
		return;
	}
	
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::objects::DataSet* dataSet = aoMgr.getFilteredDataSet( m_targetSample );
	if( dataSet == NULL ){
		return;
	}
	dataSet->clearSpectra();

	int inum = 0;
	readFun( &inum, sizeof( inum ) );
	
	
	for( int i=0; i < inum; i++ ){
		// id ì«çû		
		int id = 0;
		readFun( &id, sizeof( id ) );

		kome::objects::Sample* sample = getTargetSample();
		kome::objects::Spectrum* spec = NULL;	
		if( sample != NULL ){
			spec = sample->getSpectrumById( id );
		}
		if( spec != NULL ){
			dataSet->addSpectrum( spec );
		}
	}
	
	kome::plugin::PluginCallTool::onUpdateDataSet( *dataSet );
}

// gets the description
std::string FilterSpectraOperation::onGetDescription(){
	return m_strOptDescript;
}

// gets the parameter string
std::string FilterSpectraOperation::onGetParametersString(){
	//parameter
	std::string s;
	
	// sample Id, group Id, type, filte...
	s.append( FMT("%d,%d,%s",
		( m_targetSample == NULL ? -1 : m_targetSample->getSampleId() ),
		m_type,
		m_strRange.c_str()));

	int inum = m_filter.getNumberOfPropertyFilters();
	
	for( int i=0; i < inum; i++ ){
		std::string key,value;
		int type;
		key = m_filter.getPropertyFilterKey(i);
		type = m_filter.getPropertyFilterType(i);
		value = m_filter.getPropertyFilterValue(i);
		
		s.append( FMT( ",%s:%d:%s", key.c_str(), type, value.c_str() ));
		
	}

	return s;
}

// sets the parameter string
void FilterSpectraOperation::onSetParametersString( const char* strParam ){
	std::vector<std::string> tokens;
	stringtoken( strParam, ",", tokens );
	if( tokens.size() < 8 ){
		return;
	}

	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	m_type = (SpectraSearchManager::SearchType)toint( tokens[1].c_str(), 10, -1 );

	// stage
	m_filter.resetStageRange();
	m_filter.setStageRange( tokens[2].c_str() );

	// scan
	m_filter.resetScanRange();
	m_filter.setScanRange( tokens[3].c_str() );

	// rt
	m_filter.resetRtRange();
	m_filter.setRtRange( tokens[4].c_str() );

	// tic
	m_filter.resetTicRange();
	m_filter.setTicRange( tokens[5].c_str() );

	// bpc
	m_filter.resetBpcRange();
	m_filter.setBpcRange( tokens[6].c_str() );

	// precursor
	m_filter.resetPrecursorRange();
	m_filter.setPrecursorRange( tokens[7].c_str() );
	
	// Scan Type:1:Auto Mode,Precursor Charge:2:
	for( unsigned int i=9; i < tokens.size(); i++ ){
		std::vector< std::string > strFilter;
		stringtoken( tokens[i].c_str(), ":", strFilter );
		
		m_filter.addPropertyFilter( 
			strFilter[0].c_str(),
			( strFilter.size() == 2 ? strFilter[2].c_str() : "" ),
			(SpectraSearchFilter::PropertyFilterType)toint(strFilter[1].c_str(), 10, -1 )
			);

	}
}

// set type
void FilterSpectraOperation::setType( SpectraSearchManager::SearchType type ){
	m_type = type;
}

// get type
SpectraSearchManager::SearchType FilterSpectraOperation::getType(){
	return m_type;
}

// set filter
void FilterSpectraOperation::setFilter( SpectraSearchFilter filter ){
	m_filter = filter;
}

// get filter
SpectraSearchFilter FilterSpectraOperation::getFilter(){
	return m_filter;
}

// set descript
void FilterSpectraOperation::setDescript( const char* strDescript ){
	m_strOptDescript = strDescript;
}

// get descript
const char* FilterSpectraOperation::getDescript(){
	return m_strOptDescript.c_str();
}

// set range 
void FilterSpectraOperation::setRange( const char* strRange ){
	m_strRange = strRange;
}

// get range
const char* FilterSpectraOperation::getRange(){
	return m_strRange.c_str();
}
