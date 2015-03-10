/**
 * @file msppcmd.cpp
 * @brief implements of msppcmd functions
 *
 * @author S.Tanaka
 * @date 2006.11.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "msppcmd.h"

#include "BatchInfo.h"

#include <signal.h>
#include <vector>
#include <list>
#include <string>

#ifdef _MSC_VER
	#include <windows.h>
	#using <mscorlib.dll>
#endif		// _MSC_VER


#define PLUGIN_INIT_METHOD_TYPE_NAME	  "INIT"
#define PLUGIN_EXIT_METHOD_TYPE_NAME	  "EXIT"
#define PLUGIN_BATCH_TYPE_NAME			"BATCH"


using namespace kome::batch;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#ifdef _MSC_VER
[System::STAThread]
#endif		// _MSC_VER

// main function
int main( int argc, char** argv ) {
	// variables
	int ret = 0;
	bool optionFailure = false;
	bool executed = false;

#ifdef _MSC_VER
	// error mode
	SetErrorMode( SEM_NOGPFAULTERRORBOX );
#endif		//  _MSC_VER

	try {
		// initialize
		if( !init( argc, argv ) ) {
			ret = 1;
		}

		// gets batch manager
		kome::plugin::CommandManager& mgr = kome::plugin::CommandManager::getInstance();
		mgr.init( PLUGIN_BATCH_TYPE_NAME );

		// parse option
		if( ret == 0 && !mgr.parseOption( argc, argv ) ) {
			optionFailure = true;
			ret = 1;
		}

		// execute
		if( ret == 0 ) {
			if( mgr.empty() ) {
				printf( "%s", mgr.getUsage() );
			}
			else {
				executed = true;
				if( !mgr.execute() ) {
					ret = 1;
				}				
			}
		}
	}
	catch( ... ) {
		ret = 1;
	}

	// After calling getlasterrormessage once, the error is deleted.
	// We want to use it twice, so make a copy first.
	std::string lasterrormessage = getlasterrormessage();

	// log
	if( ret == 0 ) {
		LOG_INFO( FMT( "The batch executed successfully." ) );
	}
	else {
		LOG_ERROR( FMT( "Failed to execute batch." ) );
		fprintf( stderr, "%s\n", lasterrormessage.c_str() );

		if( optionFailure ) {
			// get module file name
			std::string prog = getfilename( getmodulefile().c_str() );
			fprintf( stderr, "Type '%s --help' for usage.\n", prog.c_str() );
		}
	}

	// finalize
	finalize();

	// last message
	if( executed ) {
		if( ret == 0 ) {
			printf( "\nComplete!\n" );
		}
		else {
			printf( "\nError! : %s\n", lasterrormessage.c_str() );
		}
	}

	return ret;
}

// initialize
bool init( int argc, char** argv ) {
	// set arguments
	setarguments( argc, argv );

	// log
	setloglevel( MAX_LOG_LEVEL + 1 );
	setloglistsize( -1 );

	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// common dir
	std::string batchDir = msppMgr.getMsppDir();
	std::string confDir = msppMgr.getConfDir();

	// get iniFile
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
	plgMgr.callAllFunctions( NULL, PLUGIN_INIT_METHOD_TYPE_NAME );

	// log
	setloglistsize( 0 );
	if( getloglevel() > MAX_LOG_LEVEL ) {
		setloglevel( LOG_LEVEL_NONE );
	}

	// signal
	signal( SIGINT, onSignal );

	return true;
}

// finalize
void finalize() {
	// save ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// exit plug-in
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	plgMgr.callAllFunctions( NULL, PLUGIN_EXIT_METHOD_TYPE_NAME );

	// clean up socket
#ifdef _MSC_VER
	WSACleanup(); 
#endif		// _MSC_VER

	// deallocate error strings object
	errorcleanup();			// Spec No.85853 @date 2012.04.26 <Add> M.Izumi

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
