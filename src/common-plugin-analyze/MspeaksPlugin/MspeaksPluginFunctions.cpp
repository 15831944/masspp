/**
 * @file MspeaksPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author A.Ozaki
 * @date 2013.07.31
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MspeaksPluginFunctions.h"
#include "MspeaksDetectionManager.h"


using namespace kome::peaks::mspeaks;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// Define parameter names
#define	PARAM_NAME_BASE					( "base" )
#define	PARAM_NAME_LEVELS				( "levels" )
#define	PARAM_NAME_NOISE				( "noise" )
#define	PARAM_NAME_MULTIPLINER			( "multipliner" )
#define	PARAM_NAME_DENOISING			( "denoising" )
#define	PARAM_NAME_PEAK_LOCATION		( "peak_location" )
#define	PARAM_NAME_FWHM_FILTER			( "fwhm_filter" )
#define	PARAM_NAME_OVER_SEGMENTATION	( "over_segmentation" )

// Define parameter initial values
#define	PARAM_INIT_BASE					( 4 )
#define	PARAM_INIT_LEVELS				( 10 )
#define	PARAM_INIT_NOISE				( 0 )
#define	PARAM_INIT_MULTIPLINER			( 1.0 )
#define	PARAM_INIT_DENOISING			( 0 )
#define	PARAM_INIT_PEAK_LOCATION		( 1.0 )
#define	PARAM_INIT_FWHM_FILTER			( 0.0 )
#define	PARAM_INIT_OVER_SEGMENTATION	( 0.0 )

// detect peaks
kome::objects::Variant mspeaksDetection( kome::objects::Parameters* params )
{
	// retrun value
	kome::objects::Variant ret;

	// data points
	kome::core::XYData	*pXyData = kome::plugin::PluginCallTool::getXYData( params );
	if  ( (kome::core::XYData *)NULL == pXyData )
	{
		return	ret;
	}

	kome::objects::Spectrum	*pSpec = (kome::objects::Spectrum *)NULL;
	kome::objects::Variant	var = params->getValue( "spectrum" );
	if  ( kome::objects::Variant::SPECTRUM == var.type )
	{
		pSpec = (kome::objects::Spectrum *)var.prim.pt;
	}

	// peaks
	kome::objects::Peaks	*peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if  ( (kome::objects::Peaks *)NULL == peaks )
	{
		return	ret;
	}

	// settings
	kome::objects::SettingParameterValues	tmp;
	kome::objects::SettingParameterValues	*pSettings = kome::plugin::PluginCallTool::getSettingValues( params );
	if  ( (kome::objects::SettingParameterValues *)NULL == pSettings )
	{
		pSettings = &tmp;
	}

	wxBeginBusyCursor( );

	// set parameters
	int	nBase = PARAM_INIT_BASE;
	int	nLevels = PARAM_INIT_LEVELS;
	int	nNoise = PARAM_INIT_NOISE;
	double	dMultipliner = PARAM_INIT_MULTIPLINER;
	int	nDenoising = PARAM_INIT_DENOISING;
	double	dPeakLocation = PARAM_INIT_PEAK_LOCATION;
	double	dFWHMFilter = PARAM_INIT_FWHM_FILTER;
	double	dOverSegmentation = PARAM_INIT_OVER_SEGMENTATION;

	if  ( (kome::objects::SettingParameterValues *)NULL != pSettings )
	{
		nBase  = pSettings->getIntValue( PARAM_NAME_BASE, nBase );
		nLevels = pSettings->getIntValue( PARAM_NAME_LEVELS, nLevels );
		nNoise = pSettings->getIntValue( PARAM_NAME_NOISE, nNoise );
		dMultipliner = pSettings->getDoubleValue( PARAM_NAME_MULTIPLINER, dMultipliner );
		nDenoising = pSettings->getIntValue( PARAM_NAME_DENOISING, nDenoising );
		dPeakLocation = pSettings->getDoubleValue( PARAM_NAME_PEAK_LOCATION, dPeakLocation );
		dFWHMFilter = pSettings->getDoubleValue( PARAM_NAME_FWHM_FILTER, dFWHMFilter);
		dOverSegmentation = pSettings->getDoubleValue( PARAM_NAME_OVER_SEGMENTATION, dOverSegmentation );
	}

	// peak detection
	MspeaksDetectionManager	&insManager = MspeaksDetectionManager::getInstance( );

	insManager.setBase( nBase );
	insManager.setLevels( nLevels );
	insManager.setNoise( nNoise );
	insManager.setMultipliner( dMultipliner );
	insManager.setDenoising( nDenoising );
	insManager.setPeakLocation( dPeakLocation );
	insManager.setFWHMFilter( dFWHMFilter );
	insManager.setOverSegmentation( dOverSegmentation );

	insManager.detection( *pXyData, *peaks );

	wxEndBusyCursor( );

	return	ret;
}


// exit plugin
kome::objects::Variant exitPlugin( kome::objects::Parameters* params )
{
	// retrun value
	kome::objects::Variant ret;

	// get manager
	MspeaksDetectionManager	&insManager = MspeaksDetectionManager::getInstance( );

	// release MATLAB application
	insManager.releaseMatlab( );

	return	ret;
}
