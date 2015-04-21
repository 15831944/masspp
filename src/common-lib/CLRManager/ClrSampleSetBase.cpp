/**
 * @file ClrSampleSetBase.cpp
 * @brief implements of ClrSampleSetBase class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrSampleBase.h"

#include "ClrSampleSet.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrSampleSetBase::ClrSampleSetBase()
		: SampleSetWrapper( ClrObjectPool::getInstance().createSampleSet() ){
	( (ClrSampleSet&)m_sampleSet ).setBaseSampleSet( this );
}

// destructor
ClrSampleSetBase::~ClrSampleSetBase() {
	ClrObjectPool::getInstance().destroySampleSet( (ClrSampleSet&)m_sampleSet );
}

