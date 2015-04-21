/**
 * @file OverlappingChromatogram.cpp
 * @brief implements of Overlapping Chromatogram class
 *
 * @author S.Tanaka
 * @date 2008.11.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "OverlappingChromatogram.h"

#include "Spectrum.h"
#include "DataSet.h"
#include "DataGroupNode.h"
#include "Spectrum.h"
#include "DataSet.h"

#include <math.h>


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
OverlappingChromatogram::OverlappingChromatogram() : Chromatogram( NULL ) {
	
	m_chroms.clear( );
	m_dMinX.clear( );
	m_dMaxX.clear( );
	m_dMinY.clear( );
	m_dMaxY.clear( );

	m_bFlags = NULL;
	m_dInts = NULL;
	m_nAllocLen = 0;
}

// destructor
OverlappingChromatogram::~OverlappingChromatogram() {
}

// add chromatogram
void OverlappingChromatogram::addChromatogram( Chromatogram* chrom ) {
	// check the chromatogram
	if( chrom == NULL || searchChromatogram( chrom ) >= 0 ) {
		return;
	}

	// add
	m_chroms.push_back( chrom );

	// set group
	if( m_chroms.size() == 1 ) {
		setGroup( m_chroms[ 0 ]->getGroup() );
	}
	else {
		setGroup( NULL );
	}
}

// clear chromatograms
void OverlappingChromatogram::clearChromatograms() {
	m_chroms.clear();

	m_dMinX.clear( );
	m_dMaxX.clear( );
	m_dMinY.clear( );
	m_dMaxY.clear( );
	if  ( m_bFlags )
	{
		delete[]	m_bFlags;
		m_bFlags = NULL;
	}
	if  ( m_dInts )
	{
		delete[]	m_dInts;
		m_dInts = NULL;
	}
	m_nAllocLen = 0;

	setGroup( NULL );
}

// remove chromatogram
void OverlappingChromatogram::removeChromatogram( Chromatogram* chrom ) {
	// search
	int idx = searchChromatogram( chrom );
	if( idx < 0 ) {
		return;
	}

	// remove
	m_chroms.erase( m_chroms.begin() + idx );

	// set group
	if( m_chroms.size() == 1 ) {
		setGroup( m_chroms[ 0 ]->getGroup() );
	}
	else {
		setGroup( NULL );
	}
}

// search chromatogram
int OverlappingChromatogram::searchChromatogram( Chromatogram* chrom ) {
	// search
	for( int i = 0; i < (int)m_chroms.size(); i++ ) {
		if( chrom == m_chroms[ i ] ) {
			return i;
		}
	}
	return -1;
}

// get xy data
void OverlappingChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// check the member
	if( m_chroms.size() == 0 ) {
		return;
	}

	// min, max
	double minX = 0.0;
	double maxX = 0.0;

#if 0
	for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
		
		kome::objects::Chromatogram* chrom = m_chroms[ i ];
		// get data points
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
#else
	// 新規に登録されたchromatogramの最大最小値をスタックして、
	// スタックされている情報から最大最小値を求めます
	// ※chromatogram内の最大最小値を求める時間が省けます
	//
	if ( m_chroms.size() > m_dMinX.size() )
	{
		kome::objects::Chromatogram* chrom = m_chroms[ m_chroms.size() - 1 ];
		// get data points
		kome::core::DataPoints dps;
		chrom->getXYData( &dps, chrom->getOperationFlag() );

		double tmpMinX = dps.getMinX();
		double tmpMaxX = dps.getMaxX();
		double tmpMinY = dps.getMinY();
		double tmpMaxY = dps.getMaxY();

		m_dMinX.push_back( tmpMinX );
		m_dMaxX.push_back( tmpMaxX );
		m_dMinY.push_back( tmpMinY );
		m_dMaxY.push_back( tmpMaxY );
	}

	for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
		double tmpMinX = m_dMinX[ i ];
		double tmpMaxX = m_dMaxX[ i ];
		double tmpMinY = m_dMinY[ i ];
		double tmpMaxY = m_dMaxY[ i ];

		if( i == 0 || tmpMinX < minX ) {
			minX = tmpMinX;
		}
		if( i == 0 || tmpMaxX > maxX ) {
			maxX = tmpMaxX;
		}		
	}
#endif

	// index
	const double spacing = 0.2;
	const int startIdx = std::max( 0, roundnum( minX / spacing ) );
	const int endIdx = std::max( 0, roundnum( maxX / spacing ) );

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
		// get data points
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
void OverlappingChromatogram::onGetSpectra(
		DataSet& dataSet,
		const double startRt,
		const double endRt,
		const SearchType startSearch,
		const SearchType endSearch
) {
	// chromatograms
	if( m_chroms.size() == 1 ) {
		Chromatogram* chrom = m_chroms[ 0 ];

		chrom->getSpectra( dataSet, startRt, endRt, startSearch, endSearch );
	}
}

// on get mass
double OverlappingChromatogram::onGetMass( const unsigned int index ) {
	return -1.0;
}

// on get MS stage
int OverlappingChromatogram::onGetMsStage( const unsigned int index ) {
	return -1;
}

// on get precursor
double OverlappingChromatogram::onGetPrecursor( const unsigned int index ) {
	return -1.0;
}

