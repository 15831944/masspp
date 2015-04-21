/**
 * @file WsdlHandler.cpp
 * @brief interfaces of WsdlHandler class
 *
 * @author S.Tanaka
 * @date 2009.08.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "WsdlHandler.h"

#include "Wsdl.h"
#include "WsdlPortType.h"
#include "WsdlBinding.h"
#include "WsdlOperation.h"


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PORT_TYPE_TAG_NAME	   "portType"
#define BINDING_TAG_NAME		 "binding"
#define OPERATION_TAG_NAME	   "operation"
#define PORT_TAG_NAME			"port"
#define ADDRESS_TAG_NAME		 "address"

#define PORT_TYPE_NAME_ATTR	  "name"
#define BINDING_NAME_ATTR		"name"
#define BINDING_TYPE_ATTR		"type"
#define OPERATION_NAME_ATTR	  "name"
#define OPERATION_ACTION_ATTR	"soapAction"
#define PORT_BINDING_ATTR		"binding"
#define ADDR_LOC_ATTR			"location"



// constructor
WsdlHandler::WsdlHandler( Wsdl& wsdl ) : m_wsdl( wsdl ) {
	onStartDocument();
}

// destructor
WsdlHandler::~WsdlHandler() {
}

// on start document
void WsdlHandler::onStartDocument() {
	// initialize
	m_wsdl.clear();

	m_currType = NULL;
	m_currBinding = NULL;
	m_currOperation = NULL;
	m_topOp = true;
	m_topBin = true;
}

// on end document
void WsdlHandler::onEndDocument() {
}

// on start element
void WsdlHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tags
	if( strcmp( name, PORT_TYPE_TAG_NAME ) == 0 ) {	// <portType>
		std::string n = attrs.getStringValue( PORT_TYPE_NAME_ATTR, "" );
		m_currType = m_wsdl.addPortType( n.c_str() );
	}

	else if( strcmp( name, BINDING_TAG_NAME ) == 0 ) {	// <binding>
		const char* n = attrs.getStringValue( BINDING_NAME_ATTR, NULL );
		std::string t = attrs.getStringValue( BINDING_TYPE_ATTR, "" );

		m_topBin = ( n != NULL );

		if( n != NULL ) {
			size_t pos = 0;
			while( ( pos = t.find( ":" ) ) != t.npos ) {
				t = t.substr( pos + 1 );
			}

			m_currBinding = m_wsdl.addBinding( n );
			m_currType = m_wsdl.addPortType( t.c_str() );

			m_currType->addBinding( m_currBinding );
		}
	}
	else if( strcmp( name, OPERATION_TAG_NAME ) == 0 ) {	// <operation>
		const char* n = attrs.getStringValue( OPERATION_NAME_ATTR, NULL );
		m_topOp = ( n != NULL );

		if( n == NULL ) {	// set action
			if( m_currOperation != NULL && m_currBinding != NULL ) {
				m_currBinding->addOperation(
					m_currOperation,
					attrs.getStringValue( OPERATION_ACTION_ATTR, "" )
				);
			}
		}
		else {	// set operation
			if( m_currType != NULL ) {
				m_currOperation = m_currType->addOperation( n );
			}
		}
	}
	else if( strcmp( name, PORT_TAG_NAME ) == 0 ) {	// <port>
		// binding
		std::string bindingName = attrs.getStringValue( PORT_BINDING_ATTR, "" );
		size_t pos = 0;
		while( ( pos = bindingName.find( ":" ) ) != bindingName.npos ) {
			bindingName = bindingName.substr( pos + 1 );
		}

		m_currBinding = m_wsdl.addBinding( bindingName.c_str() );
	}
	else if( strcmp( name, ADDRESS_TAG_NAME ) == 0 ) {	// <address>
		if( m_currBinding != NULL ) {	// set end point
			m_currBinding->setEndPoint( attrs.getStringValue( ADDR_LOC_ATTR, NULL ) );
		}			
	}
}

// on end element
void WsdlHandler::onEndElement( const char* name, const char* text ) {
	// each tags
	if( strcmp( name, PORT_TYPE_TAG_NAME ) == 0 ) {	// </portType>
		m_currType = NULL;
		m_currOperation = NULL;
		m_currBinding = NULL;
	}
	else if( strcmp( name, BINDING_TAG_NAME ) == 0 ) {	// </binding>
		if( m_topBin ) {
			m_currType = NULL;
			m_currOperation = NULL;
			m_currBinding = NULL;
		}
		m_topBin = true;
	}
	else if( strcmp( name, OPERATION_TAG_NAME ) == 0 ) {	// </operation>
		if( m_topOp ) {
			m_currOperation = NULL;
		}
		m_topOp = true;
	}
	else if( strcmp( name, PORT_TAG_NAME ) == 0 ) {	// <port>
		m_currBinding = NULL;
	}
}
