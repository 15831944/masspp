/**
 * @file SearchEngine.cpp
 * @brief implementation of SearchEngine class
 *
  * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "Substance.h"
#include "Hit.h"
#include "SearchResult.h"
#include "SearchEngine.h"
#include "IdentificationManager.h"

#include <string>
#include <vector>
#include <boost/bind.hpp>

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
SearchEngine::SearchEngine( const char* name, const char* version, const bool multipleSpec )
		: m_strName( NVL( name, "" ) ), m_strVersion( NVL( version, "" ) ) {
	IdentificationManager& identMgr = IdentificationManager::getInstance();
	identMgr.addSearchEngine(this);
	m_multipleSpec = multipleSpec;
}

// destructor
SearchEngine::~SearchEngine( void )
{
	IdentificationManager& identMgr = IdentificationManager::getInstance();
	identMgr.removeSearchEngine(this);
}

// get the search engine name
std::string SearchEngine::getName( void ) const
{
	return m_strName;
}

// get the search engine version
std::string SearchEngine::getVersion( void ) const
{
	return m_strVersion;
}

// multiple spectra or not
bool SearchEngine::isMultipleSpectrum() {
	return m_multipleSpec;
}

// search
int SearchEngine::search(
		const char* title,
		const char* comment,
		SpecInfo** spectra,
		const unsigned int specNum,
		kome::objects::SettingParameterValues* settings,
		kome::core::Progress* progress
) {
	// managers
	IdentificationManager& mgr = IdentificationManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// progress
	kome::core::Progress* prgs = progress;
	if( prgs == NULL ) {
		prgs = &( kome::core::Progress::getIgnoringProgress() );
	}

	// check parameters
	std::string msg = validateSettings( settings );
	if( !msg.empty() ) {
		LOG_ERROR_CODE( msg, ERR_OTHER );
		return -1;
	}

	// ID
	std::string id = onSearch( title, comment, spectra, specNum, settings, *prgs );
	if( id.empty() ) {
		LOG_ERROR_CODE( FMT( "Fatal error: [%s]", msg.c_str() ), ERR_OTHER );
		return -1;
	}

	// finish the search
	onEndSearch();

	// basic information
	time_t now = time( NULL );
	tm* t = localtime( &now );

	m_searchInfo.engineId = mgr.addSearchEngine( m_strName.c_str(), m_strVersion.c_str() );
	m_searchInfo.name = title;
	m_searchInfo.comment = comment;
	m_searchInfo.date = datetostring( t );
	m_searchInfo.spectra = spectra;
	m_searchInfo.specNum = specNum;
	m_searchInfo.searchId = id;

	// parameters
	while( m_searchInfo.searchProps.getNumberOfProperties() > 0 ) {
		m_searchInfo.searchProps.deleteParameter( m_searchInfo.searchProps.getKey( 0 ) );
	}
	if( settings != NULL ) {
		for( unsigned int i = 0; i < settings->getNumberOfParameters(); i++ ) {
			std::string name = settings->getParameterName( i );
			std::string val = settings->getParameterValue( i );

			m_searchInfo.searchProps.setValue( name.c_str(), val.c_str() );
		}
	}

	// get search information
	onGetSearchInfo( m_searchInfo, settings );
	_ASSERT( !m_searchInfo.db.empty() );
	_ASSERT( m_searchInfo.precMassType != SearchResult::MASS_UNDEFINED );
	_ASSERT( m_searchInfo.fragMassType != SearchResult::MASS_UNDEFINED );
//	_ASSERT( m_searchInfo.enzyme != NULL );

	int ret = mgr.addSearch( m_searchInfo );

 	return ret;
}

// get search status
SearchEngine::SearchStatus SearchEngine::getSearchStatus( const char* id ) {
	return onGetSearchStatus( id );
}

// get search result
void SearchEngine::getResult( SearchResult& result, const char* id, kome::core::Progress& progress ) {
	// manager
	IdentificationManager& mgr = IdentificationManager::getInstance();

	// set search engine
	result.setSearchEngine( this );

	// search ID
	int searchId = mgr.getSearch( this->m_strName.c_str(), m_strVersion.c_str(), id );
	SpecInfo** spectra = NULL;
	int specNum = 0;

	// spectra
	std::vector< int > specIds;
	mgr.getSearchSpectraFromSearch( specIds, searchId );
	if( specIds.size() > 0 ) {
		spectra = new SpecInfo*[ specIds.size() ];
		specNum = specIds.size();
	}

	for( unsigned int i = 0; i < specIds.size(); i++ ) {
		SpecInfo* spec = new SpecInfo();

		int specId = 0;
		int peaksId = -1;
		double precursor = -1.0;
		mgr.getSearchSpectrum( specIds[ i ], &specId, NULL, &peaksId, &precursor );
		mgr.getSpectrum( specId, *spec );
		mgr.getCharges( specIds[ i ], spec->charges );
		spec->peaks = peaksId;
		spec->precursor = precursor;

		spectra[ i ] = spec;

		result.addSpectrum( spec );
	}

	// get result
	progress.setStatus( "Getting the search result..." );
	onGetResult( result, id, spectra, specNum, progress );

	// check the result
	_ASSERT( !result.getResultLocation().empty() );

	// delete
	if( spectra != NULL ) {
		delete[] spectra;
	}
}

// display the search result referenced by the specified result location
bool SearchEngine::openResult( const char* resultLocation )
{
	return onOpenResult(resultLocation);
}

// preparation for search
bool SearchEngine::prepareSearch( kome::core::Progress& progress ) {
	// preparation
	return onPrepareSearch( progress );
}

// preparation for peak detection
int SearchEngine::preparePeakDetection(
		kome::objects::SettingParameterValues* searchSettings,
		const char* peakDetector,
		kome::objects::SettingParameterValues* peakSettings,
		const char* chargeDetector,
		kome::objects::SettingParameterValues* chargeSettings,
		const char* peakFilter
) {
	// manager
	IdentificationManager& identMgr = IdentificationManager::getInstance();

	// initialize searh info
	m_searchInfo.name.clear();
	m_searchInfo.comment.clear();
	m_searchInfo.date.clear();
	m_searchInfo.engineId = -1;
	m_searchInfo.db.clear();
	m_searchInfo.result.clear();
	m_searchInfo.precMassType = SearchResult::MASS_UNDEFINED;
	m_searchInfo.fragMassType = SearchResult::MASS_UNDEFINED;
	m_searchInfo.enzyme = NULL;
	m_searchInfo.baseName.clear();
	m_searchInfo.fixedMods.clear();
	m_searchInfo.varMods.clear();
	for( int i = (int)m_searchInfo.searchProps.getNumberOfProperties() - 1; i >= 0; i-- ) {
		const char* key = m_searchInfo.searchProps.getKey( i );
		m_searchInfo.searchProps.deleteParameter( key );
	}
	m_searchInfo.searchId.clear();
	m_searchInfo.specNum = NULL;
	m_searchInfo.specNum = 0;

	// peak detection
	int peakDetection = -1;

	if( peakDetector != NULL ) {
		peakDetection = identMgr.addPeakDetection( NVL( peakDetector, "" ) );

		kome::core::Properties peakProps;
		if( peakSettings != NULL ) {
			kome::objects::SettingParameterValues::writeAllChildParameters( *peakSettings, peakProps, "Peak Detector" );
		}

		peakProps.setValue( "Charge Detector", NVL( chargeDetector, "" ) );
		if( chargeSettings != NULL ) {
			kome::objects::SettingParameterValues::writeAllChildParameters( *chargeSettings, peakProps, "Charge Detector" );
		}
		
		std::string pkFilter = trimstring( peakFilter );
		if( !pkFilter.empty() ) {
			peakProps.setValue( "Peak Filter", pkFilter.c_str() );
		}
		
		for( unsigned int i = 0; i < peakProps.getNumberOfProperties(); i++ ) {
			identMgr.setPeakParameter( peakDetection, peakProps.getKey( i ), peakProps.getValue( i ) );
		}
	}
		
	// preparation
	if( !onPreparePeakDetection(
			searchSettings,
			peakDetector,
			peakSettings,
			chargeDetector,
			chargeSettings,
			peakFilter
	) ) {
		if( peakDetection >= 0 ) {
			identMgr.deletePeakDetection( peakDetection );
			peakDetection = -1;
		}
	}

	return peakDetection;
}
