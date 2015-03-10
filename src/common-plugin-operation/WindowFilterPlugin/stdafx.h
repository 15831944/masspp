/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author M.Fukuda
 * @date 2010
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_FILTER_WINDOW_FILTER_STD_AFX_H__
#define __KOME_FILTER_WINDOW_FILTER_STD_AFX_H__

//
// define
//
#define WIDTH_PARAM_NAME	   "width"
#define SIGMA_PARAM_NAME	   "sigma"

//
// namespace
//

/// kome project namespace
namespace kome {
	/// filter namespace
	namespace filter {
		/// window filter plug-in name space
		namespace window {




		}
	}
}


//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4018 )
	#pragma warning( disable:4190 )
	#pragma warning( disable:4244 )
	#pragma warning( disable:4251 )
	#pragma warning( disable:4267 )
	#pragma warning( disable:4561 )
	#pragma warning( disable:4793 )
	#pragma warning( disable:4819 )
	#pragma warning( disable:4996 )
#endif	// _MSC_VER

//
// libraries
//

#ifdef _MSC_VER
	#pragma comment( lib, "CoreFunctions" )
	#pragma comment( lib, "CoreToolkits" )
	#pragma comment( lib, "DataObjects" )
	#pragma comment( lib, "PluginTools" )
	#pragma comment( lib, "ImageManager" )
	#pragma comment( lib, "NumericalTools" )
#endif	// _MSC_VER


//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/PluginTools.h"
#include "kome/ImageManager.h"
#include "kome/NumericalTools.h"


#endif		// __KOME_FILTER_WINDOW_FILTER_STD_AFX_H__
