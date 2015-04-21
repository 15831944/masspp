/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2006.07.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_STD_AFX_H__
#define __KOME_OBJECTS_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// DataObjects library namespace
	namespace objects {
	}
}


//
// macros
// 

#ifdef _MSC_VER
	#define DATA_OBJECTS_CLASS __declspec(dllexport)
#else
	#define DATA_OBJECTS_CLASS
#endif	// _MSC_VER

#define __KOME_OBJECTS_DATA_OBJECTS_H__


//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4251 )
	#pragma warning( disable:4267 )
	#pragma warning( disable:4512 )
	#pragma warning( disable:4561 )
	#pragma warning( disable:4793 )
	#pragma warning( disable:4819 )
	#pragma warning( disable:4996 )
#endif	// _MSC_VER


//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/NumericalTools.h"
#include "kome/DbTools.h"

#endif	// __KOME_OBJECTS_STD_AFX_H__
