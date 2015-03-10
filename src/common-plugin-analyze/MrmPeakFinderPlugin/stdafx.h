/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_POSITION_MRM_PEAK_FINDER_STD_AFX_H__
#define __KOME_POSITION_MRM_PEAK_FINDER_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// position namespace
	namespace position {
		/// mrm
		namespace mrm {
		}
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
	#pragma comment( lib, "NumericalTools" )
	#pragma comment( lib, "DataObjects" )
	#pragma comment( lib, "DbTools" )
	#pragma comment( lib, "ImageManager" )
	#pragma comment( lib, "PluginTools" )
	#pragma comment( lib, "DataOperation" )
	#pragma comment( lib, "IdentificationTools" )
#endif	// _MSC_VER



//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/NumericalTools.h"
#include "kome/DataObjects.h"
#include "kome/DbTools.h"
#include "kome/ImageManager.h"
#include "kome/PluginTools.h"
#include "kome/DataOperation.h"
#include "kome/IdentificationTools.h"


#endif	// __KOME_POSITION_MRM_PEAK_FINDER_STD_AFX_H__
