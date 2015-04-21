/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2009.07.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_STD_AFX_H__
#define __KOME_NETWORK_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// network library namespace
	namespace net {
	}
}


//
// macros
// 

#ifdef _MSC_VER
	#define NETWORK_TOOLS_CLASS __declspec(dllexport)
#else
	#define NETWORK_TOOLS_CLASS
#endif	// _MSC_VER

#define __KOME_NETWORK_TOOLS_H__


//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4005 )
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
	#pragma comment( lib, "ws2_32" )
    #pragma comment( lib, "winhttp" )
    #pragma comment( lib, "credui" )

	#ifdef _DEBUG
		#pragma comment( lib, "xerces-c_3D" )
	#else
		#pragma comment( lib, "xerces-c_3" )
	#endif	// _DEBUG
#endif	// _MSC_VER


//
// includes
//

#ifdef _MSC_VER
	#include <Winsock2.h>
#else
	#include <sys/socket.h>
#endif


#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/ImageManager.h"
#include "kome/EventManager.h"
#include "kome/PluginTools.h"
#include "kome/NumericalTools.h"
#include "kome/DataOperation.h"
#include "kome/XMLTools.h"

#endif	// __KOME_NETWORK_STD_AFX_H_
