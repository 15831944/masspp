/**
 * @file PostgreSQLDbManager.cpp
 * @brief implements of PostgreSQLDbManager class
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "PostgreSQLManager.h"
#include "PostgreSQL.h"

using namespace kome::db::postgresql;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// SQL sentence for connection
#define POSTGRE_DB_CONN_SQL		"host=%s port=%d dbname=%s user=%s password=%s"

// Length of the character string
const int	g_nPostgresqlSqlLength = 256;

// constructor
PostgreSQLManager::PostgreSQLManager() {
	m_strCheckDbName = "postgres";
}

// destructor
PostgreSQLManager::~PostgreSQLManager() {
	// close all DB
	while( m_dbMap.size() > 0 ) {
		delete ( *( m_dbMap.begin() ) ).second;
	}
}

// open DB
PostgreSQL* PostgreSQLManager::getDb( const char* address, int port, const char* name,
					const char* user, const char* password ) {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	PGconn *conn = NULL;
	PostgreSQL* db = NULL;
	ExecStatusType status;
	PGresult *res;
	char szConnSql[g_nPostgresqlSqlLength];
	char sql[g_nPostgresqlSqlLength];

	if(!checkDbName(name)){
		return NULL;
	}

	// check the map
	sprintf_s(szConnSql, POSTGRE_DB_CONN_SQL,
		address, port, name, user, password);
	if( m_dbMap.find( szConnSql ) != m_dbMap.end() ) {
		db = m_dbMap[ szConnSql ];

		if( db->isClosed() ) {
			m_dbMap.erase( szConnSql );
			delete db;
		}
		else {
			return db;
		}
	}

	// open 'DbName' DB
	conn = PQconnectdb(szConnSql);

	if(PQstatus(conn) == CONNECTION_OK){	//success
		db = new PostgreSQL( address, port, name, user, password, conn );
		m_dbMap[ szConnSql ] = db;
		return db;
	}

	// DbName is not found.
	// open postgres DB
	sprintf_s(sql, POSTGRE_DB_CONN_SQL,
		address, port, m_strCheckDbName.c_str(), user, password);
	conn = PQconnectdb(sql);

	if(PQstatus(conn) != CONNECTION_OK){	//failed
		const char* msg = PQerrorMessage( conn );	
		LOG_ERROR( FMT( "DB Open Error (%s:%d) : %s", address, port, NVL( msg, "" ) ) );
		return NULL;
	}

	// create DbName db.	
	sprintf_s(sql, "CREATE DATABASE %s;", name);
	res = PQexec( conn, sql );
	status = PQresultStatus(res);
	PQclear(res);
	if(status != PGRES_COMMAND_OK){// failed
		LOG_ERROR( FMT( "DB Create Error : %s", name));
		PQfinish(conn);
		return NULL;
	}

	//disconnect
	PQfinish(conn);

	// open 'DbName' DB
	conn = PQconnectdb(szConnSql);

	if(PQstatus(conn) == CONNECTION_OK){	//success
		db = new PostgreSQL( address, port, name, user, password, conn );
		m_dbMap[ szConnSql ] = db;
	}
	else {	// failed
		const char* msg = PQerrorMessage( conn );
		LOG_ERROR( FMT( "DB Open Error (%s:%d) : %s", address, port, NVL( msg, "" ) ) );
		db = NULL;
	}

	return db;
}

// delete DB
void PostgreSQLManager::deleteDb( PostgreSQL* db ) {
	// search
	std::map< std::string, PostgreSQL* >::iterator dIt = m_dbMap.end();
	for( std::map< std::string, PostgreSQL* >::iterator it = m_dbMap.begin();
			it != m_dbMap.end(); it++ ) {
		if( db == (*it).second ) {	// delete
			dIt = it;
		}
	}

	// delete from the map
	if( dIt != m_dbMap.end() ) {
		m_dbMap.erase( dIt );
	}

}

// get PostgreSQL manager object
PostgreSQLManager& PostgreSQLManager::getInstance() {
	// create object (This is the only object.)
	static PostgreSQLManager mgr;
	return mgr;
}

bool PostgreSQLManager::checkDbName(const char* name){
	// forbid to direct login "postgres" db.
	if( m_strCheckDbName.length() != strlen( name ) ){
		return true;
	}

	unsigned int	i;

	for( i = 0; i < m_strCheckDbName.length(); ++i ){
		if( tolower( m_strCheckDbName.at( i ) ) != tolower( name[ i ] ) ){
			return true;
		}
	}

	LOG_ERROR( FMT( "DB Error : Database \"%s\" is Protected.", m_strCheckDbName.c_str()) );
	return false;
}
