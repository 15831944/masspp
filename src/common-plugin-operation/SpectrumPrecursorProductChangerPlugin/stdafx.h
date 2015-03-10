/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author H.Parry
 * @date 2011.03.17
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */

#pragma once

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>

// C4251 warning message does not appear when building
#pragma warning( disable:4251 )

//
// libraries
//

#ifdef _MSC_VER
	#pragma comment( lib, "CoreFunctions" )
	#pragma comment( lib, "CoreToolkits" )
	#pragma comment( lib, "DataObjects" )
	#pragma comment( lib, "ImageManager" )
	#pragma comment( lib, "PluginTools" )
	#pragma comment( lib, "WindowManager" )
	#pragma comment( lib, "NumericalTools" )
	#pragma comment( lib, "DataOperation" )

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
    #endif  // _DEBUG
#endif  // _MSC_VER

//
// includes
//

#include <kome/CoreFunctions.h>
#include <kome/CoreToolkits.h>
#include <kome/DataObjects.h>
#include <kome/ImageManager.h>
#include <kome/PluginTools.h>
#include <kome/WindowManager.h>
#include <kome/DataObjects/PeaksManager.h>
#include <kome/DataObjects/Peaks.h>
