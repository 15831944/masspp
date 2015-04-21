/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2006.07.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_STD_AFX_H__
#define __KOME_CORE_STD_AFX_H__

//
// namespace
//

/// kome project namespace
namespace kome {
	/// CoreToolkit library namespace
	namespace core {
	}
}


//
// macros
// 

#ifdef _MSC_VER
	#define CORE_TOOLKITS_CLASS __declspec(dllexport)
#else
	#define CORE_TOOLKITS_CLASS
#endif	// _MSC_VER

#define __KOME_CORE_TOOLKITS_H__


//
// message
//

#ifdef _MSC_VER
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
    #pragma comment( lib, "gtest-md" )
#endif  // _MSC_VER


//
// includes
//

#include "kome/CoreFunctions.h"


#endif	// __KOME_CORE_STD_AFX_H__

