/**
 * @file WsdlBinding.cpp
 * @brief implements of WsdlBinding class
 *
 * @author S.Tanaka
 * @date 2009.08.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "WsdlBinding.h"

#include "WsdlOperation.h"


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
WsdlBinding::WsdlBinding( const char* name ) {
	m_name = NVL( name, "" );
}

// destructor
WsdlBinding::~WsdlBinding() {
}

// get binding name
const char* WsdlBinding::getName() {
	return m_name.c_str();
}

// set end point
void WsdlBinding::setEndPoint( const char* endPoint ) {
	m_endPoint = NVL( endPoint, "" );
}

// get end point
const char* WsdlBinding::getEndPoint() {
	return m_endPoint.c_str();
}

// adds operation
void WsdlBinding::addOperation( WsdlOperation* operation, const char* action ) {
	// check the parameter
	if( operation == NULL ) {
		return;
	}

	// search
	int idx = searchOperation( operation->getName() );
	if( idx < 0 ) {	// new
		m_operations.push_back( std::make_pair( operation, NVL( action, "" ) ) );
	}
	else {
		m_operations[ idx ].first = operation;
		m_operations[ idx ].second = NVL( action, "" );
	}
}

// get the number of operations
unsigned int WsdlBinding::getNumberOfOperations() {
	return m_operations.size();
}

// search operation
int WsdlBinding::searchOperation( const char* name ) {
	int idx = -1;
	for( unsigned int i = 0; i < m_operations.size() && idx < 0; i++ ) {
		if( strcmp( name, ( m_operations[ i ].first )->getName() ) == 0 ) {
			idx = (int)i;
		}
	}

	return idx;
}

// get operation
WsdlOperation* WsdlBinding::getOperation( const unsigned int idx ) {
	if( idx >= m_operations.size() ) {
		return NULL;
	}
	return m_operations[ idx ].first;
}

// get action name
const char* WsdlBinding::getAction( const unsigned int idx ) {
	if( idx >= m_operations.size() ) {
		return NULL;
	}
	return m_operations[ idx ].second.c_str();
}
