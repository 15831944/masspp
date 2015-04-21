/**
 * @file ClrGraphicsBase.cpp
 * @brief implements of ClrGraphicsBase class
 *
 * @author S.Tanaka
 * @date 2006.09.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrGraphicsBase.h"

#include "ClrGraphics.h"
#include "ClrObjectPool.h"
#include "BitmapDataWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrGraphicsBase::ClrGraphicsBase() : GraphicsWrapper( ClrObjectPool::getInstance().createGraphics() ) {
	( (ClrGraphics&)m_graphics ).setBaseGraphics( this );
}

// destructor
ClrGraphicsBase::~ClrGraphicsBase() {
	ClrObjectPool::getInstance().destroyGraphics( (ClrGraphics&)m_graphics );
}
