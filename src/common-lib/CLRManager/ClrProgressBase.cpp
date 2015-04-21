/**
 * @file ClrProgressBase.cpp
 * @brief implements of ClrProgressBase class
 *
 * @author S.Tanaka
 * @date 2006.11.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrProgressBase.h"

#include "ClrProgress.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrProgressBase::ClrProgressBase() : ProgressWrapper( ClrObjectPool::getInstance().createProgress() ) {
	( (ClrProgress&)m_progress ).setBaseProgress( this );
}

// destructor
ClrProgressBase::~ClrProgressBase() {
	ClrObjectPool::getInstance().destroyProgress( (ClrProgress&)m_progress );
}
