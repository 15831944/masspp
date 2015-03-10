/**
 * @file IntensityRateChargeDetect.cpp
 * @brief implements of IntensityRateChargeDetect class
 *
 * @author S.Tanaka
 * @date 2006.10.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "IntensityRateChargeDetect.h"

#include <math.h>

#define DEFAULT_BORDERLINE_POS	1.0
#define DEFAULT_RATE_THRESHOLD	0.09
#define DEFAULT_MAX_CHARGE		  3

using namespace kome::charge::ir;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
IntensityRateChargeDetect::IntensityRateChargeDetect() {
	m_border = DEFAULT_BORDERLINE_POS;
	m_rateThreshold = DEFAULT_RATE_THRESHOLD;
	m_maxCharge = DEFAULT_MAX_CHARGE;
}

// destructor
IntensityRateChargeDetect::~IntensityRateChargeDetect() {
}

// peak picking
void IntensityRateChargeDetect::getCharge(
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks
) {
	// border line
	double border = peaks.getPrecursor() + m_border;

	// get total intensity
	double tic = 0.0;
	double outBorder = 0.0;
	for( unsigned int i = 0; i < xyData.getLength(); i++ ) {
		// x, y
		double x = xyData.getX( i );
		double y = xyData.getY( i );

		// total intensity
		tic += y;
		if( x > border ) {
			outBorder += y;
		}
	}
	if( tic == 0.0 ) {
		return;
	}

	// rate 
	double rate = outBorder / tic;

	// Clear any existing charges before performing charge detection.
	peaks.clearCharge();

	// get charge
	if( rate <= m_rateThreshold ) {
		peaks.addCharge( 1 );
	}
	else {
		for( int i = 2; i <= m_maxCharge; i++ ) {
			peaks.addCharge( i );
		}
	}
}
