/**
 * @file SearchResult.cpp
 * @brief implementation of SearchResult class
 *
  * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "stdafx.h"
#include "Substance.h"
#include "Hit.h"
#include "SearchEngine.h"
#include "SearchResult.h"
#include "IdentificationManager.h"
#include "Enzyme.h"

#include <wx/datetime.h>


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SearchResult::SearchResult( void )
{
	m_pSrchEngine = NULL;
	m_enzyme = NULL;
	m_precursorMassType = MASS_UNDEFINED;
	m_fragmentMassType = MASS_UNDEFINED;
}

// destructor
SearchResult::~SearchResult( void )
{
	m_pSrchEngine = NULL;

	// Must delete all Hit objects that have been created.
	Hit* hitItem = NULL;

	while (!m_arrHits.empty())
	{
		hitItem = m_arrHits.back();
		m_arrHits.pop_back();
		if( hitItem != NULL ) {
			delete hitItem;
			hitItem = NULL;
		}
	}

	// Must delete all Substance objects that have been created.
	Substance* substanceItem = NULL;

	while (!m_arrSubstances.empty())
	{
		substanceItem = m_arrSubstances.back();
		if( substanceItem != NULL ) {
			delete substanceItem;
			substanceItem = NULL;
		}
	}

	// Must free all SpecInfo structures that have been created.
	clearSpectra();
}

// reset search result
void SearchResult::clear( void )
{
	for( unsigned int i = 0; i < m_spectrumInfoList.size(); i++ ) {
		free( m_spectrumInfoList[ i ] );
	}
	m_spectrumInfoList.clear();

	m_spectrumInfoList.clear();
	for (int i = m_searchParameters.getNumberOfProperties() - 1; i >= 0 ; i--)
	{
		m_searchParameters.deleteParameter(m_searchParameters.getKey(i));
	}
	setDb("");
	setSearchEngine(NULL);
	setResultLocation("");
	for (int j = m_peakdetectionParameters.getNumberOfProperties() - 1; j >= 0 ; j--)
	{
		m_peakdetectionParameters.deleteParameter(m_peakdetectionParameters.getKey(j));
	}
	setDate( (struct tm*)NULL);
	clearSubstances();
	clearHits();
	clearFixedModifications();
	clearVariableModifications();

	m_baseName.clear();
	m_precursorMassType = MASS_UNDEFINED;
	m_fragmentMassType = MASS_UNDEFINED;
	m_enzyme = NULL;
}

// clear spectra
void SearchResult::clearSpectra() {
	for( unsigned int i = 0; i < m_spectrumInfoList.size(); i++ ) {
		delete m_spectrumInfoList[ i ];
	}

	m_spectrumInfoList.clear();
}

// add spectrum
void SearchResult::addSpectrum( SpecInfo* spec ) {
	m_spectrumInfoList.push_back( spec );
}

// get spectra
std::vector<SpecInfo*>& SearchResult::getSpectrumInfoList( void ) {
	return m_spectrumInfoList;
}


// get the search parameters object
kome::core::Properties& SearchResult::getSearchParameters( void )
{
	return m_searchParameters;
}

// set search title
void SearchResult::setTitle( const char* title ) {
	m_title = NVL( title, "" );
}

// get the search title
std::string SearchResult::getTitle( void )
{
	return m_title;
}

// set search comment
void SearchResult::setComment( const char* comment ) {
	m_comment = NVL( comment, "" );
}

// get the search comment
std::string SearchResult::getComment( void )
{
	return m_comment;
}

// set the name of the database used
void SearchResult::setDb( const char* DbName )
{
	m_db = DbName;
}

// get the name of the database used
std::string SearchResult::getDb( void ) const
{
	return m_db;
}

// set the search engine used
void SearchResult::setSearchEngine( SearchEngine* srchEngine )
{
	m_pSrchEngine = srchEngine;
}

// get the search engine used
SearchEngine* SearchResult::getSearchEngine( void ) const
{
	return m_pSrchEngine;
}

// set the result location string
void SearchResult::setResultLocation( const char* resultLocation )
{
	m_resultLocation = resultLocation;
}

// set search ID
void SearchResult::setSearchId( const char* id ) {
	m_searchId = trimstring( id );
}

// get search ID
std::string SearchResult::getSearchId() const {
	return m_searchId;
}

// get the result location string
std::string SearchResult::getResultLocation( void ) const
{
	return m_resultLocation;
}

// get the peak detection parameters object
kome::core::Properties& SearchResult::getPeakDetectionParameters( void )
{
	return m_peakdetectionParameters;
}

// set the search date/time information
void SearchResult::setDate( struct tm* date )
{
	m_date = datetostring(date);
}

// set date
void SearchResult::setDate( const char* date ) {
	m_date = NVL( date, "" );
}

// get the search date/time information
std::string SearchResult::getDate( void ) const
{
	return m_date;
}

// clear the Substance array
void SearchResult::clearSubstances( void )
{
	while( m_arrSubstances.size() > 0 ) {
		delete m_arrSubstances.back();
	}
}

// add substance information
void SearchResult::addSubstance( Substance* substance ) 
{
	// check the parameter
	if( substance == NULL ) {
		return;
	}

	const char* name = substance->getName().c_str();
	const char* accession = substance->getAccession().c_str();
	SubstanceType type = substance->getType();

	m_arrSubstances.push_back( substance );
}

// remove substance
void SearchResult::removeSubstance( Substance* substance ) {
	for( int i = (int)m_arrSubstances.size() - 1; i >= 0; i-- ) {
		if( m_arrSubstances[ i ] == substance ) {
			m_arrSubstances.erase( m_arrSubstances.begin() + i );
		}
	}
}

// search the substance
Substance* SearchResult::searchSubstance( const char* accession, const char* name, Substance* parent, SubstanceType type ) {
	// substance
	Substance* pSubstance = NULL;

	// search
	if ( ((strlen(name) > 0) || (type == TYPE_PROTEIN)) &&
		( ((type == TYPE_UNKNOWN) || (type == TYPE_METABOLITE)) ||
		(((type == TYPE_PROTEIN) && (strlen(accession) > 0)) || ((type == TYPE_PEPTIDE) && (strlen(accession) == 0))) ) )
	{
		// Check if the substance has already been added into the array.
		for (int i = 0; (i < static_cast<int>(m_arrSubstances.size())) && (!pSubstance); i++)
		{
			if ( ((m_arrSubstances[i]->getAccession().compare(accession) == 0)
				&& (m_arrSubstances[i]->getName().compare(name) == 0)) &&
				((m_arrSubstances[i]->getType() == type)
				&& (m_arrSubstances[i]->getParentSubstance() == parent)) )
			{
				// Substance already exists, so return a pointer to the existing item.
				pSubstance = m_arrSubstances[i];
			}
		}
	}

	return pSubstance;
}

// get the nunber of substances
int SearchResult::getNumberOfSubstances( void ) const
{
	return m_arrSubstances.size();
}

// get the specified substance information
Substance* SearchResult::getSubstance( const int index ) const
{
	Substance* ret = NULL;
	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_arrSubstances.size())) );

	if( index < static_cast<int>(m_arrSubstances.size()) ) {
		ret = m_arrSubstances[ index ];
	}
	return ret;
}

// clear the Hit array
void SearchResult::clearHits( void )
{
	m_arrHits.clear();
}

// add hit information
Hit* SearchResult::addHit( Substance* pSubstance )
{
	Hit* pHit = NULL;

	// Do not check if the hit has already been added into the array.
	// Always create a new one.
	pHit = new Hit(this, pSubstance);
	m_arrHits.push_back(pHit);

 	return pHit;
}

// get the nunber of hits
int SearchResult::getNumberOfHits( void ) const
{
	return m_arrHits.size();
}

// get the specified hit information
Hit* SearchResult::getHit( const int index ) const
{
	Hit* ret = NULL;
	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_arrHits.size())) );

	if( index < static_cast<int>(m_arrHits.size()) ) {
		ret = m_arrHits[ index ];
	}
	return ret;
}

// export the identification result
void SearchResult::exportResult( boost::function< int ( void*, int ) > writeFun )
{
	// Write all result information into the writeFun function object.

	// 1. Search engine
	unsigned int nSrchEngineLength = 0;
	if (m_pSrchEngine != NULL)
	{
		nSrchEngineLength = m_pSrchEngine->getName().size();
	}
	writeFun( &nSrchEngineLength, sizeof( nSrchEngineLength ) );
	if (m_pSrchEngine != NULL)
	{
		writeFun(const_cast<char*>(m_pSrchEngine->getName().c_str()), m_pSrchEngine->getName().size());
	}

	// 2. Date
	unsigned int nDateLength = m_date.size();
	writeFun( &nDateLength, sizeof( nDateLength ) );
	writeFun(const_cast<char*>(m_date.c_str()), m_date.size());

	// 3. Database
	unsigned int nDatabaseLength = m_db.size();
	writeFun( &nDatabaseLength, sizeof( nDatabaseLength ) );
	writeFun(const_cast<char*>(m_db.c_str()), m_db.size());

	// 4. Result location
	unsigned int nResultLocationLength = m_resultLocation.size();
	writeFun( &nResultLocationLength, sizeof( nResultLocationLength ) );
	writeFun(const_cast<char*>(m_resultLocation.c_str()), m_resultLocation.size());

	// 5. Peak detection parameters
	unsigned int nPeakDetPropertiesLength = m_peakdetectionParameters.getNumberOfProperties();
	writeFun( &nPeakDetPropertiesLength, sizeof( nPeakDetPropertiesLength ) );
	for (unsigned int iPropertyIndex = 0; iPropertyIndex < nPeakDetPropertiesLength; iPropertyIndex++)
	{
		std::string sPropertyKey = m_peakdetectionParameters.getKey(iPropertyIndex);
		std::string sPropertyValue = m_peakdetectionParameters.getValue(iPropertyIndex);

		unsigned int nPropertyKeyLength = sPropertyKey.size();
		writeFun( &nPropertyKeyLength, sizeof( nPropertyKeyLength ) );
		writeFun(const_cast<char*>(sPropertyKey.c_str()), sPropertyKey.size());
		unsigned int nPropertyValueLength = sPropertyValue.size();
		writeFun( &nPropertyValueLength, sizeof( nPropertyValueLength ) );
		writeFun(const_cast<char*>(sPropertyValue.c_str()), sPropertyValue.size());
	}

	// 6. Search parameters
	unsigned int nSearchPropertiesLength = m_searchParameters.getNumberOfProperties();
	writeFun( &nSearchPropertiesLength, sizeof( nSearchPropertiesLength ) );
	for (unsigned int iPropertyIndex = 0; iPropertyIndex < nSearchPropertiesLength; iPropertyIndex++)
	{
		std::string sPropertyKey = m_searchParameters.getKey(iPropertyIndex);
		std::string sPropertyValue = m_searchParameters.getValue(iPropertyIndex);

		unsigned int nPropertyKeyLength = sPropertyKey.size();
		writeFun( &nPropertyKeyLength, sizeof( nPropertyKeyLength ) );
		writeFun(const_cast<char*>(sPropertyKey.c_str()), sPropertyKey.size());
		unsigned int nPropertyValueLength = sPropertyValue.size();
		writeFun( &nPropertyValueLength, sizeof( nPropertyValueLength ) );
		writeFun(const_cast<char*>(sPropertyValue.c_str()), sPropertyValue.size());
	}

	// 7. Spectrum info list
	unsigned int nSpectrumInfoListLength = m_spectrumInfoList.size();
	writeFun( &nSpectrumInfoListLength, sizeof( nSpectrumInfoListLength ) );
	for (unsigned int iSpectrumInfoListIndex = 0; iSpectrumInfoListIndex < nSpectrumInfoListLength; iSpectrumInfoListIndex++)
	{
		SpecInfo* specInfoItem = m_spectrumInfoList[iSpectrumInfoListIndex];

		int specId = specInfoItem->specId;
		writeFun( &specId, sizeof(specId) );
		int	sampleIndex = specInfoItem->sampleIndex;
		writeFun( &sampleIndex, sizeof(sampleIndex) );
		unsigned int nSampleNameLength = specInfoItem->sampleName.size();
		writeFun( &nSampleNameLength, sizeof( nSampleNameLength ) );
		writeFun(const_cast<char*>(specInfoItem->sampleName.c_str()), specInfoItem->sampleName.size());
		unsigned int nPathLength = specInfoItem->path.size();
		writeFun( &nPathLength, sizeof( nPathLength ) );
		writeFun(const_cast<char*>(specInfoItem->path.c_str()), specInfoItem->path.size());
		double retentionTime = specInfoItem->retentionTime;
		writeFun( &retentionTime, sizeof(retentionTime) );
		double	precursor = specInfoItem->precursor;
		writeFun( &precursor, sizeof(precursor) );
        int stage = specInfoItem->stage;
		writeFun( &stage, sizeof(stage) );
		int charge = 0;
		for( unsigned int i = 0; i < specInfoItem->charges.size(); i++ ) {
			int c = specInfoItem->charges.at( i );
			c = CLAMP( c, 1, 8 );
			charge |= ( 1 << ( c - 1 ) );
		}
		writeFun( &charge, sizeof(charge) );
		int	polarity = specInfoItem->polarity;
		writeFun( &polarity, sizeof(polarity) );
		unsigned int nPeakIdsLength = specInfoItem->peakIds.size();
		writeFun( &nPeakIdsLength, sizeof( nPeakIdsLength ) );
		for (unsigned int iPeakIdsIndex = 0; iPeakIdsIndex < nPeakIdsLength; iPeakIdsIndex++)
		{
			writeFun( &specInfoItem->peakIds[iPeakIdsIndex], sizeof(int) );
		}
		unsigned int nNameLength = specInfoItem->name.size();
		writeFun( &nNameLength, sizeof( nNameLength ) );
		writeFun(const_cast<char*>(specInfoItem->name.c_str()), specInfoItem->name.size());
	}

	// 8. Substances
	unsigned int nSubstanceListLength = m_arrSubstances.size();
	writeFun( &nSubstanceListLength, sizeof( nSubstanceListLength ) );
	for (unsigned int iSubstanceIndex = 0; iSubstanceIndex < nSubstanceListLength; iSubstanceIndex++)
	{
		Substance* pSubstance = m_arrSubstances[iSubstanceIndex];

		unsigned int nAccessionLength = pSubstance->getAccession().size();
		writeFun( &nAccessionLength, sizeof( nAccessionLength ) );
		writeFun(const_cast<char*>(pSubstance->getAccession().c_str()), pSubstance->getAccession().size());
		unsigned int nNameLength = pSubstance->getName().size();
		writeFun( &nNameLength, sizeof( nNameLength ) );
		writeFun(const_cast<char*>(pSubstance->getName().c_str()), pSubstance->getName().size());
		int	substanceType = pSubstance->getType();
		writeFun( &substanceType, sizeof(substanceType) );
		std::string sParentSubstanceAccession;
		std::string sParentSubstanceName;
		int nParentSubstanceType = -1;
		if (pSubstance->getParentSubstance() != NULL)
		{
			sParentSubstanceAccession = pSubstance->getParentSubstance()->getAccession();
			sParentSubstanceName = pSubstance->getParentSubstance()->getName();
			nParentSubstanceType = pSubstance->getParentSubstance()->getType();
		}
		unsigned int nParentSubstanceAccessionLength = sParentSubstanceAccession.size();
		writeFun( &nParentSubstanceAccessionLength, sizeof( nParentSubstanceAccessionLength ) );
		writeFun(const_cast<char*>(sParentSubstanceAccession.c_str()), sParentSubstanceAccession.size());
		unsigned int nParentSubstanceNameLength = sParentSubstanceName.size();
		writeFun( &nParentSubstanceNameLength, sizeof( nParentSubstanceNameLength ) );
		writeFun(const_cast<char*>(sParentSubstanceName.c_str()), sParentSubstanceName.size());
		writeFun( &nParentSubstanceType, sizeof(nParentSubstanceType) );
		unsigned int nSubstancePropertiesLength = pSubstance->getProperties().getNumberOfProperties();
		writeFun( &nSubstancePropertiesLength, sizeof( nSubstancePropertiesLength ) );
		for (unsigned int iPropertyIndex = 0; iPropertyIndex < nSubstancePropertiesLength; iPropertyIndex++)
		{
			std::string sPropertyKey = pSubstance->getProperties().getKey(iPropertyIndex);
			std::string sPropertyValue = pSubstance->getProperties().getValue(iPropertyIndex);

			unsigned int nPropertyKeyLength = sPropertyKey.size();
			writeFun( &nPropertyKeyLength, sizeof( nPropertyKeyLength ) );
			writeFun(const_cast<char*>(sPropertyKey.c_str()), sPropertyKey.size());
			unsigned int nPropertyValueLength = sPropertyValue.size();
			writeFun( &nPropertyValueLength, sizeof( nPropertyValueLength ) );
			writeFun(const_cast<char*>(sPropertyValue.c_str()), sPropertyValue.size());
		}
	}

	// 9. Hits
	unsigned int nHitListLength = m_arrHits.size();
	writeFun( &nHitListLength, sizeof( nHitListLength ) );
	for (unsigned int iHitIndex = 0; iHitIndex < nHitListLength; iHitIndex++)
	{
		Hit* pHit = m_arrHits[iHitIndex];

		std::string sHitSubstanceAccession;
		std::string sHitSubstanceName;
		int nHitSubstanceType = -1;
		if (pHit->getSubstance() != NULL)
		{
			sHitSubstanceAccession = pHit->getSubstance()->getAccession();
			sHitSubstanceName = pHit->getSubstance()->getName();
			nHitSubstanceType = pHit->getSubstance()->getType();
		}
		unsigned int nHitSubstanceAccessionLength = sHitSubstanceAccession.size();
		writeFun( &nHitSubstanceAccessionLength, sizeof( nHitSubstanceAccessionLength ) );
		writeFun(const_cast<char*>(sHitSubstanceAccession.c_str()), sHitSubstanceAccession.size());
		unsigned int nHitSubstanceNameLength = sHitSubstanceName.size();
		writeFun( &nHitSubstanceNameLength, sizeof( nHitSubstanceNameLength ) );
		writeFun(const_cast<char*>(sHitSubstanceName.c_str()), sHitSubstanceName.size());
		writeFun( &nHitSubstanceType, sizeof(nHitSubstanceType) );

		unsigned int nSpectrumInfoListLength = pHit->getNumberOfSpectra();
		writeFun( &nSpectrumInfoListLength, sizeof( nSpectrumInfoListLength ) );
		for (unsigned int iSpectrumInfoListIndex = 0; iSpectrumInfoListIndex < nSpectrumInfoListLength; iSpectrumInfoListIndex++)
		{
			SpecInfo* pSpecInfoItem = pHit->getSpectrum(iSpectrumInfoListIndex);

			int specId = pSpecInfoItem->specId;
			writeFun( &specId, sizeof(specId) );
			int	sampleIndex = pSpecInfoItem->sampleIndex;
			writeFun( &sampleIndex, sizeof(sampleIndex) );
			unsigned int nSampleNameLength = pSpecInfoItem->sampleName.size();
			writeFun( &nSampleNameLength, sizeof( nSampleNameLength ) );
			writeFun(const_cast<char*>(pSpecInfoItem->sampleName.c_str()), pSpecInfoItem->sampleName.size());
			unsigned int nPathLength = pSpecInfoItem->path.size();
			writeFun( &nPathLength, sizeof( nPathLength ) );
			writeFun(const_cast<char*>(pSpecInfoItem->path.c_str()), pSpecInfoItem->path.size());
			double retentionTime = pSpecInfoItem->retentionTime;
			writeFun( &retentionTime, sizeof(retentionTime) );
			double	precursor = pSpecInfoItem->precursor;
			writeFun( &precursor, sizeof(precursor) );
			int stage = pSpecInfoItem->stage;
			writeFun( &stage, sizeof(stage) );
			int charge = 0;
			for( unsigned int i = 0; i < pSpecInfoItem->charges.size(); i++ ) {
				int c = pSpecInfoItem->charges.at( i );
				c = CLAMP( c, 1, 8 );
				charge |= ( 1 << ( c - 1 ) );
			}
			writeFun( &charge, sizeof(charge) );
			int	polarity = pSpecInfoItem->polarity;
			writeFun( &polarity, sizeof(polarity) );
			unsigned int nPeakIdsLength = pSpecInfoItem->peakIds.size();
			writeFun( &nPeakIdsLength, sizeof( nPeakIdsLength ) );
			for (unsigned int iPeakIdsIndex = 0; iPeakIdsIndex < nPeakIdsLength; iPeakIdsIndex++)
			{
				writeFun( &pSpecInfoItem->peakIds[iPeakIdsIndex], sizeof(int) );
			}
			unsigned int nNameLength = pSpecInfoItem->name.size();
			writeFun( &nNameLength, sizeof( nNameLength ) );
			writeFun(const_cast<char*>(pSpecInfoItem->name.c_str()), pSpecInfoItem->name.size());
		}
		unsigned int nHitValuesLength = pHit->getNumberOfValues();
		writeFun( &nHitValuesLength, sizeof( nHitValuesLength ) );
		for (unsigned int iValueIndex = 0; iValueIndex < nHitValuesLength; iValueIndex++)
		{
			std::string sValueName = pHit->getValueName(iValueIndex);
			std::string sValue = pHit->getValue(iValueIndex);
			int nValueType = pHit->getValueType(iValueIndex);

			unsigned int nValueNameLength = sValueName.size();
			writeFun( &nValueNameLength, sizeof( nValueNameLength ) );
			writeFun(const_cast<char*>(sValueName.c_str()), sValueName.size());
			unsigned int nValueLength = sValue.size();
			writeFun( &nValueLength, sizeof( nValueLength ) );
			writeFun(const_cast<char*>(sValue.c_str()), sValue.size());
			writeFun( &nValueType, sizeof(nValueType) );
		}
	}
}

// import an identification result
void SearchResult::importResult( boost::function< int ( void*, int ) > readFun )
{
	clear();

	// Read all result information from the readFun function object.

	// 1. Search engine
	unsigned int nSrchEngineLength = 0;
	readFun( &nSrchEngineLength, sizeof( nSrchEngineLength ) );
	if (nSrchEngineLength > 0)
	{
		char* buff = new char[nSrchEngineLength + 1];
		readFun(buff, nSrchEngineLength);
		buff[nSrchEngineLength] = '\0';

		// Get the search engine pointer.
		IdentificationManager& identMgr = IdentificationManager::getInstance();
		for( int i = 0; (i < identMgr.getNumberOfSearchEngines()) && (m_pSrchEngine == NULL); i++ )
		{
			SearchEngine* tmp = identMgr.getSearchEngine( i );
			if( tmp->getName().compare(buff) == 0 )
			{
				m_pSrchEngine = tmp;
			}
		}
		if (!m_pSrchEngine)
		{
			LOG_ERROR( FMT( "Failed to get the search engine." ) );
		}
		delete buff;
	}

	// 2. Date
	unsigned int nDateLength = 0;
	readFun( &nDateLength, sizeof( nDateLength ) );
	if (nDateLength > 0)
	{
		char* buff = new char[nDateLength + 1];
		readFun(buff, nDateLength);
		buff[nDateLength] = '\0';
		m_date = buff;
		delete buff;
	}

	// 3. Database
	unsigned int nDatabaseLength = 0;
	readFun( &nDatabaseLength, sizeof( nDatabaseLength ) );
	if (nDatabaseLength > 0)
	{
		char* buff = new char[nDatabaseLength + 1];
		readFun(buff, nDatabaseLength);
		buff[nDatabaseLength] = '\0';
		m_db = buff;
		delete buff;
	}

	// 4. Result location
	unsigned int nResultLocationLength = 0;
	readFun( &nResultLocationLength, sizeof( nResultLocationLength ) );
	if (nResultLocationLength > 0)
	{
		char* buff = new char[nResultLocationLength + 1];
		readFun(buff, nResultLocationLength);
		buff[nResultLocationLength] = '\0';
		m_resultLocation = buff;
		delete buff;
	}

	// 5. Peak detection parameters
	unsigned int nPeakDetPropertiesLength = 0;
	readFun( &nPeakDetPropertiesLength, sizeof( nPeakDetPropertiesLength ) );
	for (unsigned int iPropertyIndex = 0; iPropertyIndex < nPeakDetPropertiesLength; iPropertyIndex++)
	{
		std::string sPropertyKey;
		std::string sPropertyValue;

		unsigned int nPropertyKeyLength = 0;
		readFun( &nPropertyKeyLength, sizeof( nPropertyKeyLength ) );
		if (nPropertyKeyLength > 0)
		{
			char* buff = new char[nPropertyKeyLength + 1];
			readFun(buff, nPropertyKeyLength);
			buff[nPropertyKeyLength] = '\0';
			sPropertyKey = buff;
			delete buff;
		}

		unsigned int nPropertyValueLength = 0;
		readFun( &nPropertyValueLength, sizeof( nPropertyValueLength ) );
		if (nPropertyValueLength > 0)
		{
			char* buff = new char[nPropertyValueLength + 1];
			readFun(buff, nPropertyValueLength);
			buff[nPropertyValueLength] = '\0';
			sPropertyValue = buff;
			delete buff;
		}

		m_peakdetectionParameters.setValue(sPropertyKey.c_str(), sPropertyValue.c_str());
	}

	// 6. Search parameters
	unsigned int nSearchPropertiesLength = 0;
	readFun( &nSearchPropertiesLength, sizeof( nSearchPropertiesLength ) );
	for (unsigned int iPropertyIndex = 0; iPropertyIndex < nSearchPropertiesLength; iPropertyIndex++)
	{
		std::string sPropertyKey;
		std::string sPropertyValue;

		unsigned int nPropertyKeyLength = 0;
		readFun( &nPropertyKeyLength, sizeof( nPropertyKeyLength ) );
		if (nPropertyKeyLength > 0)
		{
			char* buff = new char[nPropertyKeyLength + 1];
			readFun(buff, nPropertyKeyLength);
			buff[nPropertyKeyLength] = '\0';
			sPropertyKey = buff;
			delete buff;
		}

		unsigned int nPropertyValueLength = 0;
		readFun( &nPropertyValueLength, sizeof( nPropertyValueLength ) );
		if (nPropertyValueLength > 0)
		{
			char* buff = new char[nPropertyValueLength + 1];
			readFun(buff, nPropertyValueLength);
			buff[nPropertyValueLength] = '\0';
			sPropertyValue = buff;
			delete buff;
		}

		m_searchParameters.setValue(sPropertyKey.c_str(), sPropertyValue.c_str());
	}

	// 7. Spectrum info list
	unsigned int nSpectrumInfoListLength = 0;
	readFun( &nSpectrumInfoListLength, sizeof( nSpectrumInfoListLength ) );
	m_spectrumInfoList.resize(nSpectrumInfoListLength);
	for (unsigned int iSpectrumInfoListIndex = 0; iSpectrumInfoListIndex < nSpectrumInfoListLength; iSpectrumInfoListIndex++)
	{
		SpecInfo* specInfoItem = m_spectrumInfoList[iSpectrumInfoListIndex];
		// >>>>>>	@Date:2013/07/26	<Modify>	A.Ozaki
		// Initialization method of the class was wrong.
		//
		specInfoItem->clearInfo( );
		//
		// <<<<<<	@Date:2013/07/26	<Modify>	A.Ozaki

		int specId = 0;
		readFun( &specId, sizeof( specId ) );
		specInfoItem->specId = specId;
		int sampleIndex = 0;
		readFun( &sampleIndex, sizeof( sampleIndex ) );
		specInfoItem->sampleIndex = sampleIndex;
		unsigned int nSampleNameLength = 0;
		readFun( &nSampleNameLength, sizeof( nSampleNameLength ) );
		if (nSampleNameLength > 0)
		{
			char* buff = new char[nSampleNameLength + 1];
			readFun(buff, nSampleNameLength);
			buff[nSampleNameLength] = '\0';
			specInfoItem->sampleName = buff;
			delete buff;
		}
		unsigned int nPathLength = 0;
		readFun( &nPathLength, sizeof( nPathLength ) );
		if (nPathLength > 0)
		{
			char* buff = new char[nPathLength + 1];
			readFun(buff, nPathLength);
			buff[nPathLength] = '\0';
			specInfoItem->path = buff;
			delete buff;
		}
		double retentionTime = 0;
		readFun( &retentionTime, sizeof( retentionTime ) );
		specInfoItem->retentionTime = retentionTime;
		double precursor = 0;
		readFun( &precursor, sizeof( precursor ) );
		specInfoItem->precursor = precursor;
		int stage = 0;
		readFun( &stage, sizeof( stage ) );
		specInfoItem->stage = stage;
		int charge = 0;
		readFun( &charge, sizeof( charge ) );
		for( int c = 1; c <= 8; c++ ) {
			int flg = 1 << ( c - 1 );
			if( charge | flg ) {
				specInfoItem->charges.push_back( c );
			}
		}
		int polarity = 0;
		readFun( &polarity, sizeof( polarity ) );
		specInfoItem->polarity = (kome::objects::Spectrum::Polarity) polarity;
		unsigned int nPeakIdsLength = 0;
		readFun( &nPeakIdsLength, sizeof( nPeakIdsLength ) );
		specInfoItem->peakIds.resize(nPeakIdsLength);
		for (unsigned int iPeakIdsIndex = 0; iPeakIdsIndex < nPeakIdsLength; iPeakIdsIndex++)
		{
			readFun( &specInfoItem->peakIds[iPeakIdsIndex], sizeof( int ) );
		}
		unsigned int nNameLength = 0;
		readFun( &nNameLength, sizeof( nNameLength ) );
		if (nNameLength > 0)
		{
			char* buff = new char[nNameLength + 1];
			readFun(buff, nNameLength);
			buff[nNameLength] = '\0';
			specInfoItem->name = buff;
			delete[] buff;
		}
	}

	// 8. Substances
	unsigned int nSubstanceListLength = 0;
	readFun( &nSubstanceListLength, sizeof( nSubstanceListLength ) );
	m_arrSubstances.resize(nSubstanceListLength);
	for (unsigned int iSubstanceListIndex = 0; iSubstanceListIndex < nSubstanceListLength; iSubstanceListIndex++)
	{
		std::string sAccession;
		std::string sName;
		int nSubstanceType = -1;
		Substance* pParentSubstance = NULL;

		unsigned int nAccessionLength = 0;
		readFun( &nAccessionLength, sizeof( nAccessionLength ) );
		if (nAccessionLength > 0)
		{
			char* buff = new char[nAccessionLength + 1];
			readFun(buff, nAccessionLength);
			buff[nAccessionLength] = '\0';
			sAccession = buff;
			delete buff;
		}
		unsigned int nNameLength = 0;
		readFun( &nNameLength, sizeof( nNameLength ) );
		if (nNameLength > 0)
		{
			char* buff = new char[nNameLength + 1];
			readFun(buff, nNameLength);
			buff[nNameLength] = '\0';
			sName = buff;
			delete buff;
		}
		readFun( &nSubstanceType, sizeof( nSubstanceType ) );
		std::string sParentSubstanceAccession;
		std::string sParentSubstanceName;
		int nParentSubstanceType = -1;
		unsigned int nParentSubstanceAccessionLength = 0;
		readFun( &nParentSubstanceAccessionLength, sizeof( nParentSubstanceAccessionLength ) );
		if (nParentSubstanceAccessionLength > 0)
		{
			char* buff = new char[nParentSubstanceAccessionLength + 1];
			readFun(buff, nParentSubstanceAccessionLength);
			buff[nParentSubstanceAccessionLength] = '\0';
			sParentSubstanceAccession = buff;
			delete buff;
		}
		unsigned int nParentSubstanceNameLength = 0;
		readFun( &nParentSubstanceNameLength, sizeof( nParentSubstanceNameLength ) );
		if (nParentSubstanceNameLength > 0)
		{
			char* buff = new char[nParentSubstanceNameLength + 1];
			readFun(buff, nParentSubstanceNameLength);
			buff[nParentSubstanceNameLength] = '\0';
			sParentSubstanceName = buff;
			delete buff;
		}
		readFun( &nParentSubstanceType, sizeof( nParentSubstanceType ) );

		// Check whether a parent substance was specified for this substance.
		if ((!sParentSubstanceAccession.empty()) || (!sParentSubstanceName.empty()))
		{
			// Find the parent substance if one was specified.
			for (unsigned int iSubstanceIndex = 0; (iSubstanceIndex < m_arrSubstances.size()) && (!pParentSubstance); iSubstanceIndex++)
			{
				if ( ((m_arrSubstances[iSubstanceIndex]->getAccession().compare(sParentSubstanceAccession) == 0) &&
					(m_arrSubstances[iSubstanceIndex]->getName().compare(sParentSubstanceName) == 0)) &&
					(m_arrSubstances[iSubstanceIndex]->getType() == (SubstanceType) nParentSubstanceType) )
				{
					pParentSubstance = m_arrSubstances[iSubstanceIndex];
				}
			}
		}

		m_arrSubstances[iSubstanceListIndex] = new Substance(this, sAccession.c_str(), sName.c_str(), (SubstanceType) nSubstanceType, pParentSubstance);

		unsigned int nSubstancePropertiesLength = 0;
		readFun( &nSubstancePropertiesLength, sizeof( nSubstancePropertiesLength ) );
		for (unsigned int iPropertyIndex = 0; iPropertyIndex < nSubstancePropertiesLength; iPropertyIndex++)
		{
			std::string sPropertyKey;
			std::string sPropertyValue;

			unsigned int nPropertyKeyLength = 0;
			readFun( &nPropertyKeyLength, sizeof( nPropertyKeyLength ) );
			if (nPropertyKeyLength > 0)
			{
				char* buff = new char[nPropertyKeyLength + 1];
				readFun(buff, nPropertyKeyLength);
				buff[nPropertyKeyLength] = '\0';
				sPropertyKey = buff;
				delete buff;
			}

			unsigned int nPropertyValueLength = 0;
			readFun( &nPropertyValueLength, sizeof( nPropertyValueLength ) );
			if (nPropertyValueLength > 0)
			{
				char* buff = new char[nPropertyValueLength + 1];
				readFun(buff, nPropertyValueLength);
				buff[nPropertyValueLength] = '\0';
				sPropertyValue = buff;
				delete buff;
			}

			m_arrSubstances[iSubstanceListIndex]->getProperties().setValue(sPropertyKey.c_str(), sPropertyValue.c_str());
		}
	}

	// 9. Hits
	unsigned int nHitListLength = 0;
	readFun( &nHitListLength, sizeof( nHitListLength ) );
	m_arrHits.resize(nHitListLength);
	for (unsigned int iHitListIndex = 0; iHitListIndex < nHitListLength; iHitListIndex++)
	{
		std::string sHitSubstanceAccession;
		std::string sHitSubstanceName;
		int nHitSubstanceType = -1;
		Substance* pHitSubstance = NULL;

		unsigned int nHitSubstanceAccessionLength = 0;
		readFun( &nHitSubstanceAccessionLength, sizeof( nHitSubstanceAccessionLength ) );
		if (nHitSubstanceAccessionLength > 0)
		{
			char* buff = new char[nHitSubstanceAccessionLength + 1];
			readFun(buff, nHitSubstanceAccessionLength);
			buff[nHitSubstanceAccessionLength] = '\0';
			sHitSubstanceAccession = buff;
			delete buff;
		}
		unsigned int nHitSubstanceNameLength = 0;
		readFun( &nHitSubstanceNameLength, sizeof( nHitSubstanceNameLength ) );
		if (nHitSubstanceNameLength > 0)
		{
			char* buff = new char[nHitSubstanceNameLength + 1];
			readFun(buff, nHitSubstanceNameLength);
			buff[nHitSubstanceNameLength] = '\0';
			sHitSubstanceName = buff;
			delete buff;
		}
		readFun( &nHitSubstanceType, sizeof( nHitSubstanceType ) );
		// Check whether a substance was specified for this hit.
		if ((!sHitSubstanceAccession.empty()) || (!sHitSubstanceName.empty()))
		{
			// Find the substance if one was specified.
			for (unsigned int iSubstanceIndex = 0; (iSubstanceIndex < m_arrSubstances.size()) && (!pHitSubstance); iSubstanceIndex++)
			{
				if ( ((m_arrSubstances[iSubstanceIndex]->getAccession().compare(sHitSubstanceAccession) == 0) &&
					(m_arrSubstances[iSubstanceIndex]->getName().compare(sHitSubstanceName) == 0)) &&
					(m_arrSubstances[iSubstanceIndex]->getType() == (SubstanceType) nHitSubstanceType) )
				{
					pHitSubstance = m_arrSubstances[iSubstanceIndex];
				}
			}
		}
		m_arrHits[iHitListIndex] = new Hit(this, pHitSubstance);

		unsigned int nSpectrumInfoListLength = 0;
		readFun( &nSpectrumInfoListLength, sizeof( nSpectrumInfoListLength ) );
		for (unsigned int iSpectrumInfoListIndex = 0; iSpectrumInfoListIndex < nSpectrumInfoListLength; iSpectrumInfoListIndex++)
		{
			SpecInfo* pSpecInfoItem = new SpecInfo;
			memset(pSpecInfoItem, 0, sizeof(SpecInfo));

			int specId = 0;
			readFun( &specId, sizeof( specId ) );
			pSpecInfoItem->specId = specId;
			int sampleIndex = 0;
			readFun( &sampleIndex, sizeof( sampleIndex ) );
			pSpecInfoItem->sampleIndex = sampleIndex;
			unsigned int nSampleNameLength = 0;
			readFun( &nSampleNameLength, sizeof( nSampleNameLength ) );
			if (nSampleNameLength > 0)
			{
				char* buff = new char[nSampleNameLength + 1];
				readFun(buff, nSampleNameLength);
				buff[nSampleNameLength] = '\0';
				pSpecInfoItem->sampleName = buff;
				delete buff;
			}
			unsigned int nPathLength = 0;
			readFun( &nPathLength, sizeof( nPathLength ) );
			if (nPathLength > 0)
			{
				char* buff = new char[nPathLength + 1];
				readFun(buff, nPathLength);
				buff[nPathLength] = '\0';
				pSpecInfoItem->path = buff;
				delete buff;
			}
			double retentionTime = 0;
			readFun( &retentionTime, sizeof( retentionTime ) );
			pSpecInfoItem->retentionTime = retentionTime;
			double precursor = 0;
			readFun( &precursor, sizeof( precursor ) );
			pSpecInfoItem->precursor = precursor;
			int stage = 0;
			readFun( &stage, sizeof( stage ) );
			pSpecInfoItem->stage = stage;
			int charge = 0;
			readFun( &charge, sizeof( charge ) );
			for( unsigned int c = 1; c <= 8; c++ ) {
				int flg = ( 1 << ( c - 1 ) );
				if( charge | flg ) {
					pSpecInfoItem->charges.push_back( c );
				}
			}
			int polarity = 0;
			readFun( &polarity, sizeof( polarity ) );
			pSpecInfoItem->polarity = (kome::objects::Spectrum::Polarity) polarity;
			unsigned int nPeakIdsLength = 0;
			readFun( &nPeakIdsLength, sizeof( nPeakIdsLength ) );
			pSpecInfoItem->peakIds.resize(nPeakIdsLength);
			for (unsigned int iPeakIdsIndex = 0; iPeakIdsIndex < nPeakIdsLength; iPeakIdsIndex++)
			{
				readFun( &pSpecInfoItem->peakIds[iPeakIdsIndex], sizeof( int ) );
			}
			unsigned int nNameLength = 0;
			readFun( &nNameLength, sizeof( nNameLength ) );
			if (nNameLength > 0)
			{
				char* buff = new char[nNameLength + 1];
				readFun(buff, nNameLength);
				buff[nNameLength] = '\0';
				pSpecInfoItem->name = buff;
				delete buff;
			}

			m_arrHits[iHitListIndex]->addSpectrum(pSpecInfoItem);
		}
		unsigned int nHitValuesLength = 0;
		readFun( &nHitValuesLength, sizeof( nHitValuesLength ) );
		for (unsigned int iHitValuesIndex = 0; iHitValuesIndex < nHitValuesLength; iHitValuesIndex++)
		{
			std::string sValueName;
			std::string sValue;
			int nValueType = -1;

			unsigned int nValueNameLength = 0;
			readFun( &nValueNameLength, sizeof( nValueNameLength ) );
			if (nValueNameLength > 0)
			{
				char* buff = new char[nValueNameLength + 1];
				readFun(buff, nValueNameLength);
				buff[nValueNameLength] = '\0';
				sValueName = buff;
				delete buff;
			}
			unsigned int nValueLength = 0;
			readFun( &nValueLength, sizeof( nValueLength ) );
			if (nValueLength > 0)
			{
				char* buff = new char[nValueLength + 1];
				readFun(buff, nValueLength);
				buff[nValueLength] = '\0';
				sValue = buff;
				delete buff;
			}
			readFun( &nValueType, sizeof( nValueType ) );

			m_arrHits[iHitListIndex]->addValue(sValueName.c_str(), sValue.c_str(), (ValueType) nValueType);
		}
	}
}

// set peak list file path
void SearchResult::setPeakListFile( const char* path ) {
	m_peakListFile = NVL( path, "" );
}

// get peak list file
const char* SearchResult::getPeakListFile() {
	return m_peakListFile.c_str();
}

// set enzyme
void SearchResult::setEnzyme( Enzyme* enzyme ) {
	m_enzyme = enzyme;
}

// get enzyme
Enzyme* SearchResult::getEnzyme() {
	return m_enzyme;
}

// set base name
void SearchResult::setBaseName( const char* baseName ) {
	m_baseName = NVL( baseName, "" );
}

// get base name
const char* SearchResult::getBaseName() {
	return m_baseName.c_str();
}

// set precursor mass type
void SearchResult::setPrecursorMassType( MassType type ) {
	m_precursorMassType = type;
}

// get precursor mass type
SearchResult::MassType SearchResult::getPrecursorMassType() {
	return m_precursorMassType;
}

// set fragment mass type
void SearchResult::setFragmentMassType( MassType type ) {
	m_fragmentMassType = type;
}

// get fragment mass type
SearchResult::MassType SearchResult::getFragmentMassType() {
	return m_fragmentMassType;
}

//  clear fixed modifications
void SearchResult::clearFixedModifications() {
	m_fixedMods.clear();
}

// get number of fixed modificatinos
unsigned int SearchResult::getNumberOfFixedModifications() {
	return m_fixedMods.size();
}

// get fixed modification
Modification* SearchResult::getFixedModification( const unsigned int idx ) {
	if( idx >= m_fixedMods.size() ) {
		return NULL;
	}

	return m_fixedMods[ idx ];
}

// add fixed modification
void SearchResult::addFixedModification( Modification* modification ) {
	if( modification != NULL ) {
		m_fixedMods.push_back( modification );
	}
}

// clear variable modifications
void SearchResult::clearVariableModifications() {
	m_varMods.clear();
}

// get number of variable modification
unsigned int SearchResult::getNumberOfVariableModifications() {
	return m_varMods.size();
}

// get variable modification
Modification* SearchResult::getVariableModification( const unsigned int idx ) {
	if( idx >= m_varMods.size() ) {
		return NULL;
	}

	return m_varMods[ idx ];
}

// add variable modification
void SearchResult::addVariableModification( Modification* modification ) {
	if( modification != NULL ) {
		m_varMods.push_back( modification );
	}
}
