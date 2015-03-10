/**
 * @file SQLiteResultSet.cpp
 * @brief implements of SQLiteResultSet class
 *
 * @author S.Tanaka
 * @date 2008.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SQLiteResultSet.h"

#include "SQLiteDb.h"
#include "SQLiteStatement.h"

#include <sqlite3.h>


using namespace kome::db::sqlite;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SQLiteResultSet::SQLiteResultSet( SQLiteStatement& stmt )
		: ResultSet( stmt.getDb() ), m_sqliteStmt( stmt ) {
}

// destructor
SQLiteResultSet::~SQLiteResultSet() {
	if( m_sqliteStmt.isDummy() ) {
		m_sqliteStmt.close();
	}
}

// get SQLite statement
SQLiteStatement& SQLiteResultSet::getSQLiteStatement() {
	return m_sqliteStmt;
}

// on next
bool SQLiteResultSet::onNext() {
	// statement
	sqlite3_stmt* stmt = m_sqliteStmt.getStatement();
	if( stmt == NULL ) {
		return false;
	}

	// step
	int ret = sqlite3_step( stmt );
	if( ret != SQLITE_ROW && ret != SQLITE_DONE && ret != SQLITE_OK ) {
		LOG_ERROR(
			FMT(
				"SQL Error: %s",
				sqlite3_errmsg( m_sqliteStmt.getSQLiteDb().getDb() )
			)
		);
	}

	return ( ret == SQLITE_ROW );
}

// on get string
std::string SQLiteResultSet::onGetString( const int col, const char* df ) {
	// string 
	std::string s = NVL( df, "" );

	// statement
	sqlite3_stmt* stmt = m_sqliteStmt.getStatement();
	if( stmt == NULL ) {
		return s;
	}

	// get string
	const char* text = (const char*)sqlite3_column_text( stmt, col );
	if( text == NULL ) {
		text = df;
	}

	s = NVL( text, "" );

	return s;
}

// on get int
int SQLiteResultSet::onGetInt( const int col, const int df ) {
	// statement
	sqlite3_stmt* stmt = m_sqliteStmt.getStatement();
	if( stmt == NULL ) {
		return df;
	}

	// get int
	int val = sqlite3_column_int( stmt, col );

	return val;
}

// get double
double SQLiteResultSet::onGetReal( const int col, const double df ) {
	// statement
	sqlite3_stmt* stmt = m_sqliteStmt.getStatement();
	if( stmt == NULL ) {
		return df;
	}

	// get double
	double val = sqlite3_column_double( stmt, col );

	return val;
}

// get data
void* SQLiteResultSet::onGetData( const int col ) {
	// statement
	sqlite3_stmt* stmt = m_sqliteStmt.getStatement();
	if( stmt == NULL ) {
		return NULL;
	}

	// get data
	void* data = const_cast< void* >( sqlite3_column_blob( stmt, col ) );

	return data;
}

// get data size
int SQLiteResultSet::onGetDataSize( const int col ) {
	// statement
	sqlite3_stmt* stmt = m_sqliteStmt.getStatement();
	if( stmt == NULL ) {
		return 0;
	}

	// get data size
	int size = sqlite3_column_bytes( stmt, col );

	return size;
}
