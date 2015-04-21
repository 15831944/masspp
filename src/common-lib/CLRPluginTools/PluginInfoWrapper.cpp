/**
 * @file PluginInfoWrapper.cpp
 * @brief implements of PluginInfoWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginInfoWrapper.h"

#include "PluginCallWrapper.h"
#include "PluginIconWrapper.h"
#include "ParameterSettingsWrapper.h"
#include "SettingsPageWrapper.h"
#include "PluginComponentWrapper.h"		// >>>>>>	@Date:2013/08/02	<Add>	A.Ozaki

using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PluginInfoWrapper::PluginInfoWrapper( kome::plugin::PluginInfo& plugin )
		: m_plugin( plugin ) {
}

// destructor
PluginInfoWrapper::~PluginInfoWrapper() {
}

// get plug-in object
kome::plugin::PluginInfo& PluginInfoWrapper::getPluginInfo() {
	return m_plugin;
}

// set directory
void PluginInfoWrapper::setDir( System::String^ dir ) {
	m_plugin.setDir( ClrObjectTool::convertString( dir ).c_str() );
}

// get directory
System::String^ PluginInfoWrapper::getDir() {
	return ClrObjectTool::convertString( m_plugin.getDir(), "" );
}

// set name
void PluginInfoWrapper::setName( System::String^ name ) {
	m_plugin.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name
System::String^ PluginInfoWrapper::getName() {
	return ClrObjectTool::convertString( m_plugin.getName(), "" );
}

// set version
void PluginInfoWrapper::setVersion( System::String^ version ) {
	m_plugin.setVersion( ClrObjectTool::convertString( version ).c_str() );
}

// get version
System::String^ PluginInfoWrapper::getVersion() {
	return ClrObjectTool::convertString( m_plugin.getVersion(), "" );
}

// set provider
void PluginInfoWrapper::setProvider( System::String^ provider ) {
	m_plugin.setProvider( ClrObjectTool::convertString( provider ).c_str() );
}

// get provider
System::String^ PluginInfoWrapper::getProvider() {
	return ClrObjectTool::convertString( m_plugin.getProvider(), "" );
}

// set link
void PluginInfoWrapper::setLink( System::String^ link ) {
	m_plugin.setLink( ClrObjectTool::convertString( link ).c_str() );
}

// get link
System::String^ PluginInfoWrapper::getLink() {
	return ClrObjectTool::convertString( m_plugin.getLink(), "" );
}

// clear icons
void PluginInfoWrapper::clearIcons() {
	m_plugin.clearIcons();
}

// create new plug-in icon
PluginIconWrapper^ PluginInfoWrapper::createNewIcon() {
	// create icon object
	kome::plugin::PluginIcon* icon = m_plugin.createNewIcon();
	if( icon == NULL ) {
		return nullptr;
	}

	// create wrapper
	return gcnew PluginIconWrapper( *icon );
}

// get the number of icons
unsigned int PluginInfoWrapper::getNumberOfIcons() {
	return m_plugin.getNumberOfIcons();
}

// get icon
PluginIconWrapper^ PluginInfoWrapper::getIcon( unsigned int index ) {
	// get icon object
	kome::plugin::PluginIcon* icon = m_plugin.getIcon( index );
	if( icon == NULL ) {
		return nullptr;
	}

	// create wrapper
	return gcnew PluginIconWrapper( *icon );
}

// get icon
PluginIconWrapper^ PluginInfoWrapper::getIcon( System::String^ name ) {
	// get icon object
	kome::plugin::PluginIcon* icon
		= m_plugin.getIcon( ClrObjectTool::convertString( name ).c_str() );
	if( icon == NULL ) {
		return nullptr;
	}

	// create wrapper
	return gcnew PluginIconWrapper( *icon );
}

// clear calls
void PluginInfoWrapper::clearCalls() {
	m_plugin.clearCalls();
}

// create new call
PluginCallWrapper^ PluginInfoWrapper::createNewCall() {
	// create call object
	kome::plugin::PluginCall* call = m_plugin.createNewCall();
	if( call == NULL ) {
		return nullptr;
	}

	// create wrapper
	return gcnew PluginCallWrapper( *call );
}

// get the number of calls
unsigned int PluginInfoWrapper::getNumberOfCalls() {
	return m_plugin.getNumberOfCalls();
}

// get call
PluginCallWrapper^ PluginInfoWrapper::getCall( unsigned index ) {
	// get call object
	kome::plugin::PluginCall* call = m_plugin.getCall( index );
	if( call == NULL ) {
		return nullptr;
	}

	// create wrapper
	return gcnew PluginCallWrapper( *call );
}

// get call
PluginCallWrapper^ PluginInfoWrapper::getCall( System::String^ name ) {
	// get call object
	kome::plugin::PluginCall* call
		= m_plugin.getCall( ClrObjectTool::convertString( name ).c_str() );
	if( call == NULL ) {
		return nullptr;
	}

	// create wrapper
	return gcnew PluginCallWrapper( *call );
}

// clear options
void PluginInfoWrapper::clearOptions() {
	m_plugin.clearOptions();
}

// add options file
bool PluginInfoWrapper::addOptionsFile( System::String^ path ) {
	return m_plugin.addOptionsFile( ClrObjectTool::convertString( path ).c_str() );
}

// get the number of options file
unsigned int PluginInfoWrapper::getNumberOfOptions() {
	return m_plugin.getNumberOfOptions();
}

// get options
ParameterSettingsWrapper^ PluginInfoWrapper::getOptions( unsigned int index ) {
	// get options 
	kome::plugin::ParameterSettings* options = m_plugin.getOptions( index );
	if( options == NULL ) {
		return nullptr;
	}

	return gcnew ParameterSettingsWrapper( *options );
}

// clear settings
void PluginInfoWrapper::clearParameters() {
	m_plugin.clearParameters();
}

// add parameters file
bool PluginInfoWrapper::addParametersFile( System::String^ path ) {
	return m_plugin.addParametersFile( ClrObjectTool::convertString( path ).c_str() );
}

// get the number of parameters file
unsigned int PluginInfoWrapper::getNumberOfParameters() {
	return m_plugin.getNumberOfParameters();
}

// get parameters
ParameterSettingsWrapper^ PluginInfoWrapper::getParameters( unsigned int index ) {
	// get parameters 
	kome::plugin::ParameterSettings* parameters = m_plugin.getParameters( index );
	if( parameters == NULL ) {
		return nullptr;
	}

	return gcnew ParameterSettingsWrapper( *parameters );
}

// get settings page
SettingsPageWrapper^ PluginInfoWrapper::getPage( System::String^ name ) {
	// get settings page
	kome::plugin::SettingsPage* page = m_plugin.getPage( ClrObjectTool::convertString( name ).c_str() );
	if( page == NULL ) {
		return nullptr;
	}

	return gcnew SettingsPageWrapper( *page );
}

// add Java class path
void PluginInfoWrapper::addJavaClassPath( System::String^ path ) {
	m_plugin.addJavaClassPath( ClrObjectTool::convertString( path ).c_str() );
}

// get the number of Java class pathes
unsigned int PluginInfoWrapper::getNumberOfJavaClassPathes() {
	return m_plugin.getNumberOfJavaClassPathes();
}

// get Java class path
System::String^ PluginInfoWrapper::getJavaClassPath( unsigned int idx ) {
	return ClrObjectTool::convertString( m_plugin.getJavaClassPath( idx ), NULL );
}

// >>>>>>	@Date:2013/08/02	<Add>	A.Ozaki
//

// set hidden
void	PluginInfoWrapper::setHidden( const bool bHidden )
{
	m_plugin.setHidden( bHidden );

	return;
}

// hidden plugin
bool	PluginInfoWrapper::isHidden( void )
{
	return	m_plugin.isHidden( );
}

// add plugin-call
void	PluginInfoWrapper::addCall( PluginCallWrapper^ pluginCall )
{
	m_plugin.addCall( &pluginCall->getPluginCall( ) );

	return;
}

// delete plugin-call
void	PluginInfoWrapper::deleteCall( PluginCallWrapper^ pluginCall )
{
	m_plugin.deleteCall( &pluginCall->getPluginCall( ) );

	return;
}

// add component
void	PluginInfoWrapper::addComponent( PluginComponentWrapper^ pluginComponent )
{
	m_plugin.addComponent( &pluginComponent->getPluginComponent( ) );

	return;
}

//
// <<<<<<	@Date:2013/08/02	<Add>	A.Ozaki

// >>>>>>	@Date:2013/08/22	<Add>	A.Ozaki
//

// get plug-in component
PluginComponentWrapper^	PluginInfoWrapper::getComponent( System::String^ strComponentName )
{
	// get component object
	kome::plugin::PluginComponent	*pComponent =
			m_plugin.getComponent( ClrObjectTool::convertString( strComponentName ).c_str( ) );
	if  ( (kome::plugin::PluginComponent *)NULL == pComponent )
	{
		return	nullptr;
	}

	// create wrapper
	return	gcnew PluginComponentWrapper( *pComponent );
}

// get the number of plugin components
unsigned int	PluginInfoWrapper::getNumberOfComponents( void )
{
	return	m_plugin.getNumberOfComponents( );
}

// get plug-in component
PluginComponentWrapper^	PluginInfoWrapper::getComponent( const unsigned int nIndex )
{
	// get component object
	kome::plugin::PluginComponent	*pComponent = m_plugin.getComponent( nIndex );
	if  ( (kome::plugin::PluginComponent *)NULL == pComponent )
	{
		return	nullptr;
	}

	// create wrapper
	return	gcnew PluginComponentWrapper( *pComponent );
}

//
// <<<<<<	@Date:2013/08/22	<Add>	A.Ozaki
