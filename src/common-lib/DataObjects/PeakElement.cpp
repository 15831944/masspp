/**
 * @file PeakElement.cpp
 * @brief implements of Peak2lement class
 *
 * @author S.Tanaka
 * @date 2007.12.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakElement.h"

#include "Peaks.h"

#include <float.h>
#include <math.h>

using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
PeakElement::PeakElement()
        : m_left( -1.0, -1.0 ), m_right( -1.0, -1.0 ), m_apex( -1.0, -1.0 )
{
	// initialize
	m_peaks = NULL;
	m_x = -1.0;
	m_y = -1.0;

	m_area = - FLT_MAX;
	m_fwhm = - FLT_MAX;

	m_charge = -1;
	
	m_peakId = -1;
}

// constructor
PeakElement::PeakElement( Peaks* peaks )
        : m_left( -1.0, -1.0 ), m_right( -1.0, -1.0 ), m_apex( -1.0, -1.0 )		
{
	m_peaks = peaks;
	m_x = -1.0;
	m_y = -1.0;

	m_area = - FLT_MAX;
	m_fwhm = - FLT_MAX;

	m_charge = -1;
	
	// issue peak id
	if( m_peaks != NULL ){
		m_peakId = m_peaks->issueId( this );
	}
}

// constructor
PeakElement::PeakElement( Peaks* peaks, const double x, const double y ) 
		: m_x( x ), m_y( y ), m_left( x, 0.0 ), m_right( x, 0.0 ), m_apex( x, y )
{
	m_peaks = peaks;

	m_area = - FLT_MAX;
	m_fwhm = - FLT_MAX;

	m_charge = -1;

	// issue peak id
	if( m_peaks != NULL ){
		m_peakId = m_peaks->issueId( this );
	}
}

// destructor
PeakElement::~PeakElement() {
}

// set x
void PeakElement::setX( const double x ) {
	m_x = x;

	if( m_left.px < 0.0 ) {
		m_left.px = x;
	}
	if( m_right.px < 0.0 ) {
		m_right.px = x;
	}
}

// get x
double PeakElement::getX() {
	return m_x;
}

// set y
void PeakElement::setY( const double y ) {
	m_y = y;

	if( m_left.py < 0.0 ) {
		m_left.py = 0.0;
	}
	if( m_right.py < 0.0 ) {
		m_right.py = 0.0;
	}
}

// get y
double PeakElement::getY() {
	return m_y;
}

// set peak left
void PeakElement::setLeft( const double x, const double y ) {
	if( m_left.px != x || m_left.py != y ) {
		m_left.px = x;
		m_left.py = y;

		m_area = - FLT_MAX;
		m_apex.px = -1.0;
		m_apex.py = -1.0;
	}
}

// get peak left x
double PeakElement::getLeftX() {
	return m_left.px;
}

// get peak left y
double PeakElement::getLeftY() {
	return m_left.py;
}

// set peak right
void PeakElement::setRight( const double x, const double y ) {
	if( m_right.px != x || m_right.py != y ) {
		// set 
		m_right.px = x;
		m_right.py = y;

		m_area = - FLT_MAX;
		m_apex.px = -1.0;
		m_apex.py = -1.0;
	}
}

// get peak right x
double PeakElement::getRightX() {
	return m_right.px;
}

// get peak right y
double PeakElement::getRightY() {
	return m_right.py;
}

// set peak apex
void PeakElement::setApex( const double x, const double y ) {
	m_apex.px = x;
	m_apex.py = y;
}

// get peak apex x
double PeakElement::getApexX() {
	if( m_apex.px < 0.0 ) {
		m_peaks->calcArea();
	}
	return m_apex.px;
}

// get peak apex y
double PeakElement::getApexY() {
	if( m_apex.px < 0.0 ) {
		m_peaks->calcArea();
	}
	return m_apex.py;
}

// search apex
void PeakElement::searchApex( kome::core::XYData& xyData ) {
	// left X
	const double lx = getLeftX();
	const double rx = getRightX();

	// index
	int idx0 = xyData.searchIndex( lx );
	if( idx0 < 0 ) {
		idx0 = - idx0 - 2;
		if( idx0 < 0 ) {
			idx0 = 0;
		}
	}
    int idx1 = xyData.searchIndex( rx );
	if( idx1 < 0 ) {
		idx1 = - idx1 - 1;
		if( idx1 >= (int)xyData.getLength() ) {
			idx1 = (int)xyData.getLength() - 1;
		}
	}

	// apex
	double apexY = 0.0;
	double apexX = -1.0;
	for( int i = idx0; i <= idx1; i++ ) {
		const double x = xyData.getX( i );
		const double y = xyData.getY( i );

		if( y > apexY || apexX < 0.0 ) {
			apexX = x;
			apexY = y;
		}
	}

	// set
	if( apexX >= 0.0 ) {
		m_apex.px = apexX;
		m_apex.py = apexY;
	}
}

// has apex
bool PeakElement::hasApex() {
	return ( m_apex.px >= 0.0 );
}

// set area
void PeakElement::setArea( const double area ) {
	m_area = area;
}

// get peak area
double PeakElement::getArea() {
	if( !hasArea() && m_peaks != NULL ) {
		m_peaks->calcArea();
	}

	return m_area;
}

// having area value or not
bool PeakElement::hasArea() {
	return ( m_area > -1000.0 );
}

// calculate area
double PeakElement::calcArea( kome::core::XYData& xyData ) {
	// calculate area
	double s = 0.0;

	// base line
	const double lx = getLeftX();
	const double ly = getLeftY();
	const double rx = getRightX();
	const double ry = getRightY();

	const double w = rx - lx;
	if( w < 0.0000001 ) {
		return s;
	}

	const double a = ( ry - ly ) / w;

	// index
	int idx0 = xyData.getNearestIndex( lx );
	int idx1 = xyData.getNearestIndex( rx );

	for( int i = idx0; i < idx1 - 1; i++ ) {
		// coordinates
		const double x0 = xyData.getX( i );
		const double x1 = xyData.getX( i + 1 );
		const double y0 = xyData.getY( i ) - ( a * ( x0 - lx ) + ly );
		const double y1 = xyData.getY( i + 1 ) - ( a * ( x1 - lx ) + ly );

		if( y0 >= 0.0 && y1 >= 0.0 ) {
			s += ( y0 + y1 ) * ( x1 - x0 ) / 2.0;	// @date 2011/02/23 <Comment> OKADA : ‘äŒ`‚Ì–ÊÏFiã’ê(y0){‰º’ê(y1)j~‚‚³(x1-x0)€‚Q
		}
		// baseline‚Ìã‘¤‚Ì–ÊÏ‚Ì‚Ý‹‚ß‚é
		else if( y0 > 0.0 ) {	// @date 2011/02/23 <Comment> OKADA : (y1<0.0)‚Ìê‡A
			s += - y0 * y0 * ( x1 - x0 ) / ( y1 - y0 ) / 2.0;	// ŽOŠpŒ`‚Ì‚‚³Fy0 * ( x1 - x0 ) / ( y1 - y0 )
		}
		else if( y1 > 0.0 ) {	// @date 2011/02/23 <Comment> OKADA : (y0<0.0)‚Ìê‡
			s += y1 * y1 * ( x1 - x0 ) / ( y1 - y0 ) / 2.0;
		}
	}

	// apex
	searchApex( xyData );

	return s;
}

// set the FWHM
void PeakElement::setFwhm( const double fwhm ) {
	m_fwhm = fwhm;
}

// get the FWHM
double PeakElement::getFwhm() {
	if( !hasFwhm() && m_peaks != NULL ) {
		m_peaks->calcArea();
	}

	return m_fwhm;
}

// having FWHM value or not
bool PeakElement::hasFwhm() {
	return ( m_fwhm > -1000.0 );
}

// calculate FWHM
double PeakElement::calcFwhm( kome::core::XYData& xyData ) {
	// return value
	double fwhm = -1.0;

	// apex
	if( m_apex.px < 0.0 ) {
		searchApex( xyData );
	}

	const double ax = m_apex.px;
	const double ay = m_apex.py;
	if( ax < 0.0 ) {
		return fwhm;
	}
	const double height = ay / 2.0;

	int idx = xyData.getNearestIndex( ax );
	if( idx < 0 || ax >= (double)xyData.getLength() ) {
		return fwhm;
	}

	// left
	double lx = -1.0;
	double prevX = ax;
	double prevY = ay;
	for( int i = idx - 1; i >= 0 && lx < 0.0; i-- ) {
		const double x = xyData.getX( i );
		const double y = xyData.getY( i );

		if( prevY >= height && y <= height ) {
			if( fabs( y - prevY ) < 0.00001 ) {
				lx = prevX;
			}
			else {
				lx = ( prevX * ( y - height ) + x * ( height - prevY ) ) / ( y - prevY );
			}
		}
		else if( y > ay ) {
			i = -1;
		}

		prevX = x;
		prevY = y;
	}

	if( lx < 0.0 ) {
		return fwhm;
	}

	// right
	double rx = -1.0;
	prevX = ax;
	prevY = ay;
	for( int i = idx + 1; i < (int)xyData.getLength() && rx < 0.0; i++ ) {
		const double x = xyData.getX( i );
		const double y = xyData.getY( i );

		if( prevY >= height && y <= height ) {
			if( fabs( y - prevY ) < 0.00001 ) {
				rx = prevX;
			}
			else {
				rx = ( prevX * ( y - height ) + x * ( height - prevY ) ) / ( y - prevY );
			}
		}
		else if( y > ay ) {
			i = (int)xyData.getLength();
		}

		prevX = x;
		prevY = y;
	}

	if( rx < 0.0 ) {
		return fwhm;
	}

	fwhm = fabs( rx - lx );

	return fwhm;
}

// set peak id
void PeakElement::setId( int id ){
	m_peakId = id;
}

// get peak id
int PeakElement::getId(){
	return m_peakId;
}

// set charge
void PeakElement::setCharge( const int charge ) {
	m_charge = charge;
}

// get charge
int PeakElement::getCharge() {
	return m_charge;
}
