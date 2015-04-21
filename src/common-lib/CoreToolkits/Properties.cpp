/**
 * @file Properties.cpp
 * @brief implements of Properties class
 *
 * @author S.Tanaka
 * @date 2006.08.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Properties.h"

#include <list>
#include <boost/bind.hpp>


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Properties::Properties() {
}

// destructor
Properties::~Properties() {
}

// set parameter value
void Properties::setValue( const char* key, const char* value ) {
	// create string object
	std::string k = NVL( key, "" );
	std::string v = NVL( value, "" );

	// search
	for( unsigned int i = 0; i < m_props.size(); i++ ) {
		if( k.compare( m_props[ i ].first ) == 0 ) {
			m_props[ i ].second = v;
			return;
		}
	}

	// add
	m_props.push_back( std::make_pair( k, v ) );
}

// set int value
void Properties::setIntValue( const char* key, int value ) {
	setValue( key, FMT( "%d", value ).c_str() );
}

// set double value
void Properties::setDoubleValue( const char* key, double value ) {
	setValue( key, FMT( "%f", value ).c_str() );
}

// set boolean value
void Properties::setBoolValue( const char* key, bool value ) {
	setValue( key, value ? "true" : "false" );
}

// set integer values
void Properties::setIntValues( const char* key, int* values, int size ) {
	// check paramters
	if( values == NULL || size <= 0 ) {
		setValue( key, "" );
		return;
	}

	// create string
	std::string val;
	for( int i = 0; i < size; i++ ) {
		if( !val.empty() ) {
			val.append( "," );
		}
		val.append( FMT( "%d", values[ i ] ) );
	}

	// set value
	setValue( key, val.c_str() );
}

// judges whether this object has specified parameter key
bool Properties::hasKey( const char* key ) {
	// create string object
	std::string s = NVL( key, "" );

	// search
	for( unsigned int i = 0; i < m_props.size(); i++ ) {
		if( s.compare( m_props[ i ].first ) == 0 ) {
			return true;
		}
	}

	return false;
}

// get value
const char* Properties::getStringValue( const char* key, const char* defaultValue ) {
	// string object
	std::string s = NVL( key, "" );

	// search
	for( unsigned int i = 0; i < m_props.size(); i++ ) {
		if( s.compare( m_props[ i ].first ) == 0 ) {
			return m_props[ i ].second.c_str();
		}
	}

	return defaultValue;
}

// get integer value 
int Properties::getIntValue( const char* key, int defaultValue ) {
	return toint( getStringValue( key, "" ), 10, defaultValue );
}

// get double value
double Properties::getDoubleValue( const char* key, double defaultValue ) {
	return todouble( getStringValue( key, "" ), defaultValue );
}

// get bool value
bool Properties::getBoolValue( const char* key, bool defaultValue ) {
	return tobool( getStringValue( key, "" ), defaultValue );
}

// get integer values
void Properties::getIntValues(
		const char* key,
		std::vector<int> &values
) {
	// token
	std::vector<std::string> tokens;
	std::list< std::string > strValues;
	stringtoken(
		NVL( getStringValue( key, "" ), "" ),
		", \t\r\n", tokens
	);

	// parse
	for (unsigned int i = 0; i< tokens.size(); i++) {
		int val = 0;
		if( isint( tokens[i].c_str(), 10, &val ) ) {
			values.push_back(val);
		}
	}
}

// get the number of Properties
unsigned int Properties::getNumberOfProperties() {
	return m_props.size();
}

// get the name of parameter
const char* Properties::getKey( const unsigned int index ) {
	// check parameter
	if( index >= m_props.size() ) {
		return NULL;
	}

	return m_props[ index ].first.c_str();
}

// get the value of parameter
const char* Properties::getValue( const unsigned int index ) {
	// check parameter
	if( index >= m_props.size() ) {
		return NULL;
	}

	return m_props[ index ].second.c_str();
}

// delete parameter
void Properties::deleteParameter( const char* key ) {
	// create string object
	std::string k = std::string( NVL( key, "" ) );

	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_props.size() && idx < 0; i++ ) {
		if( k.compare( m_props[ i ].first ) == 0 ) {
			idx = (int)i;
		}
	}

	// delete
	if( idx >= 0 ) {
		m_props.erase( m_props.begin() + idx );
	}
}

// replace string
std::string Properties::replaceString(
		const char* s,
		const char* prefix,
		const char* suffix,
		const char* defaultString
) {
	// string
	std::string str;

	// check parameters
	if( s == NULL || prefix == NULL || suffix == NULL
			|| *s == '\0' || *prefix == '\0' || *suffix == '\0' ) {
		return std::string( NVL( s, "" ) );
	}

	// copy to buffer
	char* buff = new char[ strlen( s ) + 1 ];
	memcpy( buff, s, strlen( s ) + 1 );

	// replace
	char* c = buff;
	bool keyFlag = false;

	while( c != NULL ) {
		if( keyFlag ) {		// search suffix
			char* cc = strstr( c, suffix );
			if( cc == NULL ) {		// suffix is not found
				str.append( prefix );
				str += std::string( c );
			}
			else {  // suffix is found
				std::string key = std::string( c, cc );
				if( hasKey( key.c_str() ) ) {
					str.append( getStringValue( key.c_str(), "" ) );
				}
				else {
					str.append( NVL( defaultString, "" ) );
				}
			}

			// change flag
			keyFlag = false;

			c = ( cc == NULL ) ? NULL : ( cc + strlen( suffix ) );
		}
		else {		// search prefix
			char* cc = strstr( c, prefix );
			if( cc == NULL ) {		// prefix is not found
				str += std::string( c );
			}
			else {  // prefix is found
				str += std::string( c, cc );
				keyFlag = true;
			}

			c = ( cc == NULL ) ? NULL : ( cc + strlen( prefix ) );
		}
	}

	// delete buff
	delete[] buff;

	return str;
}
