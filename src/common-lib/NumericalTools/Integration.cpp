/**
 * @file Integration.cpp
 * @brief implements of Integration class
 *
 * @author S.Tanaka
 * @date 2007.09.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Integration.h"

#include "Interpolation.h"


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// get integration value by trapezoid rule
double Integration::trapezoid( kome::core::XYData& xyData, const double a, const double b ) {
	// check data points
	if( xyData.getLength() == 0 ) {
		return 0.0;
	}

	// search
	bool insertA = false;
	int beginIndex = xyData.searchIndex( a );
	if( beginIndex < 0 ) {
		beginIndex = - beginIndex - 1;
		insertA = true;
	}

	bool insertB = false;
	int endIndex = xyData.searchIndex( b );
	if( endIndex < 0 ) {
		endIndex = - endIndex - 2;
		insertB = true;
	}

	// x array
	std::vector< double > xArray;
	if( insertA ) {
		xArray.push_back( a );
	}
	for( int i = beginIndex; i <= endIndex; i++ ) {
		xArray.push_back( xyData.getX( i ) );
	}
	if( insertB ) {
		xArray.push_back( b );
	}
	if( xArray.size() == 0 ) {
		return 0.0;
	}

	// linear interpolation
	kome::core::DataPoints pts;
	Interpolation::linear( xyData, pts, &( xArray[ 0 ] ), xArray.size() ); 

	// calculate
	double sum = 0.0;
	for( int i = 1; i < (int)pts.getLength(); i++ ) {
		// x, y
		double x0 = pts.getX( i - 1 );
		double y0 = pts.getY( i - 1 );
		double x1 = pts.getX( i );
		double y1 = pts.getY( i );

		// add area
		sum += ( y0 + y1 ) * ( x1 - x0 ) / 2.0;
	}

	return sum;
}
