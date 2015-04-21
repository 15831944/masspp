/**
 * @file DbManager.cpp
 * @brief implements of DbManager class
 *
 * @author S.Tanaka
 * @date 2008.12.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DbManager.h"

#include "Database.h"


using namespace kome::db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DbManager::DbManager() {
	m_db = NULL;
}

// destructor
DbManager::~DbManager() {
}

// set database
void DbManager::setDb( Database* db ) { 
	if( m_db == db ) {
		return;
	}
	if( m_db != NULL ) {
		m_db->close();
	}
	m_db = db;
}

// get database
Database* DbManager::getDb() {
	return m_db;
}

// add db
void DbManager::addDb( Database* db ) {
	m_dbSet.insert( db );
}

// remove db
void DbManager::removeDb( Database* db ) {
	m_dbSet.erase( db );
}

// close all database
void DbManager::closeAllDb() { 
	for( std::set< Database* >::iterator it = m_dbSet.begin();
			it != m_dbSet.end(); it++ ) {
		(*it)->close();
	}
}

// get instance
DbManager& DbManager::getInstance() { 
	// create object (This is the only object)
	static DbManager mgr;

	return mgr;
}
