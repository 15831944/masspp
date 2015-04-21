/**
 * @file HttpConnection.cpp
 * @brief implements of HttpConnection class
 *
 * @author S.Tanaka
 * @date 2009.07.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "HttpConnection.h"


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
HttpConnection::HttpConnection() {
	m_method = METHOD_GET;
}

// destructor
HttpConnection::~HttpConnection() {
}

// sets login information
void HttpConnection::setLoginInfo(
		const char* user,
		const char* pass
) {
	m_user = NVL( user, "" );
	m_pass = NVL( pass, "" );
}

// set path 
void HttpConnection::setPath( const char* path ) {
	m_path = NVL( path, "" );
}

// get path
const char* HttpConnection::getPath() {
	return m_path.c_str();
}

// clear headers
void HttpConnection::clearHeaders() {
	m_headers.clear();
}

// sets header value
void HttpConnection::setHeader( const char* name, const char* value ) {
	// string object
	std::string n = NVL( name, "" );
	std::string v = NVL( value, "" );
	if( n.empty() ) {
		return;
	}

	// search
	int idx = searchHeader( n.c_str() );
	if( idx >= 0 ) {
		m_headers[ idx ].second = v;
	}
	else {
		m_headers.push_back( std::make_pair( n, v ) );
	}
}

// get the number of headers
unsigned int HttpConnection::getNumberOfHeaders() {
	return m_headers.size();
}

// get header name
const char* HttpConnection::getHeaderName( const unsigned int idx ) {
	if( idx >= m_headers.size() ) {
		return NULL;
	}
	return m_headers[ idx ].first.c_str();
}

// get header value
const char* HttpConnection::getHeaderValue( const unsigned int idx ) {
	if( idx >= m_headers.size() ) {
		return NULL;
	}
	return m_headers[ idx ].second.c_str();
}

// get header value
const char* HttpConnection::getHeaderValue( const char* name ) {
	// search
	int idx = searchHeader( name );
	if( idx < 0 ) {
		return NULL;
	}

	// get value
	return m_headers[ idx ].second.c_str();
}

// clear parameters
void HttpConnection::clearParameters() {
	m_parameters.clear();
}

// sets parameter value
void HttpConnection::setParameter( const char* name, const char* value, const bool isFile ) {
	// string object
	std::string n = NVL( name, "" );
	std::string v = NVL( value, "" );
	if( n.empty() ) {
		return;
	}

	// set parameter
	m_parameters.resize( m_parameters.size() + 1 );
	m_parameters.back().name = n;
	m_parameters.back().value = v;
	m_parameters.back().isFile = isFile;
}

// delete parameter
void HttpConnection::deleteParameter( const int idx ) {
	if( idx >= 0 && idx < (int)m_parameters.size() ) {
		m_parameters.erase( m_parameters.begin() + idx );
	}
}

// get the number of parameters
unsigned int HttpConnection::getNumberOfParameters() {
	return m_parameters.size();
}

// get parameter name
const char* HttpConnection::getParameterName( const unsigned int idx ) {
	if( idx >= m_parameters.size() ) {
		return NULL;
	}
	return m_parameters[ idx ].name.c_str();
}

// get parameter value
const char* HttpConnection::getParameterValue( const unsigned int idx ) {
	if( idx >= m_parameters.size() ) {
		return NULL;
	}
	return m_parameters[ idx ].value.c_str();
}

// get parameter value
const char* HttpConnection::getParameterValue( const char* name ) {
	// search
	int idx = searchParameter( name );
	if( idx < 0 ) {
		return NULL;
	}

	// get value
	return m_parameters[ idx ].value.c_str();
}

// get boundary text
const char* HttpConnection::getBoundary() {
	// create boundary text
	if( m_boundary.empty() ) {
		m_boundary = FMT(
			"------------------------------%04x%04x%04x",
			(long)this,
			(long)time( NULL ),
			(long)( 0xffff & getcurrenttime() >> 2 )
		);
	}

	// return
	return m_boundary.c_str();
}

// search header
int HttpConnection::searchHeader( const char* name ) {
	// string
	std::string n = NVL( name, "" );

	// search
	for( unsigned int i = 0; i < m_headers.size(); i++ ) {
		if( m_headers[ i ].first.compare( n ) == 0 ) {
			return (int)i;
		}
	}

	return -1;
}

// search parameter
int HttpConnection::searchParameter( const char* name ) {
	// string
	std::string n = NVL( name, "" );

	// search
	for( unsigned int i = 0; i < m_parameters.size(); i++ ) {
		if( m_parameters[ i ].name.compare( n ) == 0 ) {
			return (int)i;
		}
	}

	return -1;
}

// set HTTP method
void HttpConnection::setMethod( HttpMethod method ) {
	m_method = method;
}

// get HTTP method
HttpConnection::HttpMethod HttpConnection::getMethod() {
	return m_method;
}

// create GET request
void HttpConnection::createGetRequest( kome::core::DataAccessor& req ) {
	// parameters string
	std::string params = getParamsString();

	// first line
	std::string s;
	if( params.empty() ) {
		s = FMT( "GET %s HTTP/1.1\r\n", m_path.c_str() );
	}
	else {
		s = FMT( "GET %s?%s HTTP/1.1\r\n", m_path.c_str(), params.c_str() );
	}

	req.write( &( s[ 0 ] ), s.length() );


	// headers
	std::string headers = getHeadersString();
	if( !headers.empty() ) {
		req.write( &( headers[ 0 ] ), headers.length() );
	}

	// blank line
	std::string newLine = "\r\n";
	req.write( &( newLine[ 0 ] ), newLine.length());

	// original request
	if( fileexists( m_tmpPath.c_str() ) ) {
		BasicConnection::onCreateRequest( req );
	}
}

// create POST request
void HttpConnection::createPostRequest( kome::core::DataAccessor& req ) {
	// parameters
	int size = 0;
	std::string params = getParamsString();
	bool paramFlg = false;
	if( !params.empty() ) {
		size = params.length();
		paramFlg = true;
	}

	// original request
	bool tmpFlg = false;
	std::string newLine = "\r\n";
	bool newLineFlg = false;
	if( fileexists( m_tmpPath.c_str() ) ) {
		if( size > 0 ) {
			size = size + newLine.length();
			newLineFlg = true;
		}
		size = size + filesize( m_tmpPath.c_str() );
		tmpFlg = true;
	}

	// first line
	std::string s = FMT( "POST %s HTTP/1.1\r\n", m_path.c_str() );
	req.write( &( s[ 0 ] ), s.length() );

	// headers
	setHeader( "Content-Length", FMT( "%d", size ).c_str() );
	std::string headers = getHeadersString();
	if( !headers.empty() ) {
		req.write( &( headers[ 0 ] ), headers.length() );
	}

	// blank line
	req.write( &( newLine[ 0 ] ), newLine.length() );

	// parameters
	if( paramFlg ) {
		req.write( &( params[ 0 ] ), params.length() );
	}

	if( newLineFlg ) {
		req.write( &( newLine[ 0 ] ), newLine.length() );
	}

	// original request
	if( tmpFlg ) {
		BasicConnection::onCreateRequest( req );
	}
}

// create multi part form request
void HttpConnection::createMultiPartFormRequest( kome::core::DataAccessor& req ) {
	// parameters
	int size = 0;
	std::string paramPath = FMT( "%s.params", m_reqPath.c_str() );
	bool paramFlg = false;
	getMultiPartParamsString( paramPath.c_str() );
	if( fileexists( paramPath.c_str() ) ) {
		size = filesize( paramPath.c_str() );
		paramFlg = true;
	}

	// original request
	bool tmpFlg = false;
	std::string newLine = "\r\n";
	bool newLineFlg = false;
	if( fileexists( m_tmpPath.c_str() ) ) {
		if( size > 0 ) {
			size = size + newLine.length();
			newLineFlg = true;
		}
		size = size + filesize( m_tmpPath.c_str() );
		tmpFlg = true;
	}

	// first line
	std::string s = FMT( "POST %s HTTP/1.1\r\n", m_path.c_str() );
	req.write( &( s[ 0 ] ), s.length() );

	// headers
	setHeader(
		"Content-Type",
		FMT( "multipart/form-data; boundary=%s", getBoundary() ).c_str()
	);
	setHeader( "Content-Length", FMT( "%d", size ).c_str() );
	std::string headers = getHeadersString();
	if( !headers.empty() ) {
		req.write( &( headers[ 0 ] ), headers.length() );
	}

	// blank line
	req.write( &( newLine[ 0 ] ), newLine.length() );

	// request
	if( paramFlg ) {
		FILE* in = fileopen( paramPath.c_str(), "rb" );
		if( in != NULL ) {
			char buff[ 2048 ];
			while( ( size = fread( buff, 1, 2048, in ) ) > 0 ) {
				req.write( buff, size );
			}
			fclose( in );
		}
	}
	if( newLineFlg ) {
		req.write( &( newLine[ 0 ] ), newLine.length() );
	}
	if( tmpFlg ) {
		FILE* in = fileopen( m_tmpPath.c_str(), "rb" );
		if( in != NULL ) {
			char buff[ 2048 ];
			while( ( size = fread( buff, 1, 2048, in ) ) > 0 ) {
				req.write( buff, size );
			}
			fclose( in );
		}
	}
}

// get headers string
std::string HttpConnection::getHeadersString() {
	// default headers
	setHeader( "Accept", "*/*" );
	setHeader( "Accept-Language", "en-us" );
	setHeader( "Accept-Encoding", "deflate" );
	setHeader( "Connection", "Close" );
	setHeader( "User-Agent", "Mass PlusPlus" );

	// user
	if( !m_user.empty() ) {
		std::string user = FMT( "%s:%s", m_user.c_str(), m_pass.c_str() );

		const unsigned int buffLen = user.length() * 2 + 10;
		char* base64 = new char[ buffLen ];
		int base64Size = kome::core::Base64::encode( &( user[ 0 ] ), user.length(), base64, buffLen );
		base64[ base64Size ] = '\0';
		std::string auth = FMT( "Basic %s", base64 );

		setHeader( "Authorization", auth.c_str() );

		delete[] base64;
	}

	// headers
	std::string headers;

	for( unsigned int i = 0; i < m_headers.size(); i++ ) {
		std::string n = m_headers[ i ].first;
		std::string v = m_headers[ i ].second;

		std::string line = FMT( "%s: %s\r\n", n.c_str(), v.c_str() );
		headers.append( line );
	}

	return headers;
}

