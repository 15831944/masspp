/**
 * @file Statistics.cpp
 * @brief implement of Statistics class
 *
 * @author S.Tanaka
 * @date 2007.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "Statistics.h"

#include <math.h>
#include <float.h>

#include <vector>
#include <algorithm>


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// select
double Statistics::select( double* values, const unsigned int size, const unsigned int k ) {
	// check parameters
	if( k == 0 || k > size ) {
		throw FMT( "Searching position is illegal." );
	}
	if( size == 0 ) {
		throw FMT( "The array size is illegal." );
	}
	if( size == 1 ) {
		return values[ 0 ];
	}

	// copy to array
	double* arr = new double[ size ];
	memcpy( arr, values, sizeof( double ) * size );

	// select
	double v = select( values, 0, (int)size - 1, k );

	// delete
	delete[] arr;

	return v;
}

// median
double Statistics::median( double* values, const unsigned int size ) {
	// check parameters
	if( size == 0 ) {
		throw FMT( "The array size is illegal." );
	}
	if( size == 1 ) {
		return values[ 0 ];
	}

	// get median
	double m = double();
	if( size % 2 == 0 ) {	// array size is even number
		double v0 = select( values, size, size / 2 );
		double v1 = select( values, size, size / 2 + 1 );

		m = ( v0 + v1 ) / 2.0;
	}
	else {  // array size if odd number
		m = select( values, size, ( size + 1 ) / 2 );
	}

	return m;
}

// average
double Statistics::average( double* values, const unsigned int size ) {
	// check parameters
	if( size == 0 ) {
		throw FMT( "The array size is illegal." );
	}
	if( size == 1 ) {
		return values[ 0 ];
	}

	// calculate
	double avg = 0.0;
	for( unsigned int i = 0; i < size; i++ ) {
		avg += values[ i ];
	}

	avg /= (double)size;

	return avg;
}

// variance
double Statistics::variance( double* values, const unsigned int size ) {
	// check parameters
	if( values == NULL || size == 0 ) {
		return 0.0;
	}

	// create array (value^2)
	double* value2s = new double[ size ];
	for( unsigned int i = 0; i < size; i++ ) {
		value2s[ i ] = values[ i ] * values[ i ];
	}

	// calculate [ V(X)=E(X^2)-{E(X)}^2 ]
	double avg = average( values, size );
	double avg2 = average( value2s, size );

	double v = avg2 - ( avg * avg );
	v = MAX( 0.0, v );

	// delete
	delete[] value2s;

	return v;
}

// standard deviation
double Statistics::sd( double* values, const unsigned int size ) {
	// calculate variance
	double v = variance( values, size );

	// standard deviation
	return sqrt( v );
}

// Pearson's correlation coefficient
double Statistics::pearson( double* xArray, double* yArray, const unsigned int size ) {
	// check parameters
	if( xArray == NULL || yArray == NULL || size == 0 ) {
		LOG_WARN( FMT( "Cannot calculate correlation coefficient." ) );
		return FLT_MAX;
	}

	// means
	double xMean = average( xArray, size );
	double yMean = average( yArray, size );

	// calculate correlation
	double xx = 0.0;
	double yy = 0.0;
	double xy = 0.0;
	for( unsigned int i = 0; i < size; i++ ) {
		// x, y
		double x0 = xArray[ i ] - xMean;
		double y0 = yArray[ i ] - yMean;

		xx += x0 * x0;
		yy += y0 * y0;
		xy += x0 * y0;
	}

	xx = MAX( 0.0, xx );
	yy = MAX( 0.0, yy );
	if( xx == 0.0 || yy == 0.0 || xy == 0.0 ) {
		return 0.0;
	}

	double corr = xy / ( sqrt( xx ) * sqrt( yy ) );

	return corr;
}

// partition
int Statistics::partition( double* values, const int left, const int right, const int pvtIdx ) {
	// get pivot value
	double pvtVal = values[ pvtIdx ];

	// swap
	std::swap( values[ pvtIdx ], values[ right ] );

	// store index
	int storeIdx = left;
	for( int i = left; i < right; i++ ) {
		if( values[ i ] < pvtVal ) {
			// swap
			std::swap( values[ i ], values[ storeIdx ] );
			storeIdx++;
		}
	}

	// swap
	std::swap( values[ right ], values[ storeIdx ] );

	return storeIdx;
}

// select
double Statistics::select( double* values, const int left, const int right, const unsigned int k ) {
	// partition
	int pvtIdx = ( left + right ) / 2;
	int partIdx = partition( values, left, right, pvtIdx );

	// search
	int kIdx = left + k - 1;
	if( partIdx > kIdx ) {
		return select( values, left, partIdx - 1, k );
	}
	if( partIdx < kIdx ) {
		return select( values, partIdx + 1, right, (int)k - 1 - ( partIdx - left ) );
	}
	return values[ kIdx ];
}
