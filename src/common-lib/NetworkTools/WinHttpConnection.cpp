/**
 * @file WinHttpConnection.cpp
 * @brief implements of WinHttpConnection class
 *
 * @author S.Tanaka
 * @date 2013.06.03
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "WinHttpConnection.h"

#include "HttpConnection.h"
#include "ProxyHttpConnection.h"

#include <windows.h>
#include <winhttp.h>
#include <wincred.h>


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SECTION "Proxy"
#define PROXY_SERVER        "SERVER"
#define PROXY_PASS          "PASS"
#define PROXY_USER_KEY   "USER"
#define PROXY_PASSWORD_KEY     "PASSWORD"


// constructor
WinHttpConnection::WinHttpConnection() {
	// initialize
	m_baseCon = NULL;

	// proxy
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini != NULL ) {
		setProxy(
			ini->getString( SECTION, PROXY_SERVER, "" ),
			ini->getString( SECTION, PROXY_PASS, "" )
		);

		m_proxyUser = trimstring( ini->getString( SECTION, PROXY_USER_KEY, "" ) );
		if( m_proxyUser.compare( "none" ) == 0 ) {
			m_proxyUser.clear();
		}

		m_proxyPassword = trimstring( ini->getString( SECTION, PROXY_PASSWORD_KEY, "" ) );
		if( m_proxyPassword.compare( "none" ) == 0 ) {
			m_proxyPassword.clear();
		}
	}
}

// destructor
WinHttpConnection::~WinHttpConnection() {
}

// set proxy
void WinHttpConnection::setProxy( const char* server, const char* e ) {
	m_proxyServer = trimstring( server );
	if( m_proxyServer.compare( "none" ) == 0 ) {
		m_proxyServer.clear();
	}

	m_proxyException = trimstring( e );
	if( m_proxyException.compare( "none" ) == 0 ) {
		m_proxyException.clear();
	}
}

// get proxy server
const char* WinHttpConnection::getProxyServer() {
	return m_proxyServer.c_str();
}

// get proxy exception
const char* WinHttpConnection::getProxyException() {
	return m_proxyException.c_str();
}

// sets proxy account
void WinHttpConnection::setProxyAccount( const char* user, const char* pass ) {
	// member
	m_proxyUser = trimstring( user );
	m_proxyPassword = trimstring( pass );

	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return;
	}

	ini->setString( SECTION, PROXY_USER_KEY, NVL( user, "" ) );
	ini->setString( SECTION, PROXY_PASSWORD_KEY, NVL( pass, "" ) );
}

// gets proxy account
void WinHttpConnection::getProxyAccount( std::string& user, std::string& pass ) {
	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return;
	}

	std::string tmpUser = ini->getString( SECTION, PROXY_USER_KEY, "" );
	std::string tmpPass = ini->getString( SECTION, PROXY_PASSWORD_KEY, "" );

	if( tmpUser.compare( "none" ) == 0 ) {
		tmpUser = "";
	}
	if( tmpPass.compare( "none" ) == 0 ) {
		tmpPass = "";
	}

	user = trimstring( tmpUser.c_str() );
	pass = trimstring( tmpPass.c_str() );
}

// shows authorization dialog
bool WinHttpConnection::showAuthorizationDialog( std::string& user, std::string& pass ) {
	// preparation
	CredUIConfirmCredentialsW( NULL, TRUE );

	const char* server = m_baseCon->getServer();
	int svLength = getCastLength( server );
	wchar_t* pServer = new wchar_t[ svLength ];
	castWChar( server, pServer, svLength );
	CredUIConfirmCredentialsW( pServer, TRUE );

	// open dialog
	WCHAR pszUserName[ 256 ] = { 0 };
	WCHAR pszUser[ 256 ] = { 0 };
	WCHAR pszPass[ 256 ] = { 0 };
	WCHAR pszDomain[ 256 ] = { 0 };

	DWORD result = CredUIPromptForCredentialsW(
			NULL,
			pServer,
			NULL,
			0,
			pszUserName,
			256,
			pszPass,
			256,
			NULL,
			CREDUI_FLAGS_DO_NOT_PERSIST
	);

	delete[] pServer;
	if( result != NO_ERROR ) {
		return false;
	}

	CredUIParseUserNameW( pszUserName, pszUser, 256, pszDomain, 256 );

	// set
	char userName[ 256 ];
	char p[ 256 ];
	size_t size = 0;

	wcstombs_s( &size, userName, 256, pszUser, 256 );
	userName[ size ] = '\0';
	user = userName;

	wcstombs_s( &size, p, 256, pszPass, 256 );
	p[ size ] = '\0';
	pass = p;

	// set
	setProxyAccount( user.c_str(), pass.c_str() );

	return true;
}

// chooses authorization scheme
DWORD WinHttpConnection::chooseAuthScheme( DWORD supportedSchemes ) {
	if( supportedSchemes & WINHTTP_AUTH_SCHEME_NEGOTIATE ) {
		return WINHTTP_AUTH_SCHEME_NEGOTIATE;
	}
	if( supportedSchemes & WINHTTP_AUTH_SCHEME_NTLM ) { 
		return WINHTTP_AUTH_SCHEME_NTLM;
	}
	if( supportedSchemes & WINHTTP_AUTH_SCHEME_PASSPORT ) {
		return WINHTTP_AUTH_SCHEME_PASSPORT;
	}
	if( supportedSchemes & WINHTTP_AUTH_SCHEME_DIGEST ) {
		return WINHTTP_AUTH_SCHEME_DIGEST;
	}

	return 0;
}

// is proxy
bool WinHttpConnection::isProxy( const char* url ) {
	// check the parameter
	if( url == NULL ) {
		return false;
	}

	// check the member
	if( m_proxyServer.empty() ) {
		return false;
	}

	// create buffer
	const unsigned int len = strlen( url );
	char* buff = new char[ len + 1 ];
	for( unsigned int i = 0; i < len; i++ ) {
		buff[ i ] = url[ i ];
	}
	buff[ len ] = '\0';

	// get server
	char* tmp = strstr( buff, "://" );
	if( tmp == NULL ) {
		tmp = buff;
	}
	else {
		tmp = tmp + 3;
	}

	char* slash = strstr( tmp, "/" );
	if( slash != NULL ) {
		*slash = '\0';
	}

	std::string server = trimstring( tmp );
	std::string::size_type pos = server.find( ":" );
	if( pos != server.npos ) {
		server = server.substr( 0, pos );
	}
	std::vector< std::string > domains;
	stringseparate( server.c_str(), ".", domains );

	delete[] buff;

	// exceptions
	std::vector< std::string > exceptions;
	stringtoken( m_proxyException.c_str(), ":;", exceptions );

	// check
	for( unsigned int i = 0; i < exceptions.size(); i++ ) {
		std::string e = exceptions[ i ];
		if( e.find( "local" ) != e.npos ) {
			if( server.compare( "localhost" ) == 0 || server.compare( "127.0.0.1" ) == 0 ) {
				return false;
			}
		}
		else {
			std::vector< std::string > tokens;
			stringseparate( e.c_str(), ".", tokens );

			bool flg = true;
			for( unsigned int j = 0; j < tokens.size() && j < domains.size() && flg; j++ ) {
				if( tokens[ j ].compare( "*" ) != 0 && tokens[ j ].compare( domains[ j ] ) != 0 ) {
					flg = false;
				}
			}

			if( flg ) {
				return false;
			}
		}
	}

	return true;
}

// cast wchar
void WinHttpConnection::castWChar( const char* castChar, wchar_t* conveWchar, int nLength  ){
	memset ( conveWchar, 0, nLength * sizeof ( wchar_t ) );

	MultiByteToWideChar ( CP_ACP,
		0,
		castChar,
		-1,
		conveWchar,
		nLength );
}

// cast char
void WinHttpConnection::castChar( wchar_t* castChar, char* conveChar, int nLength ) {
	memset( conveChar, 0, nLength * sizeof( char ) );

	WideCharToMultiByte(
		CP_ACP,
		0,
		castChar,
		-1,
		conveChar,
		nLength,
		NULL,
		NULL
	);
}

// get cast length
int WinHttpConnection::getCastLength( const char* castChar ){
	int nLength;
  
	// char -> wchar_t
	nLength = MultiByteToWideChar ( CP_ACP,
        0,
        castChar,
        -1,
        NULL,
        0 );

	// @Date:2013/04/25	Add	A.Ozaki	>>>>>>
	// add one size for termination.
	//
	if  ( 0 < nLength )
	{
		nLength++;
	}
	// <<<<<< @Date:2013/04/25	Add	A.Ozaki

	return nLength;
}

// get cast length
int WinHttpConnection::getCastLength( wchar_t* castChar ) {
	int nLength;
	nLength =  WideCharToMultiByte( CP_ACP, 0, castChar, -1, NULL, 0, NULL, NULL );

	if( 0 < nLength ) {
		nLength++;
	}

	return nLength;
}

// open url
bool WinHttpConnection::openUrl( const char* url ) {
	// close base connection
	if( m_baseCon != NULL ) {
		delete m_baseCon;
		m_baseCon = NULL;
	}

	// proxy server & exception
	if( m_proxyServer.empty() ) {
		// initialize
		m_proxyServer.clear();
		m_proxyException.clear();

		// IE Settings
		WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ieConfig;
		ZeroMemory( &ieConfig, sizeof( ieConfig ) );

		if( WinHttpGetIEProxyConfigForCurrentUser( &ieConfig ) ) {
			if( ieConfig.lpszProxy == NULL ) {
				if( ieConfig.lpszAutoConfigUrl != NULL ) {    // auto config
					// session
					HINTERNET session = WinHttpOpen(
							NULL,
							WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
							WINHTTP_NO_PROXY_NAME,
							WINHTTP_NO_PROXY_BYPASS,
							WINHTTP_FLAG_ASYNC
					);

					if( session != NULL ) {
						// Auto Proxy
						WINHTTP_AUTOPROXY_OPTIONS autoConfig;
						ZeroMemory( &autoConfig, sizeof( autoConfig ) );
						
						autoConfig.lpszAutoConfigUrl = ieConfig.lpszAutoConfigUrl;
						autoConfig.dwFlags = WINHTTP_AUTOPROXY_CONFIG_URL;
						autoConfig.fAutoLogonIfChallenged = TRUE;
						
						unsigned int urlLength = getCastLength( url );
						wchar_t* pUrl = new wchar_t[ urlLength ];
						castWChar( url, pUrl, urlLength );
						
						WINHTTP_PROXY_INFO proxyInfo;
						ZeroMemory( &proxyInfo, sizeof( proxyInfo ) );
						
						if( WinHttpGetProxyForUrl( session, pUrl, &autoConfig, &proxyInfo ) ) {
							// set proxy info
							if( proxyInfo.lpszProxy != NULL ) {
								int serverLength = getCastLength( proxyInfo.lpszProxy );
								char* proxyServer = new char[ serverLength ];
								castChar( proxyInfo.lpszProxy, proxyServer, serverLength );

								m_proxyServer = trimstring( proxyServer );

								delete[] proxyServer;
							}

							if( proxyInfo.lpszProxyBypass != NULL ) {
								int passLength = getCastLength( proxyInfo.lpszProxyBypass );
								char* proxyException = new char[ passLength ];
								castChar( proxyInfo.lpszProxyBypass, proxyException, passLength );

								m_proxyException = trimstring( proxyException );

								delete[] proxyException;
							}
						}
						else {
							LPVOID lpMsgBuf;
							int error = GetLastError();
							FormatMessage(
								FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								NULL,
								error,
								MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
								(LPTSTR)( &lpMsgBuf ),
								0,
								NULL
							);

							LOG_ERROR( FMT( "Auto Proxy Error (%d): %s", error, NVL( lpMsgBuf, "" ) ) );

							LocalFree( lpMsgBuf );
						}

						delete[] pUrl;
						WinHttpCloseHandle( session );
					}
				}
			}
			else {
				int serverLength = getCastLength( ieConfig.lpszProxy );
				char* proxyServer = new char[ serverLength ];
				castChar( ieConfig.lpszProxy, proxyServer, serverLength );

				m_proxyServer = trimstring( proxyServer );

				delete[] proxyServer;
			}

			if( ieConfig.lpszProxyBypass != NULL ) {
				int passLength = getCastLength( ieConfig.lpszProxyBypass );
				char* proxyException = new char[ passLength ];
				castChar( ieConfig.lpszProxyBypass, proxyException, passLength );

				m_proxyException = trimstring( proxyException );

				delete[] proxyException;
			}
		}
	}

	// connection
	if( isProxy( url ) ) {
		ProxyHttpConnection* con = new ProxyHttpConnection( boost::bind( &WinHttpConnection::showAuthorizationDialog, this, _1, _2 ) );
		int port = 8080;
		std::string server = m_proxyServer;
		size_t pos = server.find( ":" );
		if( pos != server.npos ) {
			port = toint( server.substr( pos + 1 ).c_str(), 10, 8080 );
			server = server.substr( 0, pos );
		}

		con->setProxy( server.c_str(), port, m_proxyUser.c_str(), m_proxyPassword.c_str() );
		con->setPath( url );
		m_baseCon = con;
	}
	else {
		m_baseCon = new HttpConnection();
	}

	const bool ret = m_baseCon->openUrl( url );
	m_opened = ret;

	return ret;
}

// on create recuest
void WinHttpConnection::onCreateRequest( kome::core::DataAccessor& req ) {
}

// on send
void WinHttpConnection::onSend( kome::core::DataAccessor& req, kome::core::DataAccessor& res ) {
}

// send request
void WinHttpConnection::sendRequest() {
	// check the member
	if( m_baseCon == NULL ) {
		return;
	}

	// type
	m_baseCon->setMethod( m_method );

	// write to request
	if( fileexists( m_tmpPath.c_str() ) ) {
		FILE* in = fileopen( m_tmpPath.c_str(), "rb" );
		if( in != NULL ) {
			char buff[ 2048 ];
			int size = 0;
			while( ( size = fread( buff, 1, 2048, in ) ) > 0 ) {
				m_baseCon->writeToRequest( buff, size );
			}
			fclose( in );
		}
	}

	// headers
	m_baseCon->clearHeaders();
	unsigned int len = m_headers.size();
	for( unsigned int i = 0; i < len; i++ ) {
		m_baseCon->setHeader( m_headers[ i ].first.c_str(), m_headers[ i ].second.c_str() );
	}

	// parameters
	m_baseCon->clearParameters();
	len = m_parameters.size();
	for( unsigned int i = 0; i < len; i++ ) {
		m_baseCon->setParameter(
			m_parameters[ i ].name.c_str(),
			m_parameters[ i ].value.c_str(),
			m_parameters[ i ].isFile
		);
	}

	// send
	m_baseCon->sendRequest();

	// response
	FILE* out = fileopen( m_resPath.c_str(), "wb" );
	kome::core::FileAccessor acc( out );

	m_baseCon->getResponse( acc );

	// close
	fflush( out );
	fclose( out );
}
