/**
 * @file MassBankJobStatusHandler.cpp
 * @brief implements of MassBankJobStatusHandler class
 *
 * @author S.Tanaka
 * @date 2010.06.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../stdafx.h"
#include "MassBankJobStatusHandler.h"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define DATE_TAG_NAME                "requestDate"
#define STATUS_TAG_NAME              "statusCode"

// constructor
MassBankJobStatusHandler::MassBankJobStatusHandler() {
	m_status = -1;
}

// destructor
MassBankJobStatusHandler::~MassBankJobStatusHandler() {
}

// get status
int MassBankJobStatusHandler::getStatus() {
	return m_status;
}

// get date
const char* MassBankJobStatusHandler::getDate() {
	return m_date.c_str();
}

// on start document
void MassBankJobStatusHandler::onStartDocument() {
	m_status = -1;
	m_date.clear();
}

// on end document
void MassBankJobStatusHandler::onEndDocument() {
}

// on start element
void MassBankJobStatusHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
}

// on end element
void MassBankJobStatusHandler::onEndElement( const char* name, const char* text ) {
	if( strcmp( name, STATUS_TAG_NAME ) == 0 ) {    // </statusCode>
		m_status = toint( text, 10, -1 );
	}
	else if( strcmp( name, DATE_TAG_NAME ) == 0 ) {
		m_date = text;
	}
}
