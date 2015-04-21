/**
 * @file ClrScriptBase.cpp
 * @brief implements of ClrScriptBase class
 *
 * @author S.Tanaka
 * @date 2012.07.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrScriptBase.h"

#include "ClrScript.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrScriptBase::ClrScriptBase() : ScriptWrapper( ClrObjectPool::getInstance().createScript() ) {
	( (ClrScript&)m_script ).setBaseScript( this );
}

// destructor
ClrScriptBase::~ClrScriptBase() {
	ClrObjectPool::getInstance().destroyScript( (ClrScript&)m_script );
}
