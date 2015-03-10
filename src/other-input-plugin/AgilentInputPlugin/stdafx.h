/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author S.Tanaka
 * @date 2009.05.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_AGILENT_STD_AFX_H__
#define __KOME_IO_AGILENT_STD_AFX_H__

//
// namespace
//

/// kome project namespace
namespace kome {
	/// file io namespace 
	namespace io {
		/// Agilent data input plug-in namespace
		namespace agilent {
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
	#pragma warning( disable:4945 )
#endif	// _MSC_VER


//
// libraries
//

#using <CLRManager.dll>
#using <CLRPluginTools.dll>

#using <MassSpecDataReader.dll>
#using <BaseDataAccess.dll>
#using <BaseTof.dll>
#using <BaseCommon.dll>
#using <BaseError.dll>
#using <agtsampleinforw.dll>

//
// includes
//

#ifndef __unix
	#include <windows.h>
#endif

#endif	// __KOME_IO_AGILENT_STD_AFX_H__
