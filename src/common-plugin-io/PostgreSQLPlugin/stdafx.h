/**
 * @file stdafx.h
 * @brief common header file
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_DB_POSTGRESQL_STD_AFX_H__
#define __KOME_DB_POSTGRESQL_STD_AFX_H__


//
// namespace
//

/// kome project namespace
namespace kome {
	/// DB namespace
	namespace db {
		/// postgresql namespace
		namespace postgresql {
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
	#pragma comment( lib, "DataObjects" )
	#pragma comment( lib, "ImageManager" )
	#pragma comment( lib, "PluginTools" )
	#pragma comment( lib, "DbTools" )

	#pragma comment(lib, "libpqdll.lib")

#endif	// _MSC_VER
//
// includes
//

#include "kome/CoreFunctions.h"
#include "kome/CoreToolkits.h"
#include "kome/DataObjects.h"
#include "kome/ImageManager.h"
#include "kome/PluginTools.h"
#include "kome/DbTools.h"

//--- libpq header file for PostgreSQL ---
#include "postgres_ext.h"
#include "libpq-fe.h"



#endif	// __KOME_DB_POSTGRESQL_STD_AFX_H__
