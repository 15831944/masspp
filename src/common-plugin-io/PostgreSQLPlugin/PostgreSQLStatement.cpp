/**
 * @file PostgreSQLStatement.cpp
 * @brief implements of PostgreSQLStatement class
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "PostgreSQLStatement.h"

#include "PostgreSQL.h"
#include "PostgreSQLResultSet.h"

#include <string>

using namespace kome::db::postgresql;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// Length of the character string
const int	g_nPostgresqlSqlLength = 256;

// constructor
PostgreSQLStatement::PostgreSQLStatement( PostgreSQL& db, const char* stmt, const char* sql , int params)
		: Statement( db, sql ), m_PostgreSQL( db ) {
	m_stmt = stmt;
	m_dummy = false;
	m_result = NULL;
	m_nParams = params;
	if(m_nParams == 0){ // No Params
		m_pnParamLength = NULL;
		m_pnParamFormats = NULL;
		m_ppnParamValues = NULL;
	}
	else{
		m_pnParamLength = new int[m_nParams];
		m_pnParamFormats = new int[m_nParams];
		m_ppnParamValues = new char*[m_nParams];
		for(int i=0; i < m_nParams; i++){
			m_ppnParamValues[i] = NULL;
			m_pnParamLength[i] = 0;
			m_pnParamFormats[i] = 0;
		}
	}
}

// destructor
PostgreSQLStatement::~PostgreSQLStatement() {
	if( m_stmt.c_str() != NULL ) {
		PGconn *conn = m_PostgreSQL.getDb();
		char sql[g_nPostgresqlSqlLength];
		sprintf_s(sql, "DEALLOCATE %s;",m_stmt.c_str());
		PGresult *res = PQexec( conn, sql );
		ExecStatusType status = PQresultStatus(res);
		PQclear(res);
		if(status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK){
			LOG_WARN( FMT( "PostgreSQL Statement Deallocate Code (%x)", status ));
		}
	}

	if( m_pnParamLength != NULL){
		delete [] m_pnParamLength;
		m_pnParamLength = NULL;
	}
	if( m_pnParamFormats != NULL ){
		delete [] m_pnParamFormats;
		m_pnParamFormats = NULL;
	}
	if( m_ppnParamValues != NULL ){
		for(int i=0;i< m_nParams;i++){
			if( m_ppnParamValues[i] != NULL ){
				delete [] m_ppnParamValues[i];
			}
		}
		delete [] m_ppnParamValues;
		m_ppnParamValues = NULL;
	}

	if(m_result != NULL){
		PQclear(m_result);
	}

}

// get PostgreSQL database
PostgreSQL& PostgreSQLStatement::getPostgreSQL() {
	return m_PostgreSQL;
}

// get statement
const char* PostgreSQLStatement::getStatement() {
	return m_stmt.c_str();
}

// set dummy
void PostgreSQLStatement::setDummy( const bool dummy ) {
	m_dummy = dummy;
}

// is dummy
bool PostgreSQLStatement::isDummy() {
	return m_dummy;
}

// on reset bind
bool PostgreSQLStatement::onResetBind() {
	if( m_stmt.c_str() == NULL ) {
		return false;
	}

	//PostgreSQL does not require.

	return true;
}

// on bind string
bool PostgreSQLStatement::onBindString( const int pos, const char* s ) {
	if( m_stmt.c_str() == NULL ) {
		LOG_ERROR(FMT("Bind Error[String]:Statement name is null."));
		return false;
	}

	if( (pos > m_nParams) || (pos == 0) ){
		LOG_ERROR(FMT("Bind Error[Integer]:Array index[%d] out of bounds exception.",pos));
		return true;
	}
	
	// if a value already set,delete it. 
	if(m_ppnParamValues[pos - 1] != NULL){
		delete m_ppnParamValues[pos - 1];
	}

	m_ppnParamValues[pos - 1] = new char[strlen(s) + 1];
	sprintf(m_ppnParamValues[pos - 1],"%s",s);
	m_pnParamFormats[pos - 1] = 0;
	m_pnParamLength[pos - 1] = strlen(s) + 1;

	return true;
}

// on bind integer
bool PostgreSQLStatement::onBindInteger( const int pos, const int val ) {
	if( m_stmt.c_str() == NULL ) {
		LOG_ERROR(FMT("Bind Error[Integer]:Statement name is null."));
		return false;
	}

	if( (pos > m_nParams) || (pos == 0) ){
		LOG_ERROR(FMT("Bind Error[Integer]:Array index[%d] out of bounds exception.",pos));
		return true;
	}

	// if a value already set,delete it. 
	if(m_ppnParamValues[pos - 1] != NULL){
		delete m_ppnParamValues[pos - 1];
	}

	
	char s[g_nPostgresqlSqlLength];
	sprintf(s,"%d",val);
	m_ppnParamValues[pos - 1] = new char[strlen(s) + 1];
	sprintf(m_ppnParamValues[pos - 1],"%s",s);
	m_pnParamFormats[pos - 1] = 0;
	m_pnParamLength[pos - 1] = strlen(s) + 1;

	return true;
}

// on bind real
bool PostgreSQLStatement::onBindReal( const int pos, const double val ) {
	if( m_stmt.c_str() == NULL ) {
		LOG_ERROR(FMT("Bind Error[Real]:Statement name is null."));
		return false;
	}

	if( (pos > m_nParams) || (pos == 0) ){
		LOG_ERROR(FMT("Bind Error[Real]:Array index[%d] out of bounds exception.",pos));
		return true;
	}
	
	// if a value already set,delete it. 
	if(m_ppnParamValues[pos - 1] != NULL){
		delete m_ppnParamValues[pos - 1];
	}
	
	char s[g_nPostgresqlSqlLength];
	sprintf(s,"%f",val);
	m_ppnParamValues[pos - 1] = new char[strlen(s) + 1];
	sprintf(m_ppnParamValues[pos - 1],"%s",s);
	m_pnParamFormats[pos - 1] = 0;
	m_pnParamLength[pos - 1] = strlen(s) + 1;



	return true;
}

// on bind data
bool PostgreSQLStatement::onBindData( const int pos, void* data, const unsigned int size ) {
	if( m_stmt.c_str() == NULL ) {
		LOG_ERROR(FMT("Bind Error[Data]:Statement name is null."));
		return false;
	}

	if( (pos > m_nParams) || (pos == 0) ){
		LOG_ERROR(FMT("Bind Error[Data]:Array index[%d] out of bounds exception.",pos));
		return true;
	}

	// if a value already set,delete it. 
	if(m_ppnParamValues[pos - 1] != NULL){
		delete m_ppnParamValues[pos - 1];
	}

	m_ppnParamValues[pos - 1] = new char[size];
	memcpy(m_ppnParamValues[pos - 1], data, size);
	m_pnParamFormats[pos - 1] = 1;
	m_pnParamLength[pos - 1] = size;

	return true;
}

// on bind NULL
bool PostgreSQLStatement::onBindNull( const int pos ) {
	if( m_stmt.c_str() == NULL ) {
		LOG_ERROR(FMT("Bind Error[Null]:Statement name is null."));
		return false;
	}

	if( (pos > m_nParams) || (pos == 0) ){
		LOG_ERROR(FMT("Bind Error[Null]:Array index[%d] out of bounds exception.",pos));
		return true;
	}

	// if a value already set,delete it. 
	if(m_ppnParamValues[pos - 1] != NULL){
		delete m_ppnParamValues[pos - 1];
	}

	m_ppnParamValues[pos - 1] = (char*)new char[1];
	m_ppnParamValues[pos - 1][0] = NULL;
	m_pnParamFormats[pos - 1] = 0;
	m_pnParamLength[pos - 1] = 0;

	return true;
}

// execute
bool PostgreSQLStatement::onExecute() {
	ExecStatusType ret = executePrepared();

	if((ret == PGRES_COMMAND_OK) || (ret == PGRES_TUPLES_OK)){
		return true;
	}

	return false;
}

// execute prepared query
ExecStatusType PostgreSQLStatement::executePrepared(){
	if( m_stmt.c_str() == NULL ) {
		return PGRES_FATAL_ERROR;
	}
	PGconn *conn = m_PostgreSQL.getDb();
	if(m_result != NULL){
		PQclear(m_result);
	}

	// execute
	m_result = PQexecPrepared(conn, m_stmt.c_str(), m_nParams,
		m_ppnParamValues, m_pnParamLength, m_pnParamFormats, 0);
	
	// keep the result
	ExecStatusType status = PQresultStatus(m_result);
	
	if(status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK){ //Error
		const char* msg = PQresultErrorMessage( m_result );
		LOG_ERROR( FMT( "SQL Error [%d]: %s", status, msg ) );
		return status;
	}

	return status;
}


// execute query
kome::db::ResultSet* PostgreSQLStatement::onExecuteQuery() {
	if( m_stmt.c_str() == NULL ) {
		return NULL;
	}
	
	// create result set
	PostgreSQLResultSet* rs = new PostgreSQLResultSet( *this );
	return rs;
}
