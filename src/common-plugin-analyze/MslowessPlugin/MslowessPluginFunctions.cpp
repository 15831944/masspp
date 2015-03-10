/**
 * @file MslowessPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author ozaki
 * @date 2013.08.23
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MslowessPluginFunctions.h"
#include "MslowessManager.h"

using namespace kome::labeling::mslowess;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define	PARAM_NAME_SMOOTHING_METHOD				"smoothing_method"
#define	PARAM_NAME_SN							"s/n"
#define	PARAM_NAME_ORDER						"order"
#define	PARAM_NAME_SPAN							"span"
#define	PARAM_NAME_KERNEL						"kernel"
#define	PARAM_NAME_ROBUST_ITERATIONS			"robust_iterations"

#define	PARAM_VALUE_MSLOWESS					"mslowess"

#define	PARAM_INIT_SN							( 5.0 )
#define	PARAM_INIT_ORDER						( 1 )
#define	PARAM_INIT_SPAN							( 20.0 )
#define	PARAM_INIT_KERNEL						( 0 )
#define	PARAM_INIT_ROBUST_ITERATIONS			( 0.0 )

#define	ERROR_MESSAGE_INITIALIZE				( "BioinformaticsToolboxFunc library cannot be initialized." )
#define	FUNCTION_NAME							( "Mslowess" )

// 
kome::objects::Variant	mslowessFilter( kome::objects::Parameters* params )
{
	// return value
	kome::objects::Variant ret;
	ret.prim.boolVal = false;
	ret.type = kome::objects::Variant::BOOL;

	// setting parameter values
	kome::objects::SettingParameterValues	*pSettings = kome::plugin::PluginCallTool::getSettingValues( params );
	if  ( (kome::objects::SettingParameterValues *)NULL == pSettings )
	{
		return	ret;
	}

	// pageã‚Ì’l‚Ì“Ç‚Ýž‚Ý
	//
	typedef enum {
		SMOOTHING_METHOD_UNKOWN = -1,
		SMOOTHING_METHOD_MSLOWESS,
	} eSmoothingMethod;
	eSmoothingMethod	eMethod = SMOOTHING_METHOD_MSLOWESS;
	double	dSN = PARAM_INIT_SN;

	// for Mslowess parameters
	int	nOrder = PARAM_INIT_ORDER;
	double	dSpan = PARAM_INIT_SPAN;
	int	nKernel = PARAM_INIT_KERNEL;
	double	dRobustIterations = PARAM_INIT_ROBUST_ITERATIONS;

	// load sub parameters
	kome::objects::SettingParameterValues	*pSubSettings = NULL;
	pSubSettings = pSettings->getSubParameterValues( PARAM_NAME_SMOOTHING_METHOD, PARAM_VALUE_MSLOWESS );
	if  ( (kome::objects::SettingParameterValues *)NULL != pSubSettings )
	{
		nOrder = pSubSettings->getIntValue( PARAM_NAME_ORDER, nOrder );
		dSpan = pSubSettings->getDoubleValue( PARAM_NAME_SPAN, dSpan );
		nKernel = pSubSettings->getIntValue( PARAM_NAME_KERNEL, nKernel );
		dRobustIterations = pSubSettings->getDoubleValue( PARAM_NAME_ROBUST_ITERATIONS, dRobustIterations );
	}
	
	dSN = pSettings->getDoubleValue( PARAM_NAME_SN, dSN );

	// filtering
	MslowessManager	&insManager = MslowessManager::getInstance( );
	
	insManager.setOrder( nOrder );
	insManager.setSpan( dSpan );
	insManager.setKernel( nKernel );
	insManager.setRobustIterations( dRobustIterations );
	insManager.setSn( dSN );

	// execute
	ret = insManager.filtering( params );

	return	ret;

}

// exit plugin
kome::objects::Variant exitPlugin( kome::objects::Parameters* params )
{
	// retrun value
	kome::objects::Variant ret;

	// get manager
	MslowessManager	&insManager = MslowessManager::getInstance( );

	// release MATLAB application
	insManager.releaseMatlab( );

	return	ret;
}

//
//