// get parameters string
std::string HttpConnection::getParamsString() {
	// parameters
	std::string params;
	
	// each parameters
	for( unsigned int i = 0; i < m_parameters.size(); i++ ) {
		std::string n = m_parameters[ i ].name;
		std::string v = m_parameters[ i ].value;
		
		if( !params.empty() ) {
			params.append( "&" );
		}
		
		std::string line = FMT( "%s=%s", n.c_str(), v.c_str() );
		params.append( line );
	}

	return params;
}

// get parameters string when multi-part-form
void HttpConnection::getMultiPartParamsString( const char* path ) {
	// file open
	FILE* fp = fopen( path, "wb" );
	if( fp == NULL ) {
		return;
	}

	// string
	std::string s;

	// fixed strings
	std::string boundary = getBoundary();
	std::string newLine = "\r\n";

	// each parameters
	for( unsigned int i = 0; i < m_parameters.size(); i++ ) {
		std::string n = m_parameters[ i ].name;
		std::string v = m_parameters[ i ].value;

		s = FMT( "--%s%s", boundary.c_str(), newLine.c_str() );
		fwrite( &( s[ 0 ] ), 1, s.length(), fp );

		s = FMT( "Content-Disposition: form-data; name=\"%s\"", n.c_str() );
		fwrite( &( s[ 0 ] ), 1, s.length(), fp );

		if( m_parameters[ i ].isFile ) {
			s = FMT( "; filename=\"%s\"%sContent-Type: text/plain%s%s", v.c_str(), newLine.c_str(), newLine.c_str(), newLine.c_str() );
			fwrite( &( s[ 0 ] ), 1, s.length(), fp );

			FILE* in = fileopen( v.c_str(), "rb" );
			if( in != NULL ) {
				char buff[ 2048 ];
				int size = 0;
				while( ( size = fread( buff, 1, 2047, in ) ) > 0 ) {
					fwrite( buff, 1, size, fp );
				}
				fclose( in );
			}
			fwrite( &( newLine[ 0 ] ), 1, newLine.length(), fp );
		}
		else {
			s = FMT( "%s%s%s%s", newLine.c_str(), newLine.c_str(), v.c_str(), newLine.c_str() );
			fwrite( &( s[ 0 ] ), 1, s.length(), fp );
		}
	}

	// end
	s = FMT( "--%s--%s", boundary.c_str(), newLine.c_str() );
	fwrite( &( s[ 0 ] ), 1, s.length(), fp );

	// close
	fflush( fp );
	fclose( fp );
}

