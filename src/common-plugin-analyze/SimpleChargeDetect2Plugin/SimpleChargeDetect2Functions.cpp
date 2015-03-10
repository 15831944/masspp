/**
 * @file SimpleChargeDetect2Functions.cpp
 * @brief implements of SimpleChargeDetect2 plug-in functions
 *
 * @author M.Fukuda
 * @date 2010.06.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "SimpleChargeDetect2Functions.h"
#include "SimpleChargeDetect2.h"

using namespace kome::charge::simple2;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// detect peaks
kome::objects::Variant detectCharge2( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// charge detect object
	SimpleChargeDetect detector(params);
	if( !detector.checkStatus() ) return ret;
	
	// charge detect
	detector.getCharge();

	return ret;
}
