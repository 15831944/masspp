/**
 * @file ExternalDB.h
 * @brief interfaces of ExternalDB class
 *
 * @author S.Tanaka
 * @date 2013.07.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "ExternalDB.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ExternalDB::ExternalDB( const char* name, const char* searchFormat ) {
	m_name = NVL( name, "" );
	m_searchFormat = NVL( searchFormat, "" );

	ExternalDBManager& mgr = ExternalDBManager::getInstance();
	mgr.addDb( this );
}

// destructor
ExternalDB::~ExternalDB() { 
	ExternalDBManager& mgr = ExternalDBManager::getInstance();
	mgr.removeDb( this );
}

// get name
const char* ExternalDB::getName() {
	return m_name.c_str();
}

// get search URL
std::string ExternalDB::getSearchUrl( const char* key ) {
	std::string url = FMT( m_searchFormat.c_str(), key );

	return url;
}

// search
void ExternalDB::search( std::vector< SearchResult >& result, const char* key ) {
	// connection
	kome::net::NetworkManager& netMgr = kome::net::NetworkManager::getInstance();
	kome::net::HttpConnection* con = netMgr.getHttpConnection();
	if( con == NULL ) {
		return;
	}

	// temporary directory
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	if( m_tmpFolder.empty() ) {
		std::string tmpDir = msppMgr.getTmpDir();
		std::string dbDir = getpath( tmpDir.c_str(), "externalDb" );
		if( !fileexists( dbDir.c_str() ) ) {
			makedirectory( dbDir.c_str() );
		}

		m_tmpFolder = dbDir;
	}

	// temporary file
	std::string fileName = FMT( "%lld", getcurrenttime() );
	fileName = msppMgr.getTmpFileName( fileName.c_str(), ".html", "externalDb" );
	std::string path = getpath( m_tmpFolder.c_str(), fileName.c_str() );

	// connect
	std::string url = getSearchUrl( key );
	if( con->openUrl( url.c_str() ) ) {
		con->sendRequest();

		FILE* fp = fileopen( path.c_str(), "wb" );
		if( fp != NULL ) {
			kome::core::FileAccessor acc( fp );
			con->getResponse( acc );
			fflush( fp );
			fclose( fp );

			fp = fileopen( path.c_str(), "r" );
			if( fp != NULL ) {
				parse( fp, result );
				fclose( fp );
			}
		}
	}
	else {
		LOG_ERROR( FMT( "Connecting the server is failed. [%s]", url.c_str() ) );
	}
}


//
// External DB Manager
//

// constructor
ExternalDBManager::ExternalDBManager() {
}

// destructor
ExternalDBManager::~ExternalDBManager() {
	while( m_dbSet.size() > 0 ) {
		delete *( m_dbSet.begin() );
	}
}

// add database
void ExternalDBManager::addDb( ExternalDB* db ) {
	m_dbSet.insert( db );
}

// remove database
void ExternalDBManager::removeDb( ExternalDB* db ) {
	m_dbSet.erase( db );
}

// get instance
ExternalDBManager& ExternalDBManager::getInstance() {
	static ExternalDBManager mgr;

	return mgr;
}
