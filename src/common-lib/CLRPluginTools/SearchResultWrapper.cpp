/**
 * @file SearchResultWrapper.cpp
 * @brief implements of SearchResultWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "SearchResultWrapper.h"

#include "HitWrapper.h"
#include "SubstanceWrapper.h"
#include "SearchEngineWrapper.h"
#include "ClrSearchEngine.h"
#include "ClrSearchEngineBase.h"
#include "SearchEngineWrapper.h"
#include "SubstanceWrapper.h"
#include "EnzymeWrapper.h"
#include "ModificationWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SearchResultWrapper::SearchResultWrapper( kome::ident::SearchResult& result )
		: m_result( result ) {
    m_deleteFlg = false;
}

// constructor
SearchResultWrapper::SearchResultWrapper() : m_result( *( new kome::ident::SearchResult() ) ) {
	m_deleteFlg = true;
}

// destrucotr
SearchResultWrapper::~SearchResultWrapper() {
	if( m_deleteFlg ) {
		delete &m_result;
	}
}

// get search result object
kome::ident::SearchResult& SearchResultWrapper::getSearchResult() {
	return m_result;
}

// clear
void SearchResultWrapper::clear() {
	m_result.clear();
}

// get spectra
cli::array< HitWrapper::SpecInfo^ >^ SearchResultWrapper::getSpectrumInfoList() {
	// spectra
	std::vector< kome::ident::SpecInfo* >& spectra = m_result.getSpectrumInfoList();

	// array
	if( spectra.size() == 0 ) {
		return nullptr;
	}

	cli::array< HitWrapper::SpecInfo^ >^ arr = gcnew cli::array< HitWrapper::SpecInfo^ >( spectra.size() );
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		arr[ i ] = HitWrapper::copySpectrum( spectra[ i ] );
	}

	return arr;
}

// get search parameters
PropertiesWrapper^ SearchResultWrapper::getSearchParameters() {
	return ClrObjectTool::createPropertiesWrapper( &m_result.getSearchParameters() );
}

// set title
void SearchResultWrapper::setTitle( System::String^ title ) {
	m_result.setTitle( ClrObjectTool::convertString( title ).c_str() );
}

// get tile
System::String^ SearchResultWrapper::getTitle() {
	return ClrObjectTool::convertString( m_result.getTitle().c_str(), NULL );
}

// set commnet
void SearchResultWrapper::setComment( System::String^ comment ) {
	m_result.setComment( ClrObjectTool::convertString( comment ).c_str() );
}

// get comment
System::String^ SearchResultWrapper::getComment() {
	return ClrObjectTool::convertString( m_result.getComment().c_str(), NULL );
}

// set DB
void SearchResultWrapper::setDb( System::String^ dbName ) {
	m_result.setDb( ClrObjectTool::convertString( dbName ).c_str() );
}

// get DB
System::String^ SearchResultWrapper::getDb() {
	return ClrObjectTool::convertString( m_result.getDb().c_str(), NULL );
}

// set search engine
void SearchResultWrapper::setSearchEngine( SearchEngineWrapper^ srchEngine ) {
	m_result.setSearchEngine( srchEngine == nullptr ? NULL : &( srchEngine->getSearchEngine() ) );
}

// get search engine
SearchEngineWrapper^ SearchResultWrapper::getSearchEngine() {
	// search engine
	kome::ident::SearchEngine* engine = m_result.getSearchEngine();
	if( engine == NULL ) {
		return nullptr;
	}

	ClrSearchEngine* clrEngine = dynamic_cast< ClrSearchEngine* >( engine );
	if( clrEngine == NULL ) {
		return gcnew SearchEngineWrapper( *engine );
	}

	return clrEngine->getBaseSearchEngine();
}

// set search ID
void SearchResultWrapper::setSearchId( System::String^ id ) {
	m_result.setSearchId( ClrObjectTool::convertString( id ).c_str() );
}

// get search ID
System::String^ SearchResultWrapper::getSearchId() {
	return ClrObjectTool::convertString( m_result.getSearchId().c_str(), NULL );
}

// set result location
void SearchResultWrapper::setResultLocation( System::String^ resultLocation ) {
	m_result.setResultLocation( ClrObjectTool::convertString( resultLocation ).c_str() );
}

// get result location
System::String^ SearchResultWrapper::getResultLocation() {
	return ClrObjectTool::convertString( m_result.getResultLocation().c_str(), NULL );
}

// get peak detection parameters
PropertiesWrapper^ SearchResultWrapper::getPeakDetectionParameters() {
	kome::core::Properties& props = m_result.getPeakDetectionParameters();
	return ClrObjectTool::createPropertiesWrapper( &props );
}

// set date
void SearchResultWrapper::setDate( System::DateTime^ date ) {
	// check the parameter
	if( date == nullptr ) {
		m_result.setDate( (struct tm*)NULL );
		return;
	}

	// set
	struct tm timeInfo;
	timeInfo.tm_sec = date->Second;
	timeInfo.tm_min = date->Minute;
	timeInfo.tm_hour = date->Hour;
	timeInfo.tm_mday = date->Day;
	timeInfo.tm_mon = date->Month - 1;
	timeInfo.tm_year = date->Year - 1900;
	timeInfo.tm_wday = (int)date->DayOfWeek;
	timeInfo.tm_yday = date->DayOfYear;
	timeInfo.tm_isdst = 0;

	m_result.setDate( &timeInfo );
}

// set date
void SearchResultWrapper::setDate( System::String^ date ) {
	m_result.setDate( ClrObjectTool::convertString( date ).c_str() );
}

// get date
System::String^ SearchResultWrapper::getDate() {
	return ClrObjectTool::convertString( m_result.getDate().c_str(), NULL );
}

// clear substances
void SearchResultWrapper::clearSubstances() {
	m_result.clearSubstances();
}

// add substance
void SearchResultWrapper::addSubstance( SubstanceWrapper^ substance ) {
	m_result.addSubstance( substance == nullptr ? NULL : &( substance->getSubstance() ) );
}

// remove substance
void SearchResultWrapper::removeSubstance( SubstanceWrapper^ substance ) {
	m_result.removeSubstance( substance == nullptr ? NULL : &( substance->getSubstance() ) );
}

// search substance
SubstanceWrapper^ SearchResultWrapper::searchSubstance( System::String^ accession, System::String^ name, SubstanceWrapper^ parent, SubstanceWrapper::SubstanceType type ) {
	// type
	kome::ident::SubstanceType t = kome::ident::TYPE_UNKNOWN;
	if( type == SubstanceWrapper::SubstanceType::PROTEIN ) {
		t = kome::ident::TYPE_PROTEIN;
	}
	else if( type == SubstanceWrapper::SubstanceType::PEPTIDE ) {
		t = kome::ident::TYPE_PEPTIDE;
	}
	else if( type == SubstanceWrapper::SubstanceType::METABOLITE ) {
		t = kome::ident::TYPE_METABOLITE;
	}

	// search
	kome::ident::Substance* substance = m_result.searchSubstance(
		ClrObjectTool::convertString( accession ).c_str(),
		ClrObjectTool::convertString( name ).c_str(),
		( parent == nullptr ? NULL : &( parent->getSubstance() ) ),
		t
	);

	if( substance == NULL ) {
		return nullptr;
	}

	return gcnew SubstanceWrapper( *substance );
}

// get number of substances
int SearchResultWrapper::getNumberOfSubstances() {
	return m_result.getNumberOfSubstances();
}

// get substance
SubstanceWrapper^ SearchResultWrapper::getSubstance( int index ) {
	kome::ident::Substance* substance = m_result.getSubstance( index );

	if( substance == NULL ) {
		return nullptr;
	}
	return gcnew SubstanceWrapper( *substance );
}

// clear hits
void SearchResultWrapper::clearHits() {
	m_result.clearHits();
}

// add hit
HitWrapper^ SearchResultWrapper::addHit( SubstanceWrapper^ pSubstance ) {
	kome::ident::Hit* hit = m_result.addHit( pSubstance == nullptr ? NULL : &( pSubstance->getSubstance() ) );

	if( hit == NULL ) {
		return nullptr;
	}

	return gcnew HitWrapper( *hit );
}

// get number of hits
int SearchResultWrapper::getNumberOfHits() {
	return m_result.getNumberOfHits();
}

// get hit
HitWrapper^ SearchResultWrapper::getHit( int index ) {
	kome::ident::Hit* hit = m_result.getHit( index );
	if( hit == NULL ) {
		return nullptr;
	}
	return gcnew HitWrapper( *hit );
}

// add spectrum
void SearchResultWrapper::addSpectrum( HitWrapper::SpecInfo^ spec ) {
	// spectrum
	kome::ident::SpecInfo* s = new kome::ident::SpecInfo();
	HitWrapper::copySpectrum( spec, *s );

	// add
	m_result.addSpectrum( s );
}

// set peak list file
void SearchResultWrapper::setPeakListFile( System::String^ path ) {
	m_result.setPeakListFile( ClrObjectTool::convertString( path ).c_str() );
}

// get peak list file
System::String^ SearchResultWrapper::getPeakListFile() {
	return ClrObjectTool::convertString( m_result.getPeakListFile(), NULL );
}

// set enzyme
void SearchResultWrapper::setEnzyme( EnzymeWrapper^ enzyme ) {
	kome::ident::Enzyme* e = NULL;
	if( enzyme != nullptr ) {
		e = &( enzyme->getEnzyme() );
	}

	m_result.setEnzyme( e );
}

// get enzyme
EnzymeWrapper^ SearchResultWrapper::getEnzyme() {
	kome::ident::Enzyme* enzyme = m_result.getEnzyme();
	if( enzyme == NULL ) {
		return nullptr;
	}

	return gcnew EnzymeWrapper( *enzyme );
}

// set base name
void SearchResultWrapper::setBaseName( System::String^ baseName ) {
	m_result.setBaseName( ClrObjectTool::convertString( baseName ).c_str() );
}

// get base name
System::String^ SearchResultWrapper::getBaseName() {
	return ClrObjectTool::convertString( m_result.getBaseName(), NULL );
}

// set precursor mass type
void SearchResultWrapper::setPrecursorMassType( MassType type ) {
	kome::ident::SearchResult::MassType t = kome::ident::SearchResult::MASS_UNKNOWN;
	if( type == MassType::MONOISOTOPIC ) {
		t = kome::ident::SearchResult::MASS_MONOISOTOPIC;
	}
	else if( type == MassType::AVERAGE ) {
		t = kome::ident::SearchResult::MASS_AVERAGE;
	}
	else if( type == MassType::UNDEFINED ) {
		t = kome::ident::SearchResult::MASS_UNDEFINED;
	}

	m_result.setPrecursorMassType( t );
}

// get precursor mass type
SearchResultWrapper::MassType SearchResultWrapper::getPrecursorMassType() {
	MassType type = MassType::UNKNOWN;
	kome::ident::SearchResult::MassType t = m_result.getPrecursorMassType();
	if( t == kome::ident::SearchResult::MASS_MONOISOTOPIC ) {
		type = MassType::MONOISOTOPIC;
	}
	else if( t == kome::ident::SearchResult::MASS_AVERAGE ) {
		type = MassType::AVERAGE;
	}
	else if( t == kome::ident::SearchResult::MASS_UNDEFINED ) {
		type = MassType::UNDEFINED;
	}

	return type;
}

// set fragment mass type
void SearchResultWrapper::setFragmentMassType( MassType type ) {
	kome::ident::SearchResult::MassType t = kome::ident::SearchResult::MASS_UNKNOWN;
	if( type == MassType::MONOISOTOPIC ) {
		t = kome::ident::SearchResult::MASS_MONOISOTOPIC;
	}
	else if( type == MassType::AVERAGE ) {
		t = kome::ident::SearchResult::MASS_AVERAGE;
	}
	else if( type == MassType::UNDEFINED ) {
		t = kome::ident::SearchResult::MASS_UNDEFINED;
	}

	m_result.setFragmentMassType( t );
}

// get fragment mass type
SearchResultWrapper::MassType SearchResultWrapper::getFragmentMassType() {
	MassType type = MassType::UNKNOWN;
	kome::ident::SearchResult::MassType t = m_result.getFragmentMassType();
	if( t == kome::ident::SearchResult::MASS_MONOISOTOPIC ) {
		type = MassType::MONOISOTOPIC;
	}
	else if( t == kome::ident::SearchResult::MASS_AVERAGE ) {
		type = MassType::AVERAGE;
	}
	else if( t == kome::ident::SearchResult::MASS_UNDEFINED ) {
		type = MassType::UNDEFINED;
	}

	return type;
}

// clear fixed modifications
void SearchResultWrapper::clearFixedModifications() {
	m_result.clearFixedModifications();
}

// get number of fixed modifications
unsigned int SearchResultWrapper::getNumberOfFixedModifications() {
	return m_result.getNumberOfFixedModifications();
}

// get fixed modification
ModificationWrapper^ SearchResultWrapper::getFixedModification( unsigned int idx ) {
	kome::ident::Modification* modification = m_result.getFixedModification( idx );
	if( modification == NULL ) {
		return nullptr;
	}

	return gcnew ModificationWrapper( *modification );
}

// add fixed modification
void SearchResultWrapper::addFixedModification( ModificationWrapper^ modification ) {
	kome::ident::Modification* m = NULL;
	if( modification != nullptr ) {
		m = &( modification->getModification() );
	}

	m_result.addFixedModification( m );
}

// clear variable modification
void SearchResultWrapper::clearVariableModifications() {
	m_result.clearVariableModifications();
}

// get number of variable modifications
unsigned int SearchResultWrapper::getNumberOfVariableModifications() {
	return m_result.getNumberOfVariableModifications();
}

// get variable modification
ModificationWrapper^ SearchResultWrapper::getVariableModification( unsigned int idx ) {
	kome::ident::Modification* modification = m_result.getVariableModification( idx );
	if( modification == NULL ) {
		return nullptr;
	}

	return gcnew ModificationWrapper( *modification );
}

// add variable modification
void SearchResultWrapper::addVariableModification( ModificationWrapper^ modification ) {
	kome::ident::Modification* m = NULL;
	if( modification != nullptr ) {
		m = &( modification->getModification() );
	}

	m_result.addVariableModification( m );
}
