/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2012.07.11
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_SCRIPT_PYTHON_STD_AFX_H__
#define __KOME_SCRIPT_PYTHON_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// script namespace 
	namespace io {
		/// python namespace
		namespace python {
		}
	}
}


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

#using <CLRManager.dll>
#using <CLRPluginTools.dll>


//
// includes
//
#ifndef __unix
	#include <windows.h>
#endif

#endif	// __KOME_SCRIPT_PYTHON_STD_AFX_H__
