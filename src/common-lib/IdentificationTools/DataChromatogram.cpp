/**
 * @file DataChromatogram.cpp
 * @brief implements of DataChromatogram class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataChromatogram.h"

#include "Substance.h"
#include "Hit.h"
#include "IdentificationManager.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// chromatogram set
std::set< DataChromatogram* > DataChromatogram::m_chromSet;


// constructor
DataChromatogram::DataChromatogram( const int peakId, const int matrixSampleId )
		: kome::objects::Chromatogram( NULL ), m_peakId( peakId ), m_matrixSampleId( matrixSampleId ) {
	// initialize
	m_normalized = false;
	m_aligned = false;

	m_matrixPos = -1.0;
	m_chromPos = -1.0;
	m_scale = 1.0;

	updateChrom();

	// add to set
	m_chromSet.insert( this );
}

// destructor
DataChromatogram::~DataChromatogram() {
	// remove from set
	m_chromSet.erase( this );
}

// get peak ID
int DataChromatogram::getMatrixPeakId() {
	return m_peakId;
}

// get matrix sample ID
int DataChromatogram::getMatrixSampleId() {
	return m_matrixSampleId;
}

// gep
double DataChromatogram::getGap() {
	double gap = 0.0;
	if( m_aligned && m_chromPos >= 0.0 ) {
		gap = m_chromPos - m_matrixPos;
	}
	return gap;
}

// get scale
double DataChromatogram::getScale() {
	double scale = 1.0;
	if( m_normalized && m_chromPos >= 0.0 ) {
		scale = m_scale;
	}
	return scale;
}

// set peak left
void DataChromatogram::setPeakLeft( const double leftX, const double leftY ) {
	m_left.px = leftX;
	m_left.py = leftY;
}

// get peak left
kome::core::Point< double > DataChromatogram::getPeakLeft() {
	// gap, scale
	const double gap = getGap();
	const double scale = getScale();

	// left
	kome::core::Point< double > pt = m_left;
	pt.px -= gap;
	pt.py *= scale;

	return pt;
}

// set peak right
void DataChromatogram::setPeakRight( const double rightX, const double rightY ) {
	m_right.px = rightX;
	m_right.py = rightY;
}

// get peak right
kome::core::Point< double > DataChromatogram::getPeakRight() {
	// gap, scale
	const double gap = getGap();
	const double scale = getScale();

	// right
	kome::core::Point< double > pt = m_right;
	pt.px -= gap;
	pt.py *= scale;

	return pt;
}

// set normalized flag value
void DataChromatogram::setNormalized( const bool normalized ) {
	m_normalized = normalized;
}

// get normalized flag value
bool DataChromatogram::isNormalized() {
	return m_normalized;
}

// set aligned flag value
void DataChromatogram::setAligned( const bool aligned ) {
	m_aligned = aligned;
}

// get aligned flag value
bool DataChromatogram::isAligned() {
	return m_aligned;
}

// update
void DataChromatogram::updateChrom() {
	// manage
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// get peak position
	kome::objects::PeakElement peak( NULL );
	identMgr.getPeakInfo( m_peakId, m_matrixSampleId, &peak );

	m_matrixPos = identMgr.getMatrixPeakRt( m_peakId );

	// get intensity
	double intensity = identMgr.getPeakValue( m_peakId, m_matrixSampleId );
	double normalizedInt = identMgr.getNormalizedPeakValue( m_peakId, m_matrixSampleId );
	if( intensity > 0.0 ) {
		m_scale = normalizedInt / intensity;
	}
	else if( intensity < 0.0 ) {
		m_scale = -1.0;
	}
	else {
		m_scale = 1.0;
	}

	// get left & right
	m_chromPos = peak.getX();

	m_left.px = ( m_chromPos >= 0.0 ? peak.getLeftX() : -1.0 );
	m_left.py = ( m_chromPos >= 0.0 ? peak.getLeftY() : -1.0 );
	m_right.px = ( m_chromPos >= 0.0 ? peak.getRightX() : -1.0 );
	m_right.py = ( m_chromPos >= 0.0 ? peak.getRightY() : -1.0 );
}

// on get xy data
void DataChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// manager
	IdentificationManager& identMgr = IdentificationManager::getInstance();

	// profile
	int profile = identMgr.getProfile( m_peakId, m_matrixSampleId );

	// check parameters
	if( profile < 0 || xyData == NULL
			|| ( m_normalized && m_scale < 0.0 ) ) {
		return;
	}

	// get data
	kome::core::DataPoints dps;
	identMgr.getProfile( profile, dps );

	// gap, scale
	const double gap = getGap();
	const double scale = getScale();

	// add data
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		const double x = dps.getX( i ) - gap;
		const double y = dps.getY( i ) * scale;

		xyData->addPoint( x, y );
	}	
}

// on get spectra
void DataChromatogram::onGetSpectra(
		kome::objects::DataSet& dataSet,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
}

// on get mass
double DataChromatogram::onGetMass( const unsigned int index ) {
	return 0.0;
}

// on get MS stage
int DataChromatogram::onGetMsStage( const unsigned int index ) {
	return 1;
}

// on get precursorn
double DataChromatogram::onGetPrecursor( const unsigned int index ) {
	return -1.0;
}
