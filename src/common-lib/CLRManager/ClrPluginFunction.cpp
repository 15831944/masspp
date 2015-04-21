/**
 * @file ClrPluginFunction.cpp
 * @brief implements of ClrPluginFunction class
 *
 * @author S.Tanaka
 * @date 2006.08.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrPluginFunction.h"

#include "AssemblyManager.h"
#include "ClrObjectTool.h"
#include "ClrParameters.h"
#include "ClrVariant.h"
#include "ClrObjectPool.h"

#include <string>
#include <boost/bind.hpp>


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PARAMETER_CLASS_NAME   "kome.clr.ClrParameters"


// static member
unsigned int ClrPluginFunction::CLR_PLUGIN_CALL_ID_COUNTER = 0;


// constructor
ClrPluginFunction::ClrPluginFunction() {
	m_id = CLR_PLUGIN_CALL_ID_COUNTER;
	CLR_PLUGIN_CALL_ID_COUNTER++;

	m_funcFlag = false;
}

// destructor
ClrPluginFunction::~ClrPluginFunction() {
	ClrObjectPool::getInstance().removePluginFunction( *this );
}

// get ID
unsigned int ClrPluginFunction::getId() {
	return m_id;
}

// set method information
void ClrPluginFunction::setMethodInfo(
		const char* dllPath,
		const char* className,
		const char* methodName,
		bool isStatic
) {
	// keep parameters
	m_dllPath = std::string( NVL( dllPath, "" ) );
	m_className = std::string( NVL( className, "" ) );
	m_methodName = std::string( NVL( methodName, "" ) );
	m_isStatic = isStatic;

	// initialize
	AssemblyManager::getInstance()->setMethod( m_id, nullptr );

	m_funcFlag = false;
}

// get the pointer to the function
PLUGIN_FUNCTION* ClrPluginFunction::getFunction( bool ifloaded ) {
	// check flag
	if( m_funcFlag ) {
		&m_func;
	}

	// get module name
	try{
		// get assembly
		System::Reflection::Assembly^ assembly = nullptr;
		if( ifloaded ) {
			assembly = AssemblyManager::getInstance()->getAssembly( m_dllPath.c_str() );
			if( assembly == nullptr ) {
				return NULL;
			}
		}
		else {
			LOG_TRACE( FMT( "Getting assembly... [%s]", m_dllPath.c_str() ) );
			assembly = AssemblyManager::getInstance()->loadAssembly( m_dllPath.c_str() );
			if( assembly == nullptr ) {
				LOG_ERROR( FMT( "Failed to load the assembly. [%s]", m_dllPath.c_str() ) );
				return NULL;
			}
		}

		// get module
		std::string moduleName = getfilename( m_dllPath.c_str() );
		LOG_TRACE( FMT( "Getting module... [%s]", moduleName.c_str() ) );
		System::Reflection::Module^ module
			= assembly->GetModule( gcnew System::String( moduleName.c_str() ) );
		if( module == nullptr ) {
			LOG_ERROR( FMT( "Failed to get the module. [%s]", moduleName.c_str() ) );
			return NULL;
		}

		// get type
		LOG_TRACE( FMT( "Getting type... [%s]", m_className.c_str() ) );
		System::Type^ type = module->GetType( gcnew System::String( m_className.c_str() ) );
		if( type == nullptr ) {
			LOG_ERROR( FMT( "Failed to get the type. [%s]", m_className.c_str() ) );
			return NULL;
		}

		// get method
		LOG_TRACE( FMT( "Getting method... [%s]", m_methodName.c_str() ) );
		cli::array<System::Type^>^ argTypes = { System::Type::GetType( gcnew System::String( PARAMETER_CLASS_NAME ) ) };
		System::Reflection::MethodInfo^ method = type->GetMethod( gcnew System::String( m_methodName.c_str() ), argTypes );
		if( method == nullptr ) {
			LOG_ERROR( FMT( "Failed to get the method. [%s]", m_methodName.c_str() ) );
			return NULL;
		}

		// set type and method
		AssemblyManager::getInstance()->setType( m_id, type );
		AssemblyManager::getInstance()->setMethod( m_id, method );

		// set function
		m_func = boost::bind( &ClrPluginFunction::invoke, this, _1 );
		m_funcFlag = true;
	}
	catch( System::Exception^ e ) {
		LOG_ERROR( ClrObjectTool::convertString( e->ToString() ) );
	}

	return &m_func;
}

// invoke
kome::objects::Variant ClrPluginFunction::invoke( kome::objects::Parameters* params ) {
	// log
	LOG_TRACE(
		FMT(
			"Calling CLR method (%s::%s) [%s]",
			m_className.c_str(),
			m_methodName.c_str(),
			getfilename( m_dllPath.c_str() ).c_str()
		)
	);

	// Variant
	kome::objects::Variant retVal;

	try{
		// get method info
		System::Reflection::MethodInfo^ method = AssemblyManager::getInstance()->getMethod( m_id );
		if( method == nullptr ) {
			LOG_ERROR(
				FMT(
					"There is no method information. Failed to call the method."
				)
			);
			return retVal;
		}

		// create instance
		System::Object^ obj = nullptr;
		if( !m_isStatic ) {
			System::Type^ type = AssemblyManager::getInstance()->getType( m_id );
			if( type == nullptr ) {
				LOG_ERROR(
					FMT(
						"There is no type information. Failed to call the method."
					)
				);
				return retVal;
			}

			obj = System::Activator::CreateInstance( type );
			if( obj == nullptr ) {
				LOG_ERROR(
					FMT(
						"There is no type information. Failed to call the method."
					)
				);
				return retVal;
			}
		}

		// Parameters -> ClrParameters
		ClrParameters^ clrParams = nullptr;
		if( params != NULL ) {
			clrParams = gcnew ClrParameters( *params );
		}
		cli::array< ClrParameters^ >^ p = { clrParams };

		// invoke
		System::Object^ retObj = method->Invoke( obj, p );
		ClrVariant^ ret = (ClrVariant^)retObj;

		// ClrVariant -> Variant
		ret->transmit( retVal );
	}
	catch( System::Exception^ e ) {
		LOG_ERROR(
			ClrObjectTool::convertString( e->ToString() )
		);				
	}

	return retVal;
}


// create new instance
ClrPluginFunction* ClrPluginFunction::createNewInstance() {

	// object pool
	ClrObjectPool& pool = ClrObjectPool::getInstance();

	// create object
	ClrPluginFunction& fun = pool.createPluginFunction();

	return &fun;
}
