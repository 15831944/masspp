/**
 * @file WsdlOperation.cpp
 * @brief implements of WsdlOperation class
 *
 * @author S.Tanaka
 * @date 2009.08.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "WsdlOperation.h"


using namespace kome::net;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
WsdlOperation::WsdlOperation( const char* name ) {
	m_name = NVL( name, "" );
}

// destructor
WsdlOperation::~WsdlOperation() {
}

// get operation name
const char* WsdlOperation::getName() {
	return m_name.c_str();
}
