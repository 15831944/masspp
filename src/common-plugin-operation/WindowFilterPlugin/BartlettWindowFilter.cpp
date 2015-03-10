/**
 * @file BartlettWindowFilter.cpp
 * @brief implements of BartlettWindowFilter class
 *
 * @author M.Fukuda
 * @date 2010.06.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "BartlettWindowFilter.h"

using namespace kome::filter::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
BartlettWindowFilter::BartlettWindowFilter(kome::objects::Parameters* params) : WindowFilter(params){}

// destructor
BartlettWindowFilter::~BartlettWindowFilter(){}

// Clac Weight with Bartlett Window Filter
double BartlettWindowFilter::getWeight(double x) {
	//Rectangula Initialize
	double weights(1.0);

	weights = (m_width - x) / m_width; // w(x) = 1-2|x-0.5| {0<=x<=1} 

	if (weights < 0.0) weights = 0.0;

	return weights;
}
