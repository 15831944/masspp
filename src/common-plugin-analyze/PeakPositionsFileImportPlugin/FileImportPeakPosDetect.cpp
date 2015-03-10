/**
 * @file FileImportPeakPosDetect.cpp
 * @brief interfaces of FileImportPeakPosDetect class
 *
 * @author M.Izumi
 * @date 2013.12.13
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "FileImportPeakPosDetect.h"
#include "PositionImporter.h"

using namespace kome::position::file;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
FileImportPeakPosDetect::FileImportPeakPosDetect( bool bCalcFlg, bool bAlignmentFlg )
	: kome::ident::PeakPositionsDetector( bCalcFlg, bAlignmentFlg ) {

	m_settings = NULL;

}

// destructor
FileImportPeakPosDetect::~FileImportPeakPosDetect(){
}

// on detect peak positons
int FileImportPeakPosDetect::onDetectPeakPositions( kome::ident::SampleGetter& getter, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress ){
	
	std::string errMsg = "";

	kome::objects::Peaks2D peak;
	// get position
	PositionImporter importer( m_settings );
	importer.execute( &peak );

	kome::objects::Sample* sample = NULL;
			
	for( unsigned int i=0; i < peak.getNumberOfPeaks() && !progress.isStopped(); i++ ){
		kome::objects::Peak2DElement* elt = peak.getPeak( i );
					
		// ピーク位置 追加
		int idx = peakPos.addPeakPosition( elt->getRt(), elt->getMz(), elt->getCharge() );
				
		// プロパティ セット
		unsigned int inum = elt->getNumberOfParameters();
		for( unsigned int j=0; j < inum; j++ ){
			std::string name = elt->getParameterName( j );
			std::string val = elt->getParameterValue( j );
			int type = elt->getParameterType( j );

			peakPos.setPeakProperty( idx, name.c_str(), val.c_str(), type );
		}
	}

	// エラーメッセージセット
	setLastError( errMsg.c_str() );

	return  ( HAS_RT | HAS_MZ );
}

// set settings
void FileImportPeakPosDetect::setSettings( kome::objects::SettingParameterValues* settings ){
	m_settings = settings;
}

// get settings
kome::objects::SettingParameterValues* FileImportPeakPosDetect::getSettings(){
	return m_settings;
}
