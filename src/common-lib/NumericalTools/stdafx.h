/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2006.07.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_STD_AFX_H__
#define __KOME_NUMERIC_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// NumericalTools library namespace
	namespace numeric {
	}
}

#define __KOME_NUMERICAL_TOOLS_H__


//
// macros
// 

#ifdef _MSC_VER
	#define NUMERICAL_TOOLS_CLASS __declspec(dllexport)
#else
	#define NUMERICAL_TOOLS_CLASS
#endif	// _MSC_VER


//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4251 )
	#pragma warning( disable:4267 )
	#pragma warning( disable:4561 )
	#pragma warning( disable:4793 )
#endif	// _MSC_VER



//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"


#endif	// __KOME_NUMERIC_STD_AFX_H__
