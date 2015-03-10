/**
 * @file CorrDummyChromatogram.cpp
 * @brief implements of CorrDummyChromatogram class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CorrDummyChromatogram.h"
#include "AlignmentManager.h"


using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
CorrDummyChromatogram::CorrDummyChromatogram()
		: kome::objects::Chromatogram( NULL ) {
}

// destructor
CorrDummyChromatogram::~CorrDummyChromatogram() {
}

// get xy data
void CorrDummyChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// xy data
	AlignmentManager& mgr = AlignmentManager::getInstance();
	kome::objects::Chromatogram* chrom = mgr.getTreatmentChrom();
	kome::core::XYData& orgData = mgr.getCorrectionPoints();

	// store points
	if( orgData.getLength() > 1 ) {
		xyData->reserve( orgData.getLength() );
		for( unsigned int i = 0; i < orgData.getLength(); i++ ) {
			xyData->addPoint( orgData.getX( i ), orgData.getY( i ) );
		}
	}
	else if( chrom != NULL ) {
		kome::core::XYData* chromPts = chrom->getXYData();

		if( chromPts != NULL ) {
			double minX = chromPts->getMinX();
			double maxX = chromPts->getMaxX();

			double midX = ( minX + maxX ) / 2.0;
			minX = std::min( midX - 0.05, minX );
			maxX = std::max( midX + 0.05, maxX );

			xyData->reserve( 2 );

			xyData->addPoint( minX, minX );
			xyData->addPoint( maxX, maxX );
		}
	}
}

// on get spectra
void CorrDummyChromatogram::onGetSpectra(
		kome::objects::DataSet& dataSet,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
}

// on get mass
double CorrDummyChromatogram::onGetMass( const unsigned int index ) {
	return -1.0;
}

// on get MS stage
int CorrDummyChromatogram::onGetMsStage( const unsigned int index ) {
	return -1;
}

// on get precursor
double CorrDummyChromatogram::onGetPrecursor( const unsigned int index ) {
	return -1.0;
}
