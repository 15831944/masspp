/**
 * @file Database.cpp
 * @brief implements of Database class
 *
 * @author S.Tanaka
 * @date 2008.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Database.h"

#include "DbManager.h"
#include "Table.h"
#include "TableColumn.h"
#include "TableIndex.h"
#include "Statement.h"
#include "ResultSet.h"


using namespace kome::db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Database::Database() {
	// initialize
	m_closed = false;
	m_initialized = true;
	m_autoCommit = false;

	// add to manager
	DbManager& mgr = DbManager::getInstance();
	mgr.addDb( this );
}

// destructor
Database::~Database() {
	// remove from manager
	DbManager& mgr = DbManager::getInstance();
	mgr.removeDb( this );

	// close
	clearSqlData();
}

// clear sql data
void Database::clearSqlData() {
	// clear result sets
	while( m_resultSets.size() > 0 ) {
 		delete *( m_resultSets.begin() );
	}

	// clear statements
	while( m_statements.size() > 0 ) {
		delete *( m_statements.begin() );
	}
}

// add result set
void Database::addResultSet( ResultSet* resultSet ) {
	if( resultSet != NULL ) {
		m_resultSets.insert( resultSet );
	}
}

// remove result set
void Database::removeResultSet( ResultSet* resultSet ) {
	m_resultSets.erase( resultSet );
}

// get the number of result sets
unsigned int Database::getNumberOfResultSets() {
	return m_resultSets.size();
}

// add statement
void Database::addStatement( Statement* statement ) {
	if( statement != NULL ) {
		m_statements.insert( statement );
	}
}

// remove statement
void Database::removeStatement( Statement* statement ) {
	m_statements.erase( statement );
}

// get the number of statements
unsigned int Database::getNumberOfStatements() {
	return m_statements.size();
}

// closed or not
bool Database::isClosed() {
	return m_closed;
}

// judges whether database is initialized or not
bool Database::isInitialized() {
	return m_initialized;
}

// set auto commit mode
void Database::setAutoCommit( const bool autoCommit ) {
	m_autoCommit = autoCommit;
}

// get auto commit mode
bool Database::isAutoCommit() {
	return m_autoCommit;
}

// close database
bool Database::close() {
	bool ret = true;

	if( !m_closed ) {
		clearSqlData();

		ret = onClose();
		m_closed = true;
	}

	return ret;
}

// commit
bool Database::commit() {
	// check the connection
	if( m_closed ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	return onCommit();
}

// rollback
bool Database::rollback() {
	// check the connection
	if( m_closed ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	return onRollback();
}

// execute SQL
bool Database::execute( const char* sql ) {
	// check the connection
	if( m_closed ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// string object
	std::string s = NVL( sql, "" );

	// execute SQL
	bool ret = onExecute( s.c_str() );

	// auto commit
	if( ret && m_autoCommit ) {
		ret = commit();
	}

	return ret;
}

// execute Query
ResultSet* Database::executeQuery( const char* sql ) {
	// check the connection
	if( m_closed ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// string object
	std::string s = NVL( sql, "" );

	// execute SQL
	return onExecuteQuery( s.c_str() );
}

// prepare
Statement* Database::prepare( const char* sql ) {
	// check the connection
	if( m_closed ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return NULL;
	}

	// string object
	std::string s = NVL( sql, "" );

	// execute SQL
	return onPrepare( s.c_str() );
}

// vacuum
bool Database::vacuum() {
	// check the connection
	if( m_closed ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// clear sql data
	clearSqlData();

	// vacuum
	return onVacuum();
}

// initialize DB
bool Database::initDb() {
	// check the connection
	if( m_closed ) {
		LOG_ERROR( FMT( "Database is already closed." ) );
		return false;
	}

	// clear sql data
	clearSqlData();

	// initialize 
	m_initialized = true;
	return onInit();
}

// create table
bool Database::createTable( Table& table ) {
	// initialized flag
	m_initialized = false;

	// table exists
	if( hasTable( table.getName() ) ) {
		return true;
	}

	// SQL
	std::string sql = getCreateTableString( table );

	if( !execute( sql.c_str() ) ) {
		return false;
	}

	// create index
	for( unsigned int i = 0; i < table.getNumberOfIndices(); i++ ) {
		TableIndex* index = table.getIndex( i );

		createIndex( *index );
	}

	return true;
}

// drop table
bool Database::dropTable( Table& table ) {
	// check the table
	if( !hasTable( table.getName() ) ) {
		return true;
	}

	// drop index
	for( unsigned int i = 0; i < table.getNumberOfIndices(); i++ ) {
		TableIndex* index = table.getIndex( i );

		dropIndex( *index );
	}

	// drop table
	return dropTable( table.getName() );
}

// drop table
bool Database::dropTable( const char* tableName ) {
	bool ret = true;

	if( hasTable( tableName ) ) {
		std::string sql = FMT( "delete from %s;", NVL( tableName, "" ) );
		execute( sql.c_str() );

		sql = FMT( "drop table %s;", NVL( tableName, "" ) );
		ret = execute( sql.c_str() );
	}

	return ret;
}

// initialize table
bool Database::initTable( Table& table ) {
	// drop
	dropTable( table );

	// create
	createTable( table );

	return true;
}

// has table
bool Database::hasTable( const char* tableName ) {
	return onHasTable( tableName );
}

// create temporaty table
bool Database::createTmpTable( Table& table ) {
	// check the table
	if( hasTable( table.getName() ) ) {
		return true;
	}

	return onCreateTmpTable( table );
}

// create index
bool Database::createIndex( TableIndex& index ) {
	// check the index
	if( index.getTable() == NULL || index.getNumberOfColumns() == 0 ) {
		return false;
	}

	// SQL
	std::string sql = FMT(
		"create index %s on %s ( ",
		index.getName(),
		index.getTable()->getName()
	);

	for( unsigned int i = 0; i < index.getNumberOfColumns(); i++ ) {
		TableColumn* col = index.getColumn( i );

		if( i > 0 ) {
			sql += ", ";
		}
		sql += col->getName();
	}

	sql += " );";

	// create
	return execute( sql.c_str() );
}

// drop index
bool Database::dropIndex( kome::db::TableIndex& index ) {
	// SQL
	std::string sql = FMT(
		"drop index if exists %s;",
		index.getName()
	);

	// drop
	return execute( sql.c_str() );
}

// get max value
int Database::getMaxValue( const char* table, const char* col ) {
	// sql
	std::string sql = FMT( "select max(%s) from %s;", col, table );
	int cnt = INT_MIN;

	ResultSet* rs = executeQuery( sql.c_str() );
	if( rs != NULL ) {
		if( rs->next() ) {
			cnt = rs->getInt( 0, cnt );
		}
		rs->close();
	}

	return cnt;
}

// get record count
int Database::getRecordCount( const char* table ) {
	// sql
	std::string sql = FMT( "select count(*) from %s;", table );
	int cnt = 0;

	ResultSet* rs = executeQuery( sql.c_str() );
	if( rs != NULL ) {
		if( rs->next() ) {
			cnt = rs->getInt( 0, cnt );
		}
		rs->close();
	}

	return cnt;
}

// get table column string
std::string Database::getColumnString( TableColumn& col ) {
	// name
	std::string s = col.getName();

	// data type
	TableColumn::DataType type = col.getDataType();

	if( type == TableColumn::INTEGER || col.isAutoIncrement() ) {
		s += " INTEGER";
	}
	else if( type == TableColumn::VARCHAR ) {
		s += " VARCHAR";
	}
	else if( type == TableColumn::REAL ) {
		s += " REAL";
	}
	else if( type == TableColumn::BINARY ) {
		s += " BYTEA";
	}

	// data length
	if( col.getDataLength() > 0 ) {
		s += FMT( "(%d)", col.getDataLength() );
	}

	// not null
	if( !col.isAutoIncrement() && col.isNotNull() ) {
		s += " NOT NULL";
	}

	return s;
}

// get SQL for creating table
std::string Database::getCreateTableString( Table& table ) {
	// create table
	std::string sql = FMT( "create table %s (", table.getName() );

	int cnt = 0;
	for( unsigned int i = 0; i < table.getNumberOfColumns(); i++ ) {
		TableColumn* col = table.getColumn( i );
		if( col != NULL ) {
			if( cnt > 0 ) {
				sql += ", ";
			}
			sql += getColumnString( *col );
			cnt++;
		}
	}
	sql += ");";

	return sql;
}

// create temporaty table
bool Database::onCreateTmpTable( Table& table ) {
	// SQL
	std::string sql = getCreateTableString( table );

	// replace
	sql = replacestring( sql.c_str(), "create table", "create temporary table" );

	// execute
	return onExecute( sql.c_str() );
}
