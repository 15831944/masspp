/**
 * @file MergedSpectrumManager.cpp
 * @brief implements of MergedSpectrum class
 *
 * @author S.Tanaka
 * @date 2009.01.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MergedSpectrum.h"
#include "MergedSpectrumManager.h"

#include <float.h>


using namespace kome::operation::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MergedSpectrum::MergedSpectrum( kome::objects::DataGroupNode* group )
		: kome::objects::AveragedSpectrum( group ) {
	// manager
	MergedSpectrumManager& mgr = MergedSpectrumManager::getInstance();

	// add
	mgr.addMergedSpectrum( this );
}

// destructor
MergedSpectrum::~MergedSpectrum() {
	// manager
	MergedSpectrumManager& mgr = MergedSpectrumManager::getInstance();

	// remove
	mgr.removeMergedSpectrum( this );
}

// on get xy data
void MergedSpectrum::onGetXYData(
		kome::core::XYData* const xyData,
		const double minX,
		const double maxX
) {
	// set merge distance
	const unsigned int num = getNumberOfSpectra();
	if( num > 0 ) {
		double d = FLT_MAX;
		for( unsigned int i = 0; i < num; i++ ) {
			kome::objects::Spectrum* spec = getSpectrum( i );
			d = std::min( d, spec->getResolution() );
		}
		setMergeDistance( d );
	}

	// default
	kome::objects::AveragedSpectrum::onGetXYData( xyData, minX, maxX );
}
