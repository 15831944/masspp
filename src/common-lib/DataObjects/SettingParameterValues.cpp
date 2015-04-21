/**
 * @file SettingParameterValues.cpp
 * @brief implements of SettingParameterValues class
 *
 * @author S.Tanaka
 * @date 2008.02.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingParameterValues.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingParameterValues::SettingParameterValues() {
}

// copy constructor
SettingParameterValues::SettingParameterValues( const SettingParameterValues& other ) {
	// copy values
	unsigned int valNum = other.m_values.size();
	if( valNum > 0 ) {
		m_values.reserve( valNum );

		for( unsigned int i = 0; i < valNum; i++ ) {
			m_values.push_back( other.m_values[ i ] );
		}
	}

	// copy subitems
	unsigned int itmNum = other.m_itemValues.size();
	if( itmNum > 0 ) {
		m_itemValues.reserve( itmNum );

		for( unsigned int i = 0; i < itmNum; i++ ) {
			m_itemValues.push_back(
				std::make_pair(	other.m_itemValues[ i ].first, new SettingParameterValues( *( other.m_itemValues[ i ].second ) ))
			);
		}
	}
}

// assignment operator
SettingParameterValues& SettingParameterValues::operator=( const SettingParameterValues& other ) {
	// copy values
	unsigned int valNum = other.m_values.size();
	if( valNum > 0 ) {
		m_values.reserve( valNum );

		for( unsigned int i = 0; i < valNum; i++ ) {
			m_values.push_back( other.m_values[ i ] );
		}
	}

	// copy subitems
	unsigned int itmNum = other.m_itemValues.size();
	if( itmNum > 0 ) {
		m_itemValues.reserve( itmNum );

		for( unsigned int i = 0; i < itmNum; i++ ) {
			m_itemValues.push_back(
				std::make_pair(
					other.m_itemValues[ i ].first,
					new SettingParameterValues( *( other.m_itemValues[ i ].second ) )
				)
			);
		}
	}

	return *this;
}

// destructor
SettingParameterValues::~SettingParameterValues() {
	clear();
}

// clear parameters
void SettingParameterValues::clear() {
	m_values.clear();

	for( unsigned int i = 0; i < m_itemValues.size(); i++ ) {
		delete m_itemValues[ i ].second;
	}
	m_itemValues.clear();
}

// get parameter index
int SettingParameterValues::getIndex( const char* name ) {
	// string object
	std::string s = NVL( name, "" );

	// search
	int idx = -1;
	for( unsigned int i = 0; i < m_values.size() && idx < 0; i++ ) {
		if( s.compare( m_values[ i ].first ) == 0 ) {
			idx = (int)i;
		}
	}

	return idx;
}

// set value
void SettingParameterValues::setValue( const unsigned int index, const char* value ) {
	// check the index
	if( index >= m_values.size() ) {
		return;
	}

	// value
	std::string v = NVL( value, "" );
	if( v.compare( MSPP_SETTINGS_NONE_VALUE ) == 0 ) {
		v = "";
	}

	// set value
	m_values[ index ].second = v;
}

// set value
void SettingParameterValues::setValue( const char* name, const char* value ) {
	// index
	int idx = getIndex( name );

	// value
	std::string v = NVL( value, "" );
	if( v.compare( MSPP_SETTINGS_NONE_VALUE ) == 0 ) {
		v = "";
	}

	// set value
	if( idx < 0 ) {
		m_values.push_back( std::make_pair ( NVL( name, "" ), v ) );
	}
	else {
		setValue( idx, v.c_str() );
	}
}

// get the number of parameters
unsigned int SettingParameterValues::getNumberOfParameters() {
	return m_values.size();
}

// get parameter name
const char* SettingParameterValues::getParameterName( const unsigned int index ) {
	if( index >= m_values.size() ) {
		return NULL;
	}
	return m_values[ index ].first.c_str();
}

// get parameter value
const char* SettingParameterValues::getParameterValue( const unsigned int index ) {
	if( index >= m_values.size() ) {
		return NULL;
	}
	return m_values[ index ].second.c_str();          //ç≈èIìIÇ…ÇÕÇ±ÇÍÇ™getParameterValue( name )Ç©ÇÁï‘ÇÈ
}

// get parameter value
const char* SettingParameterValues::getParameterValue( const char* name ) {
	// index
	int idx = getIndex( name );
	if( idx < 0 ) {
		return NULL;
	}

	// value
	return getParameterValue( idx );
}

// get boolean value
bool SettingParameterValues::getBoolValue( const unsigned int index, const bool df ) {
	// value
	const char* value = getParameterValue( index );
	const bool ret = tobool( value, df );

	return ret;
}

// get boolean value
bool SettingParameterValues::getBoolValue( const char* name, const bool df ) {
	// value
	const char* value = getParameterValue( name );
	const bool ret = tobool( value, df );

	return ret;
}

// get integer value
int SettingParameterValues::getIntValue( const unsigned int index, const int df ) {
	// value
	const char* value = getParameterValue( index );
	const int ret = toint( value, 10, df );

	return ret;
}

// get integer value
int SettingParameterValues::getIntValue( const char* name, const int df ) {
	// value
	const char* value = getParameterValue( name );
	const int ret = toint( value, 10, df );

	return ret;
}

// get double value
double SettingParameterValues::getDoubleValue( const unsigned int index, const double df ) {
	// value
	const char* value = getParameterValue( index );
	const double ret = todouble( value, df );

	return ret;
}

// get double value
double SettingParameterValues::getDoubleValue( const char* name, const double df ) {
	// value
	const char* value = getParameterValue( name );
	const double ret = todouble( value, df );

	return ret;
}

// get sub parameter values object
SettingParameterValues* SettingParameterValues::getSubParameterValues( const char* key, const char* val ) {
	// parameters
	if( val == NULL ) {
		val = getParameterValue( key );
	}

	// key, value
	std::string k = NVL( key, "" );
	if( val == NULL ) {
		val = getParameterValue( key );
	}
	std::string v = NVL( val, "" );

	// search
	int itemIdx = -1;
	for( unsigned int i = 0; i < m_itemValues.size() && itemIdx < 0; i++ ) {
		if( k.compare( m_itemValues[ i ].first.key ) == 0
				&& v.compare( m_itemValues[ i ].first.value ) == 0 ) {
			itemIdx = (int)i;
		}
	}

	// create new sub settings
	if( itemIdx < 0 ) {
		itemIdx = (int)m_itemValues.size();
		SettingParameterValues* subVals = new SettingParameterValues();

		m_itemValues.resize( m_itemValues.size() + 1 );

		m_itemValues.back().first.key = k;
		m_itemValues.back().first.value = v;
		m_itemValues.back().second = subVals;
	}

	return m_itemValues[ itemIdx ].second;
}

// check sub parameters
bool SettingParameterValues::hasSubParameterValues( const char* key, const char* val ) {
	// parameters
	if( val == NULL ) {
		val = getParameterValue( key );
	}

	// key, value
	std::string k = NVL( key, "" );
	if( val == NULL ) {
		val = getParameterValue( key );
	}
	std::string v = NVL( val, "" );

	// search
	int itemIdx = -1;
	for( unsigned int i = 0; i < m_itemValues.size() && itemIdx < 0; i++ ) {
		if( k.compare( m_itemValues[ i ].first.key ) == 0
				&& v.compare( m_itemValues[ i ].first.value ) == 0 ) {
			itemIdx = (int)i;
		}
	}

	return ( itemIdx >= 0 );
}

// write all child parameters
void SettingParameterValues::writeAllChildParameters(
		kome::objects::SettingParameterValues& settings,
		kome::core::Properties& props,
		const char* prefix
) {
	// prefix
	std::string p = NVL( prefix, "" );

	// write
	for( unsigned int i = 0; i < settings.getNumberOfParameters(); i++ ) {
		std::string name = settings.getParameterName( i );
		std::string value = settings.getParameterValue( i );
		std::string propName = name;
		if( !p.empty() ) {
			propName = FMT( "%s : %s", p.c_str(), name.c_str() );
		}

		props.setValue( propName.c_str(), value.c_str() );

		// child
		if( settings.hasSubParameterValues( name.c_str(), value.c_str() ) ) {
			kome::objects::SettingParameterValues* child = settings.getSubParameterValues( name.c_str(), value.c_str() );
			writeAllChildParameters( *child, props, propName.c_str() );
		}
	}
}
