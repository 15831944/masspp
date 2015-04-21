/**
 * @file SettingParameterValuesWrapper.cpp
 * @brief implements of SettingParameterValuesWrapper class
 *
 * @author S.Tanaka
 * @date 2008.02.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingParameterValuesWrapper.h"

#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingParameterValuesWrapper::SettingParameterValuesWrapper( kome::objects::SettingParameterValues& settings ) {
	m_settings = &settings;
	m_obj = NULL;
}

// constructor
SettingParameterValuesWrapper::SettingParameterValuesWrapper() {
	m_obj = new kome::objects::SettingParameterValues();
	m_settings = m_obj;
}

// destructor
SettingParameterValuesWrapper::~SettingParameterValuesWrapper() {
	if( m_obj != NULL ) {
		delete m_obj;
	}
	m_obj = NULL;
}

// get setting parameter values object
kome::objects::SettingParameterValues& SettingParameterValuesWrapper::getSettingParameterValues() {
	return *m_settings;
}

// clear values
void SettingParameterValuesWrapper::clear() {
	m_settings->clear();
}

// set value
void SettingParameterValuesWrapper::setValue( unsigned int index, System::String^ value ) {
	std::string v = ClrObjectTool::convertString( value );

	m_settings->setValue( index, v.c_str() );
}

// set value
void SettingParameterValuesWrapper::setValue( System::String^ name, System::String^ value ) {
	std::string n = ClrObjectTool::convertString( name );
	std::string v = ClrObjectTool::convertString( value );

	m_settings->setValue( n.c_str(), v.c_str() );
}

// get the number of parameters
unsigned int SettingParameterValuesWrapper::getNumberOfParameters() {
	return m_settings->getNumberOfParameters();
}

// get parameter name
System::String^ SettingParameterValuesWrapper::getParameterName( unsigned int index ) {
	const char* n = m_settings->getParameterName( index );
	System::String^ name = ClrObjectTool::convertString( n, NULL );

	return name;
}

// get parameter value
System::String^ SettingParameterValuesWrapper::getParameterValue( unsigned int index ) {
	const char* v = m_settings->getParameterValue( index );
	System::String^ val = ClrObjectTool::convertString( v, NULL );

	return val;
}

// get parameter value
System::String^ SettingParameterValuesWrapper::getParameterValue( System::String^ name ) {
	std::string n = ClrObjectTool::convertString( name );

	const char* v = m_settings->getParameterValue( n.c_str() );
	System::String^ val = ClrObjectTool::convertString( v, NULL );

	return val;
}

// get boolean value
bool SettingParameterValuesWrapper::getBoolValue( unsigned int index, bool df ) {
	bool ret = m_settings->getBoolValue( index, df );
	return ret;
}

// get boolean value
bool SettingParameterValuesWrapper::getBoolValue( System::String^ name, bool df ) {
	std::string n = ClrObjectTool::convertString( name );
	bool ret = m_settings->getBoolValue( n.c_str(), df );
	return ret;
}

// get integer value
int SettingParameterValuesWrapper::getIntValue( unsigned int index, int df ) {
	int ret = m_settings->getIntValue( index, df );
	return ret;
}

// get integer value
int SettingParameterValuesWrapper::getIntValue( System::String^ name, int df ) {
	std::string n = ClrObjectTool::convertString( name );
	int ret = m_settings->getIntValue( n.c_str(), df );
	return ret;
}

// get double value
double SettingParameterValuesWrapper::getDoubleValue( unsigned int index, double df ) {
	double ret = m_settings->getDoubleValue( index, df );
	return ret;
}

// get double value
double SettingParameterValuesWrapper::getDoubleValue( System::String^ name, double df ) {
	std::string n = ClrObjectTool::convertString( name );
	double ret = m_settings->getDoubleValue( n.c_str(), df );
	return ret;
}

// get sub parameter values object
SettingParameterValuesWrapper^ SettingParameterValuesWrapper::getSubParameterValues( System::String^ key, System::String^ val ) {
	// get values object
	std::string k = ClrObjectTool::convertString( key );
	std::string v = ClrObjectTool::convertString( val );

	kome::objects::SettingParameterValues* vals = m_settings->getSubParameterValues( k.c_str(), ( val == nullptr ? NULL : v.c_str() ) );

	// create wrapper object
	if( vals == NULL ) {
		return nullptr;
	}
	return gcnew SettingParameterValuesWrapper( *vals );
}

// check sub parameter
bool SettingParameterValuesWrapper::hasSubParameterValues( System::String^ key, System::String^ val ) {
	// get values object
	std::string k = ClrObjectTool::convertString( key );
	std::string v = ClrObjectTool::convertString( val );

	return m_settings->hasSubParameterValues( k.c_str(), ( val == nullptr ? NULL : v.c_str() ) );
}
