/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2007.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_EVT_STD_AFX_H__
#define __KOME_EVT_STD_AFX_H__

//
// namespace
//

/// kome project namespace
namespace kome {
	/// EventManager library namespace
	namespace evt {
	}
}


//
// macros
// 

#ifdef _MSC_VER
	#define EVENT_MANAGER_CLASS __declspec(dllexport)
#else
	#define EVENT_MANAGER_CLASS
#endif	// _MSC_VER

#define __KOME_EVENT_MANAGER_H__


//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4251 )
	#pragma warning( disable:4561 )
	#pragma warning( disable:4793 )
#endif	// _MSC_VER



//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"

#endif	// __KOME_EVT_STD_AFX_H__
