/**
 * @file PostgreSQLDb.cpp
 * @brief implements of PostgreSQLDb class
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "PostgreSQL.h"

#include "PostgreSQLStatement.h"
#include "PostgreSQLResultSet.h"
#include "PostgreSQLManager.h"

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
PostgreSQL::PostgreSQL( const char* address, int port, const char* name,
	const char* user, const char* password, PGconn* db ) {

	char portString[12];
	sprintf_s(portString, "%d", port);

	// set DB
	m_db = db;
	m_address = address;
	m_port = portString;
	m_name = name;
	m_user = user;
	m_password = password;

	// initialize statement ID
	m_nStatementId = 0;

	// set encode
	// **TBD** replace 'UTF8' in encod to 'SJIS'
	PQsetClientEncoding(m_db,"SJIS");

	// transaction
	onExecute("BEGIN;");
}

// destructor
PostgreSQL::~PostgreSQL() {
	// manager
	PostgreSQLManager& mgr = PostgreSQLManager::getInstance();
	mgr.deleteDb( this );
}

// get DB
PGconn* PostgreSQL::getDb() {
	return m_db;
}

// get column string
std::string PostgreSQL::getColumnString( kome::db::TableColumn& col ) {
	// default string
	std::string s = Database::getColumnString( col );

	// auto increment
	if( col.isAutoIncrement() ) {
		s.replace(s.find("INTEGER",0),7,FMT("SERIAL PRIMARY KEY"));
	}

	return s;
}

// on close DB
bool PostgreSQL::onClose() {
	bool ret = true;
	if( m_db != NULL ) {
		PQfinish( m_db );
	}

	return ret;
}

// on commit
bool PostgreSQL::onCommit() {
	bool ret = onExecute("COMMIT;");

	onExecute("BEGIN;");

	return ret;
}

// on rollback
bool PostgreSQL::onRollback() {
	bool ret = onExecute("ROLLBACK;" );

	onExecute("BEGIN;");

	return ret;
}

// on execute
bool PostgreSQL::onExecute( const char* sql ) {
	// check the member
	if( m_db == NULL ) {
		return false;
	}

	// string object
	std::string s = NVL( sql, "" );
	/*
	// **TBD** replace 'BLOB' in query to 'BYTEA'
	// (PostgreSQL not supports BLOB.)
	while(s.find("BLOB",0) != s.npos){
		s.replace(s.find("BLOB",0),4,FMT("BYTEA"));
	}
	
	// **TBD** remove 'if not exists' in the query if
	// there is no 'create table' in query
	// (PostgreSQL not supports 'if not exists' for CREATE INDEX.)
	while(s.find("if not exists",0) != s.npos && s.find("create table",0) == s.npos){
			s.replace(s.find("if not exists",0),13,FMT(""));
	}

	// **TBD** replace 'ID INTEGER PRIMARY KEY' to 'ID SERIAL PRIMARY KEY'
	// (PostgreSQL supports auto ID by 'SERIAL PRIMARY KEY'.)
	if(s.find("ID INTEGER PRIMARY KEY",0) != s.npos){
		s.replace(s.find("ID INTEGER PRIMARY KEY",0),22,FMT("ID SERIAL PRIMARY KEY"));
	}
	*/
	//execute
	PGresult *ret = PQexec( m_db, s.c_str());
	
	//error
	if( PQresultStatus(ret) != PGRES_COMMAND_OK ) {
		const char* msg = PQerrorMessage( m_db );
		LOG_ERROR( FMT( "SQL Error [%s]: %s", s.c_str(), msg ) );
		PQclear(ret);
		return false; 

	}
	PQclear(ret);
	return true;
}

// on execute query
kome::db::ResultSet* PostgreSQL::onExecuteQuery( const char* sql ) {
	PostgreSQLStatement* stmt = (PostgreSQLStatement*)onPrepare( sql );
	if( stmt == NULL ) {
		return NULL;
	}
	stmt->setDummy( true );
	// result set
	PostgreSQLResultSet* rs = new PostgreSQLResultSet( *stmt );
	return rs;
}

// on prepare
kome::db::Statement* PostgreSQL::onPrepare( const char* sql ) {
	// check the member
	if( m_db == NULL ) {
		return NULL;
	}

	// set statement name
	char stmt[g_nPostgresqlSqlLength];
	sprintf(stmt,"prepare_%d",m_nStatementId++);

	// replace parameter character.
	int prepare = 0;
	std::string command = sql;
	while(command.find_first_of("?",0) != command.npos){
		prepare++;
		command.replace(command.find_first_of("?",0),1,FMT("$%d",prepare));
	}
	
	// **TBD** replace 'ID INTEGER PRIMARY KEY' to 'ID SERIAL PRIMARY KEY'
	// (PostgreSQL supports auto ID by 'SERIAL PRIMARY KEY'.)
	if(command.find("ID INTEGER PRIMARY KEY",0) != command.npos){
		command.replace(command.find("ID INTEGER PRIMARY KEY",0),22,FMT("ID SERIAL PRIMARY KEY"));
	}

	PGresult *ret = PQprepare(m_db, stmt, command.c_str(), prepare, NULL);
	if( PQresultStatus(ret) != PGRES_COMMAND_OK && PQresultStatus(ret) != PGRES_TUPLES_OK) {
		//PQclear(ret);
		const char* msg = PQerrorMessage( m_db );
		LOG_ERROR( FMT( "SQL Error [%s]: %s", command.c_str(), msg ) );
	}
	PQclear(ret);

	// statement
	PostgreSQLStatement* statement = new PostgreSQLStatement( *this, stmt, sql, prepare );
	return statement;
}

