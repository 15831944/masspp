/**
 * @file AssemblyManager.cpp
 * @brief implements of AssemblyManager class
 *
 * @author S.Tanaka
 * @date 2006.06.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "AssemblyManager.h"
#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
AssemblyManager::AssemblyManager() {
}

// destructor
AssemblyManager::~AssemblyManager() {
}

// get assembly
System::Reflection::Assembly^ AssemblyManager::getAssembly( const char* dllPath ) {
	// check parameter
	if( dllPath == NULL ) {
		LOG_WARN( FMT( "dll path is null." ) );
		return nullptr;
	}

	// get absolute path
	std::string abspath = absolutepath( dllPath );
	System::String^ s = gcnew System::String( abspath.c_str() );

	// get assembly
	System::Reflection::Assembly^ assembly = nullptr;

	if( m_assemblyTable.Contains( s ) ) {
		assembly = (System::Reflection::Assembly^)m_assemblyTable[ s ];
	}

	return assembly;
}

// load assembly
System::Reflection::Assembly^ AssemblyManager::loadAssembly( const char* dllPath ) {
	// check parameter
	if( dllPath == NULL ) {
		LOG_WARN( FMT( "dll path is null." ) );
		return nullptr;
	}

	// get assembly
	std::string abspath = absolutepath( dllPath );
	System::String^ s = gcnew System::String( abspath.c_str() );

	System::Reflection::Assembly^ assembly = getAssembly( abspath.c_str() );
	if( assembly != nullptr ) {
		return assembly;
	}

	// file check
	if( !fileexists( dllPath ) ) {
		 LOG_ERROR( FMT( "File not found. (%s)", abspath.c_str() ) );
		return nullptr;
	}

	// load
	try{
		LOG_INFO( FMT( "Loading dynamic link library. (%s)", abspath.c_str() ) );
		assembly = System::Reflection::Assembly::LoadFrom( s );
		m_assemblyTable[ s ] = assembly;
	}
	catch( System::Exception^ e ) {
		LOG_ERROR( ClrObjectTool::convertString( e->ToString() ) );
		return nullptr;
	}

	if( assembly == nullptr ) {
		LOG_WARN( FMT( "Failed to get the assembly (%s).", abspath.c_str() ) );
	}

	return assembly;
}

// set type 
void AssemblyManager::setType( unsigned int id, System::Type^ type ) {
	m_typeTable[id] = type;
}

// get type
System::Type^ AssemblyManager::getType( unsigned int id ) {
	if( m_typeTable.Contains( id ) ) {
		return (System::Type^)m_typeTable[id];
	}
	return nullptr;
}

// set method
void AssemblyManager::setMethod( unsigned int id, System::Reflection::MethodInfo^ method ) {
	m_methodTable[ id ] = method;
}

// get method 
System::Reflection::MethodInfo^ AssemblyManager::getMethod( unsigned int id ) {
	if( m_methodTable.Contains( id ) ) {
		return (System::Reflection::MethodInfo^)m_methodTable[id];
	}
	return nullptr;
}

// get instance
AssemblyManager^ AssemblyManager::getInstance() {
	if( m_instance == nullptr ) {
		LOG_TRACE( FMT( "Creating AssemblyManager object" ) );
		m_instance = gcnew AssemblyManager();
	}

	return m_instance;
}
