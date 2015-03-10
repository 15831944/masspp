/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2008.11.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_PNG_STD_AFX_H__
#define __KOME_IO_PNG_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// file IO namespace 
	namespace io {
		/// png output plug-in name space
		namespace png {
		}
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
#endif	// _MSC_VER


//
// libraries
//

#ifdef _MSC_VER
	#pragma comment( lib, "CoreFunctions" )
	#pragma comment( lib, "CoreToolkits" )
	#pragma comment( lib, "DataObjects" )
	#pragma comment( lib, "ImageManager" )
	#pragma comment( lib, "PluginTools" )

	#pragma comment( lib, "cairo" )
#endif	// _MSC_VER


//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/ImageManager.h"
#include "kome/PluginTools.h"


#endif	// __KOME_IO_PNG_STD_AFX_H__
