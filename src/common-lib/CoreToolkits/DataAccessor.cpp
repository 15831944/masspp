/**
 * @file DataAccessor.cpp
 * @brief implements of DataAccessor class
 *
 * @author S.Tanaka
 * @date 2008.08.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataAccessor.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DataAccessor::DataAccessor() {
}

// destructor
DataAccessor::~DataAccessor() {
}
