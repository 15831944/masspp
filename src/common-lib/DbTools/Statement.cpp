/**
 * @file Statement.cpp
 * @brief implements of Statement class
 *
 * @author S.Tanaka
 * @date 2008.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Statement.h"

#include "Database.h"


using namespace kome::db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Statement::Statement( Database& db, const char* sql ) : m_db( db ) {
	m_db.addStatement( this );

	m_sql = NVL( sql, "" );
}

// destructor
Statement::~Statement() {
	m_db.removeStatement( this );
}

// get database
Database& Statement::getDb() {
	return m_db;
}

// close statement
void Statement::close() {
	delete this;
}

// reset bind
bool Statement::resetBind() {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// reset bind
	return onResetBind();
}

// bind string 
bool Statement::bindString( const int pos, const char* s ) {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// bind
	if( s == NULL ) {
		return onBindNull( pos );
	}
	return onBindString( pos, s );
}

// bind integer
bool Statement::bindInteger( const int pos, const int val ) {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// bind
	return onBindInteger( pos, val );
}

// bind real number
bool Statement::bindReal( const int pos, const double val ) {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// bind
	return onBindReal( pos, val );
}

// bind data
bool Statement::bindData( const int pos, void* data, const unsigned int size ) {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// bind
	if( data == NULL || size <= 0 ) {
		return onBindNull( pos );
	}
	return onBindData( pos, data, size );
}

// bind NULL
bool Statement::bindNull( const int pos ) {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// bind
	return onBindNull( pos );
}

// execute
bool Statement::execute() {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// execute
	bool ret = onExecute();

	// auto commit
	if( ret && m_db.isAutoCommit() ) {
		ret = m_db.commit();
	}

	return ret;
}

// execute query
ResultSet* Statement::executeQuery() {
	// check the connection
	if( m_db.isClosed() ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return NULL;
	}

	// execute
	ResultSet* rs = onExecuteQuery();

	return rs;
}
