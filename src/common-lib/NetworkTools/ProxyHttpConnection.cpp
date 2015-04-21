/**
 * @file ProxyHttpConnection.cpp
 * @brief implements of ProxyHttpConnection class
 *
 * @author S.Tanaka
 * @date 2009.07.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ProxyHttpConnection.h"

#include "HttpConnection.h"


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define USER_PARAM_NAME			  "user"
#define PASS_PARAM_NAME			  "password"

#define SECTION					  "proxy"
#define USER_PARAM_KEY			   "PROXY_USER"
#define PASS_PARAM_KEY			   "PROXY_PASS"


// constructor
ProxyHttpConnection::ProxyHttpConnection() {
}

// constructor
ProxyHttpConnection::ProxyHttpConnection( boost::function< bool ( std::string&, std::string& ) > fun ) {
	m_getUserFun = fun;
}

// destructor
ProxyHttpConnection::~ProxyHttpConnection() {
}

// set proxy
void ProxyHttpConnection::setProxy(
		const char* server,
		const int port,
		const char* user,
		const char* pass
) {
	m_server = NVL( server, "" );
	m_port = port;
	m_user = NVL( user, "" );
	m_pass = NVL( pass, "" );
}

// get header string
std::string ProxyHttpConnection::getHeadersString() {
	// set header
	setHeader( "Proxy-Connection", "Close" );

	// proxy user
	if( !m_user.empty() ) {
		std::string user = FMT( "%s:%s", m_user.c_str(), m_pass.c_str() );

		const unsigned int buffLen = user.length() * 2 + 10;
		char* base64 = new char[ buffLen ];
		int base64Size = kome::core::Base64::encode( &( user[ 0 ] ), user.length(), base64, buffLen );
		base64[ base64Size ] = '\0';
		std::string auth = FMT( "Basic %s", base64 );

		setHeader( "Proxy-Authorization", auth.c_str() );

		delete[] base64;
	}

	return HttpConnection::getHeadersString();
}

// open URL
bool ProxyHttpConnection::openUrl( const char* url ) {
	// set path
	m_path = NVL( url, "" );

	// copy to buffer
	const unsigned len = ( url == NULL ? 0 : strlen( url ) ) + 1;
	char* buff = new char[ len + 1 ];
	if( len > 0 ) {
		memcpy( buff, url, len );
	}
	buff[ len ] = '\0';

	// get server
	char* server = strstr( buff, "://" );
	if( server == NULL ) {
		server = buff;
	}
	else {
		server += 3;
	}

	char* end = strstr( server, "/" );
	if( end != NULL ) {
		*end = '\0';
	}
	end = strstr( server, ":" );
	if( end != NULL ) {
		*end = '\0';
	}
	setHeader( "Host", server );

	// set path
	m_path = NVL( url, "" );

	// connect
	bool ret = true;
	if( !isOpened() ) {
		ret = onOpen( m_server.c_str(), m_port );
	}

	delete[] buff;

	return ret;
}

// on open
bool ProxyHttpConnection::onOpen( const char* server, const int port ) {
	// open
	const bool ret = HttpConnection::onOpen( server, port );

	return ret;
}

// on create recuest
void ProxyHttpConnection::onCreateRequest( kome::core::DataAccessor& req ) {
	HttpConnection::onCreateRequest( req );
}

// on send
void ProxyHttpConnection::onSend( kome::core::DataAccessor& req, kome::core::DataAccessor& res ) {
	HttpConnection::onSend( req, res );
}

// send request
void ProxyHttpConnection::sendRequest() {
	// initialize the response
	if( m_reader != NULL ) {
		fclose( m_reader );
		m_reader = NULL;
	}

	// open
	if( !m_opened ) {
		openConnection( m_server.c_str(), m_port );
	}

	// send
	bool loop = true;
	do {
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

		// check the response
		in = fileopen( m_resPath.c_str(), "rb" );

		char buff[ 2048 ];
		int size = fread( buff, 1, 2047, in );
		buff[ size ] = '\0';

		// status code
		char* first = strtok( buff, " \t\r\n" );
		char* second = ( first == NULL ? NULL : strtok( NULL, " \t\r\n" ) );
		int code = 200;
		if( first != NULL && second != NULL ) {
			std::string firstStr = touppercase( first );
			std::string::size_type pos = firstStr.find( "HTTP" );
			if( pos == 0 ) {
				code = toint( second, 10, code );
			}
		}

		// authorization error
		if( code == 407 ) {
			std::string proxyUser;
			std::string proxyPass;
			if( m_getUserFun( proxyUser, proxyPass ) ) {
				m_user = proxyUser;
				m_pass = proxyPass;
			}
			else {
				loop = false;
			}
		}
		else {
			loop = false;
		}

		// close
		fclose( in );

	} while( loop );

	// close connection
	closeConnection();
}
