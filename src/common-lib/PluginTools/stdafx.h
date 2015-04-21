/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2006.06.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_STD_AFX_H__
#define __KOME_PLUGIN_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// PluginTool library namespace
	namespace plugin {
	}
}


//
// macros
// 

#ifdef _MSC_VER
	#define PLUGIN_TOOLS_CLASS __declspec(dllexport)
#else
	#define PLUGIN_TOOLS_CLASS
#endif	// _MSC_VER

#define __KOME_PLUGIN_TOOLS_H__



//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4244 )
	#pragma warning( disable:4251 )
	#pragma warning( disable:4267 )
	#pragma warning( disable:4512 )
	#pragma warning( disable:4561 )
	#pragma warning( disable:4793 )
	#pragma warning( disable:4819 )
#endif	// _MSC_VER


//
// libraries
//

#ifdef _MSC_VER
	#pragma comment( lib, "CLRManager" )

	#ifdef _DEBUG
		#pragma comment( lib, "xerces-c_3D" )
	#else
		#pragma comment( lib, "xerces-c_3" )
	#endif	// _DEBUG
#endif	// _MSC_VER


//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/XMLTools.h"
#include "kome/NumericalTools.h"
#include "kome/DataObjects.h"
#include "kome/ImageManager.h"
#include "kome/EventManager.h"
#include "kome/DbTools.h"


#ifdef _MSC_VER
	#include <windows.h>
	#include "kome/CLRManager.h"
#else
	#include <dlfcn.h>
#endif	// _MSC_VER

#endif	// __KOME_PLUGIN_STD_AFX_H__
