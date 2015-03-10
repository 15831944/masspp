/**
 * @file ApiPeakDetectionPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ApiPeakDetectionPluginFunctions.h"


// detect peaks
kome::objects::Variant detectPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// peaks
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if( peaks == NULL ) {
		return ret;
	}

	// sample
	kome::objects::Sample* sample = spec->getSample();
	if( sample == NULL ) {
		return ret;
	}

	// detect peaks
	sample->detectPeaksByAPI( spec, peaks );

	return ret;
}
