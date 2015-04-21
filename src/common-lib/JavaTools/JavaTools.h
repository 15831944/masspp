/**
 * @file JavaTools.h
 * @brief header file to use JavaTools library
 *
 * @author S.Tanaka
 * @date 2012.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_JAVA_TOOLS_H__
#define __KOME_JAVA_TOOLS_H__

#ifdef MSPP_USE_JAVA


//
// macros
// 

#ifdef _MSC_VER
	#define JAVA_TOOLS_CLASS __declspec(dllimport)
#else
	#define JAVA_TOOLS_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "JavaTools/JavaManager.h"


#endif MSPP_USE_JAVA    // MSPP_USE_JAVA


#endif	//  __KOME_JAVA_TOOLS_H__
