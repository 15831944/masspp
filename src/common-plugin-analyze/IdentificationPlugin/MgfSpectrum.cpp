/**
 * @file MgfSpectrum.cpp
 * @brief interfaces of MgfSpectrum class
 *
 * @author M.Izumi
 * @date 2013.07.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "MgfSampleSet.h"
#include "MgfSample.h"
#include "MgfSpectrum.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MgfSpectrum::MgfSpectrum( MgfSample* sample, const char* name ) 
	: kome::objects::Spectrum( sample, name ){

	m_mgfSample = sample;
	m_tmpPointPath = "";

	m_minMz = -1.0;
	m_maxMz = -1.0;
}

// destructor
MgfSpectrum::~MgfSpectrum(){	
}

// add point
void MgfSpectrum::addPoint( const double x, const double y ) {
	// check values
	if( x < 0.0 || y < 0.0 ) {
		return;
	}

	// x-range
	double minX = std::max( 0.0, 10.0 * floor( x / 10.0 ) );
	double maxX = 10.0 * ceil( x / 10.0 );

	if( m_minMz < 0.0 || minX < m_minMz ) {
		m_minMz = minX;
	}
	if( m_maxMz < 0.0 || maxX > m_maxMz ) {
		m_maxMz = maxX;
	}
}

// on get xy data
void MgfSpectrum::onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ){
	MgfSampleSet* sampleSet = (MgfSampleSet*)m_mgfSample->getSampleSet();
		
	if( !m_tmpPointPath.empty() ){
		// file open
		FILE* fp = fileopen( m_tmpPointPath.c_str(), "r" );
		if( fp == NULL ) {
			LOG_ERROR_CODE( FMT( "Failed to open the file. [%s]", m_tmpPointPath.c_str() ), ERR_FILE_OPEN_FAILED );
			return;
		}
		
		// read file
		char buff[ 2048 ];
		while( fgets( buff, 2048, fp ) != NULL ) {
			// line
			double x = double();
			double y = double();

			std::string line = trimstring( buff ).c_str();
			if( sscanf( line.c_str(), "%lf,%lf", &x, &y ) >= 2 ){
				if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
					xyData->addPoint( x, y );
				}
			}
		}
		
		// close
		fflush( fp );
		fclose( fp );
	}
}

// on get x range
void MgfSpectrum::onGetXRange( double* minX, double* maxX ){
	// check members
	if( m_minMz < 0.0 || m_maxMz < 0.0 ) {
		return;
	}

	// charge
	int charge = abs( getPrecursorCharge() );
	if( charge < 1 ) {
		charge = 1;
	}

	// range and medium
	double l = fabs( m_maxMz - m_minMz );
	if( l < 0.01 ) {
		l = 0.01;
	}

	double m = ( m_minMz + m_maxMz ) / 2.0;

	// min x and max x
	double tmpMinX = m - 0.6 * l;
	if( tmpMinX < 0.0 ) {
		tmpMinX = 0.0;
	}

	double tmpMaxX = tmpMinX + l * (double)charge * 1.2;
	if( tmpMaxX < m_maxMz * (double)charge * 1.2 ) {
		tmpMaxX = m_maxMz * (double)charge * 1.2;
	}

	if( minX != NULL ) {
		*minX = tmpMinX;
	}

	if( maxX != NULL ) {
		*maxX = tmpMaxX;
	}
}

// on get total intensity
double MgfSpectrum::onGetTotalIntensity( const double minX, const double maxX ){
	// data points
	kome::core::DataPoints dps;

	onGetXYData( &dps, minX, maxX );

	// max intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		intensity += dps.getY( i );
	}

	return intensity;
}

// on get max intensity
double MgfSpectrum::onGetMaxIntensity( const double minX, const double maxX ){
	// data points
	kome::core::DataPoints dps;

	onGetXYData( &dps, minX, maxX );

	// max intensity
	double intensity = dps.getMaxY();

	return intensity;
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	MgfSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	MgfSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	MgfSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	MgfSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	MgfSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	MgfSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	MgfSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

