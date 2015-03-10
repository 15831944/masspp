/**
 * @file WindowFilter.cpp
 * @brief implements of WindowFilter class
 *
 * @author M.Fukuda
 * @date 2010.6.4
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "WindowFilter.h"

using namespace kome::filter::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constractor
WindowFilter::WindowFilter(kome::objects::Parameters* params) {
	m_src = kome::plugin::PluginCallTool::getXYData( params );
	m_dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	m_settings = kome::plugin::PluginCallTool::getSettingValues( params );
	m_width = 1.0;
}

//destractor
WindowFilter::~WindowFilter(){}

//setotherstatus(for overwrite)
void WindowFilter::setOtherStatus(){}

// execute
void WindowFilter::execute() {
	if(checkStatus()) {
		adaptationFilter();
	}
}

// checkStatus From m_settings
bool WindowFilter::checkStatus() {
	if(m_src == NULL) return false;
	if(m_dst == NULL) return false;
	if(m_settings != NULL) {
		m_width = m_settings->getDoubleValue(WIDTH_PARAM_NAME, 1.0);
		setOtherStatus();
	}
	return true;
}

// adapte window filter
void WindowFilter::adaptationFilter() {

	// XYdate Length Set
	unsigned const int lastIndex = m_src->getLength();
	// reserve memory
	m_dst->reserve(lastIndex);

	for (unsigned int i = 0; i < lastIndex; i++ ) {

		double x = m_src->getX( i );	
		double y = m_src->getY( i );

		// total weight
		double tw = 1.0; 

		calcHalfWindowWeight(x,y,tw,i,lastIndex, -1); // FIRST  HALF
		calcHalfWindowWeight(x,y,tw,i,lastIndex, 1);  // LETTAR HALF

		if (y <= 0.0) {
			y = 0.0;
		} else {
			y /= tw;
		}
		
		m_dst->addPoint( x, y );

	}
}

// calc window weight
void WindowFilter::calcHalfWindowWeight(double x, double &y, double &tw, unsigned const int tgt, unsigned const int lst, const int step) {

	for(int j = tgt + step; 0 <= j && j < lst; j = j + step) {
		double temp_x = m_src->getX( j );
		double xlen = fabs(x - temp_x);

		if (xlen > m_width) break;
		
		double ywt = getWeight(xlen);
		y += ywt * m_src->getY( j );
		tw += ywt;
	}
}
