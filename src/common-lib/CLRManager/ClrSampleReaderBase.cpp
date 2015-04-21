/**
* @file ClrSampleReaderBase.cpp
* @brief Clr Sample Reader Base
* @author OKADA, H.
* @date 2012/07/18
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
* <•â‘«à–¾>
*/


#include "ClrSampleReaderBase.h"
#include "ClrObjectPool.h"		// @date 2012/08/22 <Add> OKADA

#include "ClrSampleReader.h"	// @date 2012/08/23 <Add> OKADA

using kome::clr::ClrSampleReaderBase;


// constructor
ClrSampleReaderBase::ClrSampleReaderBase()
	: SampleReaderWrapper( ClrObjectPool::getInstance().createSampleReader() )
{
	((ClrSampleReader&)m_sampleReader).setBaseSampleReader( this );
}


// destructor
ClrSampleReaderBase::~ClrSampleReaderBase(){
}

