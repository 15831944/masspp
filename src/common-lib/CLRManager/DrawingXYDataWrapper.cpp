/**
 * @file DrawingXYDataWrapper.cpp
 * @brief implements of DrawingXYDataWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "DrawingXYDataWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DrawingXYDataWrapper::DrawingXYDataWrapper( kome::img::DrawingXYData& data )
		: m_data( data ) {
}

// destructor
DrawingXYDataWrapper::~DrawingXYDataWrapper() {
}

// get drawing xy data object
kome::img::DrawingXYData& DrawingXYDataWrapper::getDrawingXYData() {
	return m_data;
}

// update points
void DrawingXYDataWrapper::updatePoints() {
	m_data.updatePoints();
}

// get length
unsigned int DrawingXYDataWrapper::getLength( unsigned short level ) {
	return m_data.getLength( level );
}

// get x
double DrawingXYDataWrapper::getX( unsigned short level, unsigned int index ) {
	return m_data.getX( level, index );
}

// get left y
double DrawingXYDataWrapper::getLeftY( unsigned short level, unsigned int index ) {
	return m_data.getLeftY( level, index );
}

// get top y
double DrawingXYDataWrapper::getTopY( unsigned short level, unsigned int index ) {
	return m_data.getTopY( level, index );
}

// get right y
double DrawingXYDataWrapper::getRightY( unsigned short level, unsigned int index ) {
	return m_data.getRightY( level, index );
}

// search index
int DrawingXYDataWrapper::searchIndex( unsigned short level, double x ) {
	return m_data.searchIndex( level, x );
}
