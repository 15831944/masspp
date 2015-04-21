/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2006.07.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IMG_STD_AFX_H__
#define __KOME_IMG_STD_AFX_H__

//
// namespace
//

/// kome project namespace
namespace kome {
	/// ImageManager library namespace
	namespace img {
	}
}


//
// macros
// 

#ifdef _MSC_VER
	#define IMAGE_MANAGER_CLASS __declspec(dllexport)
#else
	#define IMAGE_MANAGER_CLASS
#endif	// _MSC_VER

#define __KOME_IMG_IMAGE_MANAGER_H__


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
	#pragma warning( disable:4996 )
#endif	// _MSC_VER


//
// libraries
//

#ifdef _MSC_VER
	#pragma comment( lib, "cairo" )
#endif	// _MSC_VER


//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/NumericalTools.h"


#endif	// __KOME_IMG_STD_AFX_H__
