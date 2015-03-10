/**
 * @file TextSpectrum.cpp
 * @brief implements of TextSpectrum class
 *
 * @author S.Tanaka
 * @date 2010.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TextSpectrum.h"
#include "TextSample.h"
#include "TextSampleSet.h"

#include <math.h>


using namespace kome::io::text;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
TextSpectrum::TextSpectrum( TextSample* sample, const char* name )
		: kome::objects::Spectrum( sample, name ) {
	m_textSample = sample;
	m_pos = -1;

	m_minMz = -1.0;
	m_maxMz = -1.0;
}

// destructor
TextSpectrum::~TextSpectrum() {
}

// get text sample
TextSample* TextSpectrum::getTextSample() {
	return m_textSample;
}

// set data position
void TextSpectrum::setDataPos( const long long pos ) {
	m_pos = pos;
}

// get data position
long long TextSpectrum::getDataPos() {
	return m_pos;
}

// add point
void TextSpectrum::addPoint( const double x, const double y ) {
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
void TextSpectrum::onGetXYData(
		kome::core::XYData* const xyData,
		const double minX,
		const double maxX
) {
	// get file
	TextSampleSet* ss = ( m_textSample == NULL ? NULL : m_textSample->getTextSampleSet() );
	FILE* fp = ( ss == NULL ? NULL : ss->getFile() );

	if( fp == NULL ) {
		// @date 2011.03.17 <Mod> M.Izumi ->
		const char* path = ss->getFilePath();
		if( ss->openFile( path ) ){
			fp = ss->getFile();
		
			if( fp == NULL ){
				LOG_WARN( FMT( "Failed to get xy data." ) );
				return;
			}
		}
		// <-
	}

	// read
	fileseek( fp, m_pos, SEEK_SET );
	char line[ 2048 ];

	double prevX = 0.0;
	bool loop = true;
	bool reading = false;

	while( loop && fgets( line, 2048, fp ) != NULL ) {
		std::string l = trimstring( line );
		if( l.empty() || l[ 0 ] == '#' ) {
			if( reading ) {
				loop = false;
			}
		}
		else {
			reading = true;
			double x = double();
			double y = double();

			if( sscanf( l.c_str(), "%lf %lf", &x, &y ) >= 2 ) {
				if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
					if( x > prevX ) {
						xyData->addPoint( x, y );
					}
				}
			}

			prevX = x;
		}
	}

	ss->closeFile();	// Spec No.93991 @date 2013.08.29 <Add> M.Izumi

}

// on get x range
void TextSpectrum::onGetXRange( double* minX, double* maxX ) {
	// check members
	if( m_minMz < 0.0 || m_maxMz < 0.0 ) {
		return;
	}

	// range & midium
	double l = fabs( m_maxMz - m_minMz );
	if( l < 0.01 ) {
		l = 0.01;
	}

	double m = ( m_minMz + m_maxMz ) / 2.0;

	// min X & max X
	double tmpMinX = m - l * 0.6;
	if( tmpMinX < 0.0 ) {
		tmpMinX = 0.0;
	}
	double tmpMaxX = tmpMinX + 1.2 * l;
	if( tmpMaxX < 1.2 * m_maxMz ) {
		tmpMaxX = 1.2 * m_maxMz;
	}

	// store
	if( minX != NULL ) {
		*minX = tmpMinX;
	}
	if( maxX != NULL ) {
		*maxX = tmpMaxX;
	}
}

// on get total intenisty
double TextSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
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


// on get max intenisty
double TextSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
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
void	TextSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	TextSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	TextSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	TextSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	TextSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	TextSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	TextSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

