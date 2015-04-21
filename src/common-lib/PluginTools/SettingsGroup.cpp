/**
 * @file SettingsGroup.cpp
 * @brief implements of SettingsGroup class
 *
 * @author S.Tanaka
 * @date 2007.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "SettingsGroup.h"

using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SettingsGroup::SettingsGroup() {
}

// destructor
SettingsGroup::~SettingsGroup() {
}

// set gorup name
void SettingsGroup::setName( const char* name ) {
	m_name = std::string( NVL( name, "" ) );
}

// get group name
const char* SettingsGroup::getName() {
	return m_name.c_str();
}
