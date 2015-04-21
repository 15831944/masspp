/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2008.08.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_STD_AFX_H__
#define __KOME_DB_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// DbTools library namespace
	namespace db {
	}
}


//
// macros
// 

#ifdef _MSC_VER
	#define DB_TOOLS_CLASS __declspec(dllexport)
#else
	#define DB_TOOLS_CLASS
#endif	// _MSC_VER

#define __KOME_DB_TOOLS_H__



//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4244 )
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

#endif	// __KOME_DB_STD_AFX_H_
