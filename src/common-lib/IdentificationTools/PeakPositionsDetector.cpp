/**
 * @file PeakPositionsDetector.cpp
 * @brief PeakPositionsÅ@Detector class
 *
 * @author M.Izumi
 * @date 2013.12.10
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "PeakPositionsDetector.h"
#include "PeakPositions.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
PeakPositionsDetector::PeakPositionsDetector( bool bcalcFlg, bool balignmentFlg  ){
	m_bCalcFlg = bcalcFlg;
	m_bAlignmentFlg = balignmentFlg;
	
}

// destructor
PeakPositionsDetector::~PeakPositionsDetector(){
}

// can calculate peak values
bool PeakPositionsDetector::canCalculatePeakValues(){
	return m_bCalcFlg;
}

// is alignment suppoprted
bool PeakPositionsDetector::isAlignmentSuppoprted(){
	return m_bAlignmentFlg;
}

// detec peak positons
int PeakPositionsDetector::detectPeakPositions(
	SampleGetter& getter,
	PeakPositions& peakPos, 
	kome::core::Progress& progress
){
	int detect = onDetectPeakPositions( getter, peakPos, progress );
	
	int peaks = peakPos.getNumberOfPeaks();
	if( canCalculatePeakValues() ){
		if( peaks == 0 ){
			_ASSERT( peaks==0 );
		}
	}else{
		if( detect == -1 ){
			_ASSERT( peaks > 0 );
		}
	}
	return detect;
}

// set last error
void PeakPositionsDetector::setLastError( const char* msg ){
	m_errorMsg = msg;
}

// get last error
std::string PeakPositionsDetector::getLastError(){
	return m_errorMsg;
}
