/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2008.10.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_GL_STD_AFX_H__
#define __KOME_GL_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// GLTools library namespace
	namespace gl {
	}
}


//
// macros
// 

#ifdef _MSC_VER
	#define GL_TOOLS_CLASS __declspec(dllexport)
#else
	#define GL_TOOLS_CLASS
#endif	// _MSC_VER

#define __KOME_GL_TOOLS_H__



//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4244 )
	#pragma warning( disable:4251 )
	#pragma warning( disable:4267 )
	#pragma warning( disable:4561 )
	#pragma warning( disable:4793 )
	#pragma warning( disable:4996 )
#endif	// _MSC_VER


//
// libraries
//

#ifdef _MSC_VER
	#pragma comment( lib, "glu32" )
	#pragma comment( lib, "opengl32" )
#endif	// _MSC_VER


//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/NumericalTools.h"
#include "kome/DataObjects.h"
#include "kome/ImageManager.h"




#endif	// __KOME_GL_STD_AFX_H_
