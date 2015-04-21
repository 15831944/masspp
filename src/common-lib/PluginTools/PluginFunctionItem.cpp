/**
 * @file PluginFunctionItem.cpp
 * @brief implements of PluginFunctionItem class
 *
 * @author S.Tanaka
 * @date 2007.08.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginFunctionItem.h"

#include "PluginCall.h"
#include "PluginInfo.h"
#include "ParameterSettings.h"
#include "SettingsPage.h"


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define LONG_NAME_PARAM_NAME				   "long name"
#define SHORT_NAME_PARAM_NAME				   "short name"
#define SETTINGS_PAGE_PARAM_NAME			   "page"
#define EXT_PARAM_NAME                         "ext"
#define DESC_PARAM_NAME                        "desc"


// constructor
PluginFunctionItem::PluginFunctionItem() {
	m_call = NULL;
	m_page = NULL;
}

// destructor
PluginFunctionItem::~PluginFunctionItem() {
}

// set function
void PluginFunctionItem::setCall( PluginCall* call ) {
	m_call = call;
}

// get function
PluginCall* PluginFunctionItem::getCall() {
	return m_call;
}

// set long name
void PluginFunctionItem::setLongName( const char* name ) {
	m_longName = NVL( name, "" );
}

// get long name
const char* PluginFunctionItem::getLongName() {
	return m_longName.c_str();
}

// set short name
void PluginFunctionItem::setShortName( const char* name ) {
	m_shortName = NVL( name, "" );
}

// get short name
const char* PluginFunctionItem::getShortName() {
	return m_shortName.c_str();
}

// set settings page
void PluginFunctionItem::setSettingsPage( SettingsPage* page ) {
	m_page = page;
}

// get settings page
SettingsPage* PluginFunctionItem::getSettingsPage() {
	return m_page;
}

// set description
void PluginFunctionItem::setDescription( const char* desc ) {
	m_desc = NVL( desc, "" );
}

// get description
const char* PluginFunctionItem::getDescription() {
	return m_desc.c_str();
}

// set information
void PluginFunctionItem::setInfo( PluginCall* call ) {
	// set function
	m_call = call;

	// init
	m_longName.c_str();
	m_shortName.c_str();
	m_page = NULL;
	if( m_call == NULL ) {
		return;
	}
	kome::core::Properties& props = call->getProperties();

	// set properties
	m_longName = props.getStringValue( LONG_NAME_PARAM_NAME, "" );
	m_shortName = props.getStringValue( SHORT_NAME_PARAM_NAME, "" );
	m_desc = props.getStringValue( DESC_PARAM_NAME, "" );

	std::string ext = props.getStringValue( EXT_PARAM_NAME, "" );
	if( !ext.empty() ) {
		m_longName = FMT( "%s (*.%s)", m_longName.c_str(), ext.c_str() );
	}

	// set page
	std::string pageName = props.getStringValue( SETTINGS_PAGE_PARAM_NAME, "" );
	PluginInfo* info = call->getPlugin();
	m_page = ( info == NULL || pageName.empty() ) ? NULL : info->getPage( pageName.c_str() );
}
