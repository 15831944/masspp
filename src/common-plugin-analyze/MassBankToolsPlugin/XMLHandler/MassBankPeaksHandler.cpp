/**
 * @file MassBankPeaksHandler.cpp
 * @brief implements of MassBankPeaksHandler class
 *
 * @author S.Tanaka
 * @date 2009.09.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "../stdafx.h"
#include "MassBankPeaksHandler.h"


using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define RETURN_TAG_NAME		  "return"
#define ID_TAG_NAME			  "id"
#define MZS_TAG_NAME			 "mzs"
#define INTS_TAG_NAME			"intensities"
#define NUM_TAG_NAME			 "numPeaks"


// constructor
MassBankPeaksHandler::MassBankPeaksHandler() {
}

// destructor
MassBankPeaksHandler::~MassBankPeaksHandler() {
}

// Do Core
void MassBankPeaksHandler::doGetXYDatas(
	const char* fname, 
	const char* id, 
	kome::core::XYData& xyData
){
	this->parse(fname);
	if (!this->isError()) {
		PeakInfo* peaks = getPeakInfo( id );
		if( peaks != NULL ) {
			for( unsigned int i = 0; i < peaks->num; i++ ) {
				xyData.addPoint ((peaks->mzs)[i], (peaks->ints)[i]);
			}
		}
	}
	removefile(fname);
}

// get the number of records
unsigned int MassBankPeaksHandler::getNumberOfRecords() {
	return m_peaks.size();
}

// get peaks information
MassBankPeaksHandler::PeakInfo* MassBankPeaksHandler::getPeakInfo( const unsigned int idx ) {
	if( idx >= m_peaks.size() ) {
		return NULL;
	}
	return &( m_peaks[ idx ] );
}

// get peaks information
MassBankPeaksHandler::PeakInfo* MassBankPeaksHandler::getPeakInfo( const char* id ) {
	if( id == NULL ) {
		return NULL;
	}

	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		if( m_peaks[ i ].id.compare( id ) == 0 ) {
			return &( m_peaks[ i ] );
		}
	}

	return NULL;
}

// on start document
void MassBankPeaksHandler::onStartDocument() {
	m_peaks.clear();
}

// on end document
void MassBankPeaksHandler::onEndDocument() {
}

// on start element
void MassBankPeaksHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tags
	if( strcmp( name, RETURN_TAG_NAME ) == 0 ) {	// <return>
		m_peaks.resize( m_peaks.size() + 1 );
		m_peaks.back().num = 0;
	}
}

// on end element
void MassBankPeaksHandler::onEndElement( const char* name, const char* text ) {
	// each tags
	if( strcmp( name, ID_TAG_NAME ) == 0 ) {	// </id>
		if( m_peaks.size() > 0 ) {
			m_peaks.back().id = NVL( text, "" );
		}
	}
	else if( strcmp( name, MZS_TAG_NAME ) == 0 ) {	// </mzs>
		if( m_peaks.size() > 0 ) {
			const double mz = todouble( text, 0.0 );
			m_peaks.back().mzs.push_back( mz );
		}
	}
	else if( strcmp( name, INTS_TAG_NAME ) == 0 ) {	// </intensities>
		if( m_peaks.size() > 0 ) {
			const double intensity = todouble( text, 0.0 );
			m_peaks.back().ints.push_back( intensity );
		}
	}
	else if( strcmp( name, NUM_TAG_NAME ) == 0 ) {	// </numPeaks>
		if( m_peaks.size() > 0 ) {
			m_peaks.back().num = toint( text, 10, 0 );
		}
	}
}
