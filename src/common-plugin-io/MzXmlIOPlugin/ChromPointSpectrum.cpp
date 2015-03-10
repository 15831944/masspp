/**
 * @file ChromPointSpectrum.cpp
 * @brief implements of ChromPointSpectrum class
 *
 * @author S.Tanaka
 * @date 2008.04.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ChromPointSpectrum.h"


using namespace kome::io::mzxml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ChromPointSpectrum::ChromPointSpectrum( kome::objects::Sample* sample )
		: kome::objects::Spectrum( sample, "" ) {
}

// destructor
ChromPointSpectrum::~ChromPointSpectrum() {
}

// on get xy data
void ChromPointSpectrum::onGetXYData(
		kome::core::XYData* const xyData,
		const double minX,
		const double maxX
) {
	// check mass
	double mass = getBasePeakMass();
	if( mass < 0.0 || ( minX >= 0.0 && mass < minX ) || ( maxX >= 0.0 && mass > maxX ) ) {
		return;
	}

	// check intensity
	double intensity = getMaxIntensity();
	if( intensity <= 0.0 ) {
		return;
	}

	// add point
	xyData->addPoint( mass, intensity );
}

// on get x range
void ChromPointSpectrum::onGetXRange( double* minX, double* maxX ) {
	// mass
	double mass = getBasePeakMass();
	if( mass < 0.0 ) {
		return;
	}

	// store
	if( minX != NULL ) {
		*minX = mass;
	}
	if( maxX != NULL ) {
		*maxX = mass;
	}
}

// on get total intensity
double ChromPointSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// get points
	kome::core::DataPoints dps;


	getXYData( &dps, minX, maxX, false );

	// intensity
	if( dps.getLength() > 0 ) {
		return dps.getY( 0 );
	}
	return 0.0;
}

// on get max intensity
double ChromPointSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	return onGetTotalIntensity( minX, maxX );
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	ChromPointSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	ChromPointSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	ChromPointSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	ChromPointSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	ChromPointSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	ChromPointSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	ChromPointSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

