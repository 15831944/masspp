/**
 * @file Wsdl.cpp
 * @brief implements of Wsdl class
 *
 * @author S.Tanaka
 * @date 2009.08.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Wsdl.h"

#include "WsdlPortType.h"
#include "WsdlOperation.h"
#include "WsdlBinding.h"
#include "WsdlHandler.h"

#include <xercesc/framework/MemBufInputSource.hpp>


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Wsdl::Wsdl() {
}

// destructor
Wsdl::~Wsdl() {
	clear();
}

// clear
void Wsdl::clear() {
	// clear port types
	for( unsigned int i = 0; i < m_portTypes.size(); i++ ) {
		delete m_portTypes[ i ];
	}
	m_portTypes.clear();

	// clear bindings
	for( unsigned int i = 0; i < m_bindings.size(); i++ ) {
		delete m_bindings[ i ];
	}
	m_bindings.clear();
}

// add port type
WsdlPortType* Wsdl::addPortType( const char* name ) {
	// search port type
	WsdlPortType* port = searchPortType( name );

	// create port type
	if( port == NULL ) {
		port = new WsdlPortType( name );
		m_portTypes.push_back( port );
	}

	return port;
}

// searche port type
WsdlPortType* Wsdl::searchPortType( const char* name ) {
	// search
	WsdlPortType* port = NULL;
	if( name == NULL ) {
		return port;
	}

	for( unsigned int i = 0; i < m_portTypes.size() && port == NULL; i++ ) {
		WsdlPortType* tmp = m_portTypes[ i ];
		if( strcmp( name, tmp->getName() ) == 0 ) {
			port = tmp;
		}
	}

	return port;
}

// get the number of port types
unsigned int Wsdl::getNumberOfPortTypes() {
	return m_portTypes.size();
}

// get port type
WsdlPortType* Wsdl::getPortType( const unsigned int idx ) {
	if( idx >= m_portTypes.size() ) {
		return NULL;
	}
	return m_portTypes[ idx ];
}

// add binding
WsdlBinding* Wsdl::addBinding( const char* name ) {
	// search binding
	WsdlBinding* binding = searchBinding( name );

	// create binding
	if( binding == NULL ) {
		binding = new WsdlBinding( name );
		m_bindings.push_back( binding );
	}

	return binding;
}

// search binding
WsdlBinding* Wsdl::searchBinding( const char* name ) {
	// search
	WsdlBinding* binding = NULL;
	if( name == NULL ) {
		return binding;
	}

	for( unsigned int i = 0; i < m_bindings.size() && binding == NULL; i++ ) {
		WsdlBinding* tmp = m_bindings[ i ];
		if( strcmp( name, tmp->getName() ) == 0 ) {
			binding = tmp;
		}
	}

	return binding;
}

// get the number of bindings
unsigned int Wsdl::getNumberOfBindings() {
	return m_bindings.size();
}

// get binding
WsdlBinding* Wsdl::getBinding( const unsigned int idx ) {
	if( idx >= m_bindings.size() ) {
		return NULL;
	}
	return m_bindings[ idx ];
}

// search method
WsdlBinding* Wsdl::searchMethod( const char* method ) {
	// check the parameter
	if( method == NULL ) {
		return NULL;
	}

	// search
	WsdlBinding* binding = NULL;
	for( unsigned int i = 0; i < m_bindings.size() && binding == NULL; i++ ) {
		WsdlBinding* tmp = m_bindings[ i ];
		if( tmp->searchOperation( method ) >= 0 ) {
			binding = tmp;
		}
	}

	return binding;
}

// read WSDL
bool Wsdl::readWsdl( kome::core::DataAccessor& acc ) {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// initialize
	clear();

	// get response
	std::vector< char > wsdl;
	getSoapResponse( acc, wsdl );

	if( wsdl.size() == 0 ) {
		LOG_ERROR( FMT( "No response from server." ) );
		return false;
	}

	// save to temporary file
	const char* dirName = "wsdl";
	std::string tmpDir = getpath( msppMgr.getTmpDir(), dirName );
	if( !fileexists( tmpDir.c_str() ) ) {
		makedirectory( tmpDir.c_str() );
	}

	std::string fileName = msppMgr.getTmpFileName( "wsdl", ".xml", dirName );
	std::string tmpFile = getpath( tmpDir.c_str(), fileName.c_str() );

	FILE* fp = fileopen( tmpFile.c_str(), "wb" );
	fwrite( &( wsdl[ 0 ] ), 1, wsdl.size(), fp );
	fflush( fp );
	fclose( fp );

	// handler
	WsdlHandler handler( *this );
	handler.parse( tmpFile.c_str() );

	// remove file
	removefile( tmpFile.c_str() );

	return !( handler.isError() );
}

// get soap response
void Wsdl::getSoapResponse( kome::core::DataAccessor& acc, std::vector< char >& res ) {
	// read
	char buff[ 1024 ];
	bool inXML = false;
	int blank = 0;

	int size = 0;

	while( ( size = acc.read( buff, 1024 ) ) > 0 ) {
		for( int i = 0; i < size; i++ ) {
			// character
			char c = buff[ i ];

			// in XML
			if( !inXML && c == '<' ) {
				inXML = true;
			}

			// XML
			if( inXML ) {
				// blank
				if( c == '\n' ) {
					if( blank == 1 ) {
						blank = 2;
					}
					else if( blank == 3 ) {
						blank = 4;
					}
				}
				else {
					if( blank == 1 ) {
						blank = 0;
					}
					else if( blank == 3 ) {
						blank = 2;
					}
					else if( blank == 4 ) {
						blank = 0;
					}
				}

				if( c == '\r' ) {
					if( blank == 0 ) {
						blank = 1;
					}
					else if( blank == 2 ) {
						blank = 3;
					}
				}

				// add
				if( blank == 0 ) {
					res.push_back( c );
				}
			}
		}
	}
}
