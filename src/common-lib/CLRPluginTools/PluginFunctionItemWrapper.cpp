/**
 * @file PluginFunctionItemWrapper.cpp
 * @brief implements of PluginFunctionItemWrapper class
 *
 * @author S.Tanaka
 * @date 2007.08.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginFunctionItemWrapper.h"

#include "PluginCallWrapper.h"
#include "SettingsPageWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PluginFunctionItemWrapper::PluginFunctionItemWrapper( kome::plugin::PluginFunctionItem& item )
		: m_item( item ) {
}

// destructor
PluginFunctionItemWrapper::~PluginFunctionItemWrapper() {
}

// get plug-in function item object
kome::plugin::PluginFunctionItem& PluginFunctionItemWrapper::getPluginFunctionItem() {
	return m_item;
}

// set plug-in function
void PluginFunctionItemWrapper::setCall( PluginCallWrapper^ call ) {
	kome::plugin::PluginCall* c = NULL;
	if( call != nullptr ) {
		c = &call->getPluginCall();
	}
	m_item.setCall( c );
}

// get plug-in function
PluginCallWrapper^ PluginFunctionItemWrapper::getCall() {
	kome::plugin::PluginCall* c = m_item.getCall();
	if( c == NULL ) {
		return nullptr;
	}
	return gcnew PluginCallWrapper( *c );
}

// set long name
void PluginFunctionItemWrapper::setLongName( System::String^ name ) {
	m_item.setLongName( ClrObjectTool::convertString( name ).c_str() );
}

// get long name
System::String^ PluginFunctionItemWrapper::getLongName() {
	return ClrObjectTool::convertString( m_item.getLongName(), NULL );
}

// set short name
void PluginFunctionItemWrapper::setShortName( System::String^ name ) {
	m_item.setShortName( ClrObjectTool::convertString( name ).c_str() );
}

// get short name
System::String^ PluginFunctionItemWrapper::getShortName() {
	return ClrObjectTool::convertString( m_item.getShortName(), NULL );
}

// set settings page
void PluginFunctionItemWrapper::setSettingsPage( SettingsPageWrapper^ page ) {
	kome::plugin::SettingsPage* p = NULL;
	if( page != nullptr ) {
		p = &page->getSettingsPage();
	}
	m_item.setSettingsPage( p );
}

// get settings page
SettingsPageWrapper^ PluginFunctionItemWrapper::getSettingsPage() {
	kome::plugin::SettingsPage* p = m_item.getSettingsPage();
	if( p == NULL ) {
		return nullptr;
	}
	return gcnew SettingsPageWrapper( *p );
}

// set description
void PluginFunctionItemWrapper::setDescription( System::String^ desc ) {
	m_item.setDescription( ClrObjectTool::convertString( desc ).c_str() );
}

// get description
System::String^ PluginFunctionItemWrapper::getDescription() {
	return ClrObjectTool::convertString( m_item.getDescription(), NULL );
}

// set item information
void PluginFunctionItemWrapper::setInfo( PluginCallWrapper^ call ) {
	kome::plugin::PluginCall* c = NULL;
	if( call != nullptr ) {
		c = &call->getPluginCall();
	}
	m_item.setInfo( c );
}
