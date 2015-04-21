/**
 * @file PluginCall.cpp
 * @brief implements of PluginCall class
 *
 * @author S.Tanaka
 * @date 2006.08.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginCall.h"

#include "PluginInfo.h"
#include "PluginManager.h"

#include <list>
#include <boost/bind.hpp>


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DEFAULT_CALL_TYPE "UNKNOWN"


// constructor
PluginCall::PluginCall( PluginInfo* plugin ) {
	m_plugin = plugin;
	m_type = std::string( DEFAULT_CALL_TYPE );
	m_ptFunc = NULL;

	m_icon = NULL;

	m_clrCall = NULL;

	m_ifloaded = false;


	m_flgActive = true;		// active flag of plugin manager	// @date 2013/02/21 <Add> OKADA
	m_vstrPackages.clear();	// Packages information of plugin manager	// @date 2013/02/21 <Add> OKADA

}

// destructor
PluginCall::~PluginCall() {
}

// get plug-in information
PluginInfo* PluginCall::getPlugin() {
	return m_plugin;
}

// set type
void PluginCall::setType( const char* type ) {
	// create string object
	m_type = touppercase( NVL( type, DEFAULT_CALL_TYPE ) );
}

// get type
const char* PluginCall::getType() {
	return m_type.c_str();
}

// set call name 
void PluginCall::setName( const char* name ) {
	m_name = std::string( NVL( name, "" ) );
}

// get call name
const char* PluginCall::getName() {
	return m_name.c_str();
}

// set icon
void PluginCall::setIcon( PluginIcon* icon ) {
	m_icon = icon;
}

// get icon
PluginIcon* PluginCall::getIcon() {
	return m_icon;
}

// set order
void PluginCall::setOrder( const unsigned short order ) {
	m_order = order;
}

// get order 
unsigned short PluginCall::getOrder() {
	return m_order;
}

// add status
void PluginCall::addStatus( const char* name, const char* value ) {
	// string object
	std::string n = NVL( name, "" );
	std::string v = NVL( value, "" );

	// add
	m_statuses.push_back( std::make_pair( n, v ) );
}

// get the number of statues
unsigned int PluginCall::getNumberOfStatuses() {
	return m_statuses.size();
}

// get status name
const char* PluginCall::getStatusName( const unsigned int idx ) {
	if( idx >= m_statuses.size() ) {
		return NULL;
	}

	return m_statuses[ idx ].first.c_str();
}

// get status value
const char* PluginCall::getStatusValue( const unsigned int idx ) {
	if( idx >= m_statuses.size() ) {
		return NULL;
	}

	std::string v = m_statuses[ idx ].second;
	if( !v.empty() && v[ 0 ] == '!' ) {
		return ( m_statuses[ idx ].second.c_str() + 1 );
	}

	return m_statuses[ idx ].second.c_str();
}

// clear statuses
void PluginCall::clearStatuses() {
	m_statuses.clear();
}

// set ifloaded flag
void PluginCall::setIfLoaded( const bool ifloaded ) {
	m_ifloaded = ifloaded;
}

// get ifloaded flag
bool PluginCall::getIfLoaded() {
	return m_ifloaded;
}

// set function
void PluginCall::setFunction(
		const char* methodName,
		const char* className,
		bool isStatic
) {
	m_functionInfo.methodName = std::string( NVL( methodName, "" ) );
	m_functionInfo.className = std::string( NVL( className, "" ) );
	m_functionInfo.isStatic = isStatic;

	m_ptFunc = NULL;
}

// get method name
const char* PluginCall::getMethodName() {
	if( m_functionInfo.methodName.empty() ) {
		return NULL;
	}
	return m_functionInfo.methodName.c_str();
}

// get class name
const char* PluginCall::getClassName() {
	if( m_functionInfo.className.empty() ) {
		return NULL;
	}
	return m_functionInfo.className.c_str();
}

// judge whether class method is static
bool PluginCall::isStaticMethod() {
	return m_functionInfo.isStatic;
}

// get properties
kome::core::Properties& PluginCall::getProperties() {
	return m_properties;
}

// set plug-in description
void PluginCall::setDescription( const char* description ) {
	m_description = std::string( NVL( description, "" ) );
}

// get plug-in description
const char* PluginCall::getDescription() {
	return m_description.c_str();
}

// check status
bool PluginCall::checkStatus() {
	// manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// check statuses
	if( m_statuses.size() == 0 ) {
		return true;
	}

	for( unsigned int i = 0; i < m_statuses.size(); i++ ) {
		const char* n = m_statuses[ i ].first.c_str();
		const char* v = m_statuses[ i ].second.c_str();
		std::string val = NVL( v, "" );
		bool defaultVal = false;

		if( !val.empty() && val[ 0 ] == '!' ) {
			defaultVal = true;
			val = val.substr( 1 );
		}

		if( !statusMgr.hasStatus( n ) && defaultVal ) {
			statusMgr.setValue( n, val.c_str() );
			return true;
		}

		const char* statusVal = statusMgr.getValue( n );
		if( statusVal != NULL && val.compare( statusVal ) == 0 ) {
			return true;
		}
	}

	return false;
}

// invoke function and method
kome::objects::Variant PluginCall::invoke( kome::objects::Parameters* params ) {
	return onInvoke( params );
}

// on invoke
kome::objects::Variant PluginCall::onInvoke( kome::objects::Parameters* params ) {
	// get plug-in function
	PLUGIN_FUNCTION* func = getFunction();
	if( func == NULL ) {
		kome::objects::Variant ret;
		return ret;
	}
	
	// @date 2013/05/13 <Add> OKADA Test ------->
	if( this->m_flgActive != true ){
		kome::objects::Variant ret;
		return ret;
	}
	// @date 2013/05/13 <Add> OKADA Test <-------

	// log
	LOG_TRACE( FMT( "Invoke [plug-in=%s] [call=%s]", m_plugin->getName(), m_name.c_str() ) );

	// execute
	kome::objects::Variant ret;
	try{
		ret = ( *func )( params );
	}
	catch( ... ) {
	}

	return ret;
}

// get the pointer of call function
PLUGIN_FUNCTION* PluginCall::getFunction() {
	// check member
	if( m_plugin == NULL ) {
		LOG_WARN( FMT( "Plug-in information is not found." ) );
		return NULL;
	}

#ifdef _MSC_VER
	if( m_clrCall != NULL ) {
		kome::clr::ClrPluginFunction* clrCall = (kome::clr::ClrPluginFunction*)m_clrCall;
		return clrCall->getFunction( m_ifloaded );
	}
#endif
	if( m_ptFunc != NULL ) {
		return &m_func;
	}

	// get link path
	std::string path = getabsolutepath( m_plugin->getDir(), m_plugin->getLink() );

	// get the pointer of function
	if( m_functionInfo.className.empty() ) {
		// plug-in manager
		PluginManager& plgMgr = PluginManager::getInstance();

		// load the link
		DL_HANDLE handle = NULL;
		if( m_ifloaded ) {
			handle = (DL_HANDLE)plgMgr.getLink( path.c_str() );
			if( handle == NULL ) {
				return NULL;
			}
		}
		else{
			handle = (DL_HANDLE)plgMgr.openLink( path.c_str() );
			if( handle == NULL ) {	// It failed to load link.
				LOG_ERROR( FMT( "Failed to get the function pointer." ) );
				return NULL;
			}
		}

		// get the pointer of function
#ifdef _WIN32
		m_ptFunc = (PLUGIN_FUNCTION_PT)GetProcAddress( handle, m_functionInfo.methodName.c_str() );
		if( m_ptFunc == NULL ) {
			LOG_ERROR(
				FMT(
					"Failed to get the function pointer. [%s : %s] (Error code: %d )",
					getfilename( m_plugin->getLink() ).c_str(),
					m_functionInfo.methodName.c_str(),
					GetLastError()
				)
			);
			return NULL;
		}
#else
		m_ptFunc = (PLUGIN_FUNCTION_PT)dlsym( handle, m_functionInfo.methodName.c_str() );
		if( m_ptFunc == NULL ) {
			LOG_ERROR(
				FMT(
					"Failed to get the function pointer. [%s : %s] (%s)",
					getfilename( m_plugin->getLink() ).c_str(),
					m_functionInfo.methodName.c_str(),
					dlerror()
				)
			);
			return NULL;
		}
#endif	// _WIN32	

		if( m_ptFunc != NULL ) {	// create function object
			m_func = m_ptFunc;
		}
	}
#ifdef _MSC_VER
	else {	// CLR

		// create plug-in call information object
		kome::clr::ClrPluginFunction* clrCall = kome::clr::ClrPluginFunction::createNewInstance();
		m_clrCall = clrCall;

		// set parameters
		clrCall->setMethodInfo(
			path.c_str(),
			m_functionInfo.className.c_str(),
			m_functionInfo.methodName.c_str(),
			m_functionInfo.isStatic
		);

		PLUGIN_FUNCTION* fun = clrCall->getFunction( m_ifloaded );

		return fun;
	}
#endif	// _MSC_VER

	return &m_func;
}

// set active
void PluginCall::setActive( bool flgActive ){
	m_flgActive = flgActive;
}

// is active
bool PluginCall::isActive( void ){
	return m_flgActive;
}

// add package
void PluginCall::addPackage( const char* pszPackage ){
	if( pszPackage == NULL ){
		return;
	}

	std::string strPackage = pszPackage;

	m_vstrPackages.push_back( strPackage );
}

// clear packages
void PluginCall::clearPackages( void ){
	m_vstrPackages.clear();
}

// remove package
void PluginCall::removePackage( const char* pszPackage ){
	std::string strPackage = pszPackage;

	// vectorÇÃíÜÇ©ÇÁpszPackageÇçÌèúÇ∑ÇÈ
	std::vector< std::string >::iterator end_it = std::remove( m_vstrPackages.begin(), m_vstrPackages.end(), strPackage );
	
	// ñ{ìñÇÃà”ñ°Ç≈ÇÃçÌèúÇé¿çsÇ∑ÇÈ
	m_vstrPackages.erase( end_it, m_vstrPackages.end() );
}

// has package
bool PluginCall::hasPackage( const char* pszPackage ){
	std::string strPackage = pszPackage;

	std::vector< std::string >::iterator cIter = find( m_vstrPackages.begin(),m_vstrPackages.end() , strPackage );

	if( cIter != m_vstrPackages.end() ){
		// ë∂ç›ÇµÇƒÇ‹Ç∑
		return true;
	}

	return false;
}
