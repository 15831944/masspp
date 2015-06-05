/**
 * @file MsbackadjBaselineDetectionPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author ozaki
 * @date 2013.07.31
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MsbackadjBaselineDetectionPluginFunctions.h"
#include "MsbackadjBaselineDetectionManager.h"


using namespace kome::baseline::msbackadj;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// Define parameter names
#define	PARAM_NAME_WINDOW_SIZE			( "window_size" )
#define	PARAM_NAME_STEP_SIZE			( "step_size" )
#define	PARAM_NAME_REGRESSION_METHOD	( "regression_method" )
#define	PARAM_NAME_ESTIMATION_METHOD	( "estimation_method" )
#define	PARAM_NAME_SMOOTHING_METHOD		( "smoothing_method" )
#define	PARAM_NAME_QUANTILE				( "quantile" )

// Define parameter initial value
#define	PARAM_INIT_WINDOW_SIZE			( 200.0 )
#define	PARAM_INIT_STEP_SIZE			( 50.0 )
#define	PARAM_INIT_REGRESSION_METHOD	( 0 )
#define	PARAM_INIT_ESTIMATION_METHOD	( 0 )
#define	PARAM_INIT_SMOOTHING_METHOD		( 0 )
#define	PARAM_INIT_QUANTILE				( 0.1 )

// detect baseline
kome::objects::Variant	msbackadjBaselineDetection( kome::objects::Parameters* params )
{
	// return value
	kome::objects::Variant ret;

	// get object
	kome::core::XYData*	src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}
	kome::core::XYData* baseline = kome::plugin::PluginCallTool::getBaseline( params );
	if( baseline == NULL ) {
		return ret;
	}

	kome::objects::SettingParameterValues*	settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// manager
	MsbackadjBaselineDetectionManager	&insManager = MsbackadjBaselineDetectionManager::getInstance( );

	wxBeginBusyCursor( );

	// set parameters
	double dWindowSize = PARAM_INIT_WINDOW_SIZE;
	double dStepSize = PARAM_INIT_STEP_SIZE;
	int	nRegressionMethod = PARAM_INIT_REGRESSION_METHOD;
	int	nEstimationMethod = PARAM_INIT_ESTIMATION_METHOD;
	int	nSmoothingMethod = PARAM_INIT_SMOOTHING_METHOD;
	double	dQuantile = PARAM_INIT_QUANTILE;

	if  ( (kome::objects::SettingParameterValues *)NULL != settings )
	{
		dWindowSize = settings->getDoubleValue( PARAM_NAME_WINDOW_SIZE, dWindowSize );
		dStepSize = settings->getDoubleValue( PARAM_NAME_STEP_SIZE, dStepSize );
		nRegressionMethod = settings->getIntValue( PARAM_NAME_REGRESSION_METHOD, nRegressionMethod );
		nEstimationMethod = settings->getIntValue( PARAM_NAME_ESTIMATION_METHOD, nEstimationMethod );
		nSmoothingMethod = settings->getIntValue( PARAM_NAME_SMOOTHING_METHOD, nSmoothingMethod );
		dQuantile = settings->getDoubleValue( PARAM_NAME_QUANTILE, dQuantile );
	}

	insManager.setWindowSize( dWindowSize );
	insManager.setStepSize( dStepSize );
	insManager.setRegressionMethod( nRegressionMethod );
	insManager.setEstimationMethod( nEstimationMethod );
	insManager.setSmoothingMethod( nSmoothingMethod );
	insManager.setQuantile( dQuantile );

	// get baseline
	insManager.getMsbackadj( *src, *baseline );

	wxEndBusyCursor( );

	return ret;
}

// exit plugin
kome::objects::Variant exitPlugin( kome::objects::Parameters* params )
{
	// retrun value
	kome::objects::Variant ret;

	// get manager
	MsbackadjBaselineDetectionManager	&insManager = MsbackadjBaselineDetectionManager::getInstance( );

	// release MATLAB application
	insManager.releaseMatlab( );

	return	ret;
}

//