// on vacuume
bool PostgreSQL::onVacuum() {
	// check the member
	if( m_db == NULL ) {
		return false;
	}

	LOG_TRACE(FMT("PostgreSQLPlugin:Vacuum Start."));

	// SQL
	std::string sql = "VACUUM;";

	// execute
	bool ret = onExecute( "END;" );
	if( ret ) {
		ret = onExecute( sql.c_str() );
	}
	if( ret ) {
		LOG_TRACE(FMT("PostgreSQLPlugin:Vacuum success."));
		ret = onExecute( "BEGIN;" );
	}

	LOG_TRACE(FMT("PostgreSQLPlugin:Vacuum Complete."));

	return ret;
}

// on initialize
bool PostgreSQL::onInit() {

	// check the member
	if( m_db == NULL ) {
		return false;
	}

	LOG_TRACE(FMT("PostgreSQLPlugin:Initialize Start."));

	PGconn *conn = NULL;
	PostgreSQL* db = NULL;
	ExecStatusType status;
	PGresult *res;
	char sql[g_nPostgresqlSqlLength];
	bool ret = true;

	// disconnect
	PQfinish(m_db);

	// open postgres DB
	sprintf_s(sql, "host=%s port=%s dbname=postgres user=%s password=%s",
		m_address.c_str(), m_port.c_str(), m_user.c_str(), m_password.c_str());
	conn = PQconnectdb(sql);
	if(PQstatus(conn) != CONNECTION_OK){//failed
		const char* msg = PQerrorMessage( conn );
		LOG_ERROR( FMT( "DB Open Error (%s:%s) : %s", m_address.c_str(), m_port.c_str(), NVL( msg, "" ) ) );
		return false;
	}

	sprintf_s(sql, "DROP DATABASE %s;", m_name.c_str());
	res = PQexec( conn, sql );
	status = PQresultStatus(res);
	PQclear(res);

	if(status != PGRES_COMMAND_OK){
		LOG_ERROR( FMT( "DB Drop Error : %d", m_name.c_str()));
		PQfinish(conn);
		return false;
	}

	sprintf_s(sql, "CREATE DATABASE %s;", m_name.c_str());
	res = PQexec( conn, sql );
	status = PQresultStatus(res);
	PQclear(res);
	if(status != PGRES_COMMAND_OK){
		LOG_ERROR( FMT( "DB Create Error : %d", m_name.c_str()));
		PQfinish(conn);
		return false;
	}
	PQfinish(conn);

	m_db = NULL;
	sprintf_s(sql, "host=%s port=%s dbname=%s user=%s password=%s",
		m_address.c_str(), m_port.c_str(), m_name.c_str(), m_user.c_str(), m_password.c_str());
	m_db = PQconnectdb(sql);
	if( PQstatus(m_db) != CONNECTION_OK ){//failed
		const char* msg = PQerrorMessage( m_db );
		LOG_ERROR( FMT( "DB Open Error (%s:%s) : %s", m_address.c_str(), m_port.c_str(), NVL( msg, "" ) ) );
		ret = false;
	}

	LOG_TRACE(FMT("PostgreSQLPlugin:Initialize Complete."));

	if( ret ) {
		onExecute("BEGIN;");
	}

	return ret;
}

// on exists table
bool PostgreSQL::onHasTable( const char* tableName ) {

	char sql[g_nPostgresqlSqlLength];
	std::string tableLowerName = tableName; 

	for( unsigned int i = 0; i < tableLowerName.length(); ++i ){
		tableLowerName.at( i ) = tolower( tableLowerName.at( i ) );
	}

	sprintf_s(sql,"SELECT relname FROM pg_class WHERE relkind = 'r' AND relname = '%s';",tableLowerName.c_str());
	PGresult *ret = PQexec( m_db, sql);

	int tuples = PQntuples(ret);
	PQclear(ret);
	if(tuples > 0){
		LOG_TRACE(FMT("Database %s is exist.",tableLowerName.c_str()));
		return true;
	}
	LOG_TRACE(FMT("Database %s is not exist.",tableLowerName.c_str()));
	return false;
}
