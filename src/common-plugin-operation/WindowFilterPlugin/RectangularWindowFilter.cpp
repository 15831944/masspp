/**
 * @file RectangularWindowFilter.cpp
 * @brief implements of RectangularWindowFilter class
 *
 * @author M.Fukuda
 * @date 2010.06.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "RectangularWindowFilter.h"

using namespace kome::filter::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
RectangulaWindowFilter::RectangulaWindowFilter(kome::objects::Parameters* params) : WindowFilter(params){}

// destructor
RectangulaWindowFilter::~RectangulaWindowFilter(){}

// Clac Weight with Rectangula Window Filter
double RectangulaWindowFilter::getWeight(double x) {
	//Rectangula Initialize
	double weights(1.0);

	return weights;
}


