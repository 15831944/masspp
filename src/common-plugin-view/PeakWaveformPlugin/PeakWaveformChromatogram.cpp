/**
 * @file PeakWaveformChromatogram.cpp
 * @brief interfaces of PeakWaveformChromatogram class
 * 
 * @author M.Izumi
 * @date 2013.02.26
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved. 
 */

#include "stdafx.h"
#include "PeakWaveformChromatogram.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
PeakWaveformChromatogram::PeakWaveformChromatogram( kome::objects::Chromatogram& chrom )
	: kome::objects::Chromatogram( chrom.getSample() ), m_orgChrom( chrom ){

	m_op = true;
}

// destructor
PeakWaveformChromatogram::~PeakWaveformChromatogram(){
}

// on get xy data
void PeakWaveformChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// get data point
	kome::core::DataPoints dps;
	m_orgChrom.getXYData( &dps, true );

	// add to data
	xyData->clearPoints();

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	if( pkMgr.hasPeaks( &m_orgChrom ) ){
		kome::objects::Peaks* peaks = pkMgr.getPeaks( &m_orgChrom );

		if( peaks != NULL ){
			for( unsigned int i = 0; i < dps.getLength(); i++ ){
				kome::objects::PeakElement* peakElt = peaks->getPeak( i );
				if( peakElt != NULL ){
					xyData->addPoint( peakElt->getX(), peakElt->getY() );
				}
			}
		}
	}
}

// on get spectra
void PeakWaveformChromatogram::onGetSpectra(
		kome::objects::DataSet& dataSet,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
}

// on get mass
double PeakWaveformChromatogram::onGetMass( const unsigned int index ) {
	return -1.0;
}

// on get ms stage
int PeakWaveformChromatogram::onGetMsStage( const unsigned int index ) {
	return -1;
}

// on get precursor
double PeakWaveformChromatogram::onGetPrecursor( const unsigned int index ) {
	return -1.0;
}
