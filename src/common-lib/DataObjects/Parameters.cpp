/**
 * @file Parameters.cpp
 * @brief implements of Parameters class
 *
 * @author S.Tanaka
 * @date 2006.08.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "Parameters.h"

using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
Parameters::Parameters() {
}

// destructor
Parameters::~Parameters() {
}

// get the number of parameters
unsigned int Parameters::getNumberOfParameters() {
	return m_parameters.size();
}

// get parameter name
const char* Parameters::getName( unsigned int index ) {
	// check index
	if( index >= m_parameters.size() ) {
		return NULL;
	}

	return m_parameters[index].c_str();
}

// get parameter value
Variant& Parameters::getValue( const char* name ) {
	// creating string object
	std::string s( NVL( name, "" ) );

	// check the map
	if( m_parameterMap.find( s ) == m_parameterMap.end() ) {	// new parameter
		m_parameters.push_back( s );
	}

	// return value
	return m_parameterMap[ s ];
}

// judge whether there is the parameter with the specified name.
bool Parameters::hasParameter( const char* name ) {
	// creating string object
	std::string s( NVL( name, "" ) );

	// check the map
	return ( m_parameterMap.find( s ) != m_parameterMap.end() );
}
