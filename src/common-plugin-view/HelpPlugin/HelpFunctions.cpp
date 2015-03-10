/**
 * @file HelpFunctions.cpp
 * @brief implements of log view plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "HelpFunctions.h"
#include "HelpManager.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// show help
kome::objects::Variant showHelp( kome::objects::Parameters* ) {
	// return object
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// show help
	HelpManager& mgr = HelpManager::getInstance();
	ret.prim.boolVal = mgr.showHelpWindow();

	return ret;
}

// 2010/08/16 APPEND OKADA ------->
// DLL‚ğŒÄ‚Ño‚·‚¾‚¯‚Ìˆ—
kome::objects::Variant initDllHelp( kome::objects::Parameters* ) {
	// return object
	kome::objects::Variant ret;
	
	return ret;
}
// 2010/08/16 APPEND OKADA <-------
