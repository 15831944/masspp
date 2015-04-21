/**
 * @file WsdlPortType.cpp
 * @brief implements of WsdlPortType class
 *
 * @author S.Tanaka
 * @date 2009.08.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "WsdlPortType.h"

#include "WsdlBinding.h"
#include "WsdlOperation.h"


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
WsdlPortType::WsdlPortType( const char* name ) {
	m_name = NVL( name, "" );
}

// destructor
WsdlPortType::~WsdlPortType() {
	clear();
}

// get name
const char* WsdlPortType::getName() {
	return m_name.c_str();
}

// clear port type
void WsdlPortType::clear() {
	for( unsigned int i = 0 ; i < m_operations.size(); i++ ) {
		delete m_operations[ i ];
	}
	m_operations.clear();
}

// add operation
WsdlOperation* WsdlPortType::addOperation( const char* name ) {
	// search
	WsdlOperation* op = searchOperation( name );

	// add operation
	if( op == NULL ) {
		op = new WsdlOperation( name );
		m_operations.push_back( op );
	}

	return op;
}

// search operation
WsdlOperation* WsdlPortType::searchOperation( const char* name ) {
	// search
	WsdlOperation* op = NULL;
	if( name == NULL ) {
		return op;
	}

	for( unsigned int i = 0; i < m_operations.size() && op == NULL; i++ ) {
		WsdlOperation* tmp = m_operations[ i ];
		if( strcmp( name, tmp->getName() ) == 0 ) {
			op = tmp;
		}
	}

	return op;
}

// get the number of operations
unsigned int WsdlPortType::getNumberOfOperations() {
	return m_operations.size();
}

// get operation
WsdlOperation* WsdlPortType::getOperation( const unsigned int idx ) {
	if( idx >= m_operations.size() ) {
		return NULL;
	}
	return m_operations[ idx ];
}

// add binding
void WsdlPortType::addBinding( WsdlBinding* binding ) {
	if( binding != NULL ) {
		m_bindings.push_back( binding );
	}
}

// get the number of bindings
unsigned int WsdlPortType::getNumberOfBindings() {
	return m_bindings.size();
}

// get binding
WsdlBinding* WsdlPortType::getBinding( const unsigned int idx ) {
	if( idx >= m_bindings.size() ) {
		return NULL;
	}
	return m_bindings[ idx ];
}
