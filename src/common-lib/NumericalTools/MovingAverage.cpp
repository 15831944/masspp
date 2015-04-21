/**
 * @file MovingAverage.cpp
 * @brief implements of MovingAverage class
 *
 * @author S.Tanaka
 * @date 2012.03.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MovingAverage.h"

#include "Interpolation.h"


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// moving average
void MovingAverage::filter(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		const int width,
		const int positionAtIndex0,
		const double* weights
) {
	// total weights
	double totalW = 0.0;
	for( int i = 0; i < width; i++ ) {
		totalW += weights[ i ];
	}

	if( totalW == 0.0 ) {
		throw std::string( "Invalid weights." );
	}

	// filtered points
	std::vector< std::pair< double, double > > points;
	for( unsigned int i = 0; i < src.getLength(); i++ ) {
		const double x = src.getX( i );
		double y = 0.0;
		for( int j = 0; j < width; j++ ) {
			int idx = (int)i + positionAtIndex0 + j;
			if( idx >= 0 && idx < (int)src.getLength() ) {
				y += weights[ j ] * src.getY ( idx );
			}
		}
		y /= totalW;

		points.push_back( std::make_pair( x, y ) );
	}

	// store
	dst.clearPoints();
	for( unsigned int i = 0; i < points.size(); i++ ) {
		dst.addPoint( points[ i ].first, points[ i ].second );
	}
}
