/**
 * @file ParameterSettings.cpp
 * @brief implements of ParameterSettings class
 *
 * @author S.Tanaka
 * @date 2007.01.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ParameterSettings.h"

#include "PluginInfo.h"
#include "SettingsValue.h"
#include "SettingsParam.h"
#include "SettingsGroup.h"
#include "SettingsPage.h"
#include "PluginManager.h"


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ParameterSettings::ParameterSettings( PluginInfo* plugin, const bool isIni )
		: m_isIni( isIni ) {
	m_plugin = plugin;
}

// destructor
ParameterSettings::~ParameterSettings() {
	// delete objects
	for( unsigned int i = 0; i < m_values.size(); i++ ) {
		delete m_values[ i ];
	}
	m_values.clear();

	for( unsigned int i = 0; i < m_params.size(); i++ ) {
		delete m_params[ i ];
	}
	m_params.clear();

	for( unsigned int i = 0; i < m_groups.size(); i++ ) {
		delete m_groups[ i ];
	}
	m_groups.clear();

	for( unsigned int i = 0; i < m_pages.size(); i++ ) {
		delete m_pages[ i ];
	}
	m_pages.clear();
}

// gets plug-in information
PluginInfo* ParameterSettings::getPlugin() {
	return m_plugin;
}

// get ini file flag
bool ParameterSettings::isIni() {
	return m_isIni;
}

// bool load settings file path
bool ParameterSettings::loadSettingsFile( const char* path ) {
	// set path
	if( m_plugin != NULL ) {
		m_settingsFilePath = getpath( m_plugin->getDir(), path );
	}
	else m_settingsFilePath = NVL( path, "" );

	// load
	return PluginManager::getInstance().loadSettingsFile( m_settingsFilePath.c_str(), *this );
}

// get settings file path
const char* ParameterSettings::getSettingsFilePath() {
	return m_settingsFilePath.c_str();
}

// add settings value
SettingsValue* ParameterSettings::addValue() {
	SettingsValue* value = new SettingsValue();
	m_values.push_back( value );

	return value;
}

// get the number of settings values
unsigned int ParameterSettings::getNumberOfValues() {
	return m_values.size();
}

// get settings value
SettingsValue* ParameterSettings::getValue( const unsigned int index ) {
	// check the index 
	if( index >= m_values.size() ) {
		return NULL;
	}

	return m_values[ index ];
}

// add settings prameter
SettingsParam* ParameterSettings::addParam() {
	SettingsParam* param = new SettingsParam();
	m_params.push_back( param );
   
	return param;
}

// get the number of settings params
unsigned int ParameterSettings::getNumberOfParams() {
	return m_params.size();
}

// get settings parameter
SettingsParam* ParameterSettings::getParam( const unsigned int index ) {
	// check the index
	if( index >= m_params.size() ) {
		return NULL;
	}

	return m_params[ index ];
}

// add settings group
SettingsGroup* ParameterSettings::addGroup() {
	SettingsGroup* group = new SettingsGroup();	
	m_groups.push_back( group );

	return group;
}

// get the number of settings groups
unsigned int ParameterSettings::getNumberOfGroups() {
	return m_groups.size();
}

// get settings group
SettingsGroup* ParameterSettings::getGroup( const unsigned int index ) {
	// check the index
	if( index >= m_groups.size() ) {
		return NULL;
	}

	return m_groups[ index ];
}

// add page
void ParameterSettings::addPage( SettingsPage* page ) {
	if( page != NULL ) {
		m_pages.push_back( page );
	}
}

// get the number of pages
unsigned int ParameterSettings::getNumberOfPages() {
	return m_pages.size();
}

// get page
SettingsPage* ParameterSettings::getPage( const unsigned int index ) {
	if( index >= m_pages.size() ) {
		return NULL;
	}
	return m_pages[ index ];
}

// get page
SettingsPage* ParameterSettings::getPage( const char* name ) {
	// check the name
	if( name == NULL ) {
		return NULL;
	}

	// search
	SettingsPage* page = NULL;
	for( unsigned int i = 0; i < m_pages.size() && page == NULL; i++ ) {
		SettingsPage* p = m_pages[ i ];
		if( strcmp( p->getName(), name ) == 0 ) {
			page = p;
		}
	}

	return page;
}
