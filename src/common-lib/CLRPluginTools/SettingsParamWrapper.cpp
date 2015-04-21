/**
 * @file SettingsParamWrapper.cpp
 * @brief implements of SettingsParamWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsParamWrapper.h"

#include "SettingsValueWrapper.h"
#include "SettingsGroupWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingsParamWrapper::SettingsParamWrapper( kome::plugin::SettingsParam& param )
		: m_param( param ) {
}

// destructor
SettingsParamWrapper::~SettingsParamWrapper() {
}

// get settings parameter object
kome::plugin::SettingsParam& SettingsParamWrapper::getSettingsParam() {
	return m_param;
}

// set name
void SettingsParamWrapper::setName( System::String^ name ) {
	m_param.setName(
		ClrObjectTool::convertString( name ).c_str()
	);
}

// get name
System::String^ SettingsParamWrapper::getName() {
	return ClrObjectTool::convertString( m_param.getName(), NULL );
}

// set prefix
void SettingsParamWrapper::setPrefix( System::String^ prefix ) {
	m_param.setPrefix(
		ClrObjectTool::convertString( prefix ).c_str()
	);
}

// get prefix
System::String^ SettingsParamWrapper::getPrefix() {
	return ClrObjectTool::convertString( m_param.getPrefix(), NULL );
}

// set value
void SettingsParamWrapper::setValue( SettingsValueWrapper^ value ) {
	m_param.setValue( value == nullptr ? NULL : &value->getSettingsValue() );
}

// get value
SettingsValueWrapper^ SettingsParamWrapper::getValue() {
	kome::plugin::SettingsValue* value = m_param.getValue();
	if( value == NULL ) {
		return nullptr;
	}

	return gcnew SettingsValueWrapper( *value );
}

// set section
void SettingsParamWrapper::setSection( System::String^ section ) {
	m_param.setSection(
		ClrObjectTool::convertString( section ).c_str()
	);
}

// get section
System::String^ SettingsParamWrapper::getSection() {
	return ClrObjectTool::convertString( m_param.getSection(), NULL );
}

// set key
void SettingsParamWrapper::setKey( System::String^ key ) {
	m_param.setKey(
		ClrObjectTool::convertString( key ).c_str()
	);
}

// get key
System::String^ SettingsParamWrapper::getKey() {
	return ClrObjectTool::convertString( m_param.getKey(), NULL );
}

