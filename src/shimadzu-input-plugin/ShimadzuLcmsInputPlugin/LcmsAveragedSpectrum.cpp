/**
 * @file LcmsAveragedSpectrum.cpp
 * @brief implements of LcmsAveragedSpectrum class
 *
 * @author S.Tanaka
 * @date 2013.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LcmsAveragedSpectrum.h"

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
LcmsAveragedSpectrum::LcmsAveragedSpectrum( LcmsSample^ sample, System::String^ name )
		: LcmsSpectrumBase( sample, name ) {
	m_scans = gcnew System::Collections::ArrayList();
}

// destructor
LcmsAveragedSpectrum::~LcmsAveragedSpectrum() {
}

// add scan
void LcmsAveragedSpectrum::addScan( int scan ) {
	m_scans->Add( scan );
}

// get xy data
void LcmsAveragedSpectrum::onGetXYData(
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
	TTFLDataExport::ITTFLDataExportEx^ exp = sampleSet->getExport();
	if( exp == nullptr ) {
		return;
	}

	// array
	int len = m_scans->Count;
	if( len == 0 ) {
		return;
	}

	cli::array< long >^ scans = gcnew cli::array< long >( len );
	for( int i = 0; i < len; i++ ) {
		scans[ i ] = (long)m_scans[ i ];
	}

	// get data
	System::Object^ mzObjs = nullptr;
	System::Object^ intObjs = nullptr;
	int cnt = 0;

	exp->GetAveragedScanData( scans, mzObjs, intObjs, cnt, true, false );     // [in]scans...scan配列 [out]intObjs...マージされたintenisity 
	 
	cli::array< long >^ mzs = (cli::array< long >^)mzObjs;
	cli::array< long >^ ints = (cli::array< long >^)intObjs;

	for( int i = 0; i < cnt; i++ ) {
		xyData->addPoint( (double)mzs[ i ] * MASS_UNIT, (double)ints[ i ] );
	}
}

// get m/z range
void LcmsAveragedSpectrum::onGetXRange( double% minX, double% maxX ) {
	// data points
	kome::clr::ClrDataPoints pts;
	onGetXYData( %pts, -1.0, -1.0 );

	// set range
	minX = pts.getMinX();
	maxX = pts.getMaxX();
}

// get total intensity
double LcmsAveragedSpectrum::onGetTotalIntensity( double minX, double maxX ) {
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
double LcmsAveragedSpectrum::onGetMaxIntensity( double minX, double maxX ) {
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

// get peaks
void LcmsAveragedSpectrum::getApiPeaks( kome::clr::PeaksWrapper^ peaks ) {
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
	int len = m_scans->Count;
	if( len == 0 ) {
		return;
	}

	cli::array< long >^ scans = gcnew cli::array< long >( len );
	for( int i = 0; i < len; i++ ) {
		scans[ i ] = (long)m_scans[ i ];
	}

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

	// peaks
	kome::clr::PeaksManagerWrapper^ pkMgr = kome::clr::PeaksManagerWrapper::getInstance();
	if( !pkMgr->hasPeaks( this ) ) {
		kome::clr::PeaksWrapper^ peaks = pkMgr->createPeaks( this );
		getApiPeaks( peaks );
	}
}
