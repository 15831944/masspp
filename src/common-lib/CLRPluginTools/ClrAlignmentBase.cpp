/**
 * @file ClrAlignmentBase.cpp
 * @brief implements of ClrAlignmentBase class
 *
 * @author S.Tanaka
 * @date 2012.11.07
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ClrAlignmentBase.h"

#include "ClrAlignment.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrAlignmentBase::ClrAlignmentBase()
		: AlignmentWrapper( ClrAlignmentManager::getInstance().createAlignment() ) {
}

// destructor
ClrAlignmentBase::~ClrAlignmentBase() {
}
