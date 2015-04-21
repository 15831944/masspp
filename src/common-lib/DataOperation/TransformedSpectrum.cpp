/**
 * @file TransformedSpectrum.cpp
 * @brief implements of TransformedSpectrum class
 *
 * @author S.Tanaka
 * @date 2008.05.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TransformedSpectrum.h"


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
TransformedSpectrum::TransformedSpectrum( kome::objects::Spectrum& org, const bool op )
		: kome::objects::Spectrum( org.getSample(), org.getName() ), m_org( org ) {
	m_scX = 1.0;
	m_trX = 0.0;
	m_scY = 1.0;
	m_trY = 0.0;
	m_op = op;

	setName( org.getName() );
	setRt( org.getRt() );
	setStartRt( org.getStartRt() );
	setEndRt( org.getEndRt() );
	setSpecType( org.getSpecType() );
	setTitle( org.getTitle() );
	setIcon( org.getIcon() );
	setMinX( org.getMinX() );
	setMaxX( org.getMaxX() );
	setTotalIntensity( org.getTotalIntensity() );
	setBasePeakMass( org.getBasePeakMass() );
	setMsStage( org.getMsStage() );
	setPrecursor( org.getPrecursor() );
	setPrecursorIntensity( org.getPrecursorIntensity() );
	setParentSpectrum( org.getParentSpectrum() );
	setHasChromatogram( org.hasChromatogram() );
	setGroup( org.getGroup() );
	setScanNumber( org.getScanNumber() );
	setPolarity( org.getPolarity() );
	setCentroidMode( org.isCentroidMode() );
	setResolution( org.getResolution() );
	setOrgSpectrum( org.getOrgSpectrum() );
}

// destructor
TransformedSpectrum::~TransformedSpectrum() {
}

// set x scale
void TransformedSpectrum::setXScale( const double sc ) {
	m_scX = std::max( sc,  1.0e-10 );
}

// get x scale
double TransformedSpectrum::getXScale() {
	return m_scX;
}

// set x translation
void TransformedSpectrum::setXTranslation( const double tr ) {
	m_trX = tr;
}

// get x translation
double TransformedSpectrum::getXTranslation() {
	return m_trX;
}

// set y scale
void TransformedSpectrum::setYScale( const double sc ) {
	m_scY = std::max( sc,  1.0e-10 );
}

// get y scale
double TransformedSpectrum::getYScale() {
	return m_scY;
}

// set y translation
void TransformedSpectrum::setYTranslation( const double tr ) {
	m_trY = tr;
}

// get y translation
double TransformedSpectrum::getYTranslation() {
	return m_trY;
}

// on get xy data
void TransformedSpectrum::onGetXYData(
		kome::core::XYData* const xyData,
		const double minX,
		const double maxX
) {
	// get points
	kome::core::DataPoints dps;
	m_org.getXYData( &dps, m_op );

	// transform
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		const double x = m_scX * dps.getX( i ) + m_trX;
		const double y = m_scY * dps.getY( i ) + m_trY;

		if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
			xyData->addPoint( x, y );
		}
	}
}

// on get x range
void TransformedSpectrum::onGetXRange( double* minX, double* maxX ) {
	// get original x range
	double orgMinX = m_org.getMinX();
	double orgMaxX = m_org.getMaxX();

	// get range
	if( minX != NULL ) {
		*minX = m_scX * orgMinX + m_trX;
	}
	if( maxX != NULL ) {
		*maxX = m_scX * orgMaxX + m_trX;
	}
}

// on get total intensity
double TransformedSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
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
	double intensity = m_org.getTotalIntensity( orgMinX, orgMaxX );
	intensity = m_scY * intensity + m_trY;

	return intensity;
}

// on get max intenisty
double TransformedSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// x range
	double orgMinX = minX;
	if( orgMinX >= 0.0 ) {
		orgMinX = std::max( ( minX - m_trX ) / m_scX, 0.0 );
	}

	double orgMaxX = maxX;
	if( orgMaxX >= 0.0 ) {
		orgMaxX = std::max( ( maxX - m_trX ) / m_scX, 0.0 );
	}

	// max intensity
	double intensity = m_org.getMaxIntensity( orgMinX, orgMaxX );
	intensity = m_scY * intensity + m_trY;

	return intensity;
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	TransformedSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	TransformedSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	TransformedSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	TransformedSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	TransformedSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	TransformedSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	TransformedSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

