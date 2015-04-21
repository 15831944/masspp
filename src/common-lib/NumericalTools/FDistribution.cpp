/**
 * @file FDistribution.cpp
 * @brief implements of FDistribution class
 *
 * @author S.Tanaka
 * @date 2010.11.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "FDistribution.h"

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
std::list< FDistribution > FDistribution::m_objList;


// constructor
FDistribution::FDistribution( const int m, const int n ) : m_m( std::min( 1000, m ) ), m_n( std::min( 10000, n ) ) {
	// calculate coeffiecient
	m_coefficient = calcCoefficient( m, n );

	// create distribution
	createArray();
}

// destructor
FDistribution::~FDistribution() {
}

// creates array
void FDistribution::createArray() {
	// width
	const double width = VAL_STEP / (double)WIDTH_RATE;

	// integration with sympson method
	double sum = 0.0;
	const double start = 5000.0;
	int startIdx = roundnum( start / width );

	std::vector< double > vals;

	for( int i = startIdx; i >= 0; i-- ) {
		const double x = width * (double)i;
		const double y = calcFrequency( std::max( x, width / 100.0 ) );
		const double ym = calcFrequency( x + width / 2.0 );
		const double yl = calcFrequency( x + width );

		sum += ( y + yl + 4.0 * ym ) / 6.0 * width;

		if( i % WIDTH_RATE == 0 ) {
			vals.push_back( sum );
		}
	}

	// create array
	const double d = sum - 1.0;
	double v = 1.0;
	for( int i = (int)vals.size() - 1; i >= 0 && v >= 1.0e-6; i-- ) {
		v = std::max( 0.0, vals[ i ] - d );
		m_distribution.push_back( v );
	}
}

// calculates p - value
double FDistribution::calcP( const double f ) {
	// index
	int idx = (int)floor( fabs( f / VAL_STEP ) );
	int idx1= idx + 1;

	if( idx >= (int)m_distribution.size() ) {
		return 0.0;
	}
	if( idx == (int)m_distribution.size() - 1 ) {
		return m_distribution.back();
	}

	// p-value
	const double d = f - VAL_STEP * (double)idx;
	const double p = ( 1.0 - d ) * m_distribution[ idx ] + d * m_distribution[ idx1 ];

	return p;
}

// calculate t value
double FDistribution::calcF(
		const int num,
		double** vals,
		int* nums
) {
	// degrees of freedom
	const int f0 = num - 1;
	if( f0 <= 0 ) {
		return - FLT_MAX;
	}

	int f1 = 0;
	for( int i = 0; i < num; i++ ) {
		int tmpF = nums[ i ] - 1;
		if( tmpF <= 0 ) {
			return - FLT_MAX;
		}
		f1 += tmpF;
	}

	// F value
	std::vector< double > averages;
	double v0 = 0.0;
	double v1 = 0.0;

	double m = 0.0;
	for( int i = 0; i < num; i++ ) {
		const double tmp = Statistics::average( vals[ i ], nums[ i ] );
		averages.push_back( tmp );
		m += tmp * (double)nums[ i ];

		v1 += (double)nums[ i ] * Statistics::variance( vals[ i ], nums[ i ] );
	}

	m /= (double)( f1 + num );

	for( int i = 0; i < num; i++ ) {
		const double d = averages[ i ] - m;
		v0 += (double)nums[ i ] * d * d;
	}

	const double f = ( v0 / (double)f0 ) / ( v1 / (double)f1 );

	return f;
}

// T-Test
double FDistribution::anova(
		const int num,
		double** vals,
		int* nums
) {
	// degrees of freedom
	const int f0 = num - 1;
	if( f0 <= 0 ) {
		return - FLT_MAX;
	}

	int f1 = 0;
	for( int i = 0; i < num; i++ ) {
		int tmpF = nums[ i ] - 1;
		if( tmpF <= 0 ) {
			return - FLT_MAX;
		}
		f1 += tmpF;
	}

	// distribution object
	bool found = false;
	std::list< FDistribution >::iterator it = m_objList.begin();
	while( found = false && it != m_objList.end() ) {
		if( (*it).m_m == f0 && (*it).m_n == f1 ) {
			found = true;
		}
		else {
			it++;
		}
	}

	// create object
	if( it == m_objList.end() ) { 
		FDistribution d( f0, f1 );
		m_objList.push_back( d );
	}
	else {
		FDistribution d = *it;
		m_objList.erase( it );
		m_objList.push_back( d );
	}

	while( m_objList.size() > LIST_SIZE ) {
		m_objList.pop_front();
	}

	// f-value
	const double f = calcF( num, vals, nums );

	// p-value
	const double p = m_objList.back().calcP( f );

	return p;
}

// T-distribution frequency function
double FDistribution::calcFrequency( const double x ) {
	double y = 0.0;
	if( x < 0.0 ) {
		return - FLT_MAX;
	}
	else if( x == 0.0 ) {
		if( m_m == 1 ) {
			y = FLT_MAX;
		}
		else if( m_m == 2 ) {
			y = 1.0;
		}
		else {
			y = 0.0;
		}
	}
	else {
		y = m_coefficient * pow( x, (double)m_m / 2.0 - 1.0 );
		y /= pow( (double)m_m * x + (double)m_n, (double)( m_m + m_n ) / 2.0 );
	}

	return y;
}

// T-distribution frequency function
double FDistribution::calcFrequency( const double x, const int v0, const int v1 ) {
	double y = 0.0;

	if( x < 0.0 ) {
		return - FLT_MAX;
	}
	else if( x == 0.0 ) {
		if( v0 == 1 ) {
			return FLT_MAX;
		}
		else if( v0 == 2 ) {
			y = 1.0;
		}
		else {
			y = 0.0;
		}
	}
	else {
		y = calcCoefficient( v0, v1 );
		y *= pow( x, (double)v0 / 2.0 - 1.0 );
		y /= pow( (double)v0 * x + (double)v1, (double)( v0 + v1 ) / 2.0 );
	}

	return y;
}

// calculates coefficient
double FDistribution::calcCoefficient( const int v0, const int v1 ) {
	// odd or even
	int d0 = v0 / 2;
	int r0 = v0 % 2;
	int d1 = v1 / 2;
	int r1 = v1 % 2;

	// coefficient
	double c = 0.0;

	if( r0 == 0 && r1 == 0 ) {    // v0: even, v1: even
		int num = 1;
		c = 1.0;

		for( int i = 1; i <= d0; i++ ) {
			c *= (double)v0 * (double)num;
			if( i < d0 ) {
				c /= (double)i;
			}
			num++;
		}
		for( int i = 1; i <= d1; i++ ) {
			c *= (double)v1;
			if( i < d1 ) {
				c *= (double)num;
				c /= (double)i;
			}
			num++;
		}
	}
	else if( r0 == 1 && r1 == 0 ) {   // v0: odd, v1: even
		c = sqrt( (double)v0 );

		for( int i = 1; i <= d1; i++ ) {
			c *= (double)v1 * (double)( 2 * ( d0 + i ) - 1 ) / 2.0;

			if( i < d1 ) {
				c /= (double)i;
			}
		}
		for( int i = 1; i <= d0; i++ ) {
			c *= (double)v0;
		}
	}
	else if( r0 == 0 && r1 == 1 ) {    // v0: even, v1: odd
		c = sqrt( (double)v1 );

		for( int i = 1; i <= d0; i++ ) {
			c *= (double)v0 * (double)( 2 * ( d1 + i ) - 1 ) / 2.0;

			if( i < d0 ) {
				c /= (double)i;
			}
		}
		for( int i = 1; i <= d1; i++ ) {
			c *= (double)v1;
		}
	}
	else {    // v0: odd, v1: odd
		c = sqrt( (double)v0 ) * sqrt( (double)v1 ) / Math::PI;

		for( int i = 1; i <= d0; i++ ) {
			c *= (double)i * (double)v0 * 2.0 / (double)( 2 * i - 1 );
		}
		for( int i =1; i <= d1; i++ ) {
			c *= (double)( d0 + i ) * (double)v1 * 2.0 / (double)( 2 * i - 1 );
		}
	}

	return c;
}
