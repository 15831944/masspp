/**
 * @file NetworkManager.cpp
 * @brief implements of NetworkManager class
 *
 * @author S.Tanaka
 * @date 2009.07.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "NetworkManager.h"

#include "Connection.h"
#include "WinHttpConnection.h"


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SECTION			  "proxy"

#define USER_PARAM_NAME	  "user"
#define PASS_PARAM_NAME	  "password"

#define SERVER_PARAM_KEY	 "PROXY_SERVER"
#define PORT_PARAM_KEY	   "PROXY_PORT"
#define USER_PARAM_KEY	   "PROXY_USER"
#define PASS_PARAM_KEY	   "PROXY_PASS"


// constructor
NetworkManager::NetworkManager() {
}

// destructor
NetworkManager::~NetworkManager() {
	deleteAllConnections( false );
}

// add connection
void NetworkManager::addConnection( Connection* con ) {
	if( con != NULL ) {
		m_connections.push_back( con );
	}
}

// remove connection
void NetworkManager::removeConnection( Connection* con ) {
	// search
	int idx = searchConnection( con );

	// remove
	if( idx >= 0 ) {
		m_connections.erase( m_connections.begin() + idx );
	}
}

// get the number of connections
unsigned int NetworkManager::getNumberOfConnections() {
	return m_connections.size();
}

// get connection
Connection* NetworkManager::getConnection( const unsigned int idx ) {
	if( idx >= m_connections.size() ) {
		return NULL;
	}
	return m_connections[ idx ];
}

// delete all connections
void NetworkManager::deleteAllConnections( const bool onlyClosed ) {
	for( int i = (int)m_connections.size() - 1; i >= 0; i-- ) {
		Connection* con = m_connections[ i ];
		if( !onlyClosed || !con->isOpened() ) {
			delete con;
		}
	}
}

// search connection
int NetworkManager::searchConnection( Connection* con ) {
	for( unsigned int i = 0; i < m_connections.size(); i++ ) {
		if( m_connections[ i ] == con ) {
			return (int)i;
		}		
	}
	return -1;
}

// get HTTP connection
HttpConnection* NetworkManager::getHttpConnection() {	
	// delete closed connections
	deleteAllConnections( true );

	// create connection
	WinHttpConnection* con = NULL;
	con = new WinHttpConnection();
	con->setProxy( m_proxy.c_str(), m_pass.c_str() );
	return con;
}

// set proxy
void NetworkManager::setProxyServer( const char* proxy ) {
	m_proxy = trimstring( proxy );
}

// get proxy
const char* NetworkManager::getProxyServer() {
	return m_proxy.c_str();
}

// set proxy pass
void NetworkManager::setProxyPass( const char* pass ) {
	m_pass = trimstring( pass );
}

// get proxy pass
const char* NetworkManager::getProxyPass() {
	return m_pass.c_str();
}

// get network manager object
NetworkManager& NetworkManager::getInstance() {
	// create object (This is the only object.)
	static NetworkManager mgr;

	return mgr;
}
