/**
 * @file ResultSet.cpp
 * @brief implements of ResultSet class
 *
 * @author S.Tanaka
 * @date 2008.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ResultSet.h"

#include "Database.h"


using namespace kome::db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ResultSet::ResultSet( Database& db ) : m_db( db ) {
	m_db.addResultSet( this );

	m_hasNext = true;
}

// destructor
ResultSet::~ResultSet() {
	m_db.removeResultSet( this );
}

// get database
Database& ResultSet::getDb() {
	return m_db;
}

// close 
void ResultSet::close() {
	delete this;
}

// next item
bool ResultSet::next() {
	// check the member
	if( !m_hasNext ) {
		return false;
	}

	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// next
	m_hasNext = onNext();

	return m_hasNext;
}

// get string
std::string ResultSet::getString( const int col, const char* df ) {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// get string
	std::string s = onGetString( col, df );

	return s;
}

// get int value
int ResultSet::getInt( const int col, const int df ) {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// get int
	int v = onGetInt( col, df );

	return v;
}

// get real number value
double ResultSet::getReal( const int col, const double df ) {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// get double
	double v = onGetReal( col, df );

	return v;
}

// get data
void* ResultSet::getData( const int col ) {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// get data
	void* data = onGetData( col );

	return data;
}

// get data size
int ResultSet::getDataSize( const int col ) {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// get size
	int size = onGetDataSize( col );

	return size;
}
