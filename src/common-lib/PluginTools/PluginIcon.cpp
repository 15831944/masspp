/**
 * @file PluginIcon.cpp
 * @brief implements of PluginIcon class
 *
 * @author S.Tanaka
 * @date 2006.09.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "PluginIcon.h"

#include "PluginInfo.h"

using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
PluginIcon::PluginIcon( PluginInfo* plugin ) {
	m_plugin = plugin;
	m_width = 0;
	m_height = 0;
}

// destructor
PluginIcon::~PluginIcon() {
}

// set plug-in icon id
void PluginIcon::setId( const unsigned int id ) {
	m_id = id;
}

// get plug-in icon id
unsigned int PluginIcon::getId() {
	return m_id;
}

// set type
void PluginIcon::setType( const char* type ) {
	m_type = touppercase( type );
}

// get type
const char* PluginIcon::getType() {
	return m_type.c_str();
}

// set size
void PluginIcon::setSize( const unsigned short width, const unsigned short height ) {
	m_width = width;
	m_height = height;
}

// get width
unsigned short PluginIcon::getWidth() {
	return m_width;
}

// get height
unsigned short PluginIcon::getHeight() {
	return m_height;
}

// set name
void PluginIcon::setName( const char* name ) {
	m_name = std::string( NVL( name, "" ) );
}

// get name 
const char* PluginIcon::getName() {
	return m_name.c_str();
}

// set path
void PluginIcon::setPath( const char* path ) {
	m_path = std::string( NVL( path, "" ) );
}

// get path
const char* PluginIcon::getPath() {
	return m_path.c_str();
}
