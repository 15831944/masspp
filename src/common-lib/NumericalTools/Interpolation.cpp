/**
 * @file Interpolation.cpp
 * @brief implements of Interpolation class
 *
 * @author S.Tanaka
 * @date 2007.03.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Interpolation.h"


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// linear interpolation
void Interpolation::linear(
		kome::core::XYData& src, 
		kome::core::XYData& dst,
		double* xArray,
		unsigned int size
) {
	// check the parameter
	if( xArray == NULL || size == 0 ) {
		return;
	}

	// reserve
	dst.reserve( size );

	// check length
	if( src.getLength() == 0 ) {
		for( unsigned int i = 0; i < size; i++ ) {
			dst.addPoint( xArray[ i ], 0.0 );
		}
		return;
	}
	else if( src.getLength() == 1 ) {
		double y = src.getY( 0 );
		for( unsigned int i = 0; i < size; i++ ) {
			dst.addPoint( xArray[ i ], y );
		}
		return;
	}

	// interpolation
	unsigned index = 0;
	double x = xArray[ 0 ];
	for( unsigned int i = 1; i < src.getLength(); i++ ) {
		// get line
		double x0 = src.getX( i - 1 );
		double x1 = src.getX( i );
		double y0 = src.getY( i - 1 );
		double y1 = src.getY( i );

		double a = ( y1 - y0 ) / ( x1 - x0 );
		double b = - a * x0 + y0;

		while( index < size && ( x <= x1 || i == src.getLength() - 1 ) ) {
			double y = a * x + b;

			dst.addPoint( x, y );

			index++;
			if( index < size ) {
				x = xArray[ index ];
			}
		}
	}
}

// linear interapolation
double Interpolation::linear( kome::core::XYData& src, const double x ) {
	// check the parameter
	int maxIdx = (int)src.getLength() - 1;
	double y = 0.0;
	if( maxIdx < 0 ) {
		return y;
	}
	if( maxIdx == 0 ) {
		y = src.getY( 0 );
		return y;
	}

	// search
	int idx1 = src.searchIndex( x );
	if( idx1 >= 0 ) {
		y = src.getY( idx1 );
		return y;
	}

	idx1 = - idx1 - 1;
	idx1 = CLAMP( idx1, 1, maxIdx );
	int idx0 = idx1 - 1;
	if( idx0 < 0 ) {
		idx0 = 0;
		idx1 = idx0 + 1;
	}
	if( idx1 > maxIdx ) {
		idx1 = maxIdx;
		idx0 = idx1 - 1;
	}

	const double x0 = src.getX( idx0 );
	const double y0 = src.getY( idx0 );
	const double x1 = src.getX( idx1 );
	const double y1 = src.getY( idx1 );

	const double a = ( y1 - y0 ) / ( x1 - x0 );
	y = a * ( x - x0 ) + y0;

	return y;
}


// cubic spline interpolation
void Interpolation::spline(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		double* xArray,
		unsigned int size
) {
	// check the parameter
	if( xArray == NULL || size == 0 ) {
		return;
	}

	// reserve
	dst.reserve( size );

	// check length
	unsigned int len = src.getLength();
	if( len < 4 ) {
		linear( src, dst, xArray, size );
		return;
	}

	// make table
	double* us = new double[ len - 1 ];
	double* zs = new double[ len ];

	zs[ 0 ] = 0.0;
	us[ 0 ] = 0.0;

	for( unsigned int i = 1; i < len - 1; i++ ) {
		// x, y
		double x0 = src.getX( i - 1 );
		double y0 = src.getY( i - 1 );
		double x1 = src.getX( i );
		double y1 = src.getY( i );
		double x2 = src.getX( i + 1 );
		double y2 = src.getY( i + 1 );

		// 
		double sig = ( x1 - x0 ) / ( x2 - x0 );
		double p = sig * zs[ i - 1 ] + 2.0;
		zs[ i ] = ( sig - 1.0 ) / p;

		us[ i ] = ( y2 - y1 ) / ( x2 - x1 ) - ( y1 - y0 ) / ( x1 - x0 );
		us[ i ] = ( 6.0 * us[ i ] / ( x2 - x0 ) - sig * us[ i - 1 ] ) / p;
	}

	zs[ len - 1 ] = 0.0;
	for( int i = (int)len - 2; i >= 0; i-- ) {
		zs[ i ] = zs[ i ] * zs[ i + 1 ] + us[ i ];
	}

	// interpolation
	unsigned index = 0;
	double x = xArray[ 0 ];
	for( unsigned int i = 1; i < src.getLength(); i++ ) {
		// get line
		double x0 = src.getX( i - 1 );
		double x1 = src.getX( i );
		double y0 = src.getY( i - 1 );
		double y1 = src.getY( i );

		double h = x1 - x0;

		while( index < size && ( x <= x1 || i == src.getLength() - 1 ) ) {
			// get y value
			double a = ( x1 - x ) / h;
			double b = ( x - x0 ) / h;

			double y = a * y0 + b * y1 + ( ( a * a * a - a ) * zs[ i - 1 ] + ( b * b * b - b ) * zs[ i ] ) * ( h * h ) / 6.0;

			// add
			dst.addPoint( x, y );

			index++;
			if( index < size ) {
				x = xArray[ index ];
			}
		}
	}

	// delete
	delete[] us;
	delete[] zs;
}
