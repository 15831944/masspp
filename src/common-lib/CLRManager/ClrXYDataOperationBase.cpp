/**
 * @file ClrXYDataOperationBase.cpp
 * @brief implements of ClrXYDataOperationBase class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrXYDataOperationBase.h"

#include "ClrXYDataOperation.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrXYDataOperationBase::ClrXYDataOperationBase()
		: XYDataOperationWrapper( ClrObjectPool::getInstance().createOperation() ) {
	( ( ClrXYDataOperation& )m_operation ).setBaseOperation( this );
}

// destructor
ClrXYDataOperationBase::~ClrXYDataOperationBase() {
	ClrObjectPool::getInstance().destroyOperation( (ClrXYDataOperation&)m_operation );	
}
