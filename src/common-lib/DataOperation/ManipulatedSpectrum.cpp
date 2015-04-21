/**
 * @file ManipulatedSpectrum.cpp
 * @brief Manipulated Spectrum class
 *
 * @author M.Izumi
 * @date 2012.11.12
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "ManipulatedSample.h"
#include "ManipulatedSpectrum.h"

using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
ManipulatedSpectrum::ManipulatedSpectrum( ManipulatedSample* sample, kome::objects::Spectrum& org, const bool op )
	: kome::objects::Spectrum( org.getSample(), org.getName() ), m_org( org ){
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

	m_sample = sample;
}

// destructor
ManipulatedSpectrum::~ManipulatedSpectrum(){
}

// on get xy data
void ManipulatedSpectrum::onGetXYData(
	kome::core::XYData* const xyData,
	const double minX,
	const double maxX
){
	// manipulate
	ManipulatedSample* sample = getSample();

	// operaiton list
	std::vector< kome::objects::XYDataOperation* > operations;
	if( sample != NULL ) {
		for( unsigned int i = 0; i < sample->getNumberOfOperation(); i++ ) {
			operations.push_back( sample->getOperation( i ) );
		}
	}

	// points
	if( operations.size() == 0 ) {
		m_org.getXYData( xyData, minX, maxX, false );
	}
	else {
		kome::core::DataPoints dps;
		m_org.getXYData( &dps, -1.0, -1.0, false );

		for( unsigned int i = 0; i < operations.size(); i++ ) {
			kome::objects::XYDataOperation* opt = operations[ i ];

			kome::core::DataPoints tmp;
			opt->update( dps, tmp, *this );

			dps.clearPoints();
			for( unsigned int i = 0; i < tmp.getLength(); i++ ) {
				dps.addPoint( tmp.getX( i ), tmp.getY( i ) );
			}
		}

		// copy
		for( unsigned int i = 0; i < dps.getLength(); i++ ) {
			const double x = dps.getX( i );
			const double y = dps.getY( i );
			if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
				xyData->addPoint( x, y );
			}
		}
	}
}

// on get XRange
void ManipulatedSpectrum::onGetXRange( double* minX, double* maxX ){
}

// on get total Intensity
double ManipulatedSpectrum::onGetTotalIntensity( const double minX, const double maxX ){
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

// on GetTotalIntensity
double ManipulatedSpectrum::onGetMaxIntensity( const double minX, const double maxX ){
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

// >>>>>>	@Date:2013/09/02	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	ManipulatedSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	ManipulatedSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	ManipulatedSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	ManipulatedSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	ManipulatedSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	ManipulatedSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	ManipulatedSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/02	<Add>	A.Ozaki
