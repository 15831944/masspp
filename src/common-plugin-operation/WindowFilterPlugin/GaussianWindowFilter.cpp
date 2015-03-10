/**
 * @file GaussianWindowFilter.cpp
 * @brief implements of GaussianWindowFilter class
 *
 * @author M.Fukuda
 * @date 2010.06.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include <cmath>
#include "GaussianWindowFilter.h"

using namespace kome::filter::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
GaussianWindowFilter::GaussianWindowFilter(kome::objects::Parameters* params) 	: WindowFilter(params){}

// destructor
GaussianWindowFilter::~GaussianWindowFilter(){}

// Set Other Status
void GaussianWindowFilter::setOtherStatus(){
	m_sigma = m_settings->getDoubleValue( SIGMA_PARAM_NAME, 1.0 );
	if(m_sigma == 0.0) m_sigma = 1.0;
}

// Clac Weight with Gaussian Window Filter
double GaussianWindowFilter::getWeight(double x) {
	//Gaussian Weight Initialize
	double weights(1.0);

	weights = exp( - ( x * x / m_sigma * m_sigma));  // w(x) = exp(- (x^2 / sigma^2))

	if (weights < 0.0) weights = 0.0;

	return weights;
}
