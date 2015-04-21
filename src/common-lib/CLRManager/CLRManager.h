/**
 * @file CLRManager.h
 * @brief interfaces of CLR function
 *
 * @author S.Tanaka
 * @date 2006.06.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_MANAGER_H__
#define __KOME_CLR_MANAGER_H__

//
// macros
// 

#ifdef _MSC_VER
	#define CLR_MANAGER_CLASS __declspec(dllimport)
#else
	#define CLR_MANAGER_CLASS
#endif	// _MSC_VER


//
// includes
//


#include "CLRManager/ClrPluginFunction.h"

#ifdef _MANAGED
#include "CLRManager/ClrObjectTool.h"
#endif	// _MANAGED

#endif	// __KOME_CLR_MANAGER_H__
