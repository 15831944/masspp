/**
 * @file PostgreSQLResultSet.cpp
 * @brief implements of PostgreSQLResultSet class
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "PostgreSQLResultSet.h"

#include "PostgreSQL.h"
#include "PostgreSQLStatement.h"

using namespace kome::db::postgresql;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PostgreSQLResultSet::PostgreSQLResultSet( PostgreSQLStatement& stmt )
		: ResultSet( stmt.getDb() ), m_postgresqlStmt( stmt ) {
	m_nTuples = 0;
}

// destructor
PostgreSQLResultSet::~PostgreSQLResultSet() {
	if( m_postgresqlStmt.isDummy() ) {
		m_postgresqlStmt.close();
	}
}

// get PostgreSQL statement
PostgreSQLStatement& PostgreSQLResultSet::getPostgreSQLStatement() {
	return m_postgresqlStmt;
}

// on next
bool PostgreSQLResultSet::onNext() {
	// statement
	const char* stmtName = m_postgresqlStmt.getStatement();
	bool result = false;	
	if( stmtName == NULL ) {
		return false;
	}

	// step
	ExecStatusType ret = PGRES_TUPLES_OK;

	if(m_nTuples == 0){
		ret = m_postgresqlStmt.executePrepared();
	}

	int nTuples = PQntuples(m_postgresqlStmt.m_result);
	if(nTuples > m_nTuples){
		result = (ret == PGRES_TUPLES_OK);
		m_nTuples++;
	}
	if( (ret != PGRES_TUPLES_OK) && (ret != PGRES_COMMAND_OK) ){
		LOG_ERROR( FMT( "SQL executePrepared Error: %d", ret ) );
	}
	return result;
}

// on get string
std::string PostgreSQLResultSet::onGetString( const int col, const char* df ) {
	// string 
	std::string s = NVL( df, "" );

	// statement
	const char* stmt = m_postgresqlStmt.getStatement();

	if( stmt == NULL ) {
		return s;
	}
	s = PQgetvalue(m_postgresqlStmt.m_result, m_nTuples - 1, col);

	return s;
}

// on get int
int PostgreSQLResultSet::onGetInt( const int col, const int df ) {
	//statement
	char* val = PQgetvalue(m_postgresqlStmt.m_result, m_nTuples - 1, col);

	if(val == NULL){
		return df;
	}
	

	return atoi(val);

}

// get double
double PostgreSQLResultSet::onGetReal( const int col, const double df ) {
	// statement
	const char* val = PQgetvalue(m_postgresqlStmt.m_result, m_nTuples - 1, col);

	if(val == NULL){
		return df;
	}

	unsigned char *buf = (unsigned char*)PQgetvalue(m_postgresqlStmt.m_result, m_nTuples - 1, col);
	return atof(val);

}

// get data
void* PostgreSQLResultSet::onGetData( const int col ) {
	// statement
	const char *val = PQgetvalue(m_postgresqlStmt.m_result, m_nTuples - 1, col);

	if(val == NULL){
		return NULL;
	}

	size_t size;
	unsigned char *temp = PQunescapeBytea((const unsigned char*)val, &size); 
	return (void*)temp;
}

// get data size
int PostgreSQLResultSet::onGetDataSize( const int col ) {
	size_t size;
	unsigned char *buf = (unsigned char*)PQgetvalue(m_postgresqlStmt.m_result, m_nTuples - 1, col);
	unsigned char *temp = PQunescapeBytea(buf, &size); 
	return size;
}
