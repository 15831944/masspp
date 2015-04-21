/**
 * @file PluginManagerWrapper.cpp
 * @brief implements of PluginManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginManagerWrapper.h"

#include "PluginMenuWrapper.h"
#include "PluginInfoWrapper.h"
#include "PluginCallWrapper.h"
#include "ParameterSettingsWrapper.h"
#include "SettingsChapterWrapper.h"
#include "PluginFunctionItemWrapper.h"

#include <vector>
#include <boost/bind.hpp>


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PluginManagerWrapper::PluginManagerWrapper( kome::plugin::PluginManager& mgr )
		: m_mgr( mgr ) {
}

// destructor
PluginManagerWrapper::~PluginManagerWrapper() {
}

// get plug-in manager object
kome::plugin::PluginManager& PluginManagerWrapper::getPluginManager() {
	return m_mgr;
}

// get plug-in menu
PluginMenuWrapper^ PluginManagerWrapper::getMenu() {
	return gcnew PluginMenuWrapper( m_mgr.getMenu() );
}

// get settings root chapter
SettingsChapterWrapper^ PluginManagerWrapper::getSettingsRootChapter() {
	return gcnew SettingsChapterWrapper( m_mgr.getSettingsRootChapter() );
}

// get parameters root chapter
SettingsChapterWrapper^ PluginManagerWrapper::getParametersRootChapter() {
	return gcnew SettingsChapterWrapper( m_mgr.getParametersRootChapter() );
}

// load plug-in file
bool PluginManagerWrapper::loadPluginFile( System::String^ xmlPath ) {
	return m_mgr.loadPluginFile(
		ClrObjectTool::convertString( xmlPath ).c_str()
	);
}		

// load settings file
bool PluginManagerWrapper::loadSettingsFile( System::String^ xmlPath, ParameterSettingsWrapper^ settings ) {
	// check the parameter
	if( settings == nullptr ) {
		return false;
	}

	// load file
	return m_mgr.loadSettingsFile(
		ClrObjectTool::convertString( xmlPath ).c_str(),
		settings->getParameterSettings()
	);
}

// load paths
bool PluginManagerWrapper::loadPath( System::String^ path, PluginFilterFunction^ filterFun ) { 
	// create plugin filter helper object
	PluginFilterHelper helper;
	helper.setFilterFunction( filterFun );

	// load paths
	return m_mgr.loadPath(
		ClrObjectTool::convertString( path ).c_str(),
		boost::bind( &PluginFilterHelper::pathFilter, &helper, _1 )
	);
}

// issue menu ID
unsigned int PluginManagerWrapper::issueMenuId() {
	return m_mgr.issueMenuId();
}

// open dynamic link library
void PluginManagerWrapper::openLink( System::String^ path ) {
	m_mgr.openLink( ClrObjectTool::convertString( path ).c_str() );
}

// close dynamic link library
void PluginManagerWrapper::closeLink( System::String^ path ) {
	m_mgr.closeLink( ClrObjectTool::convertString( path ).c_str() );
}

// get the number of plug-ins
unsigned int PluginManagerWrapper::getNumberOfPlugins() {
	return m_mgr.getNumberOfPlugins();
}

// get plug-in
PluginInfoWrapper^ PluginManagerWrapper::getPlugin( unsigned int index ) {
	kome::plugin::PluginInfo* plugin = m_mgr.getPlugin( index );
	if( plugin == NULL ) {
		return nullptr;
	}

	return gcnew PluginInfoWrapper( *plugin );
}

// get plug-in
PluginInfoWrapper^ PluginManagerWrapper::getPlugin( System::String^ name ) {
	kome::plugin::PluginInfo* plugin
		= m_mgr.getPlugin( ClrObjectTool::convertString( name ).c_str() );
	if( plugin == NULL ) {
		return nullptr;
	}

	return gcnew PluginInfoWrapper( *plugin );
}

// each call
void PluginManagerWrapper::forEachCalls( System::String^ type, EachCallFunction^ eachFun, bool checkStatus ) {
	// create each call helper object
	EachCallHelper helper;
	helper.setForEachFunction( eachFun );

	// for each calls
	m_mgr.forEachCalls(
		ClrObjectTool::convertString( type ).c_str(),
		boost::bind(
			&EachCallHelper::forEach,
			&helper,
			_1
		),
		checkStatus
	);
}

// get the number of function items
unsigned int PluginManagerWrapper::getNumberOfFunctionItems( System::String^ type ) {
	return m_mgr.getNumberOfFunctionItems( ClrObjectTool::convertString( type ).c_str() );
}

// get function item
PluginFunctionItemWrapper^ PluginManagerWrapper::getFunctionItem( System::String^ type, unsigned int index ) {
	kome::plugin::PluginFunctionItem* item
		= m_mgr.getFunctionItem( ClrObjectTool::convertString( type ).c_str(), index );
	if( item == NULL ) {
		return nullptr;
	}
	return gcnew PluginFunctionItemWrapper( *item );
}


// get function item
PluginFunctionItemWrapper^ PluginManagerWrapper::getFunctionItem( System::String^ type, System::String^ name, SettingParameterValuesWrapper^ settings ) {
	kome::plugin::PluginFunctionItem* item = m_mgr.getFunctionItem(
		ClrObjectTool::convertString( type ).c_str(),
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::getSettings( settings )
	);

	if( item == NULL ) {
		return nullptr;
	}

	return gcnew PluginFunctionItemWrapper( *item );
}

// call all functions
void PluginManagerWrapper::callAllFunctions( ClrParameters^ params, System::String^ type, bool checkStatus ) {
	// check parameter
	if( params == nullptr ) {
		m_mgr.callAllFunctions( NULL, ClrObjectTool::convertString( type ).c_str(), checkStatus );
	}

	// get parameters object
	kome::objects::Parameters p;
	ClrObjectTool::convertParamsFromCLR( &p, params );

	m_mgr.callAllFunctions( &p, ClrObjectTool::convertString( type ).c_str(), checkStatus );
}

// clear plug-ins
void PluginManagerWrapper::clearPlugins() {
	m_mgr.clearPlugins();
}

// >>>>>>	@Date:2013/08/02	<Add>	A.Ozaki
//
PluginInfoWrapper^	PluginManagerWrapper::getHiddenPlugin( System::String^ strName )
{
	return	gcnew PluginInfoWrapper( *m_mgr.getHiddenPlugin( ClrObjectTool::convertString( strName ).c_str() ) );
}
//
// <<<<<<	@Date:2013/08/02	<Add>	A.Ozaki

// >>>>>>	@Date:2013/09/99	<Add>	A.Ozaki
//
void	PluginManagerWrapper::addActiveLookupTable( PluginInfoWrapper^ pInfo )
{
	m_mgr.addActiveLookupTable( &pInfo->getPluginInfo( ) );

	return;
}
//
// <<<<<<	@Date:2013/08/02	<Add>	A.Ozaki

// get instance
PluginManagerWrapper^ PluginManagerWrapper::getInstance() {
	return gcnew PluginManagerWrapper( kome::plugin::PluginManager::getInstance() );
}


/////////// plug-in filter helper ///////////////


// constructor
PluginFilterHelper::PluginFilterHelper() {
	m_fun = nullptr;
}

// destructor
PluginFilterHelper::~PluginFilterHelper() {
}

// set function
void PluginFilterHelper::setFilterFunction( gcroot< PluginFilterFunction^ > fun ) {
	m_fun = fun;
}

// filter
bool PluginFilterHelper::pathFilter( const char* path ) {
	// check the member
	PluginFilterFunction^ fun = m_fun;
	if( fun == nullptr ) {
		return true;
	}

	// path
	System::String^ p = nullptr;
	if( path != NULL ) {
		p = gcnew System::String( path );
	}

	// filter
	return fun( p );
}


/////////// for_each call helper ///////////////


// constructor
EachCallHelper::EachCallHelper() {
	m_fun = nullptr;
}

// destructor
EachCallHelper::~EachCallHelper() {
}

// set function
void EachCallHelper::setForEachFunction( gcroot< EachCallFunction^ > fun ) {
	m_fun = fun;
}

// for each
void EachCallHelper::forEach( kome::plugin::PluginCall* call ) {
	// check parameter
	if( call == NULL ) {
		return;
	}

	// check the member
	EachCallFunction^ fun = m_fun;
	if( fun == nullptr ) {
		return;
	}

	// for each
	fun( gcnew PluginCallWrapper( *call ) );
}
