/**
 * @file LogViewFunctions.cpp
 * @brief implements of log view plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.01.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LogViewFunctions.h"
#include "LogViewManager.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define LOG_SECTION				"log"
#define FILE_LEVEL_KEY			"LOG_FILE_LEVEL"
#define VIEW_LEVEL_KEY			"LOG_VIEW_LEVEL"
#define LOG_VIEW_STATUS_NAME	"LOG_VIEW"


// initialize log
kome::objects::Variant initLog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// log view manager
	LogViewManager& mgr = LogViewManager::getInstance();

	// init file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// level
	int level = LOG_LEVEL_WARN;
	if( ini != NULL ) {
		level = ini->getInt( LOG_SECTION, FILE_LEVEL_KEY, LOG_LEVEL_WARN );
	}

	// set
	mgr.setFileLogLevel( level );
 
	// set function
	mgr.setFunction();

	return ret;
}

// initialize view
kome::objects::Variant initView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// log view manager
	LogViewManager& mgr = LogViewManager::getInstance();

	// init file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// get view status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* statusVal = statusMgr.getValue( LOG_VIEW_STATUS_NAME );
	if( statusVal != NULL && strcmp( statusVal, "on" ) == 0 ) {
		// create view
		if( !mgr.hasView() ) {
			mgr.createLogView();
		}

		// set level
		int level = LOG_LEVEL_WARN;
		if( ini != NULL ) {
			level = ini->getInt( LOG_SECTION, VIEW_LEVEL_KEY, LOG_LEVEL_WARN );
		}
		mgr.setViewLogLevel( level );
	}

	// set function
	mgr.setFunction();

	return ret;
}

// finalize log
kome::objects::Variant finalizeLog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// log manager
	LogViewManager& mgr = LogViewManager::getInstance();

	// close log
	mgr.closeLog();

	return ret;
}

// finalize view
kome::objects::Variant finalizeView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// log manager
	LogViewManager& mgr = LogViewManager::getInstance();

	// status manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// set status
	statusMgr.setValue(
		LOG_VIEW_STATUS_NAME,
		( mgr.isVisibleView() ? "on" : "off" )
	);

	// release viewclose log
	mgr.releaseLogView();

	return ret;
}

// toggle the log view
kome::objects::Variant toggleLogView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// log view manager
	LogViewManager& mgr = LogViewManager::getInstance();

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// create log view
	if( !mgr.hasView() ) {
		statusMgr.setValue( LOG_VIEW_STATUS_NAME, "on" );
		initView( NULL );

		return ret;
	}

	// toggle visibility
	mgr.toggleView();

	return ret;
}

// check the visibility of the log view
kome::objects::Variant isVisibleLogView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get visibility
	LogViewManager& mgr = LogViewManager::getInstance();
	ret.prim.boolVal = mgr.isVisibleView();

	return ret;
}

// on settings
kome::objects::Variant onUpdateSettings( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// get ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return ret;
	}

	// log manager
	LogViewManager& mgr = LogViewManager::getInstance();

	// level
	int level = ini->getInt( LOG_SECTION, FILE_LEVEL_KEY, LOG_LEVEL_INFO );
	mgr.setFileLogLevel( level );

	level = ini->getInt( LOG_SECTION, VIEW_LEVEL_KEY, LOG_LEVEL_WARN );
	mgr.setViewLogLevel( level );

	return ret;
}
