/**
 * @file AgilentSpectrum.cpp
 * @brief implements of AgilentSpectrum class
 *
 * @author S.Tanaka
 * @date 2009.05.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "AgilentSpectrum.h"

#include "AgilentSampleSet.h"
#include "AgilentSample.h"

using namespace kome::io::agilent;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

namespace agilentdll = Agilent::MassSpectrometry::DataAnalysis;

// constructor
AgilentSpectrum::AgilentSpectrum(
	AgilentSample^ sample,
	System::String^ sname,
	Agilent::MassSpectrometry::DataAnalysis::IBDASpecData^ spec
) : kome::clr::ClrSpectrumBase( sample, sname )
	, m_sample(sample)
	, m_spec(spec) {
	// initialize
	setHasChromatogram( true );
}

// destructor
AgilentSpectrum::~AgilentSpectrum()
{
}

// get xy data
void AgilentSpectrum::onGetXYData(
	kome::clr::XYDataWrapper^ xyData,
	double minX,
	double maxX
) {
	int num = m_spec->TotalDataPoints;
	for(int i = 0; i < num; i++) {
		double x = m_spec->XArray[i];
		double y = (double)m_spec->YArray[i];

		if((minX < 0.0 || x >= minX) && (maxX < 0.0 || x <= maxX)) {
			xyData->addPoint(x, y);
		}
	}
}

// get x range
void AgilentSpectrum::onGetXRange(
	double% minX,
	double% maxX
) {
	Agilent::MassSpectrometry::DataAnalysis::IRange^ rng = m_spec->MeasuredMassRange;
	minX = 10.0 * System::Math::Floor( rng->Start / 10.0);
	maxX = 10.0 * System::Math::Ceiling( rng->End / 10.0);
}

// get total intensity
double AgilentSpectrum::onGetTotalIntensity(
	double minX, 
	double maxX
) {
	double intensity = 0.0;

	int num = m_spec->TotalDataPoints;
	for(int i = 0; i < num; i++) {
		double x = m_spec->XArray[i];
		double y = (double)m_spec->YArray[i];

		if((minX < 0.0 || x >= minX) && (maxX < 0.0 || x <= maxX)) {
			intensity += y;
		}
	}

	return intensity;
}

// get max intensity
double AgilentSpectrum::onGetMaxIntensity(
	double minX,
	double maxX
) {
	double intensity = 0.0;

	int num = m_spec->TotalDataPoints;
	for( int i = 0; i < num; i++ ) {
		double x = m_spec->XArray[ i ];
		double y = (double)m_spec->YArray[ i ];

		if((minX < 0.0 || x >= minX) && (maxX < 0.0 || x <= maxX)) {
			if( y > intensity ) {
				y = intensity;
			}
		}
	}

	return intensity;
}
