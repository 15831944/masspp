/**
 * @file ResultChromatogram.cpp
 * @brief implements of ResultChromatogram class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ResultChromatogram.h"
#include "AlignmentManager.h"


using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ResultChromatogram::ResultChromatogram( const bool ctrlFlg )
	: kome::objects::Chromatogram( ctrlFlg ?
			AlignmentManager::getInstance().getControlChrom()->getSample() :
			AlignmentManager::getInstance().getTreatmentChrom()->getSample() ), m_ctrlFlg( ctrlFlg ) {
}

// destructor
ResultChromatogram::~ResultChromatogram() {
}

// get xy data
void ResultChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// chromatogram
	AlignmentManager& mgr = AlignmentManager::getInstance();
	kome::objects::Chromatogram* chrom = m_ctrlFlg ? mgr.getControlChrom() : mgr.getTreatmentChrom();
	if( chrom == NULL ) {
		return;
	}
	kome::core::XYData* src = chrom->getXYData();
	if( src == NULL || src->getLength() == 0 ) {
		return;
	}

	// max intensity
	double maxInt = std::max( src->getMaxY(), 0.1 );

	// RT array
	unsigned int num = src->getLength();
	double* rts = new double[ num ];
	for( unsigned int i = 0; i < num; i++ ) {
		rts[ i ] = src->getX( i );
	}

	if( !m_ctrlFlg && mgr.getCorrectionPoints().getLength() > 0 ) {
		// get correction point
		kome::core::DataPoints dps;
		kome::numeric::Interpolation::linear( mgr.getCorrectionPoints(), dps, rts, num );

		double prevY = 0.0;
		for( unsigned int i = 0; i < num; i++ ) {
			double rt = dps.getY( i );
			rt = std::max( rt, prevY + 0.0001 );
			rts[ i ] = rt;
			prevY = rt;
		}
	}

	// xy data
	xyData->reserve( num );
	for( unsigned int i = 0; i < num; i++ ) {
		xyData->addPoint( rts[ i ], src->getY( i ) * 100.0 / maxInt );
	}

	// delete
	delete[] rts;
}

// on get spectra
void ResultChromatogram::onGetSpectra(
		kome::objects::DataSet& dataSet,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
	// chromatogram
	AlignmentManager& mgr = AlignmentManager::getInstance();
	kome::objects::Chromatogram* chrom = m_ctrlFlg ? mgr.getControlChrom() : mgr.getTreatmentChrom();
	if( chrom == NULL ) {
		return;
	}

	// get spectra
	chrom->getSpectra( dataSet, startRt, endRt, startSearch, endSearch );
}

// on get mass
double ResultChromatogram::onGetMass( const unsigned int index ) {
	// chromatogram
	AlignmentManager& mgr = AlignmentManager::getInstance();
	kome::objects::Chromatogram* chrom = m_ctrlFlg ? mgr.getControlChrom() : mgr.getTreatmentChrom();
	if( chrom == NULL ) {
		return -1.0;
	}

	return chrom->getMass( index );
}

// on get MS stage
int ResultChromatogram::onGetMsStage( const unsigned int index ) {
	// chromatogram
	AlignmentManager& mgr = AlignmentManager::getInstance();
	kome::objects::Chromatogram* chrom = m_ctrlFlg ? mgr.getControlChrom() : mgr.getTreatmentChrom();
	if( chrom == NULL ) {
		return -1;
	}

	return chrom->getMsStage( index );
}

// on get precursor
double ResultChromatogram::onGetPrecursor( const unsigned int index ) {
	// chromatogram
	AlignmentManager& mgr = AlignmentManager::getInstance();
	kome::objects::Chromatogram* chrom = m_ctrlFlg ? mgr.getControlChrom() : mgr.getTreatmentChrom();
	if( chrom == NULL ) {
		return -1.0;
	}

	return chrom->getPrecursor( index );
}
