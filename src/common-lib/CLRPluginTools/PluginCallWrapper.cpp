/**
 * @file PluginCallWrapper.cpp
 * @brief implements of PluginCallWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginCallWrapper.h"

#include "PluginInfoWrapper.h"
#include "PluginIconWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PluginCallWrapper::PluginCallWrapper( kome::plugin::PluginCall& call )
		: m_call( call ) {
}

// destructor
PluginCallWrapper::~PluginCallWrapper() {
}

// get plug-in function object
kome::plugin::PluginCall& PluginCallWrapper::getPluginCall() {
	return m_call;
}

// get plug-in
PluginInfoWrapper^ PluginCallWrapper::getPlugin() {
	if( m_call.getPlugin() == NULL ) {
		return nullptr;
	}

	return gcnew PluginInfoWrapper( *m_call.getPlugin() );
}

// set call type 
void PluginCallWrapper::setType( System::String^ type ) {
	m_call.setType( ClrObjectTool::convertString( type ).c_str() );
}

// get call type
System::String^ PluginCallWrapper::getType() {
	return ClrObjectTool::convertString( m_call.getType(), NULL );
}

// set call name
void PluginCallWrapper::setName( System::String^ name ) {
	m_call.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get call name
System::String^ PluginCallWrapper::getName() {
	return ClrObjectTool::convertString( m_call.getName(), NULL );
}

// set icon
void PluginCallWrapper::setIcon( PluginIconWrapper^ icon ) {
	if( icon == nullptr ) {
		m_call.setIcon( NULL );
	}
	else {
		m_call.setIcon( &icon->getPluginIcon() );
	}
}

// get icon
PluginIconWrapper^ PluginCallWrapper::getIcon() {
	if( m_call.getIcon() == NULL ) {
		return nullptr;
	}
	return gcnew PluginIconWrapper( *( m_call.getIcon() ) );
}

// set order
void PluginCallWrapper::setOrder( unsigned short order ) {
	m_call.setOrder( order );
}

// get order
unsigned short PluginCallWrapper::getOrder() {
	return m_call.getOrder();
}

// add status
void PluginCallWrapper::addStatus( System::String^ name, System::String^ value ) {
	m_call.addStatus(
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( value ).c_str()
	);
}

// get number of statuses
unsigned int PluginCallWrapper::getNumberOfStatuses() {
	return m_call.getNumberOfStatuses();
}

// get status name
System::String^ PluginCallWrapper::getStatusName( unsigned int idx ) {
	return ClrObjectTool::convertString( m_call.getStatusName( idx ), NULL );
}

// get status value
System::String^ PluginCallWrapper::getStatusValue( unsigned int idx ) {
	return ClrObjectTool::convertString( m_call.getStatusValue( idx ), NULL );
}

// clear statuses
void PluginCallWrapper::clearStatuses() {
	m_call.clearStatuses();
}

// set ifloaded flag
void PluginCallWrapper::setIfLoaded( bool ifloaded ) {
	m_call.setIfLoaded( ifloaded );
}

// get ifloaded flag
bool PluginCallWrapper::getIfLoaded() {
	return m_call.getIfLoaded();
}

// set function
void PluginCallWrapper::setFunction(
		System::String^ methodName,
		System::String^ className,
		bool isStatic
){
	m_call.setFunction(
		ClrObjectTool::convertString( methodName ).c_str(),
		ClrObjectTool::convertString( className ).c_str(),
		isStatic
	);
}

// get method name
System::String^ PluginCallWrapper::getMethodName() {
	return ClrObjectTool::convertString( m_call.getMethodName(), NULL );
}

// get class name
System::String^ PluginCallWrapper::getClassName() {
	return ClrObjectTool::convertString( m_call.getClassName(), NULL );
}

// is static method
bool PluginCallWrapper::isStaticMethod() {
	return m_call.isStaticMethod();
}

// get properties
PropertiesWrapper^ PluginCallWrapper::getProperties() {
	// create object
	PropertiesWrapper^ props
		= ClrObjectTool::createPropertiesWrapper( &m_call.getProperties() );

	return props;
}

// set description
void PluginCallWrapper::setDescription( System::String^ description ) {
	m_call.setDescription( ClrObjectTool::convertString( description ).c_str() );
}

// get description
System::String^ PluginCallWrapper::getDescription() {
	return ClrObjectTool::convertString( m_call.getDescription(), NULL );
}

// check status
bool PluginCallWrapper::checkStatus() {
	return m_call.checkStatus();
}

// execute
ClrVariant^ PluginCallWrapper::invoke( ClrParameters^ params ) {
	// create parameters object
	kome::objects::Parameters p;
	ClrObjectTool::convertParamsFromCLR( &p, params );

	// create return value object
	ClrVariant^ ret = gcnew ClrVariant();

	// execute
	ClrObjectTool::convertVariantToCLR( ret, &m_call.invoke( &p ) );
	
	return ret;
}
