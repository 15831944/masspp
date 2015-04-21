/**
 * @file PeaksWrapper.cpp
 * @brief implements of PeaksWrapper class
 *
 * @author S.Tanaka
 * @date 2006.11.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "PeaksWrapper.h"

#include "ClrObjectTool.h"
#include "PeakElementWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PeaksWrapper::PeaksWrapper( kome::objects::Peaks& peaks ) 
		: XYDataWrapper( peaks ), m_peaks( peaks ) {
    m_peakDelete = false;
}

// constructor
PeaksWrapper::PeaksWrapper() : m_peaks( *( new kome::objects::Peaks() ) ), XYDataWrapper( m_peaks ) {
	m_peakDelete = true;
}

// destructor
PeaksWrapper::~PeaksWrapper() {
	if( m_peakDelete ) {
		delete &m_peaks;
	}
}

// get peaks object
kome::objects::Peaks& PeaksWrapper::getPeaks() {
	return m_peaks;
}

// clear charge
void PeaksWrapper::clearCharge() {
	m_peaks.clearCharge();
}

// add charge
void PeaksWrapper::addCharge( short charge ) {
	m_peaks.addCharge( charge );
}

// get the number of charges
unsigned int PeaksWrapper::getNumberOfCharges() {
	return m_peaks.getNumberOfCharges();
}

// get charge
short PeaksWrapper::getCharge( unsigned int index ) {
	return m_peaks.getCharge( index );
}

// set precursor
void PeaksWrapper::setPrecursor( double precursor ) {
	m_peaks.setPrecursor( precursor );
}

// get precursor
double PeaksWrapper::getPrecursor() {
	return m_peaks.getPrecursor();
}

// calculate area
void PeaksWrapper::calcArea() {
	m_peaks.calcArea();
}

// set data points
void PeaksWrapper::setDataPoints( XYDataWrapper^ xyData ) {
	kome::core::XYData* xyd = ClrObjectTool::getXYData( xyData );
	if( xyd != NULL ) {
		m_peaks.setDataPoints( *xyd );
	}
}

// create peak
PeakElementWrapper^ PeaksWrapper::createPeak( double x, double y ) {
	// create
	kome::objects::PeakElement* peak = m_peaks.createPeak( x, y );
	if( peak == NULL ) {
		return nullptr;
	}
	return gcnew PeakElementWrapper( *peak );
}

// get peak
PeakElementWrapper^ PeaksWrapper::getPeak( unsigned int index ) {
	// get peak
	kome::objects::PeakElement* peak = m_peaks.getPeak( index );
	if( peak == NULL ) {
		return nullptr;
	}
	return gcnew PeakElementWrapper( *peak );
}

// sort
void PeaksWrapper::sort() {
	m_peaks.sort();
}
