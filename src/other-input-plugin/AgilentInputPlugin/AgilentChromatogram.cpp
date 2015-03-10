/**
 * @file AgilentChromatogram.cpp
 * @brief interfaces of AgilentChromatogram class
 *
 * @author M.Fukuda
 * @date 2013.12.20
 * @version 1.0.0
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */
#include "stdafx.h"
#include "AgilentChromatogram.h"

#include "AgilentSample.h"

using namespace kome::io::agilent;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
AgilentChromatogram::AgilentChromatogram(
	AgilentSample^ srcsample,
	System::String^ sname,
	System::String^ iconname,
	Agilent::MassSpectrometry::DataAnalysis::IBDAChromData^ chrom
) : kome::clr::ClrChromatogramBase(srcsample)
	, mysample_(srcsample)
	, chrom_(chrom)
{
	setName(sname);
	setTitle(sname);
	if (!System::String::IsNullOrEmpty(iconname)) {
		setIcon(iconname);
	}
}

// destructor
AgilentChromatogram::~AgilentChromatogram(){}

// getXYData
void AgilentChromatogram::onGetXYData(
	kome::clr::XYDataWrapper^ xyData
) {
	int num = chrom_->TotalDataPoints;
	for(int i = 0; i < num; i++) {
		double x = chrom_->XArray[i];
		double y = (double)chrom_->YArray[i];
		xyData->addPoint(x, y);
	}
}
