/**
 * @file Ab3dComposition.cpp
 * @brief implements of Ab3dComposition class
 *
 * @author S.Tanaka
 * @date 2007.09.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dComposition.h"

#include "Ab3dSampleSet.h"


using namespace kome::ab3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Ab3dComposition::Ab3dComposition(
		kome::objects::DataSet& ctrlSpectra,
		kome::objects::DataSet& trmtSpectra
) : m_ctrlSpectra( ctrlSpectra ), m_trmtSpectra( trmtSpectra ) {
}

// destructor
Ab3dComposition::~Ab3dComposition() {
}

// create composition data
kome::objects::Sample* Ab3dComposition::createCompositionData(
			kome::objects::SettingParameterValues* settings,
			kome::operation::Alignment* alignment,
			kome::core::Progress* progress
) {
	// create object
	Ab3dSampleSet* sampleSet = new Ab3dSampleSet(
		m_ctrlSpectra,
		m_trmtSpectra,
		settings,
		alignment,
		*progress
	);

	if( progress->isStopped() || !sampleSet->isOpened() ) {
		delete sampleSet;
		return NULL;
	}

	return sampleSet->getSample( 0 );
}
