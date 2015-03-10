/**
 * @file UTest.cpp
 * @brief implements of CommonsMath class.
 *
 * @author satstnka
 * @date 2012.07.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "UTest.h"


using namespace kome::statistic;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// p-value
double UTest::test( double* v0, const int n0, double* v1, const int n1 ) {
	// return value
	double p = -1.0;

	// check parameters
	if( v0 == NULL || v1 == NULL || n0 < 2 || n1 < 2 ) {
		return p;
	}

	// sort
	int n = n0 + n1;
	std::vector< ValueInfo > vals;
	vals.resize( n );

	for( int i = 0; i < n0; i++ ) {
		vals[ i ].val = v0[ i ];
		vals[ i ].group = 0;
	}

	for( int i = 0; i < n1; i++ ) {
		vals[ n0 + i ].val = v1[ i ];
		vals[ n0 + i ].group = 1;
	}

	std::sort( vals.begin(), vals.end(), lessValue );

	// order
	double prevVal = - FLT_MAX;
	double order = 0.0;
	int prevIdx = -1;
	std::map< int, double > vmap;
	double num = 0.0;

	for( unsigned int i = 0; i < vals.size(); i++ ) {
		double v = vals[ i ].val;

		if( v > prevVal ) {
			vals[ i ].order = (double)( i + 1 );
			order = (double)( i + 1 );
			num = 1.0;
			prevIdx = (int)i;
		}
		else {
			order += (double)( i + 1 );
			num = num + 1.0;
			for( int j = prevIdx; j <= (int)i; j++ ) {
				vals[ j ].order = order / num;
			}

			vmap[ prevIdx ] = num * num * num - num;
		}

		prevVal = v;
	}

	// u value
	double r0 = 0.0;
	double r1 = 0.0;
	for( unsigned int i = 0; i < vals.size(); i++ ) {
		int group = vals[ i ].group;
		double order = vals[ i ].order;

		if( group == 0 ) {
			r0 += order;
		}
		else if( group == 1 ) {
			r1 += order;
		}
	}

	double u0 = (double)( n0 * n1 + ( n0 * ( n0 + 1 ) / 2 ) ) - r0;
	double u1 = (double)( n0 * n1 + ( n1 * ( n1 + 1 ) / 2 ) ) - r1;
	double u = std::min( u0, u1 );

	// p-value
	double e = (double)( n0 * n1 ) / 2.0;
	double sigma = (double)( n0 * n1 );
	double tmp = (double)( n * n * n - n );
	for( std::map< int, double >::iterator it = vmap.begin(); it != vmap.end(); it++ ) {
		tmp -= (*it).second;
	}
	sigma = sigma * tmp / (double)( 12 * ( n * n - n ) );
	sigma = sqrt( sigma );

	double x = std::max( ( fabs( u - e ) - 0.5 ) / sigma, 0.0 );

	kome::numeric::NormalDistribution nd;
	p = nd.calcP( x ) * 2.0;

	return p;
}

// less value
bool UTest::lessValue( ValueInfo& v0, ValueInfo& v1 ) {
	return ( v0.val < v1.val );
}
