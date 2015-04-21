/**
 * @file ClrChromatogram.cpp
 * @brief implements of ClrChromatogram class
 *
 * @author S.Tanaka
 * @date 2008.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrChromatogram.h"

#include "ClrChromatogramBase.h"
#include "SampleWrapper.h"
#include "XYDataWrapper.h"
#include "PropertiesWrapper.h"
#include "DataSetWrapper.h"
#include "ClrObjectTool.h"
#include "ClrObjectPool.h"
#include "ConversionArrays.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrChromatogram::ClrChromatogram( SampleWrapper^ sample )
		: kome::objects::Chromatogram( sample == nullptr ? NULL : &( sample->getSample() ) ) {
}

// destructor
ClrChromatogram::~ClrChromatogram() {
	ClrObjectPool::getInstance().removeChromatogram( *this );
	m_baseChromatogram = nullptr;
}

// set base spectrum
void ClrChromatogram::setBaseChromatogram( ClrChromatogramBase^ baseChromatogram ) {
	m_baseChromatogram = baseChromatogram;
}

// get base spectrum
ClrChromatogramBase^ ClrChromatogram::getBaseChromatogram() {
	ClrChromatogramBase^ baseChromatogram = m_baseChromatogram;
	return baseChromatogram;
}

// get data points
void ClrChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	ClrChromatogramBase^ baseChromatogram = m_baseChromatogram;
	if( baseChromatogram != nullptr ) {
		baseChromatogram->onGetXYData( gcnew XYDataWrapper( *xyData ) );
	}
}

// get spectra
void ClrChromatogram::onGetSpectra(
		kome::objects::DataSet& dataSet,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
	// convert search type
	SearchType st = CONVERT_FROM_ARRAY( startSearch, SearchType::NEAR, g_searchTypes );
	SearchType et = CONVERT_FROM_ARRAY( endSearch, SearchType::NEAR, g_searchTypes );

	// spectra
	DataSetWrapper^ s = ClrObjectTool::createDataSetWrapper( &dataSet );

	// get spectra
	ClrChromatogramBase^ baseChromatogram = m_baseChromatogram;
	if( baseChromatogram != nullptr ) {
		baseChromatogram->onGetSpectra( s, startRt, endRt, st, et );
	}
}

// on get mass
double ClrChromatogram::onGetMass( const unsigned int index ) {
	// get object
	ClrChromatogramBase^ baseChromatogram = m_baseChromatogram;
	if( baseChromatogram == nullptr ) {
		return -1.0;
	}

	// get mass
	return baseChromatogram->onGetMass( index );
}

// on get MS stage
int ClrChromatogram::onGetMsStage( const unsigned int index ) {
	// get object
	ClrChromatogramBase^ baseChromatogram = m_baseChromatogram;
	if( baseChromatogram == nullptr ) {
		return -1;
	}

	// get MS stage
	return baseChromatogram->onGetMsStage( index );
}

// on get precursor
double ClrChromatogram::onGetPrecursor( const unsigned int index ) {
	// get object
	ClrChromatogramBase^ baseChromatogram = m_baseChromatogram;
	if( baseChromatogram == nullptr ) {
		return -1.0;
	}

	// get MS stage
	return baseChromatogram->onGetPrecursor( index );
}
