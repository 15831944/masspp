/**
 * @file CentroidModeLabeling.cpp
 * @brief implements of CentroidModeLabeling class
 *
 * @author S.Tanaka
 * @date 2006.10.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "CentroidModeLabeling.h"


using namespace kome::labeling::centroid;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
CentroidModeLabeling::CentroidModeLabeling() {
}

// destructor
CentroidModeLabeling::~CentroidModeLabeling() {
}

// peak picking
void CentroidModeLabeling::getPeaks(
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks
) {
	// add all data
	peaks.reserve( xyData.getLength() );
	for( unsigned int i = 0; i < xyData.getLength(); i++ ) {
		// x, y
		double x = xyData.getX( i );
		double y = xyData.getY( i );

		// add
		if( y > 0.0 ) {
			peaks.addPoint( x, y );
		}
	}
}
