/**
 * @file SQLiteDbManager.cpp
 * @brief implements of SQLiteDbManager class
 *
 * @author S.Tanaka
 * @date 2008.08.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SQLiteDbManager.h"

#include "SQLiteDb.h"

#include <sqlite3.h>


using namespace kome::db::sqlite;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SQLiteDbManager::SQLiteDbManager() {
}

// destructor
SQLiteDbManager::~SQLiteDbManager() {
	// close all DB
	while( m_dbMap.size() > 0 ) {
		delete ( *( m_dbMap.begin() ) ).second;
	}
}

// open DB
SQLiteDb* SQLiteDbManager::getDb( const char* dbPath ) {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// path
	std::string dbFile = NVL( dbPath, "" );
	if( dbFile.empty() ) {
		return NULL;
	}
	std::string path;
	if( isabsolutepath( dbFile.c_str() ) ) {
		path = dbFile;
	}
	else {
		path = getabsolutepath( msppMgr.getConfDir(), dbFile.c_str() );
	}

	// check the map
	if( m_dbMap.find( path ) != m_dbMap.end() ) {
		SQLiteDb* db = m_dbMap[ path ];

		if( db->isClosed() ) {
			m_dbMap.erase( path );
			delete db;
		}
		else {
			return db;
		}
	}

	// open DB
	sqlite3* sqliteDb = NULL;
	SQLiteDb* db = NULL;
	int ret = sqlite3_open( path.c_str(), &sqliteDb );
	if( ret == SQLITE_OK ) {	// success
		db = new SQLiteDb( path.c_str(), sqliteDb );
		m_dbMap[ path ] = db;
	}
	else {	// failed
		const char* msg = sqlite3_errmsg( sqliteDb );
		LOG_ERROR( FMT( "DB Open Error (%s) : %s", path.c_str(), NVL( msg, "" ) ) );
	}

	return db;
}

// delete DB
void SQLiteDbManager::deleteDb( SQLiteDb* db ) {
	// search
	std::map< std::string, SQLiteDb* >::iterator dIt = m_dbMap.end();
	for( std::map< std::string, SQLiteDb* >::iterator it = m_dbMap.begin();
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

// get SQLite manager object
SQLiteDbManager& SQLiteDbManager::getInstance() {
	// create object (This is the only object.)
	static SQLiteDbManager mgr;

	return mgr;
}
