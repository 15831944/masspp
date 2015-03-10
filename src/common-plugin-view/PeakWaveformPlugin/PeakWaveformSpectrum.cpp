/**
 * @file PeakWaveformSpectrum.cpp
 * @brief interfaces of PeakWaveformSpectrum class
 * 
 * @author M.Izumi
 * @date 2013.02.25
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved. 
 */

#include "stdafx.h"
#include "PeakWaveformSpectrum.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
PeakWaveformSpectrum::PeakWaveformSpectrum( kome::objects::Spectrum& spec ) 
	: kome::objects::Spectrum( spec.getSample(),  spec.getName() ), m_orgSpec( spec ){

	m_op = true;

	setName( spec.getName() );
	setRt(  spec.getRt() );
	setStartRt(  spec.getStartRt() );
	setEndRt(  spec.getEndRt() );
	setSpecType(  spec.getSpecType() );
	setTitle( spec.getTitle() );
	setIcon( spec.getIcon() );
	setMinX( spec.getMinX() );
	setMaxX( spec.getMaxX() );
	setTotalIntensity( spec.getTotalIntensity() );
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
PeakWaveformSpectrum::~PeakWaveformSpectrum(){
}

void PeakWaveformSpectrum::onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ){
	kome::core::DataPoints dps;
	m_orgSpec.getXYData( &dps, true );

	
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	
	if( pkMgr.hasPeaks( &m_orgSpec ) ){
		kome::objects::Peaks* peaks = pkMgr.getPeaks( &m_orgSpec );

		if( peaks != NULL ){
			for( unsigned int i=0; i < peaks->getLength(); i++ ){
				kome::objects::PeakElement* peakElt = peaks->getPeak(i);
				if( peakElt != NULL ){
					if( ( minX < 0.0 || peakElt->getX() >= minX ) && ( maxX < 0.0 || peakElt->getX() <= maxX ) ) {
						xyData->addPoint( peakElt->getX(), peakElt->getY() );
					}
					
				}
			}
		}
	}
}

// get x range
void PeakWaveformSpectrum::onGetXRange( double* minX, double* maxX ) {	
}

// get total intensity
double PeakWaveformSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// get data points
	kome::core::DataPoints pt;
	onGetXYData( &pt, minX, maxX );

	// get total intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < pt.getLength(); i++ ) {
		intensity += pt.getY( i );
	}

	return intensity;
}

// get max intensity
double PeakWaveformSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// get data points
	kome::core::DataPoints pt;
	onGetXYData( &pt, minX, maxX );

	// get main intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < pt.getLength(); i++ ) {
		double y = pt.getY( i );
		intensity = MAX( y, intensity );
	}

	return intensity;
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	PeakWaveformSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	PeakWaveformSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	PeakWaveformSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	PeakWaveformSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	PeakWaveformSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	PeakWaveformSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	PeakWaveformSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

