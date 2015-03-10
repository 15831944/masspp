/**
 * @file MergedSpecPeakPosDetect.cpp
 * @brief interfaces of MergedSpecPeakPosDetect class
 *
 * @author M.Izumi
 * @date 2013.12.13
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "MergedSpecPeakPosDetect.h"
#include "MergedSpectrumPeaks.h"

#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>

using namespace kome::position::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MergedSpecPeakPosDetect::MergedSpecPeakPosDetect( bool bCalcFlg, bool bAlignmentFlg )
		: kome::ident::PeakPositionsDetector( bCalcFlg, bAlignmentFlg ){

	m_settings = NULL;
}

// destructor
MergedSpecPeakPosDetect::~MergedSpecPeakPosDetect(){
}

// on detect peak positons
int MergedSpecPeakPosDetect::onDetectPeakPositions( kome::ident::SampleGetter& getter, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress ){
	
	std::string errMsg = "";

	// get position
	MergedSpectrumPeaks merge( m_settings, &getter );
	
	kome::objects::Peaks2D peak;
	merge.execute( &peak, progress );

	// sample ID, profile ID
	std::map <int, int> profileMap;
	
	profileMap.clear();

	// 全サンプルのプロファイルを抜いてくる
	for( unsigned int i=0; i < peak.getNumberOfPeaks() && !progress.isStopped(); i++ ){
		kome::objects::Peak2DElement* elt = peak.getPeak( i );

		unsigned int inum = elt->getDataListSize();
		for( unsigned int j=0; j < inum; j++ ){
			int sampleId = elt->getDataSampleId( j );
			if  ( profileMap.find( sampleId ) == profileMap.end( ) ){
				profileMap[sampleId] = elt->getDataProfileId( j );
			}
		}
	}

	for( unsigned int i=0; i < peak.getNumberOfPeaks() && !progress.isStopped(); i++ ){
		kome::objects::Peak2DElement* elt = peak.getPeak( i );
		
		// ピーク位置 追加
		int idx = peakPos.addPeakPosition( elt->getRt(), elt->getMz(), elt->getCharge() );

		std::map< int, int >	nFindMap;
		nFindMap.clear( );

		unsigned int inum = elt->getDataListSize();
		kome::core::DataPoints	dsp;

		// Peak2DElementにセットされているプロファイルでセット
		for ( unsigned int j = 0 ; j < inum ; j++ ){
			kome::objects::PeakElement p = *elt->getDataPeak(j);
			
			kome::core::DataPoints dsp;
			dsp.addPoint( p.getX(), p.getY() );
			double val =  elt->getDataPeakValue( j );
			int sampleId = elt->getDataSampleId( j );
			
			int	profileId = elt->getDataProfileId( j );
						
			peakPos.setPeakValueInfo( idx, sampleId, val, dsp, &p, false, profileId );

			nFindMap[sampleId] = profileId;
		}		

		int	nSampleId;
		int	nProfileId;

		BOOST_FOREACH( boost::tie( nSampleId, nProfileId ), profileMap ){
			if  ( nFindMap.find( nSampleId ) == nFindMap.end( ) ){
				kome::core::DataPoints dsp;
				kome::objects::PeakElement p;

				double	val = -1.0;
				int sampleId = nSampleId;
				int	profileId = nProfileId;
							
				peakPos.setPeakValueInfo( idx, sampleId, val, dsp, &p, false, profileId );
			}
		}
	}

	// エラーメッセージセット
	setLastError( errMsg.c_str() );
	return ( HAS_RT | HAS_MZ );
}

// set settings
void MergedSpecPeakPosDetect::setSettings( kome::objects::SettingParameterValues* settings ){
	m_settings = settings;
}

// get settings
kome::objects::SettingParameterValues* MergedSpecPeakPosDetect::getSettings(){
	return m_settings;
}


