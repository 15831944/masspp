/**
 * @file Ab3dSpectrum.cpp
 * @brief implements of Ab3dSpectrum class
 *
 * @author S.Tanaka
 * @date 2007.09.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dSpectrum.h"

#include "Ab3dSampleSet.h"
#include "Ab3dSample.h"


using namespace kome::ab3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Ab3dSpectrum::Ab3dSpectrum( Ab3dSample& sample, kome::objects::Spectrum* ctrlSpec, kome::objects::Spectrum* trmtSpec )
		: kome::objects::Spectrum(
				&sample,
				FMT(
					"%s : %s",
					ctrlSpec == NULL ? "null" : ctrlSpec->getName(),
					trmtSpec == NULL ? "null" : trmtSpec->getName()
				).c_str()
		), m_ab3dSample( sample ) {
	// initialize
	m_ctrlSpec = ctrlSpec;
	m_trmtSpec = trmtSpec;

	// stage
	int stage = 0;
	if( m_ctrlSpec != NULL && m_trmtSpec != NULL ) {
		if( m_ctrlSpec->getMsStage() == m_trmtSpec->getMsStage() ) {
			stage = m_ctrlSpec->getMsStage();
		}
	}
	else if( m_ctrlSpec != NULL ) {
		stage = m_ctrlSpec->getMsStage();
	}
	else if( m_trmtSpec != NULL ) {
		stage = m_trmtSpec->getMsStage();
	}

	if( stage > 0 ) {
		setMsStage( stage );
	}

	// resolution
	setResolution( sample.getAb3dSampleSet()->getMzTolerance() * 4.0 );

	// chromatogram
	setHasChromatogram( true );
}

// destructor
Ab3dSpectrum::~Ab3dSpectrum() {
}

// get control spectrum
kome::objects::Spectrum* Ab3dSpectrum::getCtrlSpectrum() {
	return m_ctrlSpec;
}

// get sample spectrum
kome::objects::Spectrum* Ab3dSpectrum::getSmplSpectrum() {
	return m_trmtSpec;
}

// add peak information
void Ab3dSpectrum::addPeakInfo( 
		const double rt,
		const double mz,
		const double intensity
) {
	// RT
	double tmpRt = getRt();
	double tmpInt = 0.0;
	for( unsigned int i = 0; i < m_points.size(); i++ ) {
		tmpInt += fabs( m_points[ i ].intensity );
	}
	tmpRt *= tmpInt;
	tmpRt += rt * fabs( intensity );
	tmpInt += fabs( intensity );

	if( tmpInt > 0.0 ) {
		setRt( tmpRt / tmpInt );
	}
	else {
		setRt( rt );
	}

	// add point
	m_points.resize( m_points.size() + 1 );
	m_points.back().mz = mz;
	m_points.back().intensity = intensity;
}

// sort data
void Ab3dSpectrum::sort() {
	if( m_points.size() > 0 ) {
		std::sort( m_points.begin(), m_points.end(), lessPoint );
	}
}

// on get xy data
void Ab3dSpectrum::onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ) {
	for( unsigned int i = 0; i < m_points.size(); i++ ) {
		// x, y
		double x = m_points[ i ].mz;
		double y = m_points[ i ].intensity;

		// add
		if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
			xyData->addPoint( x, y );
		}
	}
}

// on get x range
void Ab3dSpectrum::onGetXRange( double* minX, double* maxX ) {
	if( m_ctrlSpec == NULL && m_trmtSpec == NULL ) {
	}
	else if( m_ctrlSpec == NULL ) {
		*minX = m_trmtSpec->getMinX();
		*maxX = m_trmtSpec->getMaxX();
	}
	else if( m_trmtSpec == NULL ) {
		*minX = m_ctrlSpec->getMinX();
		*maxX = m_ctrlSpec->getMaxX();
	}
	else {
		*minX = std::min( m_ctrlSpec->getMinX(), m_trmtSpec->getMinX() );
		*maxX = std::max( m_ctrlSpec->getMaxX(), m_trmtSpec->getMaxX() );
	}
}

// on get total intensity
double Ab3dSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// data points
	kome::core::DataPoints dps;
	onGetXYData( &dps, minX, maxX );

	// sum
	double intensity = 0.0;
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		intensity += dps.getY( i );
	}
	return intensity;
}

// on get max intensity
double Ab3dSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// data points
	kome::core::DataPoints dps;
	onGetXYData( &dps, minX, maxX );

	// max
	return dps.getMaxY();
}

// compare to sort
bool Ab3dSpectrum::lessPoint( PointInfo p0, PointInfo p1 ) {
	return ( p0.mz < p1.mz );
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	Ab3dSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	Ab3dSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	Ab3dSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	Ab3dSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	Ab3dSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	Ab3dSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	Ab3dSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

