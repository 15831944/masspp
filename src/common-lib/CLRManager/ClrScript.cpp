/**
 * @file ClrScript.cpp
 * @brief implements of ClrScript class
 *
 * @author S.Tanaka
 * @date 2012.07.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrScript.h"

#include "ClrScriptBase.h"
#include "ClrObjectTool.h"
#include "ClrObjectPool.h"

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
#include "ClrParameters.h"
#include "ClrVariant.h"
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrScript::ClrScript() {
}

// destructor
ClrScript::~ClrScript() {
	m_baseScript = nullptr;
	ClrObjectPool::getInstance().removeScript( *this );
}

// set base script
void ClrScript::setBaseScript( ClrScriptBase^ baseScript ) {
	m_baseScript = baseScript;
}

// get base script
ClrScriptBase^ ClrScript::getBaseScript() {
	ClrScriptBase^ baseScript = m_baseScript;
	return baseScript;
}

// >>>>>>	@Date:2013/07/29	<Modify>	A.Ozaki
//
// on run
std::string ClrScript::onRun( const char* script, kome::objects::Variant &var, kome::objects::Parameters *pParams ) {
	std::string ret;

	ClrScriptBase^ scr = m_baseScript;
	if( scr != nullptr ) {
		System::String^ s = ClrObjectTool::convertString( script, NULL );
		ClrVariant	^pCLRVar = nullptr;
		pCLRVar = gcnew ClrVariant( var );
		ClrParameters^ pCLRParams = nullptr;
		if  ( pParams != NULL )
		{
			pCLRParams = gcnew ClrParameters( *pParams );
		}
		ClrVariant	%insCLRVariant = *pCLRVar;

		System::String^ r = scr->onRun( s, insCLRVariant, pCLRParams );

		ret = ClrObjectTool::convertString( r );
	}

	return ret;
}
//
// <<<<<<	@Date:2013/07/29	<Modify>	A.Ozaki

// on get template
std::string ClrScript::onGetTemplate() {
	std::string ret;

	ClrScriptBase^ scr = m_baseScript;
	if( scr != nullptr ) {
		System::String^ t = scr->onGetTemplate();

		ret = ClrObjectTool::convertString( t );
	}

	return ret;
}
