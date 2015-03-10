/**
 * @file WatersSpectrum.cpp
 * @brief implements of WatersSpectrum class
 *
 * @author S.Tanaka
 * @date 2006.11.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "WatersSpectrum.h"

#include "WatersSampleSet.h"
#include "WatersSample.h"


using namespace kome::io::waters;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
WatersSpectrum::WatersSpectrum( WatersSample^ sample, System::String^ name )
		: kome::clr::ClrSpectrumBase( sample, name ) {
	// initialize
	m_sample = sample;

	setHasChromatogram( true );
}

// destructor
WatersSpectrum::~WatersSpectrum() {
}

// get xy data
void WatersSpectrum::onGetXYData(
		kome::clr::XYDataWrapper^ xyData,
		double minX,
		double maxX
) {
	// spectrum object
	DACSERVERLib::DACSpectrumClass spec;

#pragma omp critical ( getWatersSpectrumData )
	{
		spec.GetSpectrum(
			m_sample->getWatersSampleSet()->getPath(),
			m_sample->getFunction(),
			0,
			getScanNumber()
		);
	}

	// get array
	cli::array< float >^ massArray = (cli::array< float >^)spec.Masses;
	cli::array< float >^ intArray  = (cli::array< float >^)spec.Intensities;

	// get index
	int startIndex = 0;
	if( minX >= 0.0 ) {
		startIndex = System::Array::BinarySearch( massArray, (float)minX );
		if( startIndex < 0 ) {
			startIndex = ~startIndex;
		}
	}

	int endIndex = spec.NumPeaks - 1;
	if( maxX >= 0.0 ) {
		endIndex = System::Array::BinarySearch( massArray, (float)maxX );
		if( endIndex < 0 ) {
			endIndex = ~endIndex - 1;
		}
	}

	// add to xy data object
	for( int i = startIndex; i <= endIndex; i++ ) {
		double x = massArray[ i ];
		double y = intArray[ i ];
		if( y > 0.0 ) {
			xyData->addPoint( x, y );
		}
	}
}

// get x range
void WatersSpectrum::onGetXRange( double% minX, double% maxX ) {
	// already settled
}

// get total intensity
double WatersSpectrum::onGetTotalIntensity( double minX, double maxX ) {
	// intensity 
	double intensity = 0.0;

	// spectrum object
	DACSERVERLib::DACSpectrumClass spec;
	spec.GetSpectrum(
		m_sample->getWatersSampleSet()->getPath(),
		m_sample->getFunction(),
		0,
		getScanNumber()
	);
	
	// get array
	cli::array< float >^ massArray = (cli::array< float >^)spec.Masses;
	cli::array< float >^ intArray  = (cli::array< float >^)spec.Intensities;

	// get index
	int startIndex = 0;
	if( minX >= 0.0 ) {
		startIndex = System::Array::BinarySearch( massArray, (float)minX );
		if( startIndex < 0 ) {
			startIndex = ~startIndex;
		}
	}

	int endIndex = spec.NumPeaks - 1;
	if( maxX >= 0.0 ) {
		endIndex = System::Array::BinarySearch( massArray, (float)maxX );
		if( endIndex < 0 ) {
			endIndex = ~endIndex - 1;
		}
	}

	// add to xy data object
	for( int i = startIndex; i <= endIndex; i++ ) {
		intensity += intArray[ i ];
	}

	return intensity;
}

// get max intensity
double WatersSpectrum::onGetMaxIntensity( double minX, double maxX ) {
	// intensity 
	double intensity = 0.0;

	// spectrum object
	DACSERVERLib::DACSpectrumClass spec;
	spec.GetSpectrum(
		m_sample->getWatersSampleSet()->getPath(),
		m_sample->getFunction(),
		0,
		getScanNumber()
	);

	// get array
	cli::array< float >^ massArray = (cli::array< float >^)spec.Masses;
	cli::array< float >^ intArray  = (cli::array< float >^)spec.Intensities;

	// get index
	int startIndex = 0;
	if( minX >= 0.0 ) {
		startIndex = System::Array::BinarySearch( massArray, (float)minX );
		if( startIndex < 0 ) {
			startIndex = ~startIndex;
		}
	}

	int endIndex = spec.NumPeaks - 1;
	if( maxX >= 0.0 ) {
		endIndex = System::Array::BinarySearch( massArray, (float)maxX );
		if( endIndex < 0 ) {
			endIndex = ~endIndex - 1;
		}
	}

	// add to xy data object
	for( int i = startIndex; i <= endIndex; i++ ) {
		double y = intArray[ i ];
		if( y > intensity ) {
			intensity = y;
		}
	}

	return intensity;
}
