/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */

#ifndef __KOME_TPP_PLUGIN_STD_AFX_H__
#define __KOME_TPP_PLUGIN_STD_AFX_H__

/// kome project namespace
namespace kome {
	namespace Tpp{
		#define TPP_PERSONAL_FOLDER				"TPP_FILES"
		#define TPP_PERSONAL_XML_FOLDERNAME		"temp\\"
		#define TPP_PERSONAL_HTML_FOLDERNAME	"html\\"

		#define TABLE_NAME "TPPRESULTS"
	}
}

// warning message does not appear when building
#pragma warning( disable:4251 )
#pragma warning( disable:4018 )
#pragma warning( disable:4482 )
#pragma warning( disable:4996 )

#define WXUSINGDLL

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
	#endif	// _DEBUG
#endif	// _MSC_VER

//
// includes
//
#include "boost/serialization/serialization.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/string.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/export.hpp"
 
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
#include "kome/GLTools.h"
#include "kome/CLRManager.h"

#endif //__KOME_TPP_PLUGIN_STD_AFX_H__
