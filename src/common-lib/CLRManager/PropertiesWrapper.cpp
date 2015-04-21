/**
 * @file PropertiesWrapper.cpp
 * @brief interfaces of PropertiesWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "PropertiesWrapper.h"

#include "ClrObjectTool.h"

#include <list>


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PropertiesWrapper::PropertiesWrapper()
		: m_props( *( new kome::core::Properties() ) ) {
	m_deleteFlag = true;
}

// constructor
PropertiesWrapper::PropertiesWrapper( kome::core::Properties& props )
		: m_props( props ) {
	m_deleteFlag = false;
}

// destructor
PropertiesWrapper::~PropertiesWrapper() {
	if( m_deleteFlag ) {
		delete &m_props;
	}
}

// get properties object
kome::core::Properties& PropertiesWrapper::getProperties() {
	return m_props;
}

// set string value
void PropertiesWrapper::setValue( System::String^ key, System::String^ value ) {
	m_props.setValue(
		ClrObjectTool::convertString( key ).c_str(),
		ClrObjectTool::convertString( value ).c_str()
	);
}

// set int value
void PropertiesWrapper::setIntValue( System::String^ key, int value ) {
	m_props.setIntValue(
		ClrObjectTool::convertString( key ).c_str(),
		value
	);
}

// set double value
void PropertiesWrapper::setDoubleValue( System::String^ key, double value ) {
	m_props.setDoubleValue(
		ClrObjectTool::convertString( key ).c_str(),
		value
	);
}

// set bool value
void PropertiesWrapper::setBoolValue( System::String^ key, bool value ) {
	m_props.setBoolValue(
		ClrObjectTool::convertString( key ).c_str(),
		value
	);
}

// set int values
void PropertiesWrapper::setIntValues( System::String^ key, cli::array< int >^ values ) {
	// create array
	int* a = NULL;
	int size = 0;
	if( values != nullptr && values->Length > 0 ) {
		size = values->Length;
		a = new int[ size ];

		for( int i = 0; i < size; i++ ) {
			a[ i ] = values[ i ];
		}
	}

	// set values
	m_props.setIntValues(
		ClrObjectTool::convertString( key ).c_str(),
		a,
		size
	);

	// delete
	if( a != NULL ) {
		delete[] a;
	}
}
			
// judge whether there is specified parameter
bool PropertiesWrapper::hasKey( System::String^ key ) {
	return m_props.hasKey( ClrObjectTool::convertString( key ).c_str() );
}

// get string value
System::String^ PropertiesWrapper::getStringValue( System::String^ key, System::String^ defaultValue ) {
	const char* value
		= m_props.getStringValue(
				ClrObjectTool::convertString( key ).c_str(),
				ClrObjectTool::convertString( defaultValue ).c_str()
		);

	return ClrObjectTool::convertString( value, NULL );
}

// get int value
int PropertiesWrapper::getIntValue( System::String^ key, int defaultValue ) {
	return m_props.getIntValue(
			ClrObjectTool::convertString( key ).c_str(),
			defaultValue
		);
}

// get double value
double PropertiesWrapper::getDoubleValue( System::String^ key, double defaultValue ) {
	return m_props.getDoubleValue(
			ClrObjectTool::convertString( key ).c_str(),
			defaultValue
		);
}

// get bool value
bool PropertiesWrapper::getBoolValue( System::String^ key, bool defaultValue ) {
	return m_props.getBoolValue(
			ClrObjectTool::convertString( key ).c_str(),
			defaultValue
		);
}

// get integer values
void PropertiesWrapper::getIntValues(
		System::String^ key,
		System::Collections::IList^ list
) {
	// get values
	std::vector<int> values;
	m_props.getIntValues(ClrObjectTool::convertString( key ).c_str(), values); // valueList);

	// add to list
	for( unsigned int i = 0; i < values.size(); i++) {
		list->Add(values[i]);
	}
}

// get number of properties
unsigned int PropertiesWrapper::getNumberOfProperties() {
	return m_props.getNumberOfProperties();
}

// get key
System::String^ PropertiesWrapper::getKey( const unsigned int index ) {
	const char* key = m_props.getKey( index );

	return ClrObjectTool::convertString( key, NULL );
}

// get value
System::String^ PropertiesWrapper::getValue( const unsigned int index ) {
	const char* value = m_props.getValue( index );

	return ClrObjectTool::convertString( value, NULL );
}

// delete parameter
void PropertiesWrapper::deleteParameter( System::String^ key ) {
	m_props.deleteParameter( ClrObjectTool::convertString( key ).c_str() );
}

// replace string
System::String^ PropertiesWrapper::replaceString(
		System::String^ s,
		System::String^ prefix,
		System::String^ suffix,
		System::String^ defaultString
) {
	std::string str
		= m_props.replaceString(
			ClrObjectTool::convertString( s ).c_str(),
			ClrObjectTool::convertString( prefix ).c_str(),
			ClrObjectTool::convertString( suffix ).c_str(),
			ClrObjectTool::convertString( defaultString ).c_str()
		);

	return gcnew System::String( str.c_str() );
}
