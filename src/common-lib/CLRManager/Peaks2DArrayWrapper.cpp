/**
 * @file Peaks2DArrayWrapper.cpp
 * @brief implements of Peaks2DArrayWrapper class
 *
 * @author S.Tanaka
 * @date 2007.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Peaks2DArrayWrapper.h"

#include "Peak2DElementWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Peaks2DArrayWrapper::Peaks2DArrayWrapper( kome::objects::Peaks2DArray& arr ) : m_arr( arr ) {
}

// destructor
Peaks2DArrayWrapper::~Peaks2DArrayWrapper() {
}

// get the number of peaks
unsigned int Peaks2DArrayWrapper::getNumberOfPeaks() {
	return m_arr.getNumberOfPeaks();
}

// get peak
Peak2DElementWrapper^ Peaks2DArrayWrapper::getPeak( unsigned int idx ) {
	kome::objects::Peak2DElement* peak = m_arr.getPeak( idx );
	if( peak == NULL ) {
		return nullptr;
	}
	return gcnew Peak2DElementWrapper( *peak );
}

// search peak
int Peaks2DArrayWrapper::searchPeak( Peak2DElementWrapper^ peak ) {
	kome::objects::Peak2DElement* p = NULL;
	if( peak != nullptr ) {
		p = &( peak->getPeakElement() );
	}

	return m_arr.searchPeak( p );
}

// sort by RT
void Peaks2DArrayWrapper::sortByRt( bool desc ) {
	m_arr.sortByRt( desc );
}

// sort by m/z
void Peaks2DArrayWrapper::sortByMz( bool desc ) {
	m_arr.sortByMz( desc );
}

// sort by intensity
void Peaks2DArrayWrapper::sortByIntensity( bool desc ) {
	m_arr.sortByIntensity( desc );
}
