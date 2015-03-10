/**
 * @file SQLiteStatement.cpp
 * @brief implements of SQLiteStatement class
 *
 * @author S.Tanaka
 * @date 2008.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SQLiteStatement.h"

#include "SQLiteDb.h"
#include "SQLiteResultSet.h"

#include <sqlite3.h>


using namespace kome::db::sqlite;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SQLiteStatement::SQLiteStatement( SQLiteDb& db, sqlite3_stmt* stmt, const char* sql )
		: Statement( db, sql ), m_sqliteDb( db ) {
	m_stmt = stmt;
	m_dummy = false;
	m_queryFlg = false;
}

// destructor
SQLiteStatement::~SQLiteStatement() {
	// finalize the statement
	if( m_stmt != NULL ) {
		int ret = sqlite3_finalize( m_stmt );
		if( ret != SQLITE_OK ) {
			LOG_WARN( FMT( "SQLite Statement Finalize Code (%x)", ret ) );
		}
	}
}

// get SQLite database
SQLiteDb& SQLiteStatement::getSQLiteDb() {
	return m_sqliteDb;
}

// get statement
sqlite3_stmt* SQLiteStatement::getStatement() {
	return m_stmt;
}

// set dummy
void SQLiteStatement::setDummy( const bool dummy ) {
	m_dummy = dummy;
}

// is dummy
bool SQLiteStatement::isDummy() {
	return m_dummy;
}

// on reset bind
bool SQLiteStatement::onResetBind() {
	if( m_stmt == NULL ) {
		return false;
	}

	// reset
	int ret = sqlite3_reset( m_stmt );
	if( ret != SQLITE_OK ) {
		LOG_ERROR( FMT( "DB Statement Error: %s", sqlite3_errmsg( m_sqliteDb.getDb() ) ) );
		return false;
	}

	return true;
}

// on bind string
bool SQLiteStatement::onBindString( const int pos, const char* s ) {
	if( m_stmt == NULL ) {
		return false;
	}

	int ret = sqlite3_bind_text( m_stmt, pos, s, -1, NULL );
	if( ret != SQLITE_OK ) {
		LOG_ERROR( FMT( "DB Statement Error: %s", sqlite3_errmsg( m_sqliteDb.getDb() ) ) );
		return false;
	}
	return true;
}

// on bind integer
bool SQLiteStatement::onBindInteger( const int pos, const int val ) {
	if( m_stmt == NULL ) {
		return false;
	}

	int ret = sqlite3_bind_int( m_stmt, pos, val );
	if( ret != SQLITE_OK ) {
		LOG_ERROR( FMT( "DB Statement Error: %s", sqlite3_errmsg( m_sqliteDb.getDb() ) ) );
		return false;
	}
	return true;
}

// on bind real
bool SQLiteStatement::onBindReal( const int pos, const double val ) {
	if( m_stmt == NULL ) {
		return false;
	}

	int ret = sqlite3_bind_double( m_stmt, pos, val );
	if( ret != SQLITE_OK ) {
		LOG_ERROR( FMT( "DB Statement Error: %s", sqlite3_errmsg( m_sqliteDb.getDb() ) ) );
		return false;
	}
	return true;
}

// on bind data
bool SQLiteStatement::onBindData( const int pos, void* data, const unsigned int size ) {
	if( m_stmt == NULL ) {
		return false;
	}

	int ret = sqlite3_bind_blob( m_stmt, pos, data, size, NULL );
	if( ret != SQLITE_OK ) {
		LOG_ERROR( FMT( "DB Statement Error: %s", sqlite3_errmsg( m_sqliteDb.getDb() ) ) );
		return false;
	}
	return true;
}

// on bind NULL
bool SQLiteStatement::onBindNull( const int pos ) {
	if( m_stmt == NULL ) {
		return false;
	}

	int ret = sqlite3_bind_null( m_stmt, pos );
	if( ret != SQLITE_OK ) {
		LOG_ERROR( FMT( "DB Statement Error: %s", sqlite3_errmsg( m_sqliteDb.getDb() ) ) );
		return false;
	}
	return true;
}

// execute
bool SQLiteStatement::onExecute() {
	if( m_stmt == NULL ) {
		return false;
	}

	int ret = sqlite3_step( m_stmt );
	if( ret != SQLITE_ROW && ret != SQLITE_DONE ) {
		LOG_ERROR( FMT( "SQL Error: %s", sqlite3_errmsg( m_sqliteDb.getDb() ) ) );

		return false;
	}

	return true;
}

// execute query
kome::db::ResultSet* SQLiteStatement::onExecuteQuery() {
	if( m_stmt == NULL ) {
		return NULL;
	}

	// create result set
	m_queryFlg = true;
	SQLiteResultSet* rs = new SQLiteResultSet( *this );
	return rs;
}
