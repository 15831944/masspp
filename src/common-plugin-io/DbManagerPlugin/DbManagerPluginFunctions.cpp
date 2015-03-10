/**
 * @file DbManagerPluginFunctions.cpp
 * @brief implements of DbManager plug-in function
 *
 * @author S.Tanaka
 * @date 2008.12.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DbManagerPluginFunctions.h"
#include "DbTool.h"
#include "DbDialog.h"


using namespace kome::io::db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// prepare DB
kome::objects::Variant prepareDb( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// get DB
	kome::db::Database* db = DbTool::getDb();
	if( db == NULL ) {
		db = DbTool::getFirstDb();
	}

	// set DB
	DbTool::setDb( db );

	return ret;
}

// check the database schema
kome::objects::Variant checkDatabaseSchema( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// database
	kome::db::DbManager& dbMgr = kome::db::DbManager::getInstance();
	kome::db::Database* db = dbMgr.getDb();
	if( db == NULL ) {
		return ret;
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	int version = identMgr.getDbVersion();
	int currentVersion = identMgr.getCurrentDbVersion();

	if( currentVersion < 0 ) {
		std::string msg = "The specified Mass++ results database version is old.\n";
		msg.append( "Quantitation and Identification processing results cannot be stored in it.\n" );
		msg.append( "You can create a new Mass++ results database from the database configuration dialog (File -> Database...).\n\n" );
		msg.append( "If you do not create a new database, Mass++ functionality except for Quantitation and Identification may be used." );
		kome::window::WindowTool::showWarning( msg.c_str() );
	}
	else if( currentVersion > version ) {
		std::string msg = "This Mass++ version is too old to access the specified Mass++ results database.\n";
		msg.append( "Quantitation and Identification processing results cannot be stored in it.\n" );
		msg.append( "Upgrade Mass++ in order to use this existing database, or create a new Mass++ results database from the database configuration dialog (File -> Database...).\n\n" );
		msg.append( "If you do not upgrade Mass++ or create a new database, Mass++ functionality except for Quantitation and Identification may be used." );
		kome::window::WindowTool::showWarning( msg.c_str() );			
	}
	else if( currentVersion < version ) {
		std::string msg = "The specified Mass++ results database version is old.\n";
		msg.append( "Quantitation and Identification processing results cannot be stored in it.\n" );
		msg.append( "Do you want to convert to the new schema to use the existing database?\n\n" );
		msg.append( "(This operation may take several minutes.\n" );
		msg.append( "There is a risk that current data may be permanently destroyed.\n" );
		msg.append( "So a backup is recommended before this operation.)\n\n" );
		msg.append( "If you do not convert to the new schema, Mass++ functionality except for Quantitation and Identification may be used." );

		if( kome::window::WindowTool::showYesNo( msg.c_str(), "Convert anyway", "Don't convert" ) ) {
			wxBeginBusyCursor();
			kome::window::DialogProgress progress( NULL, "Converting Database" );
			identMgr.convertDbSchema( progress );
			wxEndBusyCursor();
		}
	}

	return ret;
}

// change DB
kome::objects::Variant changeDb( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// set DB
	kome::db::Database* db = DbTool::getDb();
	if( db == NULL ) {
		db = DbTool::getFirstDb();
	}

	DbTool::setDb( db );
	checkDatabaseSchema( NULL );

	return ret;
}

// open DB dialog
kome::objects::Variant openDbDialog( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// dialog
	DbDialog dlg( kome::window::WindowTool::getMainWindow() );

	// get DB
	if( dlg.ShowModal() == wxID_OK ) {
		kome::db::Database* db = DbTool::getDb( dlg.getDbGetter(), dlg.getDbSettings() );
		DbTool::setDb( db );
		checkDatabaseSchema( NULL );
	}

	return ret;
}

// select DB
kome::objects::Variant selectDb( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::db::DbManager& dbMgr = kome::db::DbManager::getInstance();

	// settings object
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get the parameters." ) );
		return ret;
	}

	// get DB
	const char* db = settings->getParameterValue( "database" );
	if( db == NULL ) {
		LOG_ERROR( FMT( "Failed to get the database type." ) );
		return ret;
	}
	kome::objects::SettingParameterValues* dbSettings = settings->getSubParameterValues( "database", db );

	// function
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "GET_DB", db );
	kome::plugin::SettingsPage* page = item->getSettingsPage();
	if( page != NULL && dbSettings->getNumberOfParameters() == 0 ) {
		page->setParameters( *dbSettings );
	}

	// get DB
	kome::objects::Parameters dbParams;
	kome::plugin::PluginCallTool::setSettingValues( dbParams, *dbSettings );

	kome::db::Database* database = ( kome::db::Database* )( item->getCall()->invoke( &dbParams ).prim.pt );
	if( database == NULL ) {
		LOG_ERROR( FMT( "Failed to get the database." ) );
		return ret;
	}
	dbMgr.setDb( database );

	ret.prim.boolVal = true;
	return ret;
}

// release DB
kome::objects::Variant releaseDb( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// manager
	kome::db::DbManager& mgr = kome::db::DbManager::getInstance();

	// release
	mgr.closeAllDb();
	DbTool::setDb( NULL );

	return ret;
}
