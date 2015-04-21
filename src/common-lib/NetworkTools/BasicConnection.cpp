/**
 * @file BasicConnection.cpp
 * @brief implements of BasicConnection class
 *
 * @author S.Tanaka
 * @date 2009.07.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BasicConnection.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


#pragma comment (lib, "winhttp.lib")


#ifdef __unix
	#include <netdb.h>
	#include <sys/socket.h>
#else
	#include <io.h>
#endif  // __unix


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
BasicConnection::BasicConnection() {
	m_soc = -1;
	m_url = "";
	m_hSession = NULL;
    m_hConnect = NULL;
    m_hRequest = NULL;
}

// destructor
BasicConnection::~BasicConnection() {
}


// on open
bool BasicConnection::onOpen( const char* server, const int port ) {
	
	// get host
	struct hostent* host = gethostbyname( server );
	if( host == NULL ) {
		LOG_ERROR(
			FMT( "Failed to convert host name to IP address.", NVL( server, "" ) )
		);
		return false;
	}

	// socket
	struct sockaddr_in addr;
	fillzero( &addr, sizeof( addr ) );
	addr.sin_family = AF_INET;
	memcpy( &addr.sin_addr, host->h_addr, host->h_length );

	if( port >= 0 ) {
		addr.sin_port = htons( (unsigned short)port );
	}
	else {
		struct servent* service = getservbyname( "http", "tcp" );
		if( service != NULL ) {
			addr.sin_port = service->s_port;
		}
		else {
			addr.sin_port = htons( (unsigned short)80 );
		}
	}
	m_soc = socket( PF_INET, SOCK_STREAM, 0 );
	if( m_soc < 0 ) {
		LOG_ERROR( FMT( "Failed to create a socket." ) );
		return false;
	}

	// connect
	if( connect( m_soc, (struct sockaddr*)( &addr ), sizeof( addr ) ) < 0 ) {
		LOG_ERROR( FMT( "Failed to connect." ) );
		return false;
	}

	return true;
}

// on close
void BasicConnection::onClose() {

#ifdef __unix
	shutdown( m_soc, SHUT_RDWR );
	close( m_soc );
#else
	shutdown( m_soc, SD_BOTH );
	closesocket( m_soc );
#endif  // __unix
}

// on create recuest
void BasicConnection::onCreateRequest( kome::core::DataAccessor& req ) {
	// tmp file
	FILE* in = fileopen( m_tmpPath.c_str(), "rb" );
	if( in == NULL ) {
		return;
	}

	// copy
	char buff[ 2048 ];
	int size = 0;

	while( ( size = fread( buff, 1, 2048, in ) ) > 0 ) {
		req.write( buff, size );
	}

	fclose( in );
}

// on send
void BasicConnection::onSend( kome::core::DataAccessor& req, kome::core::DataAccessor& res ) {
	// timer
	kome::core::Timer tm;
	tm.start();

	// send request
	char buff[ 2048 ];
	int size = 0;
	while( ( size = req.read( buff, 2048 ) ) > 0 ) {
		send( m_soc, buff, size, 0 );
	}

	// receive response
	while( ( size = recv( m_soc, buff, 2048, 0 ) ) > 0 ) {
		res.write( buff, size );
	}

	tm.stop();

	LOG_DEBUG( FMT( "Network Time: %.3f s", tm.getTotalTime() ) );
}
