/**
 * @file SQLiteDb.cpp
 * @brief implements of SQLiteDb class
 *
 * @author S.Tanaka
 * @date 2008.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SQLiteDb.h"

#include "SQLiteStatement.h"
#include "SQLiteResultSet.h"

#include "SQLiteDbManager.h"

#include <sqlite3.h>


using namespace kome::db::sqlite;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SQLiteDb::SQLiteDb( const char* path, sqlite3* db ) {
	// set DB
	m_path = NVL( path, "" );
	m_db = NULL;

	setDb( db );
}

// destructor
SQLiteDb::~SQLiteDb() {
	// manager
	SQLiteDbManager& mgr = SQLiteDbManager::getInstance();
	mgr.deleteDb( this );
}

// get DB
sqlite3* SQLiteDb::getDb() {
	return m_db;
}

// set DB
void SQLiteDb::setDb( sqlite3* db ) {
	if( m_db != db ) {
		m_db = db;
		if( m_db != NULL ) {
			onExecute( "begin;" );
		}
	}
}

// get path
const char* SQLiteDb::getPath() {
	return m_path.c_str();
}

// get column string
std::string SQLiteDb::getColumnString( kome::db::TableColumn& col ) {
	// default string
	std::string s = Database::getColumnString( col );

	// auto increment
	if( col.isAutoIncrement() ) {
		s += " PRIMARY KEY";
	}

	return s;
}

// on close DB
bool SQLiteDb::onClose() {
	bool ret = true;

	if( m_db != NULL ) {
		sqlite3_close( m_db );
		m_db = NULL;
	}

	return ret;
}

// on commit
bool SQLiteDb::onCommit() {
	bool ret = onExecute( "commit;" );

	onExecute( "begin;" );

	return ret;
}

// on rollback
bool SQLiteDb::onRollback() {
	bool ret = onExecute( "rollback;" );

	onExecute( "begin;" );

	return ret;
}

// on execute
bool SQLiteDb::onExecute( const char* sql ) {
	// check the member
	if( m_db == NULL ) {
		return false;
	}

	// string object
	std::string s = NVL( sql, "" );

	// execute
	int ret = sqlite3_exec( m_db, s.c_str(), NULL, NULL, NULL );
	if( ret != SQLITE_OK ) {
		LOG_ERROR( FMT( "SQL Error [%s]: %s", s.c_str(), sqlite3_errmsg( m_db ) ) );
		return false; 
	}

	return true;
}

// on execute query
kome::db::ResultSet* SQLiteDb::onExecuteQuery( const char* sql ) {
	// statement
	SQLiteStatement* stmt = (SQLiteStatement*)onPrepare( sql );
	if( stmt == NULL ) {
		return NULL;
	}
	stmt->setDummy( true );

	// result set
	SQLiteResultSet* rs = new SQLiteResultSet( *stmt );
	return rs;
}

// on prepare
kome::db::Statement* SQLiteDb::onPrepare( const char* sql ) {
	// check the member
	if( m_db == NULL ) {
		return NULL;
	}

	// prepare
	sqlite3_stmt* stmt = NULL;
	int ret = sqlite3_prepare( m_db, sql, -1, &stmt, NULL );

	if( ret != SQLITE_OK ) {
		LOG_ERROR( FMT( "SQL Error [%s]: %s", sql, sqlite3_errmsg( m_db ) ) );
		return NULL;
	}

	// statement
	SQLiteStatement* statement = new SQLiteStatement( *this, stmt, sql );
	return statement;
}

// on vacuume
bool SQLiteDb::onVacuum() {
	// check the member
	if( m_db == NULL ) {
		return false;
	}

	// SQL
	std::string sql = "vacuum;";

	// execute
	bool ret = onExecute( "end;" );
	if( ret ) {
		ret = onExecute( "vacuum;" );
	}
	if( ret ) {
		ret = onExecute( "begin;" );
	}

	return ret;
}

// on initialize
bool SQLiteDb::onInit() {
	// check the member
	if( m_db == NULL ) {
		return false;
	}

	// close flag
	bool closed = isClosed();
	if( !closed ) {
		onClose();
	}

	// delete file
	bool ret = removefile( m_path.c_str() );

	// open
	if( !closed ) {
		m_db = NULL;
		if( sqlite3_open( m_path.c_str(), &m_db ) != SQLITE_OK ) {	// success
			const char* msg = sqlite3_errmsg( m_db );
			LOG_ERROR( FMT( "DB Open Error (%s) : %s", m_path.c_str(), NVL( msg, "" ) ) );
			ret = false;
		}
	}

	if( ret ) {
		onExecute( "begin;" );
	}

	return ret;
}

// on exists table
bool SQLiteDb::onHasTable( const char* tableName ) {
	// search in the sqlite_master table
	std::string sql = "select count(*) from sqlite_master where type = ? and name = ?;";

	bool ret = false;
	kome::db::Statement* stmt = prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, "table" );
		stmt->bindString( 2, NVL( tableName, "" ) );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				ret = ( rs->getInt( 0, 0 ) > 0 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( ret ) {
		return ret;
	}

	// temporary table
	sql = "select count(*) from sqlite_temp_master where type = ? and name = ?;";
	stmt = prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, "table" );
		stmt->bindString( 2, NVL( tableName, "" ) );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				ret = ( rs->getInt( 0, 0 ) > 0 );
			}
			rs->close();
		}
		stmt->close();
	}

	return ret;
}
