/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_STD_AFX_H__
#define __KOME_CLR_STD_AFX_H__

//
// namespace
//

/// kome project namespace
namespace kome {
	/// PluginTool library namespace
	namespace clr {
	}
}

//
// message
//

#ifdef _MSC_VER
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
	#pragma comment( lib, "CLRManager" )
    #pragma comment( lib, "DataOperation" )
    #pragma comment( lib, "IdentificationTools" )
#endif	// _MSC_VER



//
// includes
//

#using <mscorlib.dll>
#include <vcclr.h>

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/ImageManager.h" 
#include "kome/EventManager.h"
#include "kome/CLRManager.h"
#include "kome/PluginTools.h"
#include "kome/DbTools.h"
#include "kome/IdentificationTools.h"
#include "kome/DataOperation.h"


//
// macros
//

#define ENUM_CLASS		enum class


#endif	// __KOME_CLR_STD_AFX_H__
