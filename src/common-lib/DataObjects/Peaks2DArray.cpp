/**
 * @file Peaks2DArray.cpp
 * @brief implements of Peaks2DArray class
 *
 * @author S.Tanaka
 * @date 2007.10.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Peaks2DArray.h"

#include "Peak2DElement.h"

#include <boost/bind.hpp>


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Peaks2DArray::Peaks2DArray() {
}

// destructor
Peaks2DArray::~Peaks2DArray() {
}

// get the number of peaks
unsigned int Peaks2DArray::getNumberOfPeaks() {
	return m_peaks.size();
}

// get peak element
Peak2DElement* Peaks2DArray::getPeak( const unsigned int idx ) {
	if( idx >= m_peaks.size() ) {
		return NULL;
	}
	return m_peaks[ idx ];
}

// search peak
int Peaks2DArray::searchPeak( Peak2DElement* peak ) {
	int idx = -1;
	for( unsigned int i = 0; i < m_peaks.size() && idx < 0; i++ ) {
		if( peak == m_peaks[ i ] ) {
			idx = (int)i;
		}
	}

	return idx;
}

// sort by RT
void Peaks2DArray::sortByRt( const bool desc ) {
	std::sort(
		m_peaks.begin(),
		m_peaks.end(),
		boost::bind( lessRt, _1, _2, desc )
	);
}

// sort by m/z
void Peaks2DArray::sortByMz( const bool desc ) {
	std::sort(
		m_peaks.begin(),
		m_peaks.end(),
		boost::bind( lessMz, _1, _2, desc )
	);
}

// sort by intensity
void Peaks2DArray::sortByIntensity( const bool desc ) {
	std::sort(
		m_peaks.begin(),
		m_peaks.end(),
		boost::bind( lessIntensity, _1, _2, desc )
	);
}

// compare RT
bool Peaks2DArray::lessRt( Peak2DElement* p0, Peak2DElement* p1, bool desc ) {
	if( desc ) {
		return ( p0->getRt() > p1->getRt() );
	}
	return ( p0->getRt() < p1->getRt() );
}

// compare m/z
bool Peaks2DArray::lessMz( Peak2DElement* p0, Peak2DElement* p1, bool desc ) {
	if( desc ) {
		return ( p0->getMz() > p1->getMz() );
	}
	return ( p0->getMz() < p1->getMz() );
}

// compare intensity
bool Peaks2DArray::lessIntensity( Peak2DElement* p0, Peak2DElement* p1, bool desc ) {
	if( desc ) {
		return ( p0->getIntensity() > p1->getIntensity() );
	}
	return ( p0->getIntensity() < p1->getIntensity() );
}


// issue peak 2d id
int Peaks2DArray::issueId( Peak2DElement* peak2dElement ){
	if( peak2dElement != NULL && peak2dElement->getId() > 0 ){
		return peak2dElement->getId()+1;
	}
	return 0;
}

// get peak by peak 2d id
Peak2DElement* Peaks2DArray::getPeakById( int id ){
	for( unsigned int i=0; i < m_peaks.size(); i++ ){
		if( m_peaks[i]->getId() == id ){
			return m_peaks[i];
		}
	}
	return NULL;
}
