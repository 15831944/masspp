/**
 * @file ClrIniFile.cpp
 * @brief implements of ClrIniFile class
 *
 * @author S.Tanaka
 * @date 2006.11.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrIniFile.h"

#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrIniFile::ClrIniFile( kome::core::IniFile& ini ) : m_ini( ini ) {
}

// destructor
ClrIniFile::~ClrIniFile() {
}

// get ini file object
kome::core::IniFile& ClrIniFile::getIniFile() {
	return m_ini;
}

// get file path
System::String^ ClrIniFile::getFilePath() {
	return ClrObjectTool::convertString( m_ini.getFilePath(), NULL );
}

// set string value
void ClrIniFile::setString( System::String^ section, System::String^ key, System::String^ value ) {
	m_ini.setString(
		ClrObjectTool::convertString( section ).c_str(),
		ClrObjectTool::convertString( key ).c_str(),
		ClrObjectTool::convertString( value ).c_str()
	);
}

// get string value
System::String^ ClrIniFile::getString(
		System::String^ section,
		System::String^ key,
		System::String^ defaultValue
) {
	return ClrObjectTool::convertString(
		m_ini.getString(
			ClrObjectTool::convertString( section ).c_str(),
			ClrObjectTool::convertString( key ).c_str(),
			ClrObjectTool::convertString( defaultValue ).c_str()
		),
		NULL
	);
}

// set int value
void ClrIniFile::setInt( System::String^ section, System::String^ key, int value ) {
	m_ini.setInt(
		ClrObjectTool::convertString( section ).c_str(),
		ClrObjectTool::convertString( key ).c_str(),
		value
	);
}

// get int value
int ClrIniFile::getInt( System::String^ section, System::String^ key, int defaultValue ) {
	return m_ini.getInt(
		ClrObjectTool::convertString( section ).c_str(),
		ClrObjectTool::convertString( key ).c_str(),
		defaultValue
	);
}

// set double value
void ClrIniFile::setDouble( System::String^ section, System::String^ key, double value ) {
	m_ini.setDouble(
		ClrObjectTool::convertString( section ).c_str(),
		ClrObjectTool::convertString( key ).c_str(),
		value
	);
}

// get double value
double ClrIniFile::getDouble( System::String^ section, System::String^ key, double defaultValue ) {
	return m_ini.getDouble(
		ClrObjectTool::convertString( section ).c_str(),
		ClrObjectTool::convertString( key ).c_str(),
		defaultValue
	);
}

// delete value
void ClrIniFile::deleteValue( System::String^ section, System::String^ key ) {
	return m_ini.deleteValue(
		ClrObjectTool::convertString( section ).c_str(),
		ClrObjectTool::convertString( key ).c_str()
	);
}

// get the number of sections
unsigned int ClrIniFile::getNumberOfSections() {
	return m_ini.getNumberOfSections();
}

// get section
System::String^ ClrIniFile::getSection( unsigned int index ) {
	return ClrObjectTool::convertString( m_ini.getSection( index ), NULL );
}

// get the number of parameters
unsigned int ClrIniFile::getNumberOfParameters( System::String^ section ) {
	// check the section name
	if( section == nullptr ) {
		return 0;
	}

	// get the number of parameters
	return m_ini.getNumberOfParameters( ClrObjectTool::convertString( section ).c_str() );
}

// get parameter name
System::String^ ClrIniFile::getParameterName( System::String^ section, unsigned int index ) {
	// check the section name
	if( section == nullptr ) {
		return nullptr;
	}

	// get the number of parameters
	return ClrObjectTool::convertString(
		m_ini.getParameterName(
			ClrObjectTool::convertString( section ).c_str(),
			index
		),
		NULL
	);
}

// get parameter value
System::String^ ClrIniFile::getParameterValue( System::String^ section, unsigned int index ) {
	// check the section name
	if( section == nullptr ) {
		return nullptr;
	}

	// get the number of parameters
	return ClrObjectTool::convertString(
		m_ini.getParameterValue(
			ClrObjectTool::convertString( section ).c_str(),
			index
		),
		NULL
	);
}

// load
bool ClrIniFile::load( System::String^ path ) {
	if( path == nullptr ) {
		return m_ini.load( NULL );
	}
	return m_ini.load( ClrObjectTool::convertString( path ).c_str() );
}

// save
bool ClrIniFile::save( System::String^ path ) {
	if( path == nullptr ) {
		return m_ini.save( NULL );
	}
	return m_ini.save( ClrObjectTool::convertString( path ).c_str() );
}

// load ini file
ClrIniFile^ ClrIniFile::loadIniFile( System::String^ path ) {
	kome::core::IniFile* ini = NULL;

	if( path == nullptr ) {
		ini = kome::core::IniFile::loadIniFile( NULL );
	}
	else {
		ini = kome::core::IniFile::loadIniFile( ClrObjectTool::convertString( path ).c_str() );
	}

	if( ini == NULL ) {
		return nullptr;
	}
	return gcnew ClrIniFile( *ini );
}

// get ini file
ClrIniFile^ ClrIniFile::getIniFile( System::String^ path ) {
	kome::core::IniFile* ini = NULL;

	if( path == nullptr ) {
		ini = kome::core::IniFile::getIniFile( NULL );
	}
	else {
		ini = kome::core::IniFile::getIniFile( ClrObjectTool::convertString( path ).c_str() );
	}

	if( ini == NULL ) {
		return nullptr;
	}
	return gcnew ClrIniFile( *ini );

}
