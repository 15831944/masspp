/**
 * @file LinearBaselineDetection.cpp
 * @brief implements of LinearBaselineDetection class
 *
 * @author S.Tanaka
 * @date 2007.07.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LinearBaselineDetection.h"

#include <math.h>


using namespace kome::baseline::linear;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define NUMBER_OF_REGIONS				 32
#define WINDOW_WIDTH_PARAM_NAME			 "width"


// constructor
LinearBaselineDetection::LinearBaselineDetection( kome::objects::SettingParameterValues* settings ) {
	// settings
	m_settings = settings;

	// parameters
	m_windowWidth = 2.0;
	if( settings != NULL ) {
		m_windowWidth = settings->getDoubleValue( WINDOW_WIDTH_PARAM_NAME, 2.0 );
	}
}

// destructor
LinearBaselineDetection::~LinearBaselineDetection() {
}

// set window width
void LinearBaselineDetection::setWindowWidth( const double width ) {
	m_windowWidth = MAX( 0.0, width );
}

// get window width
double LinearBaselineDetection::getWindowWidth() {
	return m_windowWidth;
}

// get baseline
void LinearBaselineDetection::getBaseline( kome::core::XYData& src, kome::core::XYData& baseline ) {
	// check the parameters
	if( src.getLength() == 0 ) {
		return;
	}

	// distance
	double d = m_windowWidth / 2.0;

	// get baseline
	baseline.reserve( src.getLength() );
	for( unsigned int i = 0; i < src.getLength(); i++ ) {
		// x, y
		double x = src.getX( i );
		double y = src.getY( i );

		// left minimum
		double leftX = x;
		double leftY = y;
		double left = x - d;

		int leftIdx = (int)i - 1;
		bool leftFound = false;
		while( leftIdx >= 0 && src.getX( leftIdx ) >= left ) {
			if( src.getY( leftIdx ) < leftY ) {
				leftX = src.getX( leftIdx );
				leftY = src.getY( leftIdx );
				leftFound = true;
			}
			leftIdx--;
		}

		// right minimum
		double rightX = x;
		double rightY = y;
		double right = x + d;

		int rightIdx = (int)i + 1;
		bool rightFound = false;
		while( rightIdx < (int)src.getLength() && src.getX( rightIdx ) <= right ) {
			if( src.getY( rightIdx ) < rightY ) {
				rightX = src.getX( rightIdx );
				rightY = src.getY( rightIdx );
				rightFound = true;
			}
			rightIdx++;
		}

		// get baseline
		if( leftFound && rightFound ) {
			// linear interpolation
			y = ( ( rightX - x ) * leftY + (x - leftX ) * rightY ) / ( rightX - leftX );
		}

		baseline.addPoint( x, y );
	}
} 
