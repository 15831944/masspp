/**
 * @file ParameterSettingsWrapper.cpp
 * @brief implements of ParameterSettingsWrapper class
 *
 * @author S.Tanaka
 * @date 2007.01.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ParameterSettingsWrapper.h"

#include "PluginInfoWrapper.h"
#include "SettingsValueWrapper.h"
#include "SettingsParamWrapper.h"
#include "SettingsGroupWrapper.h"
#include "SettingsPageWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ParameterSettingsWrapper::ParameterSettingsWrapper( kome::plugin::ParameterSettings& settings ) {
	m_settings = &settings;
	m_delFlg = false;
}

// constructor
ParameterSettingsWrapper::ParameterSettingsWrapper( bool isIni ) {
	m_settings = new kome::plugin::ParameterSettings( NULL, isIni );
	m_delFlg = true;
}

// destructor
ParameterSettingsWrapper::~ParameterSettingsWrapper() {
	if( m_delFlg ) {
		delete m_settings;
	}
	m_settings = NULL;
}

// get parameter settings object
kome::plugin::ParameterSettings& ParameterSettingsWrapper::getParameterSettings() {
	return *m_settings;
}

// get plug-in
PluginInfoWrapper^ ParameterSettingsWrapper::getPlugin() {
	kome::plugin::PluginInfo* plugin = m_settings->getPlugin();
	if( plugin == NULL ) {
		return nullptr;
	}
	return gcnew PluginInfoWrapper( *plugin );
}

// get ini file flag value
bool ParameterSettingsWrapper::isIni() {
	return m_settings->isIni();
}

// load settings file
bool ParameterSettingsWrapper::loadSettingsFile( System::String^ path ) {
	return m_settings->loadSettingsFile(
		ClrObjectTool::convertString( path ).c_str() 
	);
}

// get settings file path
System::String^ ParameterSettingsWrapper::getSettingsFilePath() {
	return ClrObjectTool::convertString( m_settings->getSettingsFilePath(), NULL );
}

// add settings value
SettingsValueWrapper^ ParameterSettingsWrapper::addValue(){
	kome::plugin::SettingsValue* value = m_settings->addValue();
	if( value == NULL ) {
		return nullptr;
	}

	return gcnew SettingsValueWrapper( *value );
}

// get the number of settings values
unsigned int ParameterSettingsWrapper::getNumberOfValues() {
	return m_settings->getNumberOfValues();
}

// get settings value
SettingsValueWrapper^ ParameterSettingsWrapper::getValue( unsigned int index ) {
	kome::plugin::SettingsValue* value = m_settings->getValue( index );
	if( value == NULL ) {
		return nullptr;
	}

	return gcnew SettingsValueWrapper( *value );
}

// add parameter
SettingsParamWrapper^ ParameterSettingsWrapper::addParam() {
	kome::plugin::SettingsParam* param = m_settings->addParam();
	if( param == NULL ) {
		return nullptr;
	}

	return gcnew SettingsParamWrapper( *param );
}

// get the number of parameters
unsigned int ParameterSettingsWrapper::getNumberOfParams() {
	return m_settings->getNumberOfParams();
}

// get parameter
SettingsParamWrapper^ ParameterSettingsWrapper::getParam( unsigned int index ) {
	kome::plugin::SettingsParam* param = m_settings->getParam( index );
	if( param == NULL ) {
		return nullptr;
	}

	return gcnew SettingsParamWrapper( *param );
}

// add settings group
SettingsGroupWrapper^ ParameterSettingsWrapper::addGroup(){
	kome::plugin::SettingsGroup* group = m_settings->addGroup();
	if( group == NULL ) {
		return nullptr;
	}

	return gcnew SettingsGroupWrapper( *group );
}

// get the number of settings groups
unsigned int ParameterSettingsWrapper::getNumberOfGroups() {
	return m_settings->getNumberOfGroups();
}

// get settings group
SettingsGroupWrapper^ ParameterSettingsWrapper::getGroup( unsigned int index ) {
	kome::plugin::SettingsGroup* group = m_settings->getGroup( index );
	if( group == NULL ) {
		return nullptr;
	}

	return gcnew SettingsGroupWrapper( *group );
}

// add settings page
void ParameterSettingsWrapper::addPage( SettingsPageWrapper^ page ) {
	// get page object
	kome::plugin::SettingsPage* p = NULL;
	if( page != nullptr ) {
		p = &page->getSettingsPage();
	}

	// add
	m_settings->addPage( p );
}

// get the number of setting pages
unsigned int ParameterSettingsWrapper::getNumberOfPages() {
	return m_settings->getNumberOfPages();
}

// get settings page
SettingsPageWrapper^ ParameterSettingsWrapper::getPage( unsigned int index ) {
	// get page
	kome::plugin::SettingsPage* page = m_settings->getPage( index );
	if( page == NULL ) {
		return nullptr;
	}

	return gcnew SettingsPageWrapper( *page );
}

// get settings page
SettingsPageWrapper^ ParameterSettingsWrapper::getPage( System::String^ name ) {
	// get page
	kome::plugin::SettingsPage* page = m_settings->getPage( ClrObjectTool::convertString( name ).c_str() );
	if( page == NULL ) {
		return nullptr;
	}

	return gcnew SettingsPageWrapper( *page );

}
