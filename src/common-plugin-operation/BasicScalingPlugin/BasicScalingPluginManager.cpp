/**
 * @file BasicScalingPluginManager.cpp
 * @brief implements of BasicScaling class.
 *
 * @author M.Izumi
 * @date 2012.11.05
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "BasicScalingPluginManager.h"

#include <math.h>

using namespace kome::scaling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
BasicScalingManager::BasicScalingManager(){
}

// destructor
BasicScalingManager::~BasicScalingManager() {
}

// UV
double BasicScalingManager::UV( double p, double v ){
	double res = 0.0;

	res = v/p;
	
	return res;
}

// pareto
double BasicScalingManager::pareto( double p, double v ){
	double res = 0.0;

	res = v/sqrt(p);
	
	return res;
}

// log10
double BasicScalingManager::log( double p ){
	double res = 0.0;

	res = log10( p );

	return res;
}

// ln
double BasicScalingManager::ln( double p ){
	double res = 0.0;

    res = log( p );   // âΩåÃÇ©é©ëRëŒêîÇ≈ÇÕÇ»Ç≠èÌópëŒêîÇ≈ï‘Ç¡ÇƒÇ≠ÇÈÅB 

	return res;
}

// get instance
BasicScalingManager& BasicScalingManager::getInstance(){
	// create object
	static BasicScalingManager mgr;

	return mgr;
}
