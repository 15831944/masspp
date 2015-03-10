/**
 * @file PeakDetectionFilterFunctions.cpp
 * @brief implements of plug-in functions
 *
 * @author tanaka-s
 * @date 2014.09.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#include "stdafx.h"
#include "PeakDetectionFilterFunctions.h"
#include <map>

// The <param-name> for the "Response" dropdown list setting,
// as specified in settings.html
#define RESPONSE_PARAM_NAME				"response"
// The <param-name> for the "Number of Peaks" integer setting,
// as specified in settings.html
#define NUMBER_PARAM_NAME				"number"

// Declaration of the function called when "Number of Peaks" peak filtering is requested.
// For the "spec_peak_filter" call type, there are 4 parameters set in the params parameter object:
// 1. spectrum (Spectrum) - [in] the spectrum being peak-detected
// 2. peaks (Peaks) - [in, out] the detected peaks
// 3. settings (SettingParameterValues) - [in] the parameter settings
// 4. xyData (XYData) - [in] the XY data for the spectrum
// These parameters can be obtained using appropriate PluginCallTool::get... functions.
kome::objects::Variant SpecPeakFilterNumberOfPeaks( kome::objects::Parameters* params ){

	// Return value (Success: true, Failure: false)
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	// We initially set ret.prim.boolVal to false so that we can return a false
	// result easily using "return ret" if a failure condition occurs.
	ret.prim.boolVal = false;

	// Get the spectrum which was set in the params parameter.
	// This is the spectrum currently being peak-detected.
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	// Get the peaks which was set in the params parameter.
	// This contains the detected peaks.
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if( peaks == NULL ){
		return ret;
	}

	// Get the settings which was set in the params parameter.
	// This contains the parameter settings from the page used for this function's settings.
	// In plugin.xml: within the <call type="spec_peak_filter"> settings for
	// <call-function>SpecPeakFilterNumberOfPeaks, page is: numberofpeaks
	// numberofpeaks page definition can be found in settings.xml
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	// Get the XY data which was set in the params parameter.
	// This contains the XY data for the spectrum.
	kome::core::XYData* xyData = kome::plugin::PluginCallTool::getXYData( params );
	if( xyData == NULL ){
		return ret;
	}

	// Get the setting for the "Response" dropdown list, as a string.
	std::string response	=	settings->getParameterValue(RESPONSE_PARAM_NAME);
	// Get the setting for the "Number of Peaks" setting, and convert to an integer.
	int nMaxNumberOfPeaks	=	toint(settings->getParameterValue(NUMBER_PARAM_NAME));

	// Create a multimap of (peak response, peak index) to enable the peaks to be sorted by
	// response.
	std::multimap<double, int> peakMap;

	// Add all elements into the multimap. They are sorted automatically by key value (response).
	for( unsigned int i = 0; i < peaks->getLength(); i++ )
	{
		if ( response.compare( "intensity" ) == 0 )
		{
			// For the "Intensity" Response type, use the i'th Y value within the Peaks object.
			peakMap.insert(std::pair<double, int>(peaks->getY(i), i));
		}
		else	// "area"
		{
			// For the "Area" Response type, use the Area value from the i'th PeakElement
			// contained within the Peaks object.
			peakMap.insert(std::pair<double, int>(peaks->getPeak(i)->getArea(), i));
		}
	}

	// Get the instance of the PeaksManager. This is a singleton class whose single
	// instance can be accessed from anywhere using its getInstance static function.
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	// Create a new Peaks object in which to add the filtered peaks.
	kome::objects::Peaks addPeaks;

	// Create a multimap of (peak X, peak index) to enable the filtered peaks to be sorted by
	// X.
	std::multimap<double, int> peakXMap;

	// nPeakCount is used to count the number of peaks added so far as a result of filtering.
	int nPeakCount = 0;
	// Iterate through the (peak response, peak index) multimap in reverse order, adding the
	// first nMaxNumberOfPeaks peaks to the (peak X, peak index) multimap.
	// This will result in nMaxNumberOfPeaks peaks with the highest intensity / area being added,
	// in ascending X order.
	for (std::multimap<double, int>::reverse_iterator it = peakMap.rbegin();
		(it != peakMap.rend()) && (nPeakCount < nMaxNumberOfPeaks); it++)
	{
		// Add the X value of each peak, and its peak index (it->second) into the multimap.
		peakXMap.insert(std::pair<double, int>(peaks->getX(it->second), it->second));

		// Increment the count of the number of peaks filtered so far.
		nPeakCount++;
	}

	// newPeak is used to reference the new PeakElement information created for each peak in addPeaks.
	kome::objects::PeakElement* newPeak = NULL;
	// Iterate through the multimap of (peak X, peak index) in forward order, adding all peaks to the
	// addPeaks object.
	for (std::multimap<double, int>::iterator it = peakXMap.begin();
		(it != peakXMap.end()); it++)
	{
		// Create a new PeakElement item in addPeaks and set all the information into it
		// from the corresponding peak in the peaks object.
		// Note that "it->second" is the peak index of the current item in the multimap.
		newPeak = addPeaks.createPeak(peaks->getX(it->second), peaks->getY(it->second));
		if (newPeak != NULL)
		{
			newPeak->setLeft(peaks->getPeak(it->second)->getLeftX(), peaks->getPeak(it->second)->getLeftY());
			newPeak->setRight(peaks->getPeak(it->second)->getRightX(), peaks->getPeak(it->second)->getRightY());
			newPeak->setApex(peaks->getPeak(it->second)->getApexX(), peaks->getPeak(it->second)->getApexY());
			newPeak->setArea(peaks->getPeak(it->second)->getArea());
//			newPeak->setFwhm(peaks->getPeak(it->second)->getFwhm());
			newPeak->setId(peaks->getPeak(it->second)->getId());
		}
	}

	// Clear the Peaks object for the spectrum.
	pkMgr.createPeaks( spec );
	// Copy all the filtered peaks from the addPeaks Peaks object into the peaks object
	// to update the spectrum's detected peaks.
	for( unsigned int i=0; i < addPeaks.getLength(); i++ ){
		newPeak = peaks->createPeak(addPeaks.getX(i), addPeaks.getY(i));
		if (newPeak != NULL)
		{
			newPeak->setLeft(addPeaks.getPeak(i)->getLeftX(), peaks->getPeak(i)->getLeftY());
			newPeak->setRight(addPeaks.getPeak(i)->getRightX(), peaks->getPeak(i)->getRightY());
			newPeak->setApex(addPeaks.getPeak(i)->getApexX(), peaks->getPeak(i)->getApexY());
			newPeak->setArea(addPeaks.getPeak(i)->getArea());
//			newPeak->setFwhm(addPeaks.getPeak(i)->getFwhm());
			newPeak->setId(addPeaks.getPeak(i)->getId());
		}
	}

	// Request an update of the spectrum peak and the spectrum to display the updated peaks.
	kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
	kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );

	// Set a success value and return ret.
	ret.prim.boolVal = true;

	return ret;
}
