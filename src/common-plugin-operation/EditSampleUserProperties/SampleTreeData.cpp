/**
 * @file SampleTreeData.cpp
 * @brief implements of SampleTreeData
 *
 * @author H.Parry
 * @date 2011.04.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "SampleTreeData.h"
#include "SampleTreeManager.h"


using namespace kome::properties;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SampleTreeData::SampleTreeData() {
}

// destructor
SampleTreeData::~SampleTreeData() {
}

// data type
kome::objects::Variant& SampleTreeData::getData() {
	return m_data;
}
