/**
 * @file RawWaveformSpectrum.cpp
 * @brief interfaces of RawWaveformSpectrum class
 * 
 * @author M.Izumi
 * @date 2013.02.25
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved. 
 */

#include "stdafx.h"
#include "RawWaveformSpectrum.h"

#include <math.h>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
RawWaveformSpectrum::RawWaveformSpectrum( kome::objects::Spectrum& spec ) 
	: kome::objects::Spectrum( spec.getSample(), spec.getName() ), m_orgSpec( spec ){
	m_scX = 1.0;
	m_trX = 0.0;
	m_scY = 1.0;
	m_trY = 0.0;

	m_op = false;

	setName( spec.getName() );
	setRt( spec.getRt() );
	setStartRt( spec.getStartRt() );
	setEndRt( spec.getEndRt() );
	setSpecType( spec.getSpecType() );
	setTitle( spec.getTitle() );
	setIcon( spec.getIcon() );
	setMinX( spec.getMinX() );
	setMaxX( spec.getMaxX() );
	setTotalIntensity(spec.getTotalIntensity() );
	setBasePeakMass( spec.getBasePeakMass() );
	setMsStage( spec.getMsStage() );
	setPrecursor( spec.getPrecursor() );
	setPrecursorIntensity( spec.getPrecursorIntensity() );
	setParentSpectrum( spec.getParentSpectrum() );
	setHasChromatogram( spec.hasChromatogram() );
	setGroup( spec.getGroup() );
	setScanNumber( spec.getScanNumber() );
	setPolarity( spec.getPolarity() );
	setCentroidMode( spec.isCentroidMode() );
	setResolution( spec.getResolution() );
}

// destructor
RawWaveformSpectrum::~RawWaveformSpectrum(){
}

void RawWaveformSpectrum::onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ){
	// get data points
	kome::core::DataPoints dps;
	m_orgSpec.getXYData( &dps, false );

	
	// transform
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		const double x = m_scX * dps.getX( i ) + m_trX;
		const double y = m_scY * dps.getY( i ) + m_trY;

		if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
			xyData->addPoint( x, y );
		}
	}
}

// get x range
void RawWaveformSpectrum::onGetXRange( double* minX, double* maxX ) {
	// get original x range
	double orgMinX = m_orgSpec.getMinX();
	double orgMaxX = m_orgSpec.getMaxX();

	// get range
	if( minX != NULL ) {
		*minX = m_scX * orgMinX + m_trX;
	}
	if( maxX != NULL ) {
		*maxX = m_scX * orgMaxX + m_trX;
	}
}

// get total intensity
double RawWaveformSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// x range
	double orgMinX = minX;
	if( orgMinX >= 0.0 ) {
		orgMinX = std::max( ( minX - m_trX ) / m_scX, 0.0 );
	}

	double orgMaxX = maxX;
	if( orgMaxX >= 0.0 ) {
		orgMaxX = std::max( ( maxX - m_trX ) / m_scX, 0.0 );
	}

	// total intensity
	double intensity = m_orgSpec.getTotalIntensity( orgMinX, orgMaxX );
	intensity = m_scY * intensity + m_trY;

	return intensity;
}

// get max intensity
double RawWaveformSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	double orgMinX = minX;
	if( orgMinX >= 0.0 ) {
		orgMinX = std::max( ( minX - m_trX ) / m_scX, 0.0 );
	}

	double orgMaxX = maxX;
	if( orgMaxX >= 0.0 ) {
		orgMaxX = std::max( ( maxX - m_trX ) / m_scX, 0.0 );
	}

	// max intensity
	double intensity = m_orgSpec.getMaxIntensity( orgMinX, orgMaxX );
	intensity = m_scY * intensity + m_trY;

	return intensity;
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	RawWaveformSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	RawWaveformSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	RawWaveformSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	RawWaveformSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	RawWaveformSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	RawWaveformSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	RawWaveformSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

