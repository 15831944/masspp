/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author tanaka-s
 * @date 2012.10.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_MASCOT_LIB_STD_AFX_H__
#define __KOME_MASCOT_LIB_STD_AFX_H__



//
// namespace
//

/// kome project namespace
namespace kome {
    namespace mascot {
	}
}


//
// macros
//


#define WXUSINGDLL

#ifdef _MSC_VER
	#define MASCOT_LIB_CLASS __declspec(dllexport)
#else
	#define MASCOT_LIB_CLASS
#endif	// _MSC_VER


//
// message
//

#ifdef _MSC_VER
	#pragma warning( disable:4190 )
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
	#pragma comment( lib, "CoreFunctions" )
	#pragma comment( lib, "CoreToolkits" )
	#pragma comment( lib, "DataObjects" )
	#pragma comment( lib, "ImageManager" )
	#pragma comment( lib, "PluginTools" )
	#pragma comment( lib, "DbTools" )
	#pragma comment( lib, "DataOperation" )
	#pragma comment( lib, "IdentificationTools" )
	#pragma comment( lib, "EventManager" )
	#pragma comment( lib, "WindowManager" )
	#pragma comment( lib, "XMLTools" )
	#pragma comment( lib, "NetworkTools" )
	#pragma comment( lib, "GLTools" )

	#ifdef _DEBUG
		#pragma comment( lib, "wxbase28d" )
		#pragma comment( lib, "wxbase28d_net" )
		#pragma comment( lib, "wxbase28d_xml" )
		#pragma comment( lib, "wxexpatd" )
		#pragma comment( lib, "wxjpegd" )
		#pragma comment( lib, "wxmsw28d_adv" )
		#pragma comment( lib, "wxmsw28d_aui" )
		#pragma comment( lib, "wxmsw28d_core" )
		#pragma comment( lib, "wxmsw28d_gl" )
		#pragma comment( lib, "wxmsw28d_html" )
		#pragma comment( lib, "wxmsw28d_media" )
		#pragma comment( lib, "wxmsw28d_qa" )
		#pragma comment( lib, "wxmsw28d_richtext" )
		#pragma comment( lib, "wxmsw28d_xrc" )
		#pragma comment( lib, "wxpngd" )
		#pragma comment( lib, "wxregexd" )
		#pragma comment( lib, "wxtiffd" )
		#pragma comment( lib, "wxzlibd" )

		#pragma comment( lib, "xerces-c_3D" )
		#pragma comment( lib, "msparserD" )
	#else
		#pragma comment( lib, "wxbase28" )
		#pragma comment( lib, "wxbase28_net" )
		#pragma comment( lib, "wxbase28_xml" )
		#pragma comment( lib, "wxexpat" )
		#pragma comment( lib, "wxjpeg" )
		#pragma comment( lib, "wxmsw28_adv" )
		#pragma comment( lib, "wxmsw28_aui" )
		#pragma comment( lib, "wxmsw28_core" )
		#pragma comment( lib, "wxmsw28_gl" )
		#pragma comment( lib, "wxmsw28_html" )
		#pragma comment( lib, "wxmsw28_media" )
		#pragma comment( lib, "wxmsw28_qa" )
		#pragma comment( lib, "wxmsw28_richtext" )
		#pragma comment( lib, "wxmsw28_xrc" )
		#pragma comment( lib, "wxpng" )
		#pragma comment( lib, "wxregex" )
		#pragma comment( lib, "wxtiff" )
		#pragma comment( lib, "wxzlib" )

		#pragma comment( lib, "xerces-c_3" )
		#pragma comment( lib, "msparser" )
	#endif	// _DEBUG
#endif	// _MSC_VER


//
// includes
//

#include <windows.h>


#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/ImageManager.h"
#include "kome/PluginTools.h"
#include "kome/DbTools.h"
#include "kome/DataOperation.h"
#include "kome/IdentificationTools.h"
#include "kome/EventManager.h"
#include "kome/WindowManager.h"
#include "kome/XMLTools.h"
#include "kome/NetworkTools.h"


#endif	// __KOME_MASCOT_LIB_STD_AFX_H__
