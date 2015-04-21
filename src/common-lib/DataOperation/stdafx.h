/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2007.08.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DATA_OPERATION_STD_AFX_H__
#define __KOME_DATA_OPERATION_STD_AFX_H__

//
// namespace
//

/// kome project namespace
namespace kome {
	/// DataOperation library namespace
	namespace operation {
	}
}


//
// macros
// 

#ifdef _MSC_VER
	#define DATA_OPERATION_CLASS __declspec(dllexport)
#else
	#define DATA_OPERATION_CLASS
#endif	// _MSC_VER

#define __KOME_DATA_OPERATION_H__


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
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/NumericalTools.h"
#include "kome/PluginTools.h"


#endif		// __KOME_DATA_OPERATION_STD_AFX_H__
