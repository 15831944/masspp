/**
 * @file WindowFilterFunctions.cpp
 * @brief implements of WindowFilter plug-in functions
 *
 * @author M.Fukuda
 * @date 2010.06.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "WindowFilterFunctions.h"

using namespace kome::filter::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// exec Recatngular Window Filter
kome::objects::Variant execRectangularWindow( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	// SetParametaers
	RectangulaWindowFilter filter(params);
	// Execute
	filter.execute();
	return ret;
}

// exec Bartlet Window Filter
kome::objects::Variant execBartlettWindow( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	// SetParametaers
	BartlettWindowFilter filter(params);
	// Execute
	filter.execute();
	return ret;
}

// exec Gaussian Window Filter
kome::objects::Variant execGaussianWindow( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	// SetParametaers
	GaussianWindowFilter filter(params);
	// Execute
	filter.execute();
	return ret;
}
