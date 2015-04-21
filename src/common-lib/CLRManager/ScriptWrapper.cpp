/**
 * @file ScriptWrapper.cpp
 * @brief implements of ScriptWrapper class
 *
 * @author S.Tanaka
 * @date 2012.07.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ScriptWrapper.h"

#include "ClrObjectTool.h"

// >>>>>>	@Date:2013/07/24	<Modify>	A.Ozaki
//
// Moved the ScriptClass to DataObjects library from CoreToolkits library.
//  ex.
//   kome::core::Script -> kome::objects::Script
//
// <<<<<<	@Date:2013/07/24	<Modify>	A.Ozaki

using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ScriptWrapper::ScriptWrapper( kome::objects::Script& script )
		: m_script( script ) {
}

// destructor
ScriptWrapper::~ScriptWrapper() {
}

// get script object
kome::objects::Script& ScriptWrapper::getScript() {
	return m_script;
}

// get template
System::String^ ScriptWrapper::getTemplate() {
	std::string t = m_script.getTemplate();

	return ClrObjectTool::convertString( t.c_str(), NULL );
}

// >>>>>>	@Date:2013/07/29	<Modify>	A.Ozaki
// run
System::String^ ScriptWrapper::run( System::String^ script, ClrVariant CLRVar, ClrParameters^ pCLRParams ) {
	std::string s = ClrObjectTool::convertString( script );
	kome::objects::Variant	*pVal = (kome::objects::Variant *)NULL;
	kome::objects::Parameters	*pParams = (kome::objects::Parameters *)NULL;
	ClrObjectTool::convertVariantFromCLR( pVal, %CLRVar );
	ClrObjectTool::convertParamsFromCLR( pParams, pCLRParams );

	m_script.setScript( s.c_str( ) );
	m_script.setVariant( *pVal );
	m_script.setParameters( pParams );

	std::string ret = m_script.run( s.c_str(), *pVal, pParams );

	if  ( (kome::objects::Variant *)NULL != pVal )
	{
		delete	pVal;
	}
	if  ( (kome::objects::Parameters *)NULL != pParams )
	{
		delete	pParams;
	}

	return ClrObjectTool::convertString( ret.c_str(), NULL );
}
// <<<<<<	@Date:2013/07/29	<Modify>	A.Ozaki

// run
System::String^ ScriptWrapper::run() {
	std::string ret = m_script.run();
	return ClrObjectTool::convertString( ret.c_str(), NULL );
}

// clear variables
void ScriptWrapper::clearVariables() {
	m_script.clearVariables();
}

// add variable
void ScriptWrapper::addVariable( System::String^ name, System::String^ desc ) {
	m_script.addVariable(
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( desc ).c_str()
	);
}

// get the number of variables
unsigned int ScriptWrapper::getNumberOfVariables() {
	return m_script.getNumberOfVariables();
}

// get varible name
System::String^ ScriptWrapper::getVariableName( unsigned int idx ) {
	return ClrObjectTool::convertString( m_script.getVariableName( idx ), NULL );
}

// get variable description
System::String^ ScriptWrapper::getVariableDescription( unsigned int idx ) {
	return ClrObjectTool::convertString( m_script.getVariableDescription( idx ), NULL );
}

// set script file path
void ScriptWrapper::setScriptFilePath( System::String^ path ) {
	kome::objects::Script::setScriptFilePath( ClrObjectTool::convertString( path ).c_str() );
}

// get script file path
System::String^ ScriptWrapper::getScriptFilePath() {
	return ClrObjectTool::convertString( kome::objects::Script::getScriptFilePath(), NULL );
}
