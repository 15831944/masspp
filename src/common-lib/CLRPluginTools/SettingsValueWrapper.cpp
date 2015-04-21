/**
 * @file SettingsValueWrapper.cpp
 * @brief implements of SettingsValueWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsValueWrapper.h"

#include "PluginCallWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



kome::core::ConversionElement< kome::plugin::SettingsValue::ValueType, SettingsValueWrapper::ValueType > g_settingsValueTypes[] = {
	{ kome::plugin::SettingsValue::TYPE_UNKNOWN,           SettingsValueWrapper::ValueType::UNKNOWN },
	{ kome::plugin::SettingsValue::TYPE_BOOL,		          SettingsValueWrapper::ValueType::BOOL },
	{ kome::plugin::SettingsValue::TYPE_INT,		           SettingsValueWrapper::ValueType::INT },
	{ kome::plugin::SettingsValue::TYPE_DOUBLE,	            SettingsValueWrapper::ValueType::DOUBLE },
	{ kome::plugin::SettingsValue::TYPE_STRING,	            SettingsValueWrapper::ValueType::STRING },
	{ kome::plugin::SettingsValue::TYPE_INPUT,               SettingsValueWrapper::ValueType::INPUT },
	{ kome::plugin::SettingsValue::TYPE_OUTPUT,	            SettingsValueWrapper::ValueType::OUTPUT },
	{ kome::plugin::SettingsValue::TYPE_RAWDATA,           SettingsValueWrapper::ValueType::RAWDATA },
	{ kome::plugin::SettingsValue::TYPE_DIR,                   SettingsValueWrapper::ValueType::DIR },
	{ kome::plugin::SettingsValue::TYPE_COLOR,               SettingsValueWrapper::ValueType::COLOR },
	{ kome::plugin::SettingsValue::TYPE_FONT,                 SettingsValueWrapper::ValueType::FONT },
	{ kome::plugin::SettingsValue::TYPE_INT_RANGE,       SettingsValueWrapper::ValueType::INT_RANGE },
	{ kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE, SettingsValueWrapper::ValueType::DOUBLE_RANGE },
	{ kome::plugin::SettingsValue::TYPE_LIST,                 SettingsValueWrapper::ValueType::LIST },
	{ kome::plugin::SettingsValue::TYPE_EMAIL,               SettingsValueWrapper::ValueType::EMAIL }
};

kome::core::ConversionElement< kome::plugin::SettingsValue::EnumType, SettingsValueWrapper::EnumType > g_settingsValueEnumTypes[] = {
	{ kome::plugin::SettingsValue::ENUM_NONE,	  SettingsValueWrapper::EnumType::NONE },
	{ kome::plugin::SettingsValue::ENUM_PLUGIN,	  SettingsValueWrapper::EnumType::PLUGIN },
	{ kome::plugin::SettingsValue::ENUM_INI,      SettingsValueWrapper::EnumType::INI },
	{ kome::plugin::SettingsValue::ENUM_FUNCTION, SettingsValueWrapper::EnumType::FUNCTION }
};

// constructor
SettingsValueWrapper::SettingsValueWrapper( kome::plugin::SettingsValue& value )
		: m_value( value ) {
}

// destructotr
SettingsValueWrapper::~SettingsValueWrapper() {
}

// get settings value object
kome::plugin::SettingsValue& SettingsValueWrapper::getSettingsValue() {
	return m_value;
}

// set type
void SettingsValueWrapper::setType( ValueType type ) {
	kome::plugin::SettingsValue::ValueType t
		= INVERT_FROM_ARRAY( type, kome::plugin::SettingsValue::TYPE_UNKNOWN, g_settingsValueTypes );
	m_value.setType( t );
}

void SettingsValueWrapper::setType( System::String^ type ) {
	m_value.setType( ClrObjectTool::convertString( type ).c_str() );
}

// get type
SettingsValueWrapper::ValueType SettingsValueWrapper::getType() {
	ValueType t = CONVERT_FROM_ARRAY( m_value.getType(), ValueType::UNKNOWN, g_settingsValueTypes );
	return t;
}

// set name
void SettingsValueWrapper::setName( System::String^ name ) {
	m_value.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name
System::String^ SettingsValueWrapper::getName() {
	System::String^ name = ClrObjectTool::convertString( m_value.getName(), NULL );
	return name;
}

// set default value
void SettingsValueWrapper::setDefaultValue( System::String^ value ) {
	m_value.setDefaultValue( ClrObjectTool::convertString( value ).c_str() );
}

// get default value
System::String^ SettingsValueWrapper::getDefaultValue() {
	return ClrObjectTool::convertString( m_value.getDefaultValue(), NULL );
}

// add exclude value
void SettingsValueWrapper::addExcludeValue( System::String^ val ) {
	m_value.addExcludeValue( ClrObjectTool::convertString( val ).c_str() );
}

// excluded value or not
bool SettingsValueWrapper::isExcluded( System::String^ val ) {
	return m_value.isExcluded( ClrObjectTool::convertString( val ).c_str() );
}

// set enumeration key
void SettingsValueWrapper::setEnumKey( System::String^ key ) {
	m_value.setEnumKey( ClrObjectTool::convertString( key ).c_str() );
}

// get enumeration key
System::String^ SettingsValueWrapper::getEnumKey() {
	return ClrObjectTool::convertString( m_value.getEnumKey(), NULL );
}

// set enumeration type
void SettingsValueWrapper::setEnumType( EnumType type ) {
	kome::plugin::SettingsValue::EnumType t
		= INVERT_FROM_ARRAY( type, kome::plugin::SettingsValue::ENUM_NONE, g_settingsValueEnumTypes );
	m_value.setEnumType( t );
}

// get enumeration type
SettingsValueWrapper::EnumType SettingsValueWrapper::getEnumType() {
	EnumType t = CONVERT_FROM_ARRAY( m_value.getEnumType(), EnumType::NONE, g_settingsValueEnumTypes );
	return t;
}

// set required
void SettingsValueWrapper::setRequired( bool required ) {
	m_value.setRequired( required );
}

// is required
bool SettingsValueWrapper::isRequired() {
	return m_value.isRequired();
}

// clear enumeration
void SettingsValueWrapper::clearEnumItems() {
	m_value.clearEnumItems();
}

// add enumeration item
void SettingsValueWrapper::addEnumItem( System::String^ name, System::String^ value, System::String^ desc ) {
	m_value.addEnumItem(
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( value ).c_str(),
		ClrObjectTool::convertString( desc ).c_str()
	);
}

// get enumeration
cli::array< SettingsValueWrapper::EnumItem^ >^ SettingsValueWrapper::getEnumeration() {
	// enumeration
	std::vector< kome::plugin::SettingsValue::EnumItem > enumeration;
	m_value.getEnumeration( enumeration );
	if( enumeration.size() == 0 ) {
		return nullptr;
	}

	// array
	cli::array< EnumItem^ >^ arr = gcnew cli::array< EnumItem^ >( enumeration.size() );
	for( unsigned int i = 0; i < enumeration.size(); i++ ) {
		EnumItem^ item = gcnew EnumItem();
		item->name = ClrObjectTool::convertString( enumeration[ i ].name.c_str(), NULL );
		item->value = ClrObjectTool::convertString( enumeration[ i ].value.c_str(), NULL );
		item->description = ClrObjectTool::convertString( enumeration[ i ].desc.c_str(), NULL ); 

		arr[ i ] = item;
	}

	return arr;
}

// enumeration of not
bool SettingsValueWrapper::isEnumValue() {
	return m_value.isEnumValue();
}

// get number restriction
NumberRestrictionWrapper^ SettingsValueWrapper::getNumberRestriction() {
	NumberRestrictionWrapper^ restriction
		= ClrObjectTool::createNumberRestrictionWrapper( &m_value.getNumberRestriction() );

	return restriction;
}

// set the parent value
void SettingsValueWrapper::setParentValue( SettingsValueWrapper^ parent ) {
	kome::plugin::SettingsValue* val = NULL;
	if( parent != nullptr ) {
		val = &( parent->m_value );
	}

	m_value.setParentValue( val );
}

// get the parent value
SettingsValueWrapper^ SettingsValueWrapper::getParentValue() {
	kome::plugin::SettingsValue* val = m_value.getParentValue();
	if( val == NULL ) {
		return nullptr;
	}

	return gcnew SettingsValueWrapper( *val );
}

// get the number of child values
unsigned int SettingsValueWrapper::getNumberOfChildValues() {
	return m_value.getNumberOfChildValues();
}

// add child value
void SettingsValueWrapper::addChildValue( SettingsValueWrapper^ child ) {
	kome::plugin::SettingsValue* val = NULL;

	if( child != nullptr ) {
		val = &( child->getSettingsValue() );
	}

	m_value.addChildValue( val );
}

// clear child value
void SettingsValueWrapper::clearChildValue() {
	m_value.clearChildValue();
}

// get child value
SettingsValueWrapper^ SettingsValueWrapper::getChildValue( unsigned int idx ) {
	kome::plugin::SettingsValue* child = m_value.getChildValue( idx );

	SettingsValueWrapper^ val = nullptr;
	if( child != NULL ) {
		val = gcnew SettingsValueWrapper( *child );
	}

	return val;
}

// check value
System::String^ SettingsValueWrapper::checkValue( System::String^ value, System::String^ name, bool canEmpty ) {
	return ClrObjectTool::convertString( 
				m_value.checkValue(
					ClrObjectTool::convertString( value ).c_str(),
					ClrObjectTool::convertString( name ).c_str(),
					canEmpty
				).c_str(),
				NULL
			);
}

// convet to saving string
System::String^ SettingsValueWrapper::convertToSavingString( System::String^ s ) {
	return ClrObjectTool::convertString(
		kome::plugin::SettingsValue::convertToSavingString(
			ClrObjectTool::convertString( s ).c_str()
		).c_str(),
		NULL
	);
}

// convet to displaying string
System::String^ SettingsValueWrapper::convertToDisplayingString( System::String^ s ) {
	return ClrObjectTool::convertString(
		kome::plugin::SettingsValue::convertToDisplayingString(
			ClrObjectTool::convertString( s ).c_str()
		).c_str(),
		NULL
	);
}

// remove outer bracket
System::String^ SettingsValueWrapper::removeOuterBracket( System::String^ s ) {
	return ClrObjectTool::convertString(
		kome::plugin::SettingsValue::removeOuterBracket(
			ClrObjectTool::convertString( s ).c_str()
		).c_str(),
		NULL
	);
}

// separate list value
cli::array< System::String^ >^ SettingsValueWrapper::separateListValue( System::String^ v ) {
	// separate
	std::vector< std::string > vals;
	kome::plugin::SettingsValue::separateListValue(
		ClrObjectTool::convertString( v ).c_str(),
		vals
	);

	if( vals.size() == 0 ) {
		return nullptr;
	}

	// string array
	cli::array< System::String^ >^ arr = gcnew cli::array< System::String^ >( vals.size() );
	for( unsigned int i = 0; i < vals.size(); i++ ) {
		arr[ i ] = ClrObjectTool::convertString( vals[ i ].c_str(), NULL );
	}

	return arr;
}
