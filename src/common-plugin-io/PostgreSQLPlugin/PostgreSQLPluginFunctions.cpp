/**
 * @file PostgreSQLPluginFunctions.cpp
 * @brief interfaces of PostgreSQLOutput plug-in function
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "PostgreSQLPluginFunctions.h"
#include <string>
#include <map>
#include "PostgreSQLManager.h"


using namespace kome::db::postgresql;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define POSTGRE_SECTION			"PostgreSQL"
#define POSTGRE_DB_PARAM0_KEY	   "COMMON_DB_SERVER"
#define POSTGRE_DB_PARAM0_NAME	  "server"
#define POSTGRE_DB_PARAM1_KEY	   "COMMON_DB_PORT"
#define POSTGRE_DB_PARAM1_NAME	  "port"
#define POSTGRE_DB_PARAM2_KEY	   "COMMON_DB_NAME"
#define POSTGRE_DB_PARAM2_NAME	  "name"
#define POSTGRE_DB_PARAM3_KEY	   "COMMON_DB_USER"
#define POSTGRE_DB_PARAM3_NAME	  "user"
#define POSTGRE_DB_PARAM4_KEY	   "COMMON_DB_PASSWORD"
#define POSTGRE_DB_PARAM4_NAME	  "password"

// get PostgreSQL DB object
kome::objects::Variant getPostgreSQLDatabaseObject( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;
	// ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// file path
	//std::string filePath;
	std::string address;
	std::string user;
	std::string password;
	std::string dbname;
	int port;

	if( dbname.empty() && ini != NULL ) {
		dbname = ini->getString( POSTGRE_SECTION, POSTGRE_DB_PARAM2_KEY, "" );
	}
	address = ini->getString( POSTGRE_SECTION, POSTGRE_DB_PARAM0_KEY, "" );

	port = atoi(ini->getString( POSTGRE_SECTION, POSTGRE_DB_PARAM1_KEY, "" ));

	if( user.empty() && ini != NULL ) {
		user = ini->getString( POSTGRE_SECTION, POSTGRE_DB_PARAM3_KEY, "" );
	}

	if( password.empty() && ini != NULL ) {
		password = ini->getString( POSTGRE_SECTION, POSTGRE_DB_PARAM4_KEY, "" );
	}

	PostgreSQLManager& mgr = PostgreSQLManager::getInstance();
	ret.prim.pt = mgr.getDb( address.c_str(),port,dbname.c_str(),user.c_str(),password.c_str() );

	return ret;
}
