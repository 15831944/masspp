/**
 * @file SettingsGroupWrapper.cpp
 * @brief implements of SettingsGroupWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsGroupWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingsGroupWrapper::SettingsGroupWrapper( kome::plugin::SettingsGroup& group )
		: m_group( group ) {
}

// destructor
SettingsGroupWrapper::~SettingsGroupWrapper() {
}

// get settings group object
kome::plugin::SettingsGroup& SettingsGroupWrapper::getSettingsGroup() {
	return m_group;
}

// set name
void SettingsGroupWrapper::setName( System::String^ name ) {
	m_group.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name
System::String^ SettingsGroupWrapper::getName() {
	return ClrObjectTool::convertString( m_group.getName(), NULL );
}
