/**
 * @file ClrSearchEngine.cpp
 * @brief implements of ClrSearchEngine class
 *
 * @author S.Tanaka
 * @date 2012.11.07
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ClrSearchEngine.h"

#include "ClrSearchEngineBase.h"
#include "HitWrapper.h"
#include "SearchResultWrapper.h"

#include "IdentificationManagerWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrSearchEngine::ClrSearchEngine( const char* name, const char* version, const bool multipleSpec )
		: kome::ident::SearchEngine( name, version, multipleSpec ) {
}

// destructor
ClrSearchEngine::~ClrSearchEngine() {
	m_baseSearchEngine = nullptr;
	ClrSearchEngineManager::getInstance().removeSearchEngine( *this );
}

// set base search engine
void ClrSearchEngine::setBaseSearchEngine( ClrSearchEngineBase^ baseSearchEngine ) {
	m_baseSearchEngine = baseSearchEngine;
}

// get base search engine
ClrSearchEngineBase^ ClrSearchEngine::getBaseSearchEngine() {
	ClrSearchEngineBase^ baseSearchEngine = m_baseSearchEngine;
	return baseSearchEngine;
}

// on search
std::string ClrSearchEngine::onSearch(
		const char* title,
		const char* comment,
		kome::ident::SpecInfo** spectra,
		const unsigned int specNum,
		kome::objects::SettingParameterValues* settings,
		kome::core::Progress& progress
) {
	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return false;
	}

	// array
	cli::array< HitWrapper::SpecInfo^ >^ arr = nullptr;
	if( spectra != NULL && specNum > 0 ) {
		arr = gcnew cli::array< HitWrapper::SpecInfo^ >( specNum );
		for( unsigned int i = 0; i < specNum; i++ ) {
			arr[ i ] = HitWrapper::copySpectrum( spectra[ i ] );
		}
	}

	// search
	System::String^ ret = engine->onSearch(
		ClrObjectTool::convertString( title, NULL ),
		ClrObjectTool::convertString( comment, NULL ),
		arr,
		ClrObjectTool::createSettingsWrapper( settings ),
		ClrObjectTool::createProgressWrapper( &progress )
	);

	return ClrObjectTool::convertString( ret );
}

// on prepare peak detection
bool ClrSearchEngine::onPreparePeakDetection(
		kome::objects::SettingParameterValues* searchSettings,
		const char* peakDetector,
		kome::objects::SettingParameterValues* peakSettings,
		const char* chargeDetector,
		kome::objects::SettingParameterValues* chargeSettings,
		const char* peakFilter
) {
	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return false;
	}

	// prepare
	return engine->onPreparePeakDetection(
		ClrObjectTool::createSettingsWrapper( searchSettings ),
		ClrObjectTool::convertString( peakDetector, NULL ),
		ClrObjectTool::createSettingsWrapper( peakSettings ),
		ClrObjectTool::convertString( chargeDetector, NULL ),
		ClrObjectTool::createSettingsWrapper( chargeSettings ),
		ClrObjectTool::convertString( peakFilter, NULL )
	);
}

// on get search status
kome::ident::SearchEngine::SearchStatus ClrSearchEngine::onGetSearchStatus( const char* id ) {
	// return valeu
	kome::ident::SearchEngine::SearchStatus status = kome::ident::SearchEngine::STATUS_UNKNOWN;

	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return status;
	}

	// status
	SearchEngineWrapper::SearchStatus s = engine->getSearchStatus( ClrObjectTool::convertString( id, NULL ) );
	if( s == SearchEngineWrapper::SearchStatus::WAITING ) {
		status = kome::ident::SearchEngine::STATUS_WAITING;
	}
	else if( s == SearchEngineWrapper::SearchStatus::RUNNING ) {
		status = kome::ident::SearchEngine::STATUS_RUNNING;
	}
	else if( s == SearchEngineWrapper::SearchStatus::COMPLETED ) {
		status = kome::ident::SearchEngine::STATUS_COMPLETED;
	}
	else if( s == SearchEngineWrapper::SearchStatus::ERROR ) {
		status = kome::ident::SearchEngine::STATUS_ERROR;
	}

	return status;
}

// on get search result
void ClrSearchEngine::onGetResult(
		kome::ident::SearchResult& result,
		const char* id,
		kome::ident::SpecInfo** spectra,
		const unsigned int specNum,
		kome::core::Progress& progress
) {
	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return;
	}

	// array
	cli::array< HitWrapper::SpecInfo^ >^ specArr = nullptr;
	if( spectra != NULL && specNum > 0 ) {
		specArr = gcnew cli::array< HitWrapper::SpecInfo^ >( specNum );

		for( unsigned int i = 0; i < specNum; i++ ) {
			specArr[ i ] = HitWrapper::copySpectrum( spectra[ i ] );
		}
	}

	// get result
	engine->onGetResult(
		gcnew SearchResultWrapper( result ),
		ClrObjectTool::convertString( id, NULL ),
		specArr,
		ClrObjectTool::createProgressWrapper( &progress )
	);
}

// open result
bool ClrSearchEngine::onOpenResult( const char* resultLocation ) {
	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return false;
	}

	// open result
	return engine->onOpenResult( ClrObjectTool::convertString( resultLocation, NULL ) );
}

// on prepare search
bool ClrSearchEngine::onPrepareSearch( kome::core::Progress& progress ) {
	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return false;
	}

	// on prepare search
	return engine->onPrepareSearch(
		ClrObjectTool::createProgressWrapper( &progress )
	);
}

// on end search
void ClrSearchEngine::onEndSearch() {
	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return;
	}

	// on end search
	engine->onEndSearch();
}

// is valid spectrum
bool ClrSearchEngine::isValidSpectrum( kome::objects::Spectrum* spec ) {
	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return false;
	}

	// judge
	return engine->isValidSpectrum( ClrObjectTool::createSpectrumWrapper( spec ) );
}


// on spectrum peaks
void ClrSearchEngine::onSpectrumPeaks(
		kome::objects::Spectrum* spec,
		kome::ident::SpecInfo* specInfo,
		kome::objects::Peaks* peaks,
		int* peakIds,
		const unsigned int peakIdsSize
) {
	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return;
	}

	// array
	cli::array< int >^ arr = nullptr;
	if( peakIds != NULL && peakIdsSize > 0 ) {
		arr = gcnew cli::array< int >( peakIdsSize );
		for( unsigned int i = 0; i < peakIdsSize; i++ ) {
			arr[ i ] = peakIds[ i ];
		}
	}

	// spectrum peaks
	engine->onSpectrumPeaks(
		ClrObjectTool::createSpectrumWrapper( spec ),
		HitWrapper::copySpectrum( specInfo ),
		ClrObjectTool::createPeaksWrapper( peaks ),
		arr
	);
} 

// on finish peak detection
bool ClrSearchEngine::onFinishPeakDetection() {
	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return false;
	}

	// on finish peak detection
	return engine->onFinishPeakDetection();
}

// on get search information
void ClrSearchEngine::onGetSearchInfo( kome::ident::SearchInfo& searchInfo, kome::objects::SettingParameterValues* settings ) {
	// manager
	IdentificationManagerWrapper^ mgr = IdentificationManagerWrapper::getInstance();

	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return;
	}

	// search info
	SearchEngineWrapper::SearchInfo^ si = gcnew SearchEngineWrapper::SearchInfo();
	engine->onGetSearchInfo( si, ClrObjectTool::createSettingsWrapper( settings ) );

	SearchEngineWrapper::copySearchInfo( si, searchInfo );
}

// validate settings
std::string ClrSearchEngine::validateSettings( kome::objects::SettingParameterValues* paramSettings ) {
	// return value
	std::string ret;

	// get base object
	ClrSearchEngineBase^ engine = getBaseSearchEngine();
	if( engine == nullptr ) {
		return ret;
	}

	// validate
	System::String^ s = engine->validateSettings( ClrObjectTool::createSettingsWrapper( paramSettings ) );
	ret = ClrObjectTool::convertString( s );

	return ret;
}

//
// ClrSearchEngineManager
//

// constructor
ClrSearchEngineManager::ClrSearchEngineManager() {
}

// destructor
ClrSearchEngineManager::~ClrSearchEngineManager() {
	while( m_engineSet.size() > 0 ) {
		delete *( m_engineSet.begin() );
	}
}

// create search engine object
ClrSearchEngine& ClrSearchEngineManager::createSearchEngine( const char* name, const char* version, const bool multipleSpec ) {
	ClrSearchEngine* engine = new ClrSearchEngine( name, version, multipleSpec );
	m_engineSet.insert( engine );

	return *engine;
}

// remove search engine
void ClrSearchEngineManager::removeSearchEngine( ClrSearchEngine& engine ) {
	m_engineSet.erase( &engine );
}

// destroy search engine
void ClrSearchEngineManager::destroySearchEngine( ClrSearchEngine& engine ) {
	if( m_engineSet.find( &engine ) != m_engineSet.end() ) {
		delete &engine;
	}
}

// get instance
ClrSearchEngineManager& ClrSearchEngineManager::getInstance() {
	// create object
	static ClrSearchEngineManager mgr;

	return mgr;
}
