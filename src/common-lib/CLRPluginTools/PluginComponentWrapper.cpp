/**
 * @file PluginComponentWrapper.cpp
 * @brief implements of PluginComponentWrapper class
 *
 * @author A.Ozaki
 * @date 2013.08.02
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "PluginComponentWrapper.h"

#include "PluginCallWrapper.h"
#include "PluginIconWrapper.h"
#include "ParameterSettingsWrapper.h"
#include "SettingsPageWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PluginComponentWrapper::PluginComponentWrapper( kome::plugin::PluginComponent& insComponent )
		: m_insComponent( insComponent )
{
}

// destructor
PluginComponentWrapper::~PluginComponentWrapper( )
{
}

// get plug-in component object
kome::plugin::PluginComponent&	PluginComponentWrapper::getPluginComponent( void )
{
	return	m_insComponent;
}

// set component name
void	PluginComponentWrapper::setName( System::String^ strName )
{
	m_insComponent.setName( ClrObjectTool::convertString( strName ).c_str( ) );

	return;
}

// get component name
System::String^	PluginComponentWrapper::getName( void )
{
	return	ClrObjectTool::convertString( m_insComponent.getName( ), "" );
}

// set component description
void	PluginComponentWrapper::setDesc( System::String^ strDescription )
{
	m_insComponent.setDesc( ClrObjectTool::convertString( strDescription ).c_str( ) );

	return;
}

// get component description
System::String^	PluginComponentWrapper::getDesc( void )
{
	return	ClrObjectTool::convertString( m_insComponent.getDesc( ), "" );
}

// set plug-in call
void	PluginComponentWrapper::addCall( PluginCallWrapper^ pluginCall )
{
	if  ( nullptr == pluginCall )
	{
		m_insComponent.addCall( NULL );
	}
	else
	{
		m_insComponent.addCall( &pluginCall->getPluginCall( ) );
	}

	return;
}

// get the number of call
unsigned int	PluginComponentWrapper::getNumberOfCalls( void )
{
	return	m_insComponent.getNumberOfCalls( );
}

// get plug-in call
PluginCallWrapper^	PluginComponentWrapper::getCall( const unsigned int unIndex )
{
	if  ( NULL == m_insComponent.getCall( unIndex ) )
	{
		return	nullptr;
	}

	return	gcnew PluginCallWrapper( *m_insComponent.getCall( unIndex ) );
}

// add package name
void	PluginComponentWrapper::addPackage( System::String^ strPackage )
{
	m_insComponent.addPackage( ClrObjectTool::convertString( strPackage ).c_str( ) );
}

// has package
bool	PluginComponentWrapper::hasPackage( System::String^ strPackage )
{
	return	m_insComponent.hasPackage( ClrObjectTool::convertString( strPackage ).c_str( ) );
}

// >>>>>>	@Date:2013/08/22	<Add>	A.Ozaki
//

// set enable
void	PluginComponentWrapper::setEnable( void )
{
	m_insComponent.setEnable( );

	return;
}

// reset enable
void	PluginComponentWrapper::resetEnable( void )
{
	m_insComponent.resetEnable( );

	return;
}

// is enable
bool	PluginComponentWrapper::isEnable( void )
{
	return	m_insComponent.isEnable( );
}

//
// <<<<<<	@Date:2013/08/22	<Add>	A.Ozaki
