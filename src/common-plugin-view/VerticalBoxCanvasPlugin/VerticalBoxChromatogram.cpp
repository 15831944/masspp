// VerticalBoxChromatogram.cpp
/**
 * @file VerticalBoxChromatogram.cpp
 * @brief VerticalBox Chromatogram
 * @author OKADA, H.
 * @date 2010.11
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 * 
 */

#include "stdafx.h"
#include "VerticalBoxChromatogram.h"
#include "VerticalBoxCanvasPluginFunctions.h"	// @date 2010/12/06 <Add> OKADA

#include <math.h>

using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
VerticalBoxChromatogram::VerticalBoxChromatogram() : Chromatogram( NULL ) {
}

// destructor
VerticalBoxChromatogram::~VerticalBoxChromatogram() {
}

// add chromatogram
void VerticalBoxChromatogram::addChromatogram( Chromatogram* chrom ){
	// check the chromatogram
	if( chrom == NULL || searchChromatogram( chrom ) >= 0 ) {
		return;
	}

	// add
	m_chroms.push_back( chrom );

	// set group
	setGroup( m_chroms[ 0 ]->getGroup() );
}

// clear chromatograms
void VerticalBoxChromatogram::clearChromatograms() {
	m_chroms.clear();
	setGroup( NULL );
}

// remove chromatogram
void VerticalBoxChromatogram::removeChromatogram( Chromatogram* chrom ) {
	// search
	int idx = searchChromatogram( chrom );
	if( idx < 0 ) {
		return;
	}

	// remove
	m_chroms.erase( m_chroms.begin() + idx );

	// set group
	setGroup( m_chroms.size() == 0 ? NULL : m_chroms[ 0 ]->getGroup() );
}

// search chromatogram
int VerticalBoxChromatogram::searchChromatogram( Chromatogram* chrom ) {
	// search
	for( int i = 0; i < (int)m_chroms.size(); i++ ) {
		if( chrom == m_chroms[ i ] ) {
			return i;
		}
	}
	return -1;
}

// get xy data
void VerticalBoxChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// check the member
	if( m_chroms.size() == 0 ) {
		return;
	}

	// min, max
	double minX = 0.0;
	double maxX = 0.0;
	for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
		kome::objects::Chromatogram* chrom = m_chroms[ i ];
		//get data points
		kome::core::DataPoints dps;
		chrom->getXYData( &dps, chrom->getOperationFlag() );

		double tmpMinX = dps.getMinX();
		double tmpMaxX = dps.getMaxX();

		if( i == 0 || tmpMinX < minX ) {
			minX = tmpMinX;
		}

		if( i == 0 || tmpMaxX > maxX ) {
			maxX = tmpMaxX;
		}
	}

	// index
	const double spacing = 0.2;
	const int startIdx = max( 0, roundnum( minX / spacing ) );
	const int endIdx = max( 0, roundnum( maxX / spacing ) );

	// length
	const int len = endIdx - startIdx + 1;
	if( len <= 0 ) {
		return;
	}

	// array
	bool* flags = new bool[ len ];
	double* ints = new double[ len ];

	for( int i = 0; i < len; i++ ) {
		flags[ i ] = false;
		ints[ i ] = 0.0;
	}

	// fill array
	for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
		kome::objects::Chromatogram* chrom = m_chroms[ i ];
		// get data point
		kome::core::DataPoints dps;
		chrom->getXYData( &dps, chrom->getOperationFlag() );

		for( unsigned int j = 0; j < dps.getLength(); j++ ){
			const double x = dps.getX( j );
			const double y = dps.getY( j );

			int idx = roundnum( x / spacing );
			idx = CLAMP( idx, startIdx, endIdx ) - startIdx;

			flags[ idx ] = true;
			if( fabs( y ) > fabs( ints[ idx ] ) ) {
				ints[ idx ] = y;
			}
		}
	}

	// add array
	for( int i = 0; i < len; i++ ) {
		if( flags[ i ] ) {
			double x = spacing * ( i + startIdx );
			double y = ints[ i ];

			xyData->addPoint( x, y );
		}
	}

	delete[] ints;
	delete[] flags;
}


// on get spectra
void VerticalBoxChromatogram::onGetSpectra(
		DataSet& dataSet,
		const double startRt,
		const double endRt,
		const SearchType startSearch,
		const SearchType endSearch
) {
}

// on get mass
double VerticalBoxChromatogram::onGetMass( const unsigned int index ) {
	return -1.0;
}

// on get MS stage
int VerticalBoxChromatogram::onGetMsStage( const unsigned int index ) {
	return -1;
}

// on get precursor
double VerticalBoxChromatogram::onGetPrecursor( const unsigned int index ) {
	return -1.0;
}
