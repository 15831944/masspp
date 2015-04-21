/**
 * @file DbTools.h
 * @brief header file to use DbTools library
 *
 * @author S.Tanaka
 * @date 2008.08.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_TOOLS_H__
#define __KOME_DB_TOOLS_H__


//
// macros
// 

#ifdef _MSC_VER
	#define DB_TOOLS_CLASS __declspec(dllimport)
#else
	#define DB_TOOLS_CLASS
#endif	// _MSC_VER


//
// includes
//


#include "DbTools/Table.h"
#include "DbTools/TableColumn.h"
#include "DbTools/TableIndex.h"

#include "DbTools/Database.h"
#include "DbTools/Statement.h"
#include "DbTools/ResultSet.h"

#include "DbTools/DbManager.h"


#endif //  __KOME_DB_TOOLS_H__
