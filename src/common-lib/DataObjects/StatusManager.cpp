/**
 * @file StatusManager.cpp
 * @brief implements of StatusManager class
 *
 * @author S.Tanaka
 * @date 2007.01.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "StatusManager.h"

using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
StatusManager::StatusManager() {
}

// destructor
StatusManager::~StatusManager() {
}

// set value
void StatusManager::setValue( const char* name, const char* value ) {
	// create string object
	std::string n = NVL( name, "" );
	std::string v = NVL( value, "" );

	// get old value and add name to array
	std::string oldValue;
	bool flg = hasStatus( name );
	if( flg ) {
		oldValue = m_statusMap[ n ];
	}
	else {
		m_statuses.push_back( n );
	}

	// set value to map
	if( value == NULL ) {    // remove
		m_statusMap.erase( n );
		int idx = -1;
		for( int i = 0; i < (int)m_statuses.size() && idx < 0; i++ ) {
			if( m_statuses[ i ].compare( n ) == 0 ) {
				idx = i;
			}
		}
		if( idx >= 0 ) {
			m_statuses.erase( m_statuses.begin() + idx );
		}

		if( flg ) {
			LOG_DEBUG( FMT( "Removing status value [%s=%s]", n.c_str(), oldValue.c_str() ) );
		}
	}
	else {    // update
		m_statusMap[ n ] = v;
		if( v.compare( oldValue ) != 0 ) {
			LOG_DEBUG( FMT( "Setting status value [%s=%s]", n.c_str(), v.c_str() ) );
		}
	}
}

// get value
const char* StatusManager::getValue( const char* name ) {
	// check the map
	if( m_statusMap.find( name ) != m_statusMap.end() ) {
		return m_statusMap[ NVL( name, "" ) ].c_str();
	}

	return NULL;
}

// there is status specified name or not
bool StatusManager::hasStatus( const char* name ) {
	const char* val = getValue( name );

	return ( val != NULL );
}

// get the number of statuses
unsigned int StatusManager::getNumberOfStatuses() {
	return m_statuses.size();
}

// get status variable name
const char* StatusManager::getStatusName( const unsigned int index ) {
	// check the parameter
	if( index >= m_statuses.size() ) {
		return NULL;
	}

	return m_statuses[ index ].c_str();
}

// set status values to ini file object
void StatusManager::setToIniFile( kome::core::IniFile& ini, const char* section ) {
	// set to ini file
	for( unsigned int i = 0; i < m_statuses.size(); i++ ) {
		ini.setString( section, m_statuses[ i ].c_str(), m_statusMap[ m_statuses[ i ] ].c_str() );
	}
}

// get status values from ini file object
void StatusManager::getFromIniFile( kome::core::IniFile& ini, const char* section ) {
	// get from ini file
	for( unsigned int i = 0; i < ini.getNumberOfParameters( section ); i++ ) {
		setValue( ini.getParameterName( section, i ), ini.getParameterValue( section, i ) );
	}
}

// get status value
const char* StatusManager::getStatusValue( const unsigned int index ) {
	// check the parameter
	if( index >= m_statuses.size() ) {
		return NULL;
	}

	return m_statusMap[ m_statuses[ index ] ].c_str();
}

// get instance
StatusManager& StatusManager::getInstance() { 
	// create object (This is the only object)
	static StatusManager mgr;

	return mgr;
}
