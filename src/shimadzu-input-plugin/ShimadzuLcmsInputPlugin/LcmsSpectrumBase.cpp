/**
 * @file LcmsSpectrumBase.cpp
 * @brief implements of LcmsSpectrumBase class
 *
 * @author S.Tanaka
 * @date 2013.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LcmsSpectrumBase.h"

#include "LcmsSample.h"
#include "LcmsSampleSet.h"


using namespace kome::io::lcms;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define EVENT_NUMBER_KEY	   "Event Number"
#define SEGMENT_NUMBER_KEY	 "Segment Number"
#define SCAN_TYPE_KEY		  "Scan Type"
#define PRECURSOR_CHARGE_KEY   "Precursor Charge"

#define MASS_UNIT			  0.0001


// constructor
LcmsSpectrumBase::LcmsSpectrumBase( LcmsSample^ sample, System::String^ name )
		: kome::clr::ClrSpectrumBase( sample, name ) {
	// initialize
	m_sample = sample;

	m_scanType = "";
	m_precCharge = -1;
	m_segNum = 0;
	m_evtNum = 0;
}

// destructor
LcmsSpectrumBase::~LcmsSpectrumBase() {
}

// set scan type
void LcmsSpectrumBase::setScanType( int type ) {
	// scan type
	System::String^ scanType = "";
	if( type == 0 ) {
		scanType = "MS";
	}
	else if( type == 1 ) {
		scanType = "Auto Mode";
	}
	else if( type == 2 ) {
		scanType = "Manual Mode";
	}
	else if( type == 3 ) {
		scanType = "Direct Mode";
	}

	// set
	setScanType( scanType );
}

// set scan type
void LcmsSpectrumBase::setScanType( System::String^ type ) {
	// check parameter
	if( type == nullptr ) {
		return;
	}

	// set value
	m_scanType = type;

	// properties
	kome::clr::PropertiesWrapper^ props = getProperties();
	props->setValue( SCAN_TYPE_KEY, type );
}

// get scan type
System::String^ LcmsSpectrumBase::getScanType() {
	return m_scanType;
}

// set the segment number
void LcmsSpectrumBase::setSegmentNumber( int segNum ) {
	// set value
	m_segNum = segNum;

	// properties
	kome::clr::PropertiesWrapper^ props = getProperties();
	if( segNum > 0 ) {
		props->setIntValue( SEGMENT_NUMBER_KEY, segNum );
	}
	else {
		props->setValue( SEGMENT_NUMBER_KEY, "" );
	}
}

// get the segment number
int LcmsSpectrumBase::getSegmentNumber() {
	return m_segNum;
}

// set the event number
void LcmsSpectrumBase::setEventNumber( int evtNum ) {
	// set value
	m_evtNum = evtNum;

	// properties
	kome::clr::PropertiesWrapper^ props = getProperties();
	if( evtNum > 0 ) {
		props->setIntValue( EVENT_NUMBER_KEY, evtNum );
	}
	else {
		props->setValue( EVENT_NUMBER_KEY, "" );
	}
}

// get the event number
int LcmsSpectrumBase::getEventNumber() {
	return m_evtNum;
}
