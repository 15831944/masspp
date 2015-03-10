/**
 * @file AIASpectrum.cpp
 * @brief implementation of AIASpectrum class
 *
 * @author fujita
 * @date 2012.07.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "AIASpectrum.h"

#include "AIASampleSet.h"
#include "AIASample.h"

#include <netcdf.h>


using namespace kome::io::aia;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
AIASpectrum::AIASpectrum( AIASample* sample, int scanNumber )
		: kome::objects::Spectrum( sample, FMT( "Scan %d", scanNumber ).c_str() ) {
	// initialize
	m_aiaSample = sample;
	m_index = -1;
	m_num = -1;

	// scan number
	setScanNumber( scanNumber );
}

// destructor
AIASpectrum::~AIASpectrum() {
}

// set xy data information
void AIASpectrum::setXYInfo( long index, long num ) {
	m_index = index;
	m_num = num;
}

// get xy data
void AIASpectrum::onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ) {
	// check members
	if( m_num <= 0 || m_index < 0 ) {
		return;
	}

	// get ID
	int ncid = m_aiaSample->getCdfId();
	int xId = 0;
	int yId = 0;
	int status = nc_inq_varid( ncid, "mass_values", &xId );
	if( status != NC_NOERR ) {
		AIASampleSet::error( status );
		return;
	}
	status = nc_inq_varid( ncid, "intensity_values", &yId );
	if( status != NC_NOERR ) {
		AIASampleSet::error( status );
		return;
	}

	// create array
	double* xArray = new double[ m_num ];
	double* yArray = new double[ m_num ];

	// get array
	size_t start[] = { m_index };
	size_t count[] = { m_num };

	status = nc_get_vara_double( ncid, xId, start, count, xArray );
	if( status == NC_NOERR ) {		
		status = nc_get_vara_double( ncid, yId, start, count, yArray );
	}

	// add array
	if( status == NC_NOERR ) {
		for( int i = 0; i < m_num; i++ ) {
			double x = xArray[ i ];
			double y = yArray[ i ];

			if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) && y > 0.0 ) {
				xyData->addPoint( x, y );
			}
		}
	}
	else {
		AIASampleSet::error( status );
	}

	// delete array
	delete[] xArray;
	delete[] yArray;
}

// get x range
void AIASpectrum::onGetXRange( double* minX, double* maxX ) {
	// x range is already settled.
}

// get total intensity
double AIASpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// get xy data
	kome::core::DataPoints dps;
	onGetXYData( &dps, minX, maxX );

	// intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		intensity += dps.getY( i );
	}

	return intensity;
}

// get max intensity
double AIASpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// get xy data
	kome::core::DataPoints dps;
	onGetXYData( &dps, minX, maxX );

	// intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		intensity = std::max( intensity, dps.getY( i ) );
	}

	return intensity;
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	AIASpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	AIASpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	AIASpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	AIASpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	AIASpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	AIASpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	AIASpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

