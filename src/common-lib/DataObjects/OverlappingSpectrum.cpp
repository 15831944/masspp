/**
 * @file OverlappingSpectrum.cpp
 * @brief implements of OverlappingSpectrum class
 *
 * @author S.Tanaka
 * @date 2008.11.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "OverlappingSpectrum.h"

#include "DataGroupNode.h"
#include "Sample.h"
#include "SampleSet.h"

#include <math.h>


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
OverlappingSpectrum::OverlappingSpectrum() : Spectrum( NULL, NULL ) {

	m_spectra.clear( );
	m_dMinX.clear( );
	m_dMaxX.clear( );
	m_dMinY.clear( );
	m_dMaxY.clear( );

	m_bFlags = NULL;
	m_dInts = NULL;
	m_nAllocLen = 0;
}

// destructor
OverlappingSpectrum::~OverlappingSpectrum() {
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
}

// add spectrum
void OverlappingSpectrum::addSpectrum( Spectrum* spec ) {
	// check the spectrum
	if( spec == NULL || searchSpectrum( spec ) >= 0 ) {
		return;
	}

	// add
	m_spectra.push_back( spec );

	// set group
	if( m_spectra.size() == 1 ) {
		setGroup( m_spectra[ 0 ]->getGroup() );
	}
	else {
		setGroup( NULL );
	}

	// update range
	double minX = double();
	double maxX = double();

	onGetXRange( &minX, &maxX );
	setXRange( minX, maxX );
}

// remove spectrum
void OverlappingSpectrum::removeSpectrum( Spectrum* spec ) {
	// search
	int idx = searchSpectrum( spec );
	if( idx < 0 ) {
		return;
	}

	// remove
	m_spectra.erase( m_spectra.begin() + idx );

	// set group
	if( m_spectra.size() == 1 ) {
		setGroup( m_spectra[ 0 ]->getGroup() );
	}
	else {
		setGroup( NULL );
	}

	// update range
	double minX = double();
	double maxX = double();

	onGetXRange( &minX, &maxX );
	setXRange( minX, maxX );
}

// clear spectra
void OverlappingSpectrum::clearSpectra() {
	m_spectra.clear();
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

	// update range
	double minX = double();
	double maxX = double();

	onGetXRange( &minX, &maxX );
	setXRange( minX, maxX );
}

// search spectra
int OverlappingSpectrum::searchSpectrum( Spectrum* spec ) {
	// search
	for( int i = 0; i < (int)m_spectra.size(); i++ ) {
		if( spec == m_spectra[ i ] ) {
			return i;
		}
	}
	return -1;
}

// get xy data
void OverlappingSpectrum::onGetXYData(
		kome::core::XYData* const xyData,
		const double minX,
		const double maxX
) {
	// check the member
	if( m_spectra.size() == 0 ) {
		return;
	}

	// min, max
	double minMz = 0.0;
	double maxMz = 0.0;

	double minInt = 0.0;
	double maxInt = 0.0;

	// 登録されているspectrumのなかで、最大・最小をを求めます
	//

	// 新規に登録されたspectrumの最大最小値をスタックして、
	// スタックされている情報から最大最小値を求めます
	// ※spectrum内の最大最小値を求める時間が省けます
	//
	if  ( m_spectra.size( ) > m_dMinX.size( ) )
	{
		kome::objects::Spectrum* spec = m_spectra[ m_spectra.size( ) - 1 ];
		// get data points
		kome::core::DataPoints dps;
		spec->getXYData( &dps, spec->getOperationFlag( ) );

		double	tmpMinX = dps.getMinX( );
		double	tmpMaxX = dps.getMaxX( );
		double	tmpMinY = dps.getMinY( );
		double	tmpMaxY = dps.getMaxY( );

		m_dMinX.push_back( tmpMinX );
		m_dMaxX.push_back( tmpMaxX );
		m_dMinY.push_back( tmpMinY );
		m_dMaxY.push_back( tmpMaxY );
	}
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		double tmpMinX = m_dMinX[ i ];
		double tmpMaxX = m_dMaxX[ i ];
		double tmpMinY = m_dMinY[ i ];
		double tmpMaxY = m_dMaxY[ i ];

		if( i == 0 || tmpMinX < minMz ) {
			minMz = tmpMinX;
		}
		if( i == 0 || tmpMaxX > maxMz ) {
			maxMz = tmpMaxX;
		}
		//
		if( i == 0 || tmpMinY < minInt ) {
			minInt = tmpMinY;
		}
		if( i == 0 || tmpMaxY > maxInt ) {
			maxInt = tmpMaxY;
		}
	}

	// index
	const double spacing = 0.2;
	const int startIdx = std::max( 0, roundnum( minMz / spacing ) );
	const int endIdx = std::max( 0, roundnum( maxMz / spacing ) );

	// length
	const int len = endIdx - startIdx + 1;
	if( len <= 0 ) {
		return;
	}

	// array
//	static	bool	*flags = NULL;
//	static	double	*ints = NULL;

	static	int	nPrevStartIdx = 0;
	static	int	nPrevEndIdx = 0;
	static	double	dPrevMinMz = 0.0;
	static	double	dPrevMaxMz = 0.0;
	if  ( 0 == m_nAllocLen )
	{
		m_bFlags = new bool[ len ];
		m_dInts = new double[ len ];
		for ( int i = 0 ; i < len ; i++ )
		{
			m_bFlags[ i ] = false;
			m_dInts[ i ] = 0.0;
		}
		m_nAllocLen = len;

		nPrevStartIdx = startIdx;
		nPrevEndIdx = endIdx;
		dPrevMinMz = minMz;
		dPrevMaxMz = maxMz;
	}
	else if  ( m_nAllocLen < len )
	{
		bool	*bFlags = new bool[ len ];
		double	*dInts = new double[ len ];

		int	nStoreMin = roundnum( dPrevMinMz / spacing );
		nStoreMin = CLAMP( nStoreMin, startIdx, endIdx ) - startIdx;
		for ( int i = 0 ; i < len ; i++ )
		{
			if  ( i >= nStoreMin && i < ( nStoreMin + m_nAllocLen ) )
			{
				bFlags[i] = m_bFlags[i-nStoreMin];
				dInts[i] = m_dInts[i-nStoreMin];
			}
			else
			{
				bFlags[ i ] = false;
				dInts[ i ] = 0.0;
			}
		}
		delete[]	m_dInts;
		delete[]	m_bFlags;		

		m_dInts = dInts;
		m_bFlags = bFlags;
		m_nAllocLen = len;
		dPrevMinMz = minMz;
		dPrevMaxMz = maxMz;
	}



	// fill array
//
// 全スペクトラムの最大Intensityのデータを抽出しているようです
// 拡大したときに範囲内の最大値を求めるのに使用しています
//

	kome::objects::Spectrum* spec = m_spectra[ m_spectra.size( ) - 1 ];
	// get data points
	kome::core::DataPoints dps;
	spec->getXYData( &dps, spec->getOperationFlag() );

	for ( unsigned int j = 0 ; j < dps.getLength( ) ; j++ )
	{
		const double x = dps.getX( j );
		const double y = dps.getY( j );

		if  ( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) )
		{
			int	idx = roundnum( x / spacing );
			idx = CLAMP( idx, startIdx, endIdx ) - startIdx;

			m_bFlags[ idx ] = true;
			if  ( fabs( y ) > fabs( m_dInts[ idx ] ) )
			{
				m_dInts[ idx ] = y;
			}
		}
	}
	// add array
	for( int i = 0; i < len; i++ ) {
		if( m_bFlags[ i ] ) {
			double x = spacing * ( i + startIdx );
			double y = m_dInts[ i ];

			xyData->addPoint( x, y );
		}
	}
}

// get x range
void OverlappingSpectrum::onGetXRange( double* minX, double* maxX ) {
	double minXX = -1.0;
	double maxXX = -1.0;

	// get x range
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		// spectrum
		Spectrum* spec = m_spectra[ i ];

		double tmpMinX = spec->getMinX();
		double tmpMaxX = spec->getMaxX();

		if( minXX < 0.0 || tmpMinX < minXX ) {
			minXX = tmpMinX;
		}
		if( maxXX < 0.0 || tmpMaxX > maxXX ) {
			maxXX = tmpMaxX;
		}
	}

	// store
	*minX = minXX;
	*maxX = maxXX;
}

// get total intensity
double OverlappingSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// get data points
	kome::core::DataPoints dps;
	getXYData( &dps, -1.0, -1.0, false );
	
	// get total intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		intensity += dps.getY( i );
	}

	return intensity;
}

// get max intensity
double OverlappingSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// get data points
	kome::core::DataPoints dps;
	getXYData( &dps, -1.0, -1.0, false );
	
	// get total intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		intensity += std::max( dps.getY( i ), intensity );
	}

	return intensity;
}
