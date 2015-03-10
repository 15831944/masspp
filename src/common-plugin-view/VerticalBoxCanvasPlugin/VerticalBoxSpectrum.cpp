// VerticalBoxSpectrum.cpp
/**
 * @file VerticalBoxSpectrum.cpp
 * @brief VerticalBox Spectrum
 * @author OKADA, H.
 * @date 2010.11
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 * 
 */

#include "stdafx.h"
#include "VerticalBoxSpectrum.h"
#include "VerticalBoxCanvasPluginFunctions.h"	// @date 2010/12/06 <Add> OKADA

#include <math.h>


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
VerticalBoxSpectrum::VerticalBoxSpectrum() : Spectrum( NULL, NULL ) {
}

// destructor
VerticalBoxSpectrum::~VerticalBoxSpectrum() {
}

// add spectrum
void VerticalBoxSpectrum::addSpectrum( Spectrum* spec ) {
	// check the spectrum
	if( spec == NULL || searchSpectrum( spec ) >= 0 ) {
		return;
	}

	// add
	m_spectra.push_back( spec );

	// set group
	setGroup( m_spectra[ 0 ]->getGroup() );

	// update range
	double minX = double();
	double maxX = double();

	onGetXRange( &minX, &maxX );
	setXRange( minX, maxX );
}

// remove spectrum
void VerticalBoxSpectrum::removeSpectrum( Spectrum* spec ) {
	// search
	int idx = searchSpectrum( spec );
	if( idx < 0 ) {
		return;
	}

	// remove
	m_spectra.erase( m_spectra.begin() + idx );

	// set group
	setGroup( m_spectra.size() == 0 ? NULL : m_spectra[ 0 ]->getGroup() );

	// update range
	double minX = double();
	double maxX = double();

	onGetXRange( &minX, &maxX );
	setXRange( minX, maxX );
}

// clear spectra
void VerticalBoxSpectrum::clearSpectra() {
	m_spectra.clear();
	setGroup( NULL );

	// update range
	double minX = double();
	double maxX = double();

	onGetXRange( &minX, &maxX );
	setXRange( minX, maxX );
}

// search spectra
int VerticalBoxSpectrum::searchSpectrum( Spectrum* spec ) {
	// search
	for( int i = 0; i < (int)m_spectra.size(); i++ ) {
		if( spec == m_spectra[ i ] ) {
			return i;
		}
	}
	return -1;
}

// get xy data
void VerticalBoxSpectrum::onGetXYData(
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
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		kome::objects::Spectrum* spec = m_spectra[ i ];
		// get data points
		kome::core::DataPoints dps;
		spec->getXYData( &dps, spec->getOperationFlag() );

		double tmpMinX = dps.getMinX();
		double tmpMaxX = dps.getMaxX();

		if( i == 0 || tmpMinX < minMz ) {
			minMz = tmpMinX;
		}

		if( i == 0 || tmpMaxX > maxMz ) {
			maxMz = tmpMaxX;
		}
	}

	// index
	const double spacing = 0.2;
	const int startIdx = max( 0, roundnum( minMz / spacing ) );
	const int endIdx = max( 0, roundnum( maxMz / spacing ) );

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
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		kome::objects::Spectrum* spec = m_spectra[ i ];
		// get data points
		kome::core::DataPoints dps;
		spec->getXYData( &dps, spec->getOperationFlag() );

		for( unsigned int j = 0; j < dps.getLength(); j++ ){
			const double x = dps.getX( j );
			const double y = dps.getY( j );

			if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {

				int idx = roundnum( x / spacing );
				idx = CLAMP( idx, startIdx, endIdx ) - startIdx;

				flags[ idx ] = true;
				if( fabs( y ) > fabs( ints[ idx ] ) ) {
					ints[ idx ] = y;
				}
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

// get x range
void VerticalBoxSpectrum::onGetXRange( double* minX, double* maxX ) {
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
double VerticalBoxSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
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
double VerticalBoxSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// get data points
	kome::core::DataPoints dps;
	getXYData( &dps, -1.0, -1.0, false );
	
	// get total intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		intensity += max( dps.getY( i ), intensity );
	}

	return intensity;
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	VerticalBoxSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	VerticalBoxSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	VerticalBoxSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	VerticalBoxSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	VerticalBoxSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	VerticalBoxSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	VerticalBoxSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

