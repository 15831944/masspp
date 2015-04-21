/**
 * @file ClrDataManagerBase.cpp
 * @brief implements of ClrDataManagerBase class
 *
 * @author S.Tanaka
 * @date 2007.12.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrDataManagerBase.h"

#include "SampleWrapper.h"
#include "ClrDataManager.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrDataManagerBase::ClrDataManagerBase()
		: DataManagerWrapper( ClrObjectPool::getInstance().createDataManager() ) {
	( (ClrDataManager&)m_dataMgr ).setBaseDataManager( this );
}

// destructor
ClrDataManagerBase::~ClrDataManagerBase() {
	ClrObjectPool::getInstance().destroyDataManager( (ClrDataManager&)m_dataMgr );
}
