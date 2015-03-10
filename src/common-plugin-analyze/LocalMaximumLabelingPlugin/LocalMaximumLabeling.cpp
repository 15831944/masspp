/**
 * @file LocalMaximumLabeling.cpp
 * @brief implements of LocalMaximumLabeling class
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LocalMaximumLabeling.h"


using namespace kome::labeling::local;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
LocalMaximumLabeling::LocalMaximumLabeling() {
}

// destructor
LocalMaximumLabeling::~LocalMaximumLabeling() {
}

// peak picking
void LocalMaximumLabeling::pickPeaks(
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks
) {
	// check points
	if( xyData.getLength() == 0 ) {
		return;
	}

	// variables
	double startX	= xyData.getX( 0 );
	double startY	= xyData.getY( 0 );
	double endX	  = -1.0;
	double endY	  = -1.0;
	double prevX	 = startX;
	double prevY	 = startY;
	double sumX	  = 0.0;
	double intensity = 0.0;
	int num		  = 0;

	// search
	for( unsigned int i = 1; i < xyData.getLength(); i++ ) {
		// get data point
		double x = xyData.getX( i );
		double y = xyData.getY( i );

		// start
		if( num == 0 && y <= prevY ) {
			startX = x;
			startY = y;
		}

		// end
		if( num > 0 && y < prevY ) {
			endX = x;
			endY = y;
		}

		// add
		if( num > 0 && endX >= 0.0
				&& ( y >= prevY || i == xyData.getLength() - 1 ) ) {
			sumX /= (double)num;

			kome::objects::PeakElement* peak = peaks.createPeak( sumX, intensity );

			peak->setLeft( startX, 0.0 );
			peak->setRight( endX, 0.0 );
			peak->setApex( sumX, intensity );
			
			startX = prevX;
			startY = prevY;
			num = 1;
			intensity = y;
			sumX = x;
			endX = -1.0;
			endY = -1.0;
		}
		else {
			// apex
			if( y > intensity ) {
				num = 1;
				intensity = y;
				sumX = x;
				endX = -1.0;
				endY = -1.0;
			}
			else if( y == intensity ) {
				sumX += x;
				num++;
			}
		}

		// previous
		prevX = x;
		prevY = y;
	}
}
