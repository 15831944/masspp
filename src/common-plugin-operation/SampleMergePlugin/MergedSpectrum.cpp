/**
 * @file MergedSpectrum.cpp
 * @brief implements of MergedSpectrum class
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MergedSpectrum.h"
#include "MergedSampleSet.h"
#include "MergedSample.h"


using namespace kome::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MergedSpectrum::MergedSpectrum( MergedSample& sample )
		: kome::objects::AveragedSpectrum( sample.getRootDataGroupNode() ) {
	setHasChromatogram( true );
}

// destructor
MergedSpectrum::~MergedSpectrum() {
}
