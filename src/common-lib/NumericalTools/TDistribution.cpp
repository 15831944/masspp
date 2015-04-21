/**
 * @file TDistribution.cpp
 * @brief implements of TDistribution class
 *
 * @author S.Tanaka
 * @date 2010.11.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TDistribution.h"

#include "Statistics.h"
#include "Math.h"


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define VAL_STEP      0.01
#define WIDTH_RATE    100
#define LIST_SIZE     20


// static member
std::list< TDistribution > TDistribution::m_objList;


// constructor
TDistribution::TDistribution( const int n ) : m_n( std::min( 10000, n ) ) {
	// calculate coeffiecient
	m_coefficient = calcCoefficient( n );

	// create distribution
	createArray();
}

// destructor
TDistribution::~TDistribution() {
}

// creates array
void TDistribution::createArray() {
	// width
	const double width = VAL_STEP / (double)WIDTH_RATE;

	// integration with sympson method
	double sum = 0.0;
	const double start = -500.0;
	int startIdx = roundnum( start / width );

	std::vector< double > vals;

	for( int i = startIdx; i <= 0; i++ ) {
		const double x = width * (double)i;
		const double y = calcFrequency( x );
		const double ym = calcFrequency( x - width / 2.0 );
		const double yl = calcFrequency( x - width );

		sum += ( y + yl + 4.0 * ym ) / 6.0 * width;


		if( i % WIDTH_RATE == 0 ) {
			vals.push_back( sum );
		}
	}

	// create array
	const double d = sum - 0.5;
	double v = 0.5;
	for( int i = (int)vals.size() - 1; i >= 0 && v >= 1.0e-6; i-- ) {
		v = std::max( 0.0, vals[ i ] - d );
		m_distribution.push_back( v );
	}
}

// calculates p - value
double TDistribution::calcP( const double t ) {
	// index
	int idx = (int)floor( fabs( t / VAL_STEP ) );
	int idx1= idx + 1;

	if( idx >= (int)m_distribution.size() ) {
		return 0.0;
	}
	if( idx == (int)m_distribution.size() - 1 ) {
		return m_distribution.back();
	}

	// p-value
	const double d = t - VAL_STEP * (double)idx;
	const double p = ( 1.0 - d ) * m_distribution[ idx ] + d * m_distribution[ idx1 ];

	return ( 2.0 * p );
}

// calculate t value
double TDistribution::calcT(
		double* vals0,
		const unsigned int num0,
		double* vals1,
		const unsigned int num1
) {
	// degree of freedom
	const int dof0 = (int)num0 - 1;
	const int dof1 = (int)num1 - 1;

	if( dof0 <= 0 || dof1 <= 0 ) {
		return 0.0;
	}

	// mean
	const double m0 = Statistics::average( vals0, num0 );
	const double m1 = Statistics::average( vals1, num1 );

	// sum of squared deviation
	const double sum0 = Statistics::variance( vals0, num0 ) * (double)num0;
	const double sum1 = Statistics::variance( vals1, num1 ) * (double)num1;

	// estimated polulation variance
	const double epv = ( sum0 + sum1 ) / (double)( dof0 + dof1 );

	// sample standard error
	const double sse = sqrt( epv * ( 1.0 / (double)num0 + 1.0 / (double)num1 ) );

	// t value
	const double t = ( m1 - m0 ) / sse;
	return t;
}

// T-Test
double TDistribution::ttest(
		double* vals0,
		const unsigned int num0,
		double* vals1,
		const unsigned int num1
) {
	// distribution object
	if( num0 <= 1 || num1 <= 1 ) {
		return -FLT_MAX;
	}
	int v = (int)num0 + (int)num1 - 2;

	bool found = false;
	std::list< TDistribution >::iterator it = m_objList.begin();
	while( found = false && it != m_objList.end() ) {
		if( (*it).m_n == v ) {
			found = true;
		}
		else {
			it++;
		}
	}

	// create object
	if( it == m_objList.end() ) { 
		TDistribution d( v );
		m_objList.push_back( d );
	}
	else {
		TDistribution d = *it;
		m_objList.erase( it );
		m_objList.push_back( d );
	}

	while( m_objList.size() > LIST_SIZE ) {
		m_objList.pop_front();
	}

	// t-value
	const double t = calcT( vals0, num0, vals1, num1 );

	// p-value
	const double p = m_objList.back().calcP( t );

	return p;
}

// T-distribution frequency function
double TDistribution::calcFrequency( const double x ) {
	const double y = m_coefficient * pow( 1.0 + x * x / (double)m_n, - (double)( m_n + 1 ) / 2.0 );

	return y;
}

// T-distribution frequency function
double TDistribution::calcFrequency( const double x, const int v ) {
	double y = calcCoefficient( v );
	y *= pow( 1.0 + x * x / (double)v, - (double)( v + 1 ) / 2.0 );

	return y;
}

// calculates coefficient
double TDistribution::calcCoefficient( const int v ) {
	// odd or even
	int d = v / 2;
	int r = v % 2;

	double c = 0.0;
		
	if( r == 0 ) {
		c = sqrt( Math::PI ) / 2.0;
		for( int i = 1; i < d; i++ ) {
			c *= ( 2.0 * (double)i + 1.0 ) / 2.0 / ( double )i;
		}
	}
	else {
		c = 1.0 / sqrt( Math::PI );
		for( int i = 1; i <= d; i++ ) {
			c *= (double)i * 2.0 / ( 2.0 * (double)i - 1.0 );
		}
	}
	
	c /= sqrt( (double)v * Math::PI );

	return c;
}
