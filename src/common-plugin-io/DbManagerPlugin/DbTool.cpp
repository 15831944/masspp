/**
 * @file DbTool.cpp
 * @brief implements of DbTool class
 *
 * @author S.Tanaka
 * @date 2008.12.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DbTool.h"


using namespace kome::io::db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// get DB
kome::db::Database* DbTool::getDb() {
	// ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// get DB function
	const char* dbName = ini->getString( SECTION, DB_PARAM_KEY, NULL );
	kome::plugin::PluginFunctionItem* dbFun = plgMgr.getFunctionItem( DB_FUNCTION_TYPE, NVL( dbName, "sqlite" ) );
	if( dbFun == NULL ) {
		return NULL;
	}

	// settings
	kome::objects::SettingParameterValues settings;

	kome::plugin::SettingsPage* page = dbFun->getSettingsPage();
	if( page != NULL ) {
		page->setParameters( settings, NULL );
	}

	// DB
	kome::db::Database* db = getDb( dbFun, &settings );

	return db;
}

// get first DB
kome::db::Database* DbTool::getFirstDb() {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// getting DB function
	kome::plugin::PluginFunctionItem* dbFun = plgMgr.getFunctionItem( DB_FUNCTION_TYPE, (unsigned int)0 );
	if( dbFun == NULL ) {
		return NULL;
	}

	// settings
	kome::objects::SettingParameterValues settings;

	kome::plugin::SettingsPage* page = dbFun->getSettingsPage();
	if( page != NULL ) {
		page->setParameters( settings, NULL );
	}

	// DB
	kome::db::Database* db = getDb( dbFun, &settings );

	return db;
}

// set DB
void DbTool::setDb( kome::db::Database* db ) {
	// manager
	kome::db::DbManager& dbMgr = kome::db::DbManager::getInstance();

	// current db
	kome::db::Database* oldDb = dbMgr.getDb();
	if( db == oldDb ) {
		return;
	}

	// close
	if( oldDb != NULL ) {
		oldDb->close();
		delete oldDb;
	}

	// set DB
	dbMgr.setDb( db );
}

// get DB
kome::db::Database* DbTool::getDb(
		kome::plugin::PluginFunctionItem* dbGetter,
		kome::objects::SettingParameterValues* settings
) {
	// check the parameter
	if( dbGetter == NULL ) {
		return NULL;
	}

	// parameters
	kome::objects::Parameters params;
	if( settings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params, *settings );
	}

	// get DB
	kome::db::Database* db = (kome::db::Database*)dbGetter->getCall()->invoke( &params ).prim.pt;
	return db;
}
