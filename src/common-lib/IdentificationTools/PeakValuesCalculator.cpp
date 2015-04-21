/**
 * @file PeakValuesCalculator.cpp
 * @brief Peak Values Calculator class
 *
 * @author M.Izumi
 * @date 2013.12.10
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "PeakValuesCalculator.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
PeakValuesCalculator::PeakValuesCalculator(){
	m_errorMsg = "";
}

// destructor
PeakValuesCalculator::~PeakValuesCalculator(){
}

// calculate Values
void PeakValuesCalculator::calculateValues( SampleGetter& sample, PeakPositions& peakPos, kome::core::Progress& progress ){
	onCalculateValues( sample, peakPos, progress );
}

// set last error
void PeakValuesCalculator::setLastError( const char* msg ){
	m_errorMsg = msg;
}

// get last error
std::string PeakValuesCalculator::getLastError(){
	return m_errorMsg;
}
