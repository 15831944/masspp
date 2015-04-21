/**
 * @file GLTools.h
 * @brief header file to use GLTools library
 *
 * @author S.Tanaka
 * @date 2008.11.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_GL_TOOLS_H__
#define __KOME_GL_TOOLS_H__


//
// macros
// 

#ifdef _MSC_VER
	#define GL_TOOLS_CLASS __declspec(dllimport)
#else
	#define GL_TOOLS_CLASS
#endif	// _MSC_VER


//
// includes
//


#include "GLTools/GLDataMapInfo.h"


#endif //  __KOME_GL_TOOLS_H__
