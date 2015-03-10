/**
 * @file NeighborPeakValuesCalculator.cpp
 * @brief interfaces of NeighborPeakValuesCalculator class
 *
 * @author M.Izumi
 * @date 2013.12.20
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "NeighborPeakValuesCalculator.h"
#include "NeighborSpecPeaks.h"
#include "NeighborChromPeaks.h"

using namespace kome::search::neighbor;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
NeighborPeakValuesCalculator::NeighborPeakValuesCalculator( bool bSpec )
	: kome::ident::PeakValuesCalculator() {
	m_bSpec = bSpec;
	m_settings = NULL;
}

// destructor
NeighborPeakValuesCalculator::~NeighborPeakValuesCalculator(){
}

// on calculate values
void NeighborPeakValuesCalculator::onCalculateValues( kome::ident::SampleGetter& getter, kome::ident::PeakPositions& peakPos, kome::core::Progress& progress ){
	std::string errMsg = "";
		
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	
	// selected matrix
	int matrix = identMgr.getSelectedMatrix();
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, matrix );
	
	// alignment
	kome::operation::Alignment* alignment = identMgr.getAlignment( matrix );    // アライメント実行
		
	std::string val;
	std::string strName;
	for( unsigned int i=0; i < m_settings->getNumberOfParameters(); i++ ){
		strName = m_settings->getParameterName(i);
		
		if( strName.compare( "spec_data_manipulat" ) == 0 ){			
			val = m_settings->getParameterValue( strName.c_str() );
			break;
		}else if( strName.compare( "chrom_data_manipulat" ) == 0 ){
			val = m_settings->getParameterValue( strName.c_str() );
			break;
		}
	}

	// sample 
	kome::objects::Sample* sample = NULL;
	int i=0;
	
	while( ( sample = getter.next() ) != NULL && !progress.isStopped() ){
		// progress
		kome::core::Progress* subProgress = progress.getSubProgress( i + 1 );
		progress.setStatus( 
			FMT( "Peak Searching [%d] ... %s", ( i + 1 ), sample->getRootDataGroupNode()->getName() ).c_str()
		);

		// alignment
		kome::operation::Alignment* alignment = getter.getAlignment();
				
		// search
		if( m_bSpec ){ // spectrum
			
			NeighborSpecPeaks search( m_settings );
			search.searchPeaks( sample, peakPos, alignment, getter.getMatrixSampleId(), progress, val.c_str() );

		}else{	// chromatogram
			NeighborChromPeaks search( m_settings );
			search.searchPeaks( sample, peakPos, alignment, getter.getMatrixSampleId(), progress, val.c_str() );
		}
		
		i++;	
	}	
}

// set settings
void NeighborPeakValuesCalculator::setSettings( kome::objects::SettingParameterValues* settings ){
	m_settings = settings;
}

// get settings
kome::objects::SettingParameterValues* NeighborPeakValuesCalculator::getSettings(){
	return m_settings;
}

