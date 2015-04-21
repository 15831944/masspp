/**
 * @file SearchEngineWrapper.cpp
 * @brief implements of SearchEngineWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "SearchEngineWrapper.h"

#include "HitWrapper.h"
#include "SearchResultWrapper.h"

#include <vector>


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SearchEngineWrapper::SearchEngineWrapper( kome::ident::SearchEngine& engine )
		: m_engine( engine ) {
}

// destrucotr
SearchEngineWrapper::~SearchEngineWrapper() {
}

// get search engine object
kome::ident::SearchEngine& SearchEngineWrapper::getSearchEngine() {
	return m_engine;
}

// get name
System::String^ SearchEngineWrapper::getName() {
	return ClrObjectTool::convertString( m_engine.getName().c_str(), NULL );
}

// get version
System::String^ SearchEngineWrapper::getVersion() {
	return ClrObjectTool::convertString( m_engine.getVersion().c_str(), NULL );
}

// is multiple spectrum
bool SearchEngineWrapper::isMultipleSpectrum() {
	return m_engine.isMultipleSpectrum();
}

// search
int SearchEngineWrapper::search(
		System::String^ title,
		System::String^ comment,
		cli::array< HitWrapper::SpecInfo^ >^ spectra,
		SettingParameterValuesWrapper^ settings,
		ProgressWrapper^ progress
) {
	// array
	std::vector< kome::ident::SpecInfo* > arr;
	if( spectra != nullptr ) {
		for( int i = 0; i < spectra->Length; i++ ) {
			kome::ident::SpecInfo* spec = new kome::ident::SpecInfo();
			HitWrapper::copySpectrum( spectra[ i ], *spec );
			arr.push_back( spec );
		}
	}

	// search
	int ret = m_engine.search(
		ClrObjectTool::convertString( title ).c_str(),
		ClrObjectTool::convertString( comment ).c_str(),
		( arr.size() == 0 ? NULL : &( arr[ 0 ] ) ),
		arr.size(),
		ClrObjectTool::getSettings( settings ),
		ClrObjectTool::getProgress( progress )
	);

	// delete
	for( unsigned int i = 0; i < arr.size(); i++ ) {
		delete arr[ i ];
	}
	arr.clear();

	return ret;
}

// get status
SearchEngineWrapper::SearchStatus SearchEngineWrapper::getSearchStatus( System::String^ id ) {
	kome::ident::SearchEngine::SearchStatus status =  m_engine.getSearchStatus( ClrObjectTool::convertString( id ).c_str() );

	SearchStatus s = SearchStatus::UNKNOWN;
	if( status == kome::ident::SearchEngine::STATUS_WAITING ) {
		s = SearchStatus::WAITING;
	}
	else if( status == kome::ident::SearchEngine::STATUS_RUNNING ) {
		s = SearchStatus::RUNNING;
	}
	else if( status == kome::ident::SearchEngine::STATUS_COMPLETED ) {
		s = SearchStatus::COMPLETED;
	}
	else if( status == kome::ident::SearchEngine::STATUS_ERROR ) {
		s = SearchStatus::ERROR;
	}

	return s;
}

// get result
void SearchEngineWrapper::getResult( SearchResultWrapper^ result, System::String^ id, ProgressWrapper^ progress  ) {
	// check the parameter
	if( result == nullptr ) {
		return;
	}

	// progress
	kome::core::Progress* prgs = ClrObjectTool::getProgress( progress );
	if( prgs == NULL ) {
		prgs = &( kome::core::Progress::getIgnoringProgress() );
	}

	// get result
	m_engine.getResult(
		result->getSearchResult(),
		ClrObjectTool::convertString( id ).c_str(),
		*prgs
	);
}

// open result
bool SearchEngineWrapper::openResult( System::String^ resultLocation ) {
	return m_engine.openResult( ClrObjectTool::convertString( resultLocation ).c_str() );
}

// prepare search
bool SearchEngineWrapper::prepareSearch( ProgressWrapper^ progress ) {
	kome::core::Progress* prgs = ClrObjectTool::getProgress( progress );
	if( prgs == NULL ) {
		prgs = &( kome::core::Progress::getIgnoringProgress() );
	}

	return m_engine.prepareSearch( *prgs );
}

// valid spectrum
bool SearchEngineWrapper::isValidSpectrum( SpectrumWrapper^ spec ) {
	return m_engine.isValidSpectrum( ClrObjectTool::getSpectrum( spec ) );
}

// prepare peak detection
int SearchEngineWrapper::preparePeakDetection(
		SettingParameterValuesWrapper^ searchSettings,
		System::String^ peakDetector,
		SettingParameterValuesWrapper^ peakSettings,
		System::String^ chargeDetector,
		SettingParameterValuesWrapper^ chargeSettings,
		System::String^ peakFilter
) {
	return m_engine.preparePeakDetection(
		ClrObjectTool::getSettings( searchSettings ),
		ClrObjectTool::convertString( peakDetector ).c_str(),
		ClrObjectTool::getSettings( peakSettings ),
		ClrObjectTool::convertString( chargeDetector ).c_str(),
		ClrObjectTool::getSettings( chargeSettings ),
		ClrObjectTool::convertString( peakFilter ).c_str()
	);
}
			
// on spectrum peaks
void SearchEngineWrapper::onSpectrumPeaks(
		SpectrumWrapper^ spec,
		HitWrapper::SpecInfo^ specInfo,
		PeaksWrapper^ peaks,
		cli::array< int >^ peakIds
) {
	// SpecInfo
	kome::ident::SpecInfo s;
	HitWrapper::copySpectrum( specInfo, s );

	// ids
	std::vector< int > ids;
	if( peakIds != nullptr ) {
		for( int i = 0; i < peakIds->Length; i++ ) {
			ids.push_back( peakIds[ i ] );
		}
	}
		
	m_engine.onSpectrumPeaks(
		ClrObjectTool::getSpectrum ( spec ),
		&s,
		ClrObjectTool::getPeaks( peaks ),
		( ids.size() == 0 ? NULL : &( ids[ 0 ] ) ),
		ids.size()
	);
}

// on finish peak detection
bool SearchEngineWrapper::onFinishPeakDetection() {
	return m_engine.onFinishPeakDetection();
}

// validate settings
System::String^ SearchEngineWrapper::validateSettings( SettingParameterValuesWrapper^ paramSettings ) {
	return ClrObjectTool::convertString( 
		m_engine.validateSettings( ClrObjectTool::getSettings( paramSettings ) ).c_str(),
		NULL
	);
}

// get search engine object
SearchEngineWrapper^ SearchEngineWrapper::getEngineFromVariant( ClrVariant^ var ) {
	long long pt = (long long)( var->obj );
	kome::ident::SearchEngine* engine = (kome::ident::SearchEngine*)pt;
	if( engine == NULL ) {
		return nullptr;
	}

	return gcnew SearchEngineWrapper( *engine );
}

// copy search info
SearchEngineWrapper::SearchInfo^ SearchEngineWrapper::copySearchInfo( kome::ident::SearchInfo* search ) {
	if( search == NULL ) {
		return nullptr;
	}

	SearchInfo^ s = gcnew SearchInfo();
	s->name = ClrObjectTool::convertString( search->name.c_str(), NULL );
	s->comment = ClrObjectTool::convertString( search->comment.c_str(), NULL );
	s->date = ClrObjectTool::convertString( search->date.c_str(), NULL );
	s->engineId = search->engineId;
	s->db = ClrObjectTool::convertString( search->db.c_str(), NULL );
	s->result = ClrObjectTool::convertString( search->result.c_str(), NULL );
	s->searchProps = ClrObjectTool::createPropertiesWrapper( &( search->searchProps ) );

	return s;
}

// copy search Info
void SearchEngineWrapper::copySearchInfo(  SearchInfo^ search, kome::ident::SearchInfo& dst ){
	kome::core::Properties* pSearch = NULL;
	if( search->searchProps != nullptr ){
		pSearch = ClrObjectTool::getProperties( search->searchProps );
	}
	
	dst.name = ClrObjectTool::convertString( search->name ).c_str();
	dst.comment = ClrObjectTool::convertString( search->comment ).c_str();
	dst.date = ClrObjectTool::convertString( search->date ).c_str();
	dst.engineId = search->engineId;
	dst.db = ClrObjectTool::convertString( search->db ).c_str();
	dst.result = ClrObjectTool::convertString( search->result ).c_str();
	dst.searchProps = *pSearch;
}
