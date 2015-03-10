/**
 * @file ColorTheme.cpp
 * @brief implements of ColorTheme class
 *
 * @author S.Tanaka
 * @date 2007.09.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ColorTheme.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ColorTheme::ColorTheme( const char* name ) {
	m_name = NVL( name, "" );

	m_zeroMiddle = false;
	m_width = 0.0;
	m_highWidth = 0.0;
}

// destructor
ColorTheme::~ColorTheme() {
}

// set name
void ColorTheme::setName( const char* name ) {
	m_name = NVL( name, "" );
}

// get name
const char* ColorTheme::getName() {
	return m_name.c_str();
}

// set zero middle flag
void ColorTheme::setZeroMiddle( const bool flg ) {
	m_zeroMiddle = flg;
}

// get zero middle flag
bool ColorTheme::getZeroMiddle() {
	return m_zeroMiddle;
}

// set high width
void ColorTheme::setHighWidth( const double width ) {
	m_width -= m_highWidth;
	m_highWidth = MAX( width, 0.0 );
	m_width += m_highWidth;
}

// get high width
double ColorTheme::getHighWidth() {
	return m_highWidth;
}

// add color
void ColorTheme::addColor( const double width, const COLOR color ) {
	double w = MAX( width, 0.0 );
	m_width += w;
	m_colors.push_back( std::make_pair( w, color ) );
}

// get color
COLOR ColorTheme::getColor( const unsigned int idx ) {
	if( idx >= m_colors.size() ) {
		return kome::img::ColorTool::BLACK;
	}
	return m_colors[ idx ].second;
}

// get color width
double ColorTheme::getWidth( const unsigned int idx ) {
	if( idx >= m_colors.size() ) {
		return 0.0;
	}
	return m_colors[ idx ].first;
}

// get minimum intensity
double ColorTheme::getMinIntensity( kome::objects::DataMapInfo& dataMap ) {
	// intensity
	double minInt = dataMap.getMinIntensity();
	double maxInt = dataMap.getMaxIntensity();

	if( m_zeroMiddle ) {
		minInt = - getMaxIntensity( dataMap );
	}
	minInt = std::min( 0.0, minInt );

	return minInt;
}

// get minimum intensity
double ColorTheme::getMaxIntensity( kome::objects::DataMapInfo& dataMap ) {
	// intensity
	double minInt = dataMap.getMinIntensity();
	double maxInt = dataMap.getMaxIntensity();

	if( m_zeroMiddle ) {
		maxInt = std::max( fabs( minInt ), fabs( maxInt ) );
		maxInt = std::max( maxInt, 0.1 );
	}
	else {
		maxInt = std::max( maxInt, getMinIntensity( dataMap ) + 1.0 );
	}

	return maxInt;
}

// get color
COLOR ColorTheme::getColor( const double intensity, const double minInt, const double maxInt ) {
	// check the member
	int len = (int)m_colors.size();
	if( len == 0 ) {
		return kome::img::ColorTool::BLACK;
	}	

	// value
	double val = ( intensity - minInt ) / ( maxInt - minInt );
	val = CLAMP( val, 0.0, 1.0 );
	val *= m_width;

	// get color
	double w = m_colors[ 0 ].first;
	if( val <= w ) {
		return m_colors[ 0 ].second;
	}

	for( int i = 1; i < len; i++ ) {
		// val
		val -= w;
		w = m_colors[ i ].first;

		// color
		if( val <= w ) {
			COLOR c0 = m_colors[ i - 1 ].second;
			COLOR c1 = m_colors[ i ].second;

			COLOR color = kome::img::ColorTool::getAdditiveColor(
				c0,
				c1,
				w - val,
				val
			);
			return color;
		}
	}
	return m_colors[ len - 1 ].second;
}
