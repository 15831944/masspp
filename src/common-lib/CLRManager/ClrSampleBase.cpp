/**
 * @file ClrSampleBase.cpp
 * @brief implements of ClrSampleBase class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrSampleBase.h"

#include "SampleSetWrapper.h"
#include "ClrSample.h"
#include "ClrObjectPool.h"
#include "ClrSampleReader.h"
#include "SpectrumWrapper.h"
#include "PeaksWrapper.h"
#include "ClrLogger.h"

using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrSampleBase::ClrSampleBase( SampleSetWrapper^ sampleSet )
		: SampleWrapper( ClrObjectPool::getInstance().createSample( sampleSet ) ) {
	( (ClrSample&)m_sample).setBaseSample( this );
}

// destructor
ClrSampleBase::~ClrSampleBase() {
	ClrObjectPool::getInstance().destroySample( (ClrSample&)m_sample );
}

// detect peaks using API
void ClrSampleBase::onDetectPeaksByAPI( SpectrumWrapper^ spec, PeaksWrapper^ peaks ) {
	// log
	ClrLogger^ logger = ClrLogger::getInstance();

	logger->warn( "This file type does not support API peak detection." );
}

// set supported API peaks
void ClrSampleBase::setSupportedAPIPeaks( bool supported ) {
	m_sample.setSupportedAPIPeaks( supported );
}
