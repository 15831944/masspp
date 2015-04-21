/**
 * @file TransformedChromatogram.cpp
 * @brief implements of TransformedChromatogram class
 *
 * @author S.Tanaka
 * @date 2008.05.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TransformedChromatogram.h"


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
TransformedChromatogram::TransformedChromatogram( kome::objects::Chromatogram& org, const bool op )
		: kome::objects::Chromatogram( org.getSample() ), m_org( org ) {
	m_scX = 1.0;
	m_trX = 0.0;
	m_scY = 1.0;
	m_trY = 0.0;
	m_op = op;

	setName( org.getName() );
	setMz( org.getMz() );
	setGroup( org.getGroup() );
	setIcon( org.getIcon() );
	setTitle( org.getTitle() );
	setAutoCreated( org.isAutoCreated() );
	setOrgChromatogram( org.getOrgChromatogram() );
	setQ1( org.getQ1() );
	setQ3( org.getQ3() );
}

// destructor
TransformedChromatogram::~TransformedChromatogram() {
}

// set x scale
void TransformedChromatogram::setXScale( const double sc ) {
	m_scX = std::max( sc,  1.0e-10 );
}

// get x scale
double TransformedChromatogram::getXScale() {
	return m_scX;
}

// set x translation
void TransformedChromatogram::setXTranslation( const double tr ) {
	m_trX = tr;
}

// get x translation
double TransformedChromatogram::getXTranslation() {
	return m_trX;
}

// set y scale
void TransformedChromatogram::setYScale( const double sc ) {
	m_scY = std::max( sc,  1.0e-10 );
}

// get y scale
double TransformedChromatogram::getYScale() {
	return m_scY;
}

// set y translation
void TransformedChromatogram::setYTranslation( const double tr ) {
	m_trY = tr;
}

// get y translation
double TransformedChromatogram::getYTranslation() {
	return m_trY;
}

// on get xy data
void TransformedChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// get original data
	kome::core::DataPoints dps;
	m_org.getXYData( &dps, m_op );

	// add to data
	xyData->reserve( dps.getLength() );
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		double x = m_scX * dps.getX( i ) + m_trX;
		double y = m_scY * dps.getY( i ) + m_trY;

		xyData->addPoint( x, y );
	}
}

// on get spectra
void TransformedChromatogram::onGetSpectra(
		kome::objects::DataSet& spectra,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
	// rt range
	double orgStartRt = ( startRt - m_trX ) / m_scX;
	double orgEndRt = ( endRt - m_trX ) / m_scX;

	// get spectra
	m_org.getSpectra( spectra, orgStartRt, orgEndRt, startSearch, endSearch );
}

// on get mass
double TransformedChromatogram::onGetMass( const unsigned int index ) {
	return m_org.getMass( index );
}

// get get MS stage
int TransformedChromatogram::onGetMsStage( const unsigned int index ) {
	return m_org.getMsStage( index );
}  

// on get precursor
double TransformedChromatogram::onGetPrecursor( const unsigned int index ) {
	return m_org.getPrecursor( index );
}
