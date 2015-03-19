/**
 * @file MascotLib.h
 * @brief header file to use MascotLib library
 *
 * @author S.Tanaka
 * @date 2012.10.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MASCOT_LIB_H__
#define __KOME_MASCOT_LIB_H__



//
// macros
//

#ifdef _MSC_VER
	#define MASCOT_LIB_CLASS __declspec(dllimport)
#else
	#define MASCOT_LIB_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "MascotManager.h"
#include "MascotSearchEngineBase.h"


#endif    // __KOME_MASCOT_LIB_H__
