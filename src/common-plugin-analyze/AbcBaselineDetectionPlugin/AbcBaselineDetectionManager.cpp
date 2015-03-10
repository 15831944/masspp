/**
 * @file AbcBaselineDetectionManager.cpp
 * @brief implements of AbcBaselineDetectionManager class
 *
 * @author S.Tanaka
 * @date 2007.07.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "AbcBaselineDetectionManager.h"

#include <math.h>
#include <float.h>


using namespace kome::baseline::abc;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define NUMBER_OF_REGIONS				   32


// constructor
AbcBaselineDetectionManager::AbcBaselineDetectionManager() {
	m_windowWidth = 1.0;
	m_noiseFactor = 3.0;
	m_smoothingWidth = 10;
}

// destructor
AbcBaselineDetectionManager::~AbcBaselineDetectionManager() {
}

// set window width
void AbcBaselineDetectionManager::setWindowWidth( const double width ) {
	m_windowWidth = MAX( 0.0, width );
}

// get window width
double AbcBaselineDetectionManager::getWindowWidth() {
	return m_windowWidth;
}

// set noise factor
void AbcBaselineDetectionManager::setNoiseFactor( const double nf ) {
	m_noiseFactor = MAX( 0.0, nf );
}

// get noise factor
double AbcBaselineDetectionManager::getNoiseFactor() {
	return m_noiseFactor;
}

// set smoothing width
void AbcBaselineDetectionManager::setSmoothingWidth( const int width ) {
	m_smoothingWidth = MAX( 0, width );
}

// get smoothing width
int AbcBaselineDetectionManager::getSmoothingWidth() {
	return m_smoothingWidth;
}

// get baseline
void AbcBaselineDetectionManager::getBaseline( kome::core::XYData& src, kome::core::XYData& baseline ) {
	// check the parameters
	if( src.getLength() == 0 ) {
		return;
	}

	// get noise threshold
	double noiseThreshold = getNoiseThreshold( src );

	// get noise flag
	double* noiseFlags = new double[ src.getLength() ];
	bool defaultFlag = false;
	fillarray( noiseFlags, &defaultFlag, sizeof( bool ), src.getLength() );

	int length = (int)src.getLength();
	double rectDistance = m_windowWidth / 2.0;
	int rectBegin = 0;
	int rectEnd = 0;
	for( int i = 0; i < length; i++ ) {
		// x
		double x = src.getX( i );

		// rect
		double minX = x - rectDistance;
		double maxX = x + rectDistance;
		while( src.getX( rectBegin ) < minX ) {
			rectBegin++;
		}
		while( rectEnd < length && src.getX( rectEnd ) <= maxX ) {
			rectEnd++;
		}

		// intensity range
		double minY = src.getY( i );
		double maxY = src.getY( i );
		for( int j = rectBegin; j < rectEnd; j++ ) {
			double y = src.getY( j );
			minY = MIN( y, minY );
			maxY = MAX( y, maxY );
		}
		double range = maxY - minY;

		// set flag
		noiseFlags[ i ] = ( range < noiseThreshold );
	}

	// get noise points
	kome::core::DataPoints pt;
	for( unsigned int i = 0; i < src.getLength(); i++ ) {
		// noise flag
		bool noiseFlag = true;
		if( i > 0 && i < src.getLength() - 1 ) {
			noiseFlag = noiseFlags[ i ] || ( noiseFlags[ i - 1 ] && noiseFlags[ i + 1 ] );
		}

		// add noise points
		if( noiseFlag ) {
			pt.addPoint( src.getX( i ), src.getY( i ) );
		}
	}

	// baseline
	baseline.reserve( pt.getLength() );
	for( int i = 0; i < (int)pt.getLength(); i++ ) {
		// x
		double x = pt.getX( i );

		// smoothing
		double y = 0.0;
		int beginIdx = std::max( i - m_smoothingWidth, 0 );
		int endIdx = std::min( i + m_smoothingWidth + 1, (int)pt.getLength() );
		int count = endIdx - beginIdx;
		for( int j = beginIdx; j < endIdx; j++ ) {
			y += pt.getY( j );
		}
		y /= (double)count;

		// add
		baseline.addPoint( x, y );
	}

	// delete
	delete[] noiseFlags;
} 

// get noise threshold
double AbcBaselineDetectionManager::getNoiseThreshold( kome::core::XYData& src ) {
	// length
	int length = (int)src.getLength();

	// minimum sigma
	double minSigma = FLT_MAX;
	for( int i = 0; i < NUMBER_OF_REGIONS; i++ ) {
		// index
		int beginIdx = length * i / NUMBER_OF_REGIONS;
		int endIdx = length * ( i + 1 ) / NUMBER_OF_REGIONS;

		// get sigma
		double sigma = 0.0;
		if( endIdx > beginIdx ) {
			// intensity array
			std::vector< double > intensities;
			intensities.reserve( endIdx - beginIdx );
			for( int j =  beginIdx; j < endIdx; j++ ) {
				intensities.push_back( src.getY( j ) );
			}

			// sigma
			sigma = kome::numeric::Statistics::sd( &( intensities[ 0 ] ), intensities.size() );
			minSigma = MIN( sigma, minSigma );
		}
	}

	// noise threshold
	return ( m_noiseFactor * minSigma );
}

// get instance
AbcBaselineDetectionManager& AbcBaselineDetectionManager::getInstance() {
	// create object
	static AbcBaselineDetectionManager mgr;

	return mgr;
}
