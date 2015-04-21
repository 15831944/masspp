/**
 * @file SettingsParam.cpp
 * @brief implements of SettingsParam class
 *
 * @author S.Tanaka
 * @date 2007.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsParam.h"

using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingsParam::SettingsParam() {
	m_value = NULL;
}

// destructor
SettingsParam::~SettingsParam() {
}

// set parameter name
void SettingsParam::setName( const char* name ) {
	m_name = NVL( name, "" );
}

// get parameter name
const char* SettingsParam::getName() {
	return m_name.c_str();
}

// set prefix
void SettingsParam::setPrefix( const char* prefix ) {
	m_prefix = NVL( prefix, "" );
}

// get prefix
const char* SettingsParam::getPrefix() {
	return m_prefix.c_str();
}

// set settings value
void SettingsParam::setValue( SettingsValue* value ) {
	m_value = value;
}

// get settings value
SettingsValue* SettingsParam::getValue() {
	return m_value;
}

// set parameter section
void SettingsParam::setSection( const char* section ) {
	m_section = std::string( NVL( section, "" ) );
}

// get parameter section
const char* SettingsParam::getSection() {
	return m_section.c_str();
}

// set parameter key
void SettingsParam::setKey( const char* key ) {
	m_key = std::string( NVL( key, "" ) );
}

// get paramter key
const char* SettingsParam::getKey() {
	return m_key.c_str();
}
