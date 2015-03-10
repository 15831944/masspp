/**
 * @file OperationManagerFunctions.cpp
 * @brief implements of OperationManager plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "OperationManagerFunctions.h"
#include "OperationManager.h"


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// open operations dialog
kome::objects::Variant openOperationDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// open dialog
	OperationManager& mgr = OperationManager::getInstance();
	ret.prim.boolVal = mgr.openDialog();

	return ret;
}
