/**
 * @file LcmsSpectrum.cpp
 * @brief implements of LcmsSpectrum class
 *
 * @author S.Tanaka
 * @date 2008.02.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LcmsSpectrum.h"

#include "LcmsSample.h"
#include "LcmsSampleSet.h"


using namespace kome::io::lcms;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define MASS_UNIT			  0.0001


// constructor
LcmsSpectrum::LcmsSpectrum( LcmsSample^ sample, System::String^ name )
		: LcmsSpectrumBase( sample, name ) {
}

// destructor
LcmsSpectrum::~LcmsSpectrum() {
}

// get xy data
void LcmsSpectrum::onGetXYData(
		kome::clr::XYDataWrapper^ xyData,
		double minX,
		double maxX
) {
	// get export object
	if( m_sample == nullptr || xyData == nullptr ) {
		return;
	}
	LcmsSampleSet^ sampleSet = m_sample->getLcmsSampleSet();
	if( sampleSet == nullptr ) {
		return;
	}
	TTFLDataExport::ITTFLDataExport^ exp = sampleSet->getExport();
	if( exp == nullptr ) {
		return;
	}


	// get data
	System::Object^ mzObjs = nullptr;
	System::Object^ intObjs = nullptr;
	int count = int();

	exp->GetScanData(
		getScanNumber(),
		mzObjs,
		intObjs,
		count,
		0
	);

	if( count <= 0 ) {
		return;
	}

	cli::array< long >^ mzs = (cli::array< long >^ )mzObjs;
	cli::array< long >^ ints = (cli::array< long >^ )intObjs;

	// add
	xyData->reserve( count );
	for( int i = 0; i < count; i++ ) {
		double mz = (double)mzs[ i ] * MASS_UNIT;
		double intensity = (double)ints[ i ];

		if( ( minX < 0.0 || mz >= minX )
				&& ( maxX < 0.0 || mz <= maxX ) ) {
			xyData->addPoint( mz, intensity );                // m/z ‚Æintenisty‚ðƒZƒbƒg
		}
	}
}

// get m/z range
void LcmsSpectrum::onGetXRange( double% minX, double% maxX ) {
	// data points
	kome::clr::ClrDataPoints pts;
	onGetXYData( %pts, -1.0, -1.0 );

	// set range
	minX = pts.getMinX() - 2.0;      // @date 2013/02/05 <Add> FUJITA  pts.getMinX() -> minX-2.0
	maxX = pts.getMaxX() + 2.0;      // @date 2013/02/05 <Add> FUJITA  pts.getMaxX() -> maxX+2.0
}

// get total intensity
double LcmsSpectrum::onGetTotalIntensity( double minX, double maxX ) {
	// data points
	kome::clr::ClrDataPoints pts;
	onGetXYData( %pts, minX, maxX );

	// max intensity 
	double intensity = 0.0;
	for( unsigned int i = 0; i < pts.getLength(); i++ ) {	
		intensity += pts.getY( i );
	}

	return intensity;
}

// get max intensity
double LcmsSpectrum::onGetMaxIntensity( double minX, double maxX ) {
	// data points
	kome::clr::ClrDataPoints pts;
	onGetXYData( %pts, minX, maxX );

	// max intensity 
	double intensity = 0.0;
	for( unsigned int i = 0; i < pts.getLength(); i++ ) {	
		intensity = System::Math::Max( intensity, pts.getY( i ) );
	}

	return intensity;
}

// get API peaks
void LcmsSpectrum::getApiPeaks( kome::clr::PeaksWrapper^ peaks ) {
	// get export object
	if( m_sample == nullptr || peaks == nullptr ) {
		return;
	}
	LcmsSampleSet^ sampleSet = m_sample->getLcmsSampleSet();
	if( sampleSet == nullptr ) {
		return;
	}
	TTFLDataExport::ITTFLDataExportEx^ exp = sampleSet->getExport();
	if( exp == nullptr ) {
		return;
	}

	// array
	cli::array< long >^ scans = gcnew cli::array< long >( 1 );
	scans[ 0 ] = getScanNumber();

	// get data
	System::Object^ mzObjs = nullptr;
	System::Object^ intObjs = nullptr;
	int cnt = 0;

	exp->GetAveragedScanData( scans, mzObjs, intObjs, cnt, true, true );

	cli::array< long >^ mzs = (cli::array< long >^)mzObjs;
	cli::array< long >^ ints = (cli::array< long >^)intObjs;

	for( int i = 0; i < cnt; i++ ) {
		peaks->addPoint( (double)mzs[ i ] * MASS_UNIT, (double)ints[ i ] );
	}
}
