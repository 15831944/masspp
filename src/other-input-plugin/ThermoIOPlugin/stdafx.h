/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author M.Fukuda
 * @date 2011.10.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_IO_THERMO_STD_AFX_H__
#define __KOME_IO_THERMO_STD_AFX_H__

//
// namespace
//

/// kome project namespace
namespace kome {
	/// file io namespace 
	namespace io {
		/// thermo data input plug-in namespace
		namespace thermo {
		}
	}
}

#pragma warning( disable:4251 )

		#pragma comment( lib, "CoreFunctions" )
		#pragma comment( lib, "CoreToolkits" )
		#pragma comment( lib, "DataObjects" )
		#pragma comment( lib, "PluginTools" )
	#ifdef _DEBUG
	#else
	#endif	// _DEBUG




//
// includes
//
#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/PluginTools.h"

typedef kome::objects::DataGroupNode DGN;

#endif