// open connection using URL
bool HttpConnection::openUrl( const char* url ) {
	// check the parameter
	if( url == NULL ) {
		return false;
	}

	// create buffer
	const unsigned int len = strlen( url );
	char* buff = new char[ len + 1 ];
	for( unsigned int i = 0; i < len; i++ ) {
		buff[ i ] = url[ i ];
	}
	buff[ len ] = '\0';

	// get host
	char* host = strstr( buff, "://" );
	if( host == NULL ) {
		host = buff;
	}
	else {
		host = host + 3;
	}

	// get path
	char* path = strstr( host, "/" );
	if( path != NULL ) {
		m_path = path;
		*path = '\0';
	}

	// port
	int port = 80;
	char* portStr = strstr( host, ":" );
	if( portStr != NULL ) {
		port = toint( portStr + 1, 10, port );
		*portStr = '\0';
	}

	// connect
	const bool ret = openConnection( host, port );

	delete[] buff;

	return ret;
}

// on create recuest
void HttpConnection::onCreateRequest( kome::core::DataAccessor& req ) {
	// set to header
	setHeader( "Host", m_server.c_str() );

	// create request
	if( m_method == METHOD_GET && !fileexists( m_tmpPath.c_str() ) )  {    // GET
		createGetRequest( req );
	}
	else if( m_method == METHOD_MULTI_PART ) {    // POST (multi part form)
		createMultiPartFormRequest( req );
	}
	else {    // POST
		createPostRequest( req );
	}
}

// on send
void HttpConnection::onSend( kome::core::DataAccessor& req, kome::core::DataAccessor& res ) {
	// send
	BasicConnection::onSend( req, res );
}
