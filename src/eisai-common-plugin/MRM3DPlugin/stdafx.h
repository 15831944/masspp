/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author M.Fukuda
 * @date 2012.10.12
 * 
 * Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MRM3D_STD_AFX_H__
#define __KOME_MRM3D_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// ab3d alignment namespace
	namespace mrm3d {
	}
}

//
// macros
//

#define WXUSINGDLL

//
// message
//

#ifdef _MSC_VER
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
	#pragma comment( lib, "WindowManager" )
	#pragma comment( lib, "NumericalTools" )
	#pragma comment( lib, "DataOperation" )

	#ifdef _DEBUG
		#pragma comment( lib, "wxbase28d" )
		#pragma comment( lib, "wxmsw28d_core" )
	#else
		#pragma comment( lib, "wxbase28" )
		#pragma comment( lib, "wxmsw28_core" )
	#endif	// _DEBUG
#endif	// _MSC_VER


//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/PluginTools.h"
#include "kome/ImageManager.h"
#include "kome/WindowManager.h"
#include "kome/NumericalTools.h"
#include "kome/DataOperation.h"


#endif
