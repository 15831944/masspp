/**
 * @file mspptest.cpp
 * @brief implements of mspptest functions
 *
 * @author S.Tanaka
 * @date 2014.04.01
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "mspptest.h"

#include "BatchInfo.h"

#include <signal.h>
#include <vector>
#include <list>
#include <string>

#ifdef _MSC_VER
	#include <windows.h>
#endif		// _MSC_VER

         

#include <gtest/gtest.h>

    

using namespace kome::test;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// main function
int main( int argc, char** argv ) {
	// variables
	int ret = 0;

#ifdef _MSC_VER
	// error mode
	SetErrorMode( SEM_NOGPFAULTERRORBOX );
#endif		//  _MSC_VER

	try {
		// initialize
		if( !init( argc, argv ) ) {
			ret = 1;
		}

		// run test
		testing::InitGoogleTest( &argc, argv );
		RUN_ALL_TESTS();
	}
	catch( ... ) {
		ret = 1;
	}

	// finalize
	finalize();

	return ret;
}

// initialize
bool init( int argc, char** argv ) {
	// set arguments
	setarguments( argc, argv );



	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// get iniFile
	std::string confDir = msppMgr.getConfDir();
	std::string iniFile = getabsolutepath( confDir.c_str(), INI_FILE );
	kome::core::IniFile* ini = kome::core::IniFile::getIniFile( iniFile.c_str() );
	msppMgr.setIniFile( ini );

	if( ini != NULL ) {
		for( unsigned int i = 0; i < ini->getNumberOfSections(); i++ ) {
			std::string section = ini->getSection( i );
			for( unsigned int j = 0; j < ini->getNumberOfParameters( section.c_str() ); j++ ) {
				std::string key = ini->getParameterName( section.c_str(), j );
				std::string val = kome::plugin::SettingsValue::convertToDisplayingString(
					ini->getParameterValue( section.c_str(), j )
				);

				ini->setString( section.c_str(), key.c_str(), val.c_str() );
			}
		}
	}			

	// get parameters
	std::string paramsFile = getabsolutepath( confDir.c_str(), PARAMS_FILE );
	kome::core::IniFile* params = kome::core::IniFile::getIniFile( paramsFile.c_str() );
	msppMgr.setParameters( params );
	
	// initialize socket
#ifdef _MSC_VER
	WSADATA data;
	WSAStartup( MAKEWORD(2, 0), &data );
#endif		// _MSC_VER

	// initialize xerces
	if( !kome::xml::XercesTool::initXml() ) {
		LOG_FATAL( FMT( "Failed to initialize XML." ) );
		return false;
	}

	// get plugin
	if( !loadPluginFiles() ) {
		LOG_FATAL( FMT( "Failed to load plug-in files." ) );
		return false;
	}

	// init plug-in
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// load
	for( unsigned int i = 0; i < plgMgr.getNumberOfPlugins(); i++ ) {
		kome::plugin::PluginInfo* plugin = plgMgr.getPlugin( i );
		bool flg = true;

		for( unsigned int j = 0; j < plugin->getNumberOfCalls() && flg; j++ ) {
			kome::plugin::PluginCall* call = plugin->getCall( j );

			const char* className = call->getClassName();
			std::string s = NVL( className, "" );

			if( !s.empty() ) {
				flg = false;
			}
		}

		if( flg ) {
			std::string dllPath = getabsolutepath( plugin->getDir(), plugin->getLink() );
			printf( "Loading: %s\n", dllPath.c_str() );
			
			plgMgr.openLink( dllPath.c_str() );
		}
	}

	// signal
	signal( SIGINT, onSignal );

	return true;
}

// finalize
void finalize() {
	// clean up socket
#ifdef _MSC_VER
	WSACleanup(); 
#endif		// _MSC_VER

	// deallocate error strings object
	errorcleanup();

	// terminate XML
	kome::xml::XercesTool::terminateXml();
}

// load plug-in files
bool loadPluginFiles() {
	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::CommandManager& comMgr = kome::plugin::CommandManager::getInstance();

	// get module path
	std::string batchDir = msppMgr.getMsppDir();

	// get plug-in path
	std::string pluginPath = getpath( batchDir.c_str(), PLUGIN_DIR );

	// set XML schema
	std::string pluginXsd = getpath( batchDir.c_str(), PLUGIN_XML_SCHEMA_FILE );
	std::string settingsXsd = getpath( batchDir.c_str(), SETTINGS_XML_SCHEMA_FILE );

	plgMgr.setXMLSchema( pluginXsd.c_str(), settingsXsd.c_str() );

	// load plugins
	if( !plgMgr.loadPath( pluginPath.c_str(), isPluginPath ) ) {
		return false;
	}

	std::vector< std::string > strPaths;
	strPaths = 	msppMgr.getPluginDir();
	
	for( unsigned int i=0; i < strPaths.size(); i++ ){
		// plug-in path
		std::string pluginPath = strPaths[i].c_str();
		
		// load plugins
		if( !plgMgr.loadPath( pluginPath.c_str(), isPluginPath ) ) {
			return false;
		}
	}

	for( unsigned int i = 0; i < comMgr.getNumberOfPluginPaths(); i++ ) {
		if( !plgMgr.loadPath( comMgr.getPluginPath( i ), isPluginPath ) ) {
			return false;
		}
	}

	return true;
}

// plug-in file or not
bool isPluginPath( const char* path ) {
	// get child
	std::string child = getfilename( path );
	if( child.empty() ) {
		return false;
	}

	// check
	if( isdirectory( path ) ) {	// directory
		return ( child.at( 0 ) != '.' );
	}

	return ( child.compare( PLUGIN_XML_FILE ) == 0 );
}

// on signal
void onSignal( int ) {
	// message
	printf( "\n\nOuch!!!\n" );
	LOG_INFO_CODE( FMT( "Receiving Signal" ) , ERR_OTHER );

	// finalize
	finalize();

	// exit
	exit( 1 );
}
