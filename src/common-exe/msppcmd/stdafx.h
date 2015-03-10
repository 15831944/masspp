/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2006.11.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MSPPCMD_STD_AFX_H__
#define __KOME_MSPPCMD_STD_AFX_H__

//
// namespace
//

/// kome project namespace
namespace kome {
	/// batch command namespace
	namespace batch {
	}
}


//
// macros
//

#define WXUSINGDLL


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
#endif	// _MSC_VER


//
// libraries
//

#ifdef _MSC_VER
	#pragma comment( lib, "ws2_32" )

	#pragma comment( lib, "CoreFunctions" )
	#pragma comment( lib, "CoreToolkits" )
	#pragma comment( lib, "DbTools" )
	#pragma comment( lib, "XMLTools" )
	#pragma comment( lib, "DataObjects" )
	#pragma comment( lib, "PluginTools" )

#endif	// _MSC_VER


//
// includes
//

#ifdef _MSC_VER
	#include <Winsock2.h>
#else
	#include <unistd.h>
#endif

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DbTools.h"
#include "kome/XMLTools.h"
#include "kome/DataObjects.h"
#include "kome/PluginTools.h"



#endif	// __KOME_MSPPCMD_STD_AFX_H__
