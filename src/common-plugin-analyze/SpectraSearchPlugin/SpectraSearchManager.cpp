/**
 * @file SpectraSearchManager.cpp
 * @brief implements of SpectraSearchManager class
 *
 * @author S.Tanaka
 * @date 2007.01.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectraSearchManager.h"               
#include "SpectraSearchFilter.h"
#include "SpectraSearchDialog.h"
#include "FilterSpectraOperation.h" 


#include <set>

using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SpectraSearchManager::SpectraSearchManager() {
	m_dataSet = NULL;
}

// destructor
SpectraSearchManager::~SpectraSearchManager() {
}

// open dialog
void SpectraSearchManager::openDialog( kome::objects::DataGroupNode* group ) {	
	// check the group
	if( group == NULL ) {
		LOG_ERROR( FMT("A group is not active.") );
		return;
	}
	
	// create dialog
	SpectraSearchDialog dlg( kome::window::WindowTool::getMainWindow(), group );
	dlg.ShowModal();
}

// open dialog
bool SpectraSearchManager::openDialog( kome::objects::DataSet* dataSet, kome::objects::DataSet& ds ){
	bool	bRet = false;									// @Date:2014/02/26	<Add>	A.Ozaki

	// check data set
	if( dataSet == NULL ){
		LOG_ERROR( FMT("A dataSet is not active.") );
		return	bRet;										// @Date:2014/02/26	<Modified>	A.Ozaki
	}

	// create dialog
	SpectraSearchDialog dlg( kome::window::WindowTool::getMainWindow(), dataSet, ds, true );

	// >>>>>>	@Date:2014/02/26	<Modified>	A.Ozaki
	//
	if  ( wxID_OK == dlg.ShowModal() )
	{
		ds = *m_filterSpectra;
		bRet = true;
	}

	return	bRet;
	//
	// <<<<<<	@Date:2014/02/26	<Modified>	A.Ozaki
}

// search spectra
void SpectraSearchManager::searchSpectra( 
	kome::objects::DataSet* dataSet, 
	kome::objects::DataSet& ds,
	SearchType type, 
	SpectraSearchFilter& filter
){
	if( dataSet == NULL ) {
		return;
	}
	// get source data set
	std::vector< kome::objects::Spectrum* > src;
	std::set< kome::objects::Spectrum* > allowed;
	if( type == SEARCH_NARROW ) {
		for( unsigned i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
			src.push_back( dataSet->getSpectrum( i ) );
		}
	}
	else {
		kome::objects::DataSet org( dataSet->getGroup() );
		dataSet->getGroup()->getDataSet( &org );
		for( unsigned int i = 0; i < org.getNumberOfSpectra(); i++ ) {
			src.push_back( org.getSpectrum( i ) );
		}

		if( type == SEARCH_APPEND ) {
			for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
				allowed.insert( dataSet->getSpectrum( i ) );
			}
		}
	}
	if( src.size() == 0 ) {
		wxMessageBox(
			wxT( "Filter Spectra cannot be used because there are no spectra to filter. Open a data file containing spectra." ),
			wxT( "Mass++" ),
			wxOK | wxICON_HAND
		);
		return;
	}

	// search
	std::vector< kome::objects::Spectrum* > dst;
	filter.filter( src, dst );
	for( unsigned int i = 0; i < dst.size(); i++ ) {
		allowed.insert( dst[ i ] );
	}

	// update
	//dataSet->clearSpectra();
	ds.clearSpectra();
	for( unsigned int i = 0; i < src.size(); i++ ) {
		kome::objects::Spectrum* spec = src[ i ];
		if( allowed.find( spec ) != allowed.end() ) {
			ds.addSpectrum( spec );
		}
	}

	// clear chromatograms
	ds.clearChromatograms();
}

// search spectra
void SpectraSearchManager::searchSpectra( kome::objects::Sample* sample, SearchType type, SpectraSearchFilter& filter ) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// check the group
	if( sample == NULL ) {
		LOG_ERROR( FMT("A sample is not active." ));
		return;
	}

	kome::objects::DataSet* dataSet = aoMgr.getFilteredDataSet( sample );
	
	if( dataSet == NULL ) {
		return;
	}

	// get source data set
	std::vector< kome::objects::Spectrum* > src;
	std::set< kome::objects::Spectrum* > allowed;
	if( type == SEARCH_NARROW ) {
		for( unsigned i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
			src.push_back( dataSet->getSpectrum( i ) );
		}
	}
	else {
		kome::objects::DataSet org( sample->getRootDataGroupNode() );
		sample->getRootDataGroupNode()->getDataSet( &org );
		for( unsigned int i = 0; i < org.getNumberOfSpectra(); i++ ) {
			src.push_back( org.getSpectrum( i ) );
		}

		if( type == SEARCH_APPEND ) {
			for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
				allowed.insert( dataSet->getSpectrum( i ) );
			}
		}
	}
	if( src.size() == 0 ) {
		wxMessageBox(
			wxT( "Filter Spectra cannot be used because there are no spectra to filter. Open a data file containing spectra." ),
			wxT( "Mass++" ),
			wxOK | wxICON_HAND
		);
		return;
	}

	// search
	std::vector< kome::objects::Spectrum* > dst;
	filter.filter( src, dst );
	for( unsigned int i = 0; i < dst.size(); i++ ) {
		allowed.insert( dst[ i ] );
	}

	// update
	dataSet->clearSpectra();
	for( unsigned int i = 0; i < src.size(); i++ ) {
		kome::objects::Spectrum* spec = src[ i ];
		if( allowed.find( spec ) != allowed.end() ) {
			dataSet->addSpectrum( spec );
		}
	}

	// clear chromatograms
	dataSet->clearChromatograms();

	// broadcast
	kome::plugin::PluginCallTool::onUpdateDataSet( *dataSet );

	// set DataSet
	setDataSet( dataSet );
}

// execute filter
void SpectraSearchManager::executeFilter(
		SpectraSearchFilter& filter,
		kome::objects::DataSet& dataSet,
		std::set< kome::objects::Spectrum* >& specSet
) {
	// create array
	std::vector< kome::objects::Spectrum* > src;
	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		src.push_back( dataSet.getSpectrum( i ) );
	}
	std::vector< kome::objects::Spectrum* > dst;

	// filter
	filter.filter( src, dst );

	// insert to set
	for( unsigned int i = 0; i < dst.size(); i++ ) {
		specSet.insert( dst[ i ] );
	}
}

// updates spectra
void SpectraSearchManager::updateSpectra(
		kome::objects::DataSet& dataSet,
		std::set< kome::objects::Spectrum* >& specSet
) {
	// create array
	std::vector< kome::objects::Spectrum* > specArray;
	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
		if( specSet.find( spec ) != specSet.end() ) {
			specArray.push_back( spec );
		}
	}

	// update
	dataSet.clearSpectra();
	for( unsigned int i = 0; i < specArray.size(); i++ ) {
		dataSet.addSpectrum( specArray[ i ] );
	}
}

// get instance
SpectraSearchManager& SpectraSearchManager::getInstance() {
	// create object (This is the only object.)
	static SpectraSearchManager mgr;

	return mgr;
}

// get search type name タイプから文字列取得
const char* SpectraSearchManager::getSearchTypeName( SpectraSearchManager::SearchType type ){
	std::string strTypeName = "";
	
	switch( type ){
	case SEARCH_NEW:
		strTypeName = "SEARCH_NEW";
		break;
	case SEARCH_APPEND:
		strTypeName = "SEARCH_APPEND";
		break;
	case SEARCH_NARROW:
		strTypeName = "SEARCH_NARROW";
		break;
	default:
		break;
	};
	m_strTypeName = strTypeName;
	return m_strTypeName.c_str();
}

// get search type 文字列からタイプ取得
SpectraSearchManager::SearchType SpectraSearchManager::getSearchType( std::string typeName ){

	SpectraSearchManager::SearchType type;
	 
	if( typeName.compare( "SEARCH_NEW" ) == 0 ){
		type = SEARCH_NEW;
	}else if( typeName.compare( "SEARCH_APPEND" ) == 0 ){
		type = SEARCH_APPEND;
	}else if( typeName.compare( "SEARCH_NARROW" ) == 0 ){
		type = SEARCH_NARROW;
	}

	m_type = type;
	return m_type;
}

// set DataSet 
void SpectraSearchManager::setDataSet( kome::objects::DataSet* dataset ){
	m_dataSet = dataset;
}

// get DataSet
kome::objects::DataSet* SpectraSearchManager::getDataSet(){
	return m_dataSet;
}

// set filter spectra
void SpectraSearchManager::setFilterSpectra( kome::objects::DataSet* dataset ){

	m_filterSpectra = dataset;
}

// get filter spectra
kome::objects::DataSet* SpectraSearchManager::getFilterSpectra(){
	return m_filterSpectra;
}
