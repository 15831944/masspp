/**
 * @file Connection.cpp
 * @brief implements of Connection class
 *
 * @author S.Tanaka
 * @date 2009.07.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Connection.h"

#include "NetworkManager.h"


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


unsigned long Connection::m_counter = 0;


// constructor
Connection::Connection() {
	// initialize 
	m_opened = false;
	m_port = -1;
	m_reader = NULL;

	// add to  the manager
	NetworkManager& nwMgr = NetworkManager::getInstance();
	nwMgr.addConnection( this );

	// request and response file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	std::string conDir = getpath( msppMgr.getTmpDir(), "connection" );
	if( !fileexists( conDir.c_str() ) ) {
		makedirectory( conDir.c_str() );
	}

	std::string dir = getpath( conDir.c_str(), FMT( "%02x", ( m_counter % 0x100 ) ).c_str() );
	if( !fileexists( dir.c_str() ) ) {
		makedirectory( dir.c_str() );
	}

	m_reqPath = getpath( dir.c_str(), FMT( "%05x.req", ( m_counter / 0x100 ) ).c_str() );
	m_resPath = getpath( dir.c_str(), FMT( "%05x.res", ( m_counter / 0x100 ) ).c_str() );
	m_tmpPath = FMT( "%s.tmp", m_reqPath.c_str() );

	// counter
	m_counter = m_counter + 1;
}

// destructor
Connection::~Connection() {
	// close
	closeConnection();

	// delete from the manager
	NetworkManager& nwMgr = NetworkManager::getInstance();
	nwMgr.removeConnection( this );
}

// initializes request
void Connection::initRequest() {
	if( fileexists( m_reqPath.c_str() ) ) {
		removefile( m_reqPath.c_str() );
	}
}

// writes to request
void Connection::writeToRequest( char* req, const unsigned int len ) {
	// check parameters
	if( req == NULL || len == 0 ) {
		return;
	}

	// file
	FILE* fp = fileopen( m_tmpPath.c_str(), "ab" );
	fwrite( req, 1, len, fp );
	fflush( fp );
	fclose( fp );
}

// get response
void Connection::getResponse( kome::core::DataAccessor& acc ) {
	// file open
	FILE* fp = fileopen( m_resPath.c_str(), "rb" );
	if( fp == NULL ) {
		return;
	}

	// get the response
	char buff[ 2048 ];
	int size = 0;
	while( ( size = fread( buff, 1, 2048, fp ) ) > 0 ) {
		acc.write( buff, size );
	}

	// close
	fclose( fp );
}

// get response
void* Connection::getResponse() {
	if( m_responseBuffer.getLength() == 0 ) {
		getResponse( m_responseBuffer );
	}

	return m_responseBuffer.getBuffer();
}

// get response length
unsigned int Connection::getResponseLength() {
	if( m_responseBuffer.getLength() == 0 ) {
		getResponse( m_responseBuffer );
	}

	return m_responseBuffer.getLength();
}

// get server
const char* Connection::getServer() {
	return m_server.c_str();
}

// get port number
int Connection::getPort() {
	return m_port;
}

// open
bool Connection::openConnection( const char* server, const int port ) {
	// check the connection
	if( m_opened ) {
		closeConnection();
	}

	// set to members
	m_server = NVL( server, "" );
	m_port = port;

	// open
	m_opened = onOpen( server, port );

	return m_opened;
}

// close
void Connection::closeConnection() {
	if( m_opened ) {
		onClose();
		m_opened = false;
	}
}

// send request
void Connection::sendRequest() {
	// initialize the response
	if( m_reader != NULL ) {
		fclose( m_reader );
		m_reader = NULL;
	}

	// open
	if( !m_opened ) {
		openConnection( m_server.c_str(), m_port );
	}

	// clear response
	FILE* out = fileopen( m_resPath.c_str(), "wb" );
	if( out == NULL ) {
		return;
	}
	m_responseBuffer.clear();

	// request
	FILE* in = fileopen( m_reqPath.c_str(), "wb" );
	if( in == NULL ) {
		return;
	}
	kome::core::FileAccessor acc( in );
	onCreateRequest( acc );

	fflush( in );
	fclose( in );
	in = fileopen( m_reqPath.c_str(), "rb" );
	if( in == NULL ) {
		return;
	}

	// send
	kome::core::FileAccessor req( in );
	kome::core::FileAccessor res( out );
	onSend( req, res );

	// close files
	fflush( out );
	fclose( out );
	fclose( in );

	// close connection
	closeConnection();
}

// opened or closed
bool Connection::isOpened() {
	return m_opened;
}

// releases the connection
void Connection::release() {
	closeConnection();
	delete this;
}

// write
int Connection::write( void* addr, int size ){
	if( addr == NULL || size <= 0 ) {
		return 0;
	}

	// write
	writeToRequest( (char*)addr, size );
	return size;
}

// read
int Connection::read( void* addr, int size ) {
	// open
	if( m_reader == NULL ) {
		m_reader = fileopen( m_resPath.c_str(), "rb" );
	}
	if( m_reader == NULL ) {
		return 0;
	}

	// read
	int s = fread( addr, 1, size, m_reader );
	return s;
}
