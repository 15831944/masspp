/**
 * @file LinearFilterManager.cpp
 * @brief implements of LinearFilterManager class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LinearFilterManager.h"
#include "LinearFilter.h"


#include <math.h>


using namespace kome::filter::linear;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
LinearFilterManager::LinearFilterManager() {
}

// destructor
LinearFilterManager::~LinearFilterManager() {
}

// execute Averaging filter
void LinearFilterManager::averaging( const int width, kome::core::XYData& src, kome::core::XYData& dst ) {
	// create filter
	LinearFilter filter;

	filter.addWeight( 0, 1.0 );
	for( int i = 1; i <= width; i++ ) {
		filter.addWeight( i, 1.0 );
		filter.addWeight( -i, 1.0 );
	}

	// execute
	filter.execute( src, dst );
}

// execute Gaussian filter
void LinearFilterManager::gaussian( const double sigma, const int width, kome::core::XYData& src, kome::core::XYData& dst ) {
	// create filter
	LinearFilter filter;

	double a = 1.0 / ( sqrt( 2.0 * kome::numeric::Math::PI ) * sigma );
	double b = 2.0 * sigma * sigma;

	filter.addWeight( 0, a );
	for( int i = 1; i <= width; i++ ) {
		double w = a * exp( - (double)( i * i ) / b );

		filter.addWeight( i, w );
		filter.addWeight( -i, w );
	}

	// execute
	filter.execute( src, dst );
}

// execute Sharpening filter
void LinearFilterManager::sharpening( const double strength, const int width, kome::core::XYData& src, kome::core::XYData& dst ) {
	// create filter
	LinearFilter filter;

	filter.addWeight( 0, (double)( 2 * width ) * strength + 1.0 );
	for( int i = 1; i <= width; i++ ) {
		filter.addWeight( i, -strength );
		filter.addWeight( -i, -strength );
	}

	// execute
	filter.execute( src, dst );
}

// get instance
LinearFilterManager& LinearFilterManager::getInstance() {
	// create object (This is the only object.)
	static LinearFilterManager mgr;

	return mgr;
}
