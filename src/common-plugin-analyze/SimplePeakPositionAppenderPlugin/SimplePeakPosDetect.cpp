/**
 * @file SimplePeakPosDetect.cpp
 * @brief interfaces of SimplePeakPosDetect class
 *
 * @author M.Izumi
 * @date 2013.12.13
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "SimplePeakPosDetect.h"

#define RT_PARAM_NAME		"rt"
#define MZ_PARAM_NAME		"mz"
#define CHARGE_PARAM_NAME	"charge"

using namespace kome::position::simple;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
SimplePeakPosDetect::SimplePeakPosDetect( bool bCalcFlg, bool bAlignmentFlg )
		: kome::ident::PeakPositionsDetector( bCalcFlg, bAlignmentFlg ){
	m_settings = NULL;

}

// destructor
SimplePeakPosDetect::~SimplePeakPosDetect(){
}

// on detect peak positons
int SimplePeakPosDetect::onDetectPeakPositions( kome::ident::SampleGetter& getter, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress ){
	// get position
	double rt = m_settings->getDoubleValue( RT_PARAM_NAME, -1.0 );
	double mz = m_settings->getDoubleValue( MZ_PARAM_NAME, -1.0 );
	int charge = m_settings->getIntValue( CHARGE_PARAM_NAME, -1 );

	if( rt < 0.0 && mz < 0.0 ) {
		LOG_ERROR_CODE( FMT( "Both of RT and m/z is empty. At least one of RT and m/z is required. Input RT or m/z." ), ERR_OTHER );
		return -1;
	}

	kome::objects::Peaks2D peaks;
	// add position
	kome::objects::Peak2DElement* peak = peaks.createPeak( rt, mz, -1.0 );
	peak->setCharge( charge );
	
	// ピーク位置 追加
	int idx = peakPos.addPeakPosition( peak->getRt(), peak->getMz(), peak->getCharge() );

	progress.fill();

	return ( HAS_RT | HAS_MZ | HAS_CHARGE );
}

// set settings
void SimplePeakPosDetect::setSettings( kome::objects::SettingParameterValues* settings ){
	m_settings = settings;
}

// get settings
kome::objects::SettingParameterValues* SimplePeakPosDetect::getSettings(){
	return m_settings;
}

