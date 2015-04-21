/**
 * @file PluginHelp.cpp
 * @brief implements of PluginHelp class
 *
 * @author S.Tanaka
 * @date 2007.03.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginHelp.h"


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PluginHelp::PluginHelp() {
	m_plugin = NULL;
	m_order = 5;
}

// destructor
PluginHelp::~PluginHelp() {
}

// set plug-in
void PluginHelp::setPlugin( PluginInfo* plugin ) {
	m_plugin = plugin;
}

// get plug-in
PluginInfo* PluginHelp::getPlugin() {
	return m_plugin;
}

// set help file path
void PluginHelp::setHelpFile( const char* path ) {
	m_path = NVL( path, "" );
}

// get help file path
const char* PluginHelp::getHelpFile() {
	return m_path.c_str();
}

// set order
void PluginHelp::setOrder( const int order ) {
	m_order = order;
}

// get order
int PluginHelp::getOrder() {
	return m_order;
}
