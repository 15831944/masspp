/**
 * @file IronPythonManager.cpp
 * @brief implements of IronPythonManager class
 *
 * @author S.Tanaka
 * @date 2012.07.11
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "IronPythonManager.h"
#include "IronPythonScript.h"


using namespace kome::script::python;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
IronPythonManager::IronPythonManager() {
	m_script = nullptr;
}

// destructor
IronPythonManager::~IronPythonManager() {
}

// get script
IronPythonScript^ IronPythonManager::getScript() {
	if( m_script == nullptr ) {
		m_script = gcnew IronPythonScript();
	}

	return m_script;
}

// get the instance
IronPythonManager^ IronPythonManager::getInstance() {
	if( m_mgr == nullptr ) {
		m_mgr = gcnew IronPythonManager();
	}

	return m_mgr;
}

// get the script
kome::clr::ClrVariant^ IronPythonManager::getScript( kome::clr::ClrParameters^ params ) {
	// manager
	IronPythonManager^ mgr = getInstance();

	// script
	IronPythonScript^ script = mgr->getScript();

	// return value
	kome::clr::ClrVariant^ ret = gcnew kome::clr::ClrVariant();
	ret->type = kome::clr::ClrVariant::DataType::SCRIPT;
	ret->obj = script;

	return ret;
}
