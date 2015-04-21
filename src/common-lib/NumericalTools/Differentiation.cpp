/**
 * @file Differentiation.cpp
 * @brief implements of Differentiation class
 *
 * @author S.Tanaka
 * @date 2012.03.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Differentiation.h"

#include "Interpolation.h"


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// differentiate
void Differentiation::diffrentiate( kome::core::XYData& src, kome::core::XYData& dst ) {
	// differentiate
	std::vector< std::pair< double, double > > points;
	for( unsigned int i = 1; i < src.getLength(); i++ ) {
		const double x0 = src.getX( i - 1 );
		const double x1 = src.getX( i );
		const double y0 = src.getY( i - 1 );
		const double y1 = src.getY( i );

		const double x = ( x0 + x1 ) / 2.0;
		const double y = ( y1 - y0 ) / std::max( 1.0e-10, fabs( x1 - x0 ) );

		points.push_back( std::make_pair( x, y ) );
	}

	// store
	dst.clearPoints();
	for( unsigned int i = 0; i < points.size(); i++ ) {
		dst.addPoint( points[ i ].first, points[ i ].second );
	}
}
