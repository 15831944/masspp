/**
 * @file MassBankRecordHandler.cpp
 * @brief implements of MassBankRecordHandler class
 *
 * @author S.Tanaka
 * @date 2009.09.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "../stdafx.h"
#include "MassBankRecordHandler.h"


using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define RETURN_TAG_NAME		  "return"
#define ID_TAG_NAME			  "id"
#define INFO_TAG_NAME			"info"


// constructor
MassBankRecordHandler::MassBankRecordHandler() {
}

// destructor
MassBankRecordHandler::~MassBankRecordHandler() {
}

void MassBankRecordHandler::doGetRecordInfo(
	const char* fname,
	const char* id,
	std::string& info
){
	//do parse
	parse(fname);
	if( !isError() ) {
		info = getRecordInfo(id);
	}
	// delete temporary file
	removefile(fname);

}

// get the number of records
unsigned int MassBankRecordHandler::getNumberOfRecords() {
	return m_records.size();
}

// get record ID
const char* MassBankRecordHandler::getId( const unsigned int idx ) {
	if( idx >= m_records.size() ) {
		return NULL;
	}
	return m_records[ idx ].first.c_str();
}

// get record info
const char* MassBankRecordHandler::getRecordInfo( const unsigned int idx ) {
	if( idx >= m_records.size() ) {
		return NULL;
	}
	return m_records[ idx ].second.c_str();
}

// get record info
const char* MassBankRecordHandler::getRecordInfo( const char* id ) {
	if( id == NULL ) {
		return NULL;
	}
	for( unsigned int i = 0; i < m_records.size(); i++ ) {
		if( m_records[ i ].first.compare( id ) == 0 ) {
			return m_records[ i ].second.c_str();
		}
	}
	return NULL;
}

// on start document
void MassBankRecordHandler::onStartDocument() {
	m_records.clear();
}

// on end document
void MassBankRecordHandler::onEndDocument() {
}

// on start element
void MassBankRecordHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tags
	if( strcmp( name, RETURN_TAG_NAME ) == 0 ) {	// <return>
		m_records.resize( m_records.size() + 1 );
	}
}

// on end element
void MassBankRecordHandler::onEndElement( const char* name, const char* text ) {
	// each tags
	if( strcmp( name, ID_TAG_NAME ) == 0 ) {	// </id>
		if( m_records.size() > 0 ) {
			m_records.back().first = NVL( text, "" );
		}
	}
	else if( strcmp( name, INFO_TAG_NAME ) == 0 ) {	// </info>
		if( m_records.size() > 0 ) {
			m_records.back().second = NVL( text, "" );
		}
	}
}
