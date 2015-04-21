/**
 * @file PeakElementWrapper.cpp
 * @brief implements of PeakElementWrapper class
 *
 * @author S.Tanaka
 * @date 2007.12.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakElementWrapper.h"

#include "XYDataWrapper.h"
#include "ClrPoint.h"
#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PeakElementWrapper::PeakElementWrapper( kome::objects::PeakElement& peak ) : m_peak( peak ) {
	m_deleteFlg = false;
}

// constructor
PeakElementWrapper::PeakElementWrapper() : m_peak( *( new kome::objects::PeakElement( NULL ) ) ) {
	m_deleteFlg = true;
}

// destructor
PeakElementWrapper::~PeakElementWrapper() {
	if( m_deleteFlg ) {		
		delete &m_peak;
	}
}

// get peak element object
kome::objects::PeakElement& PeakElementWrapper::getPeakElement() {
	return m_peak;
}

// set x
void PeakElementWrapper::setX( double x ) {
	m_peak.setX( x );
}

// get x
double PeakElementWrapper::getX() {
	return m_peak.getX();
}

// set y
void PeakElementWrapper::setY( double y ) {
	m_peak.setY( y );
}

// get y 
double PeakElementWrapper::getY() {
	return m_peak.getY();
}

// set peak left
void PeakElementWrapper::setLeft( double x, double y ) {
	m_peak.setLeft( x, y );
}

// get peak left x
double PeakElementWrapper::getLeftX() {
	return m_peak.getLeftX();
}

// get peak left y 
double PeakElementWrapper::getLeftY() {
	return m_peak.getLeftY();
}

// set peak right
void PeakElementWrapper::setRight( double x, double y ) {
	m_peak.setRight( x, y );
}

// get peak right x
double PeakElementWrapper::getRightX() {
	return m_peak.getRightX();
}

// get peak right y
double PeakElementWrapper::getRightY() {
	return m_peak.getRightY();
}

// se peak apex
void PeakElementWrapper::setApex( double x, double y ) {
	m_peak.setApex( x, y );
}

// get peak apex x
double PeakElementWrapper::getApexX() {
	return m_peak.getApexX();
}

// get peak apex y
double PeakElementWrapper::getApexY() {
	return m_peak.getApexY();
}

// has apex
bool PeakElementWrapper::hasApex() {
	return m_peak.hasApex();
}

// set peak area
void PeakElementWrapper::setArea( double area ) {
	m_peak.setArea( area );
}

// get peak area
double PeakElementWrapper::getArea() {
	return m_peak.getArea();
}

// having area value or not
bool PeakElementWrapper::hasArea() {
	return m_peak.hasArea();
}

// calculate area
double PeakElementWrapper::calcArea( XYDataWrapper^ xyData ) {
	// get xy data
	kome::core::XYData* xyd = ClrObjectTool::getXYData( xyData );
	if( xyd == NULL ) {
		return 0.0;
	}

	// calculate
	double area = m_peak.calcArea( *xyd );
	return area;
}

// set FWHM
void PeakElementWrapper::setFwhm( double fwhm ) {
	m_peak.setFwhm( fwhm );
}

// get FWHM
double PeakElementWrapper::getFwhm() {
	return m_peak.getFwhm();
}

// having the FWHM or not
bool PeakElementWrapper::hasFwhm() {
	return m_peak.hasFwhm();
}

// calculate FWHM
double PeakElementWrapper::calcFwhm( XYDataWrapper^ xyData ) {
	// get xy data
	kome::core::XYData* xyd = ClrObjectTool::getXYData( xyData );
	if( xyd == NULL ) {
		return 0.0;
	}

	// calculate
	double fwhm = m_peak.calcFwhm( *xyd );
	return fwhm;
}

// set ID
void PeakElementWrapper::setId( int id ) {
	m_peak.setId( id );
}

// get ID
int PeakElementWrapper::getId() {
	return m_peak.getId();
}

// set charge
void PeakElementWrapper::setCharge( int charge ) {
	m_peak.setCharge( charge );
}

// get charge
int PeakElementWrapper::getCharge() {
	return m_peak.getCharge();
}
