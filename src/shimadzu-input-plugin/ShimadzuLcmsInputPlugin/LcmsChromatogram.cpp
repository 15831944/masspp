/**
 * @file LcmsChromatgram.cpp
 * @brief implements of LcmsChromatogram class
 *
 * @author ----
 * @date ----
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LcmsSample.h"
#include "LcmsSampleSet.h"
#include "LcmsChromatogram.h"

using namespace kome::io::lcms;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define MASS_UNIT			  0.0001

using namespace kome::io::lcms;


// constructor
LcmsChromatogram::LcmsChromatogram( LcmsSample^ sample, const int segNum, const int evtNum )
		: kome::clr::ClrChromatogramBase( sample ) {
	// initialize
	m_sample = sample;
	m_segNum = segNum;
	m_evtNum = evtNum;
}

// destructor
LcmsChromatogram::~LcmsChromatogram() {
}

// get segment number
int LcmsChromatogram::getSegmentNumber() {
	return m_segNum;
}

// get event number
int LcmsChromatogram::getEventNumber() {
	return m_evtNum;
}

// on get xy data
void LcmsChromatogram::onGetXYData( kome::clr::XYDataWrapper^ xyData ) {
	// get export object
	if( m_sample == nullptr || xyData == nullptr ) {
		return;
	}
	LcmsSampleSet^ sampleSet = m_sample->getLcmsSampleSet();
	if( sampleSet == nullptr ) {
		return;
	}
	TTFLDataExport::ITTFLDataExportEx^ exp = sampleSet->getExport();
	if( exp == nullptr ) {
		return;
	}

	// chromatogram
	System::Object^ rtObjs = nullptr;
	System::Object^ intObjs = nullptr;
	int cnt = 0;
	exp->GetEventTIC( m_segNum, m_evtNum, rtObjs, intObjs, cnt, 1 );
	
	cli::array< long >^ rts = (cli::array< long >^)rtObjs;
	cli::array< long >^ ints = (cli::array< long >^)intObjs;

	for( int i = 0; i < cnt; i++ ) {
		double rt = (double)rts[ i ] / 60000.0;
		double intensity = (double)ints[ i ];

		xyData->addPoint( rt, intensity );
	}
}

// on get spectra
void LcmsChromatogram::onGetSpectra(
						kome::clr::DataSetWrapper^ dataSet,
						double startRt,
						double endRt,
						kome::clr::SearchType startSearch,
						kome::clr::SearchType endSearch
) {
	return;
}

// on get mass
double LcmsChromatogram::onGetMass( unsigned int index ) {
	double i = 1.0;
	return i;
}

// on get stage
int LcmsChromatogram::onGetMsStage( unsigned int index ) {
	int i = 1.0;
	return i;
}

// on get precursor
double LcmsChromatogram::onGetPrecursor( unsigned int index ) {
	double i = 1.0;
	return i;
}

