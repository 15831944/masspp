/**
 * @file MergePeakPosDetector.cpp
 * @brief interfaces of MergePeakPosDetector class
 *
 * @author M.Izumi
 * @date 2013.12.13
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "MergePeakPosDetect.h"
#include "LabelingAndMergePeakPosition.h"

using namespace kome::position::merge;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MergePeakPosDetect::MergePeakPosDetect( bool bCalcFlg, bool bAlignmentFlg )
		: kome::ident::PeakPositionsDetector( bCalcFlg, bAlignmentFlg ){
	m_settings = NULL;
}

// destructor
MergePeakPosDetect::~MergePeakPosDetect(){
}

// on detect peak positons
int MergePeakPosDetect::onDetectPeakPositions( kome::ident::SampleGetter& getter, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress ){
	
	std::string errMsg = "";

	// get position
	LabelingAndMergePeakPosition mergePeakPos( m_settings, &getter );

	kome::objects::Peaks2D peak;
	mergePeakPos.execute( &peak, progress );

	for( unsigned int i=0; i < peak.getNumberOfPeaks() && !progress.isStopped(); i++ ){
		kome::objects::Peak2DElement* elt = peak.getPeak( i );
		
		// ピーク位置 追加
		int idx = peakPos.addPeakPosition( elt->getRt(), elt->getMz(), elt->getCharge() );
		unsigned int inum = elt->getDataListSize();
		for( unsigned int j=0; j < inum; j++ ){

			kome::objects::PeakElement p = *elt->getDataPeak(j);
			
			kome::core::DataPoints dsp;
			dsp.addPoint( p.getX(), p.getY() );
			double val =  elt->getDataPeakValue( j );
			int sampleId = elt->getDataSampleId( j );
						
			peakPos.setPeakValueInfo( idx, sampleId, val, dsp, &p, false );			
		}			
	}
	
	// エラーメッセージセット
	setLastError( errMsg.c_str() );

	return ( HAS_RT | HAS_MZ | HAS_CHARGE );
}

// set settings
void MergePeakPosDetect::setSettings( kome::objects::SettingParameterValues* settings ){
	m_settings = settings;
}

// get settings
kome::objects::SettingParameterValues* MergePeakPosDetect::getSettings(){
	return m_settings;
}


