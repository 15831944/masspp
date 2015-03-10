/**
 * @file LinearFilter.cpp
 * @brief implements of LinearFilter class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LinearFilter.h"


using namespace kome::filter::linear;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
LinearFilter::LinearFilter() {
}

// destructor
LinearFilter::~LinearFilter() {
}

// add weight
void LinearFilter::addWeight( const int pos, double w ) {
	m_weights.push_back( std::make_pair( pos, w ) );
}

// clear weight
void LinearFilter::clearWeight() {
	m_weights.clear();
}

// update point
void LinearFilter::execute( kome::core::XYData& src, kome::core::XYData& dst ) {
	// reserve
	dst.reserve( src.getLength() );

	// check negative intensity
	bool negativeOk = false;
	for( unsigned int i = 0; i < src.getLength() && !negativeOk; i++ ) {
		if( src.getY( i ) < 0.0 ) {
			negativeOk = true;
		}
	}

	// weights
	int minIdx = 0;
	int maxIdx = 0;
	for( unsigned int i = 0; i < m_weights.size(); i++ ) {
		minIdx = std::min( m_weights[ i ].first, minIdx );
		maxIdx = std::max( m_weights[ i ].first, maxIdx );
	}
	const int num = maxIdx - minIdx + 1;
	double* weights = new double[ num ];
	for( int i = 0; i < num; i++ ) {
		weights[ i ] = 0.0;
	}
	for( unsigned int i = 0; i < m_weights.size(); i++ ) {
		weights[ m_weights[ i ].first - minIdx ] = m_weights[ i ].second;
	}

	// moving average
	kome::core::DataPoints dps;
	kome::numeric::MovingAverage::filter( src, dps, num, minIdx, weights );
	delete[] weights;

	// store
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		double x = dps.getX( i );
		double y = dps.getY( i );
		if( !negativeOk ) {
			y = std::max( y, 0.0 );
		}

		dst.addPoint( x, y );
	}
}
