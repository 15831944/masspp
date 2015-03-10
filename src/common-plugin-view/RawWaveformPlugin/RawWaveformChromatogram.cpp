/**
 * @file RawWaveformChromatogram.cpp
 * @brief interfaces of RawWaveformChromatogram class
 * 
 * @author M.Izumi
 * @date 2013.02.26
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved. 
 */

#include "stdafx.h"
#include "RawWaveformChromatogram.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
RawWaveformChromatogram::RawWaveformChromatogram( kome::objects::Chromatogram& chrom )
	: kome::objects::Chromatogram( chrom.getSample() ), m_orgChrom( chrom ){

	m_op = false;
}

// destructor
RawWaveformChromatogram::~RawWaveformChromatogram(){
}

// on get xy data
void RawWaveformChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// get data point
	kome::core::DataPoints dps;
	m_orgChrom.getXYData( &dps, false );

	// add to data
	xyData->clearPoints();
	for( unsigned int i = 0; i < dps.getLength(); i++ ){
		xyData->addPoint( dps.getX( i ), dps.getY( i ) );
	}
}

// on get spectra
void RawWaveformChromatogram::onGetSpectra(
		kome::objects::DataSet& dataSet,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
}

// on get mass
double RawWaveformChromatogram::onGetMass( const unsigned int index ) {
	return -1.0;
}

// on get ms stage
int RawWaveformChromatogram::onGetMsStage( const unsigned int index ) {
	return -1;
}

// on get precursor
double RawWaveformChromatogram::onGetPrecursor( const unsigned int index ) {
	return -1.0;
}
