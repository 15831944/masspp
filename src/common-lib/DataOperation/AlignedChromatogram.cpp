/**
 * @file AlignedChromatogram.cpp
 * @brief implements of AlignedChromatogram class
 *
 * @author S.Tanaka
 * @date 2009.02.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "AlignedChromatogram.h"


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
AlignedChromatogram::AlignedChromatogram( kome::objects::Chromatogram& org, Alignment* align )
		: kome::objects::Chromatogram( org.getSample() ), m_org( org ) {
	// initialize
	m_align = align;
	if( m_align == NULL ) {
		m_align = &( Alignment::getDefaultAlignment() );
	}

	setOrgChromatogram( org.getOrgChromatogram() );
}

// destructor
AlignedChromatogram::~AlignedChromatogram() {
}

// on get xy data
void AlignedChromatogram::onGetXYData( kome::core::XYData* const xyData ) {
	// check the parameter
	if( xyData == NULL ) {
		return;
	}

	// get xy data
	kome::core::DataPoints dps;
	m_org.getXYData( &dps, false );

	// add points
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		const double x = m_align->convertRt( dps.getX( i ), m_org.getMz() );
		const double y = dps.getY( i );

		xyData->addPoint( x, y );
	}
}

// on get spectra
void AlignedChromatogram::onGetSpectra(
		kome::objects::DataSet& spectra,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
) {
	// get xy data
	kome::core::DataPoints dps0;
	kome::core::DataPoints dps1;
	kome::core::DataPoints dps2;

	m_org.getXYData( &dps0, false );
	onGetXYData( &dps1 );

	// add aligned xy data
	if( dps0.getLength() > 0 ) {
		dps2.reserve( dps0.getLength() );
	}

	for( unsigned int i = 0; i < dps0.getLength(); i++ ) {
		dps2.addPoint( dps1.getX( 0 ), dps0.getX( i ) );
	}

	// get org RT
	const double orgStartRt = kome::numeric::Interpolation::linear( dps2, startRt );
	const double orgEndRt = kome::numeric::Interpolation::linear( dps2, endRt );

	// get spectra
	m_org.getSpectra( spectra, orgStartRt, orgEndRt, startSearch, endSearch );
}

// on get mass
double AlignedChromatogram::onGetMass( const unsigned int index ) {
	return m_org.getMass( index );
}

// on get MS stage
int AlignedChromatogram::onGetMsStage( const unsigned int index ) {
	return m_org.getMsStage( index );
}

// on get precursor
double AlignedChromatogram::onGetPrecursor( const unsigned int index ) {
	return m_org.getPrecursor( index );
}
