/**
 * @file PluginIconWrapper.cpp
 * @brief implements of PluginIconWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginIconWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PluginIconWrapper::PluginIconWrapper( kome::plugin::PluginIcon& icon )
		: m_icon( icon ) {
}

// destructor
PluginIconWrapper::~PluginIconWrapper() {
}

// get plug-in icon object
kome::plugin::PluginIcon& PluginIconWrapper::getPluginIcon() {
	return m_icon;
}

// set ID
void PluginIconWrapper::setId( unsigned int id ) {
	m_icon.setId( id );
}

// get ID
unsigned int PluginIconWrapper::getId() {
	return m_icon.getId();
}

// set type
void PluginIconWrapper::setType( System::String^ type ) {
	m_icon.setType( ClrObjectTool::convertString( type ).c_str() );
}

// get type
System::String^ PluginIconWrapper::getType() {
	return ClrObjectTool::convertString( m_icon.getType(), "" );
}

// set size
void PluginIconWrapper::setSize( unsigned short width, unsigned short height ){
	m_icon.setSize( width, height );
}

// get width
unsigned short PluginIconWrapper::getWidth() {
	return m_icon.getWidth();
}

// get height
unsigned short PluginIconWrapper::getHeight() {
	return m_icon.getHeight();
}

// set name
void PluginIconWrapper::setName( System::String^ name ) {
	m_icon.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name
System::String^ PluginIconWrapper::getName() {
	return ClrObjectTool::convertString( m_icon.getName(), "" );
}

// set path
void PluginIconWrapper::setPath( System::String^ path ) {
	m_icon.setPath( ClrObjectTool::convertString( path ).c_str() );
}

// get path
System::String^ PluginIconWrapper::getPath() {
	return ClrObjectTool::convertString( m_icon.getPath(), "" );
}
