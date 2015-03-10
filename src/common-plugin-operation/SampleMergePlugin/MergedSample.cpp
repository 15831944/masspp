/**
 * @file MergedSample.cpp
 * @brief implement of MergedSample class
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MergedSample.h"
#include "MergedSampleSet.h"
#include "MergedSpectrum.h"


using namespace kome::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MergedSample::MergedSample( MergedSampleSet* sampleSet )
		: kome::objects::Sample( sampleSet ) {
	m_sampleSet = sampleSet;
}

// destructor
MergedSample::~MergedSample() {
}

// get SAMPLE_MERGE sample set
MergedSampleSet* MergedSample::getMergedSampleSet() {
	return m_sampleSet;
}

// on open sample
bool MergedSample::onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress ) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// check the member
	if( m_sampleSet == NULL ) {
		return true;
	}

	// spectra
	std::vector< kome::objects::Spectrum* > spectra;
	for( unsigned int i = 0; i < m_sampleSet->getNumberOfMeargeSamples(); i++ ) {    // samples
		kome::objects::Sample* sample = m_sampleSet->getMergeSample( i );
		kome::objects::DataSet* ds = aoMgr.getFilteredDataSet( sample );

		for( unsigned int j = 0; j < ds->getNumberOfSpectra(); j++ ) {
			spectra.push_back( ds->getSpectrum( j ) );
		}
	}

	std::sort( spectra.begin(), spectra.end(), lessSpec );

	// merged spectra
	MergedSpectrum* mergedSpec = NULL;
	double prevRt = 0.0;
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		kome::objects::Spectrum* spec = spectra[ i ];

		if( mergedSpec == NULL || spec->getRt() > prevRt + m_sampleSet->getRtTol() ) {	// new spectrum
			mergedSpec = new MergedSpectrum( *this );
			prevRt = spec->getRt();
			rootGroup->addSpectrum( mergedSpec );
		}

		mergedSpec->addSpectrum( spec );
		mergedSpec->setMergeDistance( m_sampleSet->getMzTol() );
		mergedSpec->setAutoZeroPoints( false );
	}

	return true;
}
 
// on close file
bool MergedSample::onCloseSample() {
	return true;
}

// compare spectra
bool MergedSample::lessSpec( kome::objects::Spectrum* s0, kome::objects::Spectrum* s1 ) {
	return ( s0->getRt() < s1->getRt() );
}
