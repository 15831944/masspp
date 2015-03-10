/**
 * @file MsbChromatogram.cpp
 * @brief implements of MsbChromatogram class
 *
 * @author S.Tanaka
 * @date 2008.04.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MsbChromatogram.h"

#include "MsbSample.h"


using namespace kome::io::msb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MsbChromatogram::MsbChromatogram( MsbSample* sample, FILE* fp )
		: kome::objects::Chromatogram( sample ) {
	// initialize
	m_fp = fp;
	m_length = 0;
	m_rtAlign = 0;
	m_intAlign = 0;
	m_massAlign = 0;
	m_msStageAlign = 0;
	m_precursorAlign = 0;
}

// destructor
MsbChromatogram::~MsbChromatogram() {
}

// set data length
void MsbChromatogram::setDataLength( const unsigned int len ) {
	m_length = len;
}

// get data length
unsigned int MsbChromatogram::getDataLength() {
	return m_length;
}

// set RT data align
void MsbChromatogram::setRtDataAlign( const unsigned long long align ) {
	m_rtAlign = align;
}

// get RT data align
unsigned long long MsbChromatogram::getRtDataAlign() {
	return m_rtAlign;
}

// set intensity data align
void MsbChromatogram::setIntensityDataAlign( const unsigned long long align ) {
	m_intAlign = align;
}

// get intensity data align
unsigned long long MsbChromatogram::getIntensityDataAlign() {
	return m_intAlign;
}

// set mass data align
void MsbChromatogram::setMassDataAlign( const unsigned long long align ) {
	m_massAlign = align;
}

// get mass data align
unsigned long long MsbChromatogram::getMassDataAlign() {
	return m_massAlign;
}

// set MS stage data align
void MsbChromatogram::setMsStageDataAlign( const unsigned long long align ) {
	m_msStageAlign = align;
}

// get MS stage data align
unsigned long long MsbChromatogram::getMsStageDataAlign() {
	return m_msStageAlign;
}

// set precursor data align
void MsbChromatogram::setPrecursorDataAlign( const unsigned long long align ) {
	m_precursorAlign = align;
}

// get precursor data align
unsigned long long MsbChromatogram::getPrecursorDataAlign() {
	return m_precursorAlign;
}

// on get xy data
void MsbChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// check member and parameters
	if( xyData == NULL || m_fp == NULL
			|| m_length == 0 || m_rtAlign == 0 || m_intAlign == 0 ) {
		return;
	}

	// get data
	float* rts = new float[ m_length ];
	float* ints = new float[ m_length ];

	fileseek( m_fp, m_rtAlign, SEEK_SET );
	fread( rts, sizeof( float ), m_length, m_fp );

	fileseek( m_fp, m_intAlign, SEEK_SET );
	fread( ints, sizeof( float ), m_length, m_fp );

	// add data
	xyData->reserve( m_length );
	for( unsigned int i = 0; i < m_length; i++ ) {
		xyData->addPoint( (double)rts[ i ], (double)ints[ i ] );
	}

	// delete 
	delete[] rts;
	delete[] ints;
}

// on get spectra
void MsbChromatogram::onGetSpectra(
		kome::objects::DataSet& dataSet,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
}

// on get mass data
double MsbChromatogram::onGetMass( const unsigned int index ) {
	// check member and parameters
	if( m_fp == NULL || m_massAlign == 0 || index >= m_length ) {
		return -1.0;
	}

	// get mass
	float mass = -1.0;
	fileseek( m_fp, m_massAlign + index, SEEK_SET );
	fread( &mass, sizeof( float ), 1, m_fp );

	return (double)mass;
}

// on get MS stage data
int MsbChromatogram::onGetMsStage( const unsigned int index ) {
	// check member and parameters
	if( m_fp == NULL || m_msStageAlign == 0 || index >= m_length ) {
		return -1;
	}

	// get MS stage
	int stage = -1;
	fileseek( m_fp, m_msStageAlign + index, SEEK_SET );
	fread( &stage, sizeof( int ), 1, m_fp );

	return stage;
}

// on get precursor
double MsbChromatogram::onGetPrecursor( const unsigned int index ) {
	// check member and parameters
	if( m_fp == NULL || m_precursorAlign == 0 || index >= m_length ) {
		return -1.0;
	}

	// get precursor
	float precursor = -1.0;
	fileseek( m_fp, m_precursorAlign + index, SEEK_SET );
	fread( &precursor, sizeof( float ), 1, m_fp );

	return (double)precursor;
}
