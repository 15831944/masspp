/**
 * @file XYDataOperation.cpp
 * @brief implements of XYDataOperation class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "XYDataOperation.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
XYDataOperation::XYDataOperation() {
	m_index = -1;
}

// destructor
XYDataOperation::~XYDataOperation() {
}

// set name
void XYDataOperation::setName( const char* name ) {
	m_name = NVL( name, "" );
}

// get name
const char* XYDataOperation::getName() {
	return m_name.c_str();
}

// set index
void XYDataOperation::setIndex( int index ){
	m_index = index;
}

// get index
int XYDataOperation::getIndex(){
	return m_index;
}

// update spectrum
void XYDataOperation::update(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		Spectrum& spec 
) {
	// copy
	kome::core::DataPoints pt;
	pt.reserve( src.getLength() );
	for( unsigned int i = 0; i < src.getLength(); i++ ) {
		pt.addPoint( src.getX( i ), src.getY( i ) );
	}

	// clear
	dst.clearPoints();

	// update
	onUpdate( pt, dst, spec );
}

// update chromatogram
void XYDataOperation::update(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		Chromatogram& chrom
) {
	// copy
	kome::core::DataPoints pt;
	pt.reserve( src.getLength() );
	for( unsigned int i = 0; i < src.getLength(); i++ ) {
		pt.addPoint( src.getX( i ), src.getY( i ) );
	}

	// clear
	dst.clearPoints();

	// update
	onUpdate( pt, dst, chrom );
}

// update
void XYDataOperation::update( 
		kome::core::XYData& src, 
		kome::core::XYData& dst, 
		Sample& sample
){
	// copy
	kome::core::DataPoints pt;
	pt.reserve( src.getLength() );
	for( unsigned int i = 0; i < src.getLength(); i++ ) {
		pt.addPoint( src.getX( i ), src.getY( i ) );
	}

	// clear
	dst.clearPoints();

	// update
	onUpdate( pt, dst, sample );
}
