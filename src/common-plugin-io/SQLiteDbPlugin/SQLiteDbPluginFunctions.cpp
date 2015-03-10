/**
 * @file SQLiteOutputPluginFunctdbns.cpp
 * @brief implements of SQLiteOutput plug-in functdbn
 *
 * @author S.Tanaka
 * @date 2008.12.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SQLiteDbPluginFunctions.h"

#include "SQLiteDbManager.h"

#include <boost/filesystem.hpp>


using namespace kome::db::sqlite;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SECTION			"SQLite"
#define DB_PARAM_KEY	"COMMON_DB_FILE_PATH"
#define DB_PARAM_NAME	"db"


// get SQLite DB object
kome::objects::Variant getSQLiteDatabaseObject( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;

	// ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// file path
	std::string filePath;

	if( filePath.empty() && ini != NULL ) {
		filePath = ini->getString( SECTION, DB_PARAM_KEY, "" );
	}

	if( filePath.empty() ) {

		// If the filePath is empty, it means that this is the first time Mass++ 2.0.0
		// has been run on this PC. Need to decide and set a new .db file for the peak
		// matrix storage.
		// First of all, try to use the default dababase name specified in settings.xml

		kome::objects::SettingParameterValues* settings
			= kome::plugin::PluginCallTool::getSettingValues( params );
		if( settings != NULL ) {
			const char* fp = settings->getParameterValue( DB_PARAM_NAME );
			filePath = NVL( fp, "" );
		}

		// Check to see if a database with this name already exists.
		// If it already exists, must generate new names until we find one that doesn't
		// already exist. When it is decided, set the value in the INI file so that it
		// will be used from now on.
		std::string path;
		path = getabsolutepath( msppMgr.getConfDir(), filePath.c_str() );
		std::string tempString = path;
		char numString[10];
		int nCount = 0;
		while ( fileexists( tempString.c_str() ) )
		{
			tempString = path;
			int nDotPos = tempString.rfind('.');

			sprintf_s(numString, 10, "%d", nCount++);
			tempString.insert(nDotPos, numString);
		}

		filePath = tempString;
		ini->setString( SECTION, DB_PARAM_KEY, filePath.c_str() );
	}

	// get DB
	SQLiteDbManager& mgr = SQLiteDbManager::getInstance();
	ret.prim.pt = mgr.getDb( filePath.c_str() );

	return ret;
}

// check DB file
kome::objects::Variant checkDbFile( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// file path
	std::string filePath;
	if( ini != NULL ) {
		filePath = ini->getString( SECTION, DB_PARAM_KEY, "" );
	}
	if( filePath.empty() ) {
		return ret;
	}

	boost::filesystem::path p( filePath.c_str() );

	// file size
	long long pos = boost::filesystem::file_size( p );
	if( pos < 0x40000000 ) {
		return ret;
	}

	// message
	std::string message = " The file size of SQLite data is over 1GB.\n";
	message.append( " Changing the data file is recommended.\n" );
	message.append( " If you want to do it, specify the new db file with [File] > [Database]." );

	kome::window::MessageCheckDialog::openDialogAfterChecking(
		kome::window::WindowTool::getMainWindow(),
		"SQLite",
		message.c_str(),
		"SQLITE_DBFILE_CHECK"
	);

	return ret;
}
