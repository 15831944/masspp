/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2006.06.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_STD_AFX_H__
#define __KOME_CLR_STD_AFX_H__

//
// check environment
//

#ifndef _MANAGED
	#error CLRManager library depends on Common Language Runtime (/clr)
#endif	// _MANAGED



//
// namespace
//

/// kome project namespace
namespace kome {
	/// CLR namespace
	namespace clr {
	}
}


//
// macros
//

#define CLR_MANAGER_CLASS __declspec(dllexport)
#define ENUM_CLASS		enum class

#define __KOME_CLR_MANAGER_H__


//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4100 )
	#pragma warning( disable:4251 )
	#pragma warning( disable:4267 )
	#pragma warning( disable:4561 )
	#pragma warning( disable:4793 )
	#pragma warning( disable:4819 )
#endif	// _MSC_VER


//
// includes
//

#using <mscorlib.dll>
#include <vcclr.h>

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/ImageManager.h"
#include "kome/EventManager.h"


#endif	// __KOME_CLR_STD_AFX_H__
