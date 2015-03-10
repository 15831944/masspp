/**
 * @file CommandLineBatchService.cpp
 * @brief implementation of plug-in functions
 *
 * @author satstnka
 * @date 2012.03.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "CommandLineBatchService.h"

#include <stdlib.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


using namespace kome::batchservice::command;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define FINISHED_STRING           "FINISHED!!!"

#ifdef _MSC_VER
    #define strtoll _strtoi64
#endif    // _MSC_VER


//
////// CommandLineBatchService
//

// constructor
CommandLineBatchService::CommandLineBatchService() {
}

// destructor
CommandLineBatchService::~CommandLineBatchService() {
}

// delete files
void CommandLineBatchService::deleteFiles() {
	// command dir
	std::string commandDir = getCommandDir();

	// current time
	long long t = getcurrenttime() - 1000 * 60 * 60 * 24 * 7;    // 1 week

	// get remove files
	std::vector< std::string > files;
	boost::filesystem::path p( commandDir.c_str(), boost::filesystem::native );
	boost::filesystem::directory_iterator end;
	for( boost::filesystem::directory_iterator it( p ); it != end; it++ ) {
		std::string fileName = (*it).path().leaf().string();
		std::string baseName = fileName;
		std::string::size_type pos = baseName.find( "." );
		if( pos != baseName.npos ) {
			baseName = baseName.substr( 0, pos );
		}

		char* endptr = NULL;
		long long tmp = strtoll( baseName.c_str(), &endptr, 10 );
		if( endptr != NULL && *endptr != '\0' ) {
			tmp = -1;
		}

		if( tmp < t ) {
			std::string path = getpath( commandDir.c_str(), fileName.c_str() );
			files.push_back( path );
		}
	}

	// delete files
	for( unsigned int i = 0; i < files.size(); i++ ) {
		removefile( files[ i ].c_str() );
	}
}

// on execute
std::string CommandLineBatchService::onExecute( kome::plugin::BatchInfo* ptrBatchInfo ) {
	// return value
	std::string ret;

	// check the parameter
	if( ptrBatchInfo == NULL ) {
		return ret;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// command directory
	std::string commandDir = getCommandDir();
	if( commandDir.empty() ) {
		return ret;
	}
	
	// batch file
	long long t = getcurrenttime() - 1;
	std::string name;
	std::string path;

	do {
		t++;
		name = FMT( "%lld", t );
		path = getpath( commandDir.c_str(), name.c_str() );
	} while( fileexists( path.c_str() ) );

	// job file
	FILE* fp = fileopen( path.c_str(), "w" );
	if( fp == NULL ) {
		return ret;
	}
	fclose( fp );

	// error file
	std::string errorFile = FMT( "%s.error", path.c_str() );

	// batch file
	std::string batchFile = FMT( "%s.bat", path.c_str() );
	fp = fileopen( batchFile.c_str(), "w" );
	if( fp == NULL ) {
		return ret;
	}

	std::string binDir = getpath( msppMgr.getMsppDir(), "bin" );
	std::string baseCommand = getpath( binDir.c_str(), "msppcmd.exe" );
	baseCommand = FMT( "\"%s\"", baseCommand.c_str() );
	for( int i = 0; i < ptrBatchInfo->getNumberOfBatchFunctions(); i++ ) {
		kome::plugin::PluginFunctionItem* item = ptrBatchInfo->getBatchFunction( i );
		kome::objects::SettingParameterValues* settings = ptrBatchInfo->getBatchParameter( i );

		if( item != NULL ) {
			baseCommand.append( FMT( " -%s", item->getShortName() ) );
			kome::plugin::SettingsPage* page = item->getSettingsPage();
			if( page != NULL && settings != NULL ) {
				for( unsigned int j = 0; j < page->getNumberOfForms(); j++ ) {
					kome::plugin::SettingsForm* form = page->getForm( j );
					kome::plugin::SettingsParam* param = ( form == NULL ? NULL : form->getParam() );
					kome::plugin::SettingsValue* val = ( param == NULL ? NULL : param->getValue() );

					std::string v;

					bool flg = true;

					if( param != NULL ) {
						const char* tmp = settings->getParameterValue( param->getName() );
						v = NVL( tmp, ( val == NULL ? "" : val->getDefaultValue() ) );
					}
					if( v.empty() ) {
						v = "none";
					}
					else {
						if( val->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {
							kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( val->getEnumKey(), v.c_str() );
							kome::plugin::SettingsPage* subPage = ( item == NULL ? NULL : item->getSettingsPage() );
							if( subPage != NULL ) {
								kome::objects::SettingParameterValues* subSettings = settings->getSubParameterValues( param->getName(), item->getShortName() );
								std::string subVal;
								if( subSettings != NULL ) {
									subVal = subPage->getParameterString( *subSettings );
								}

								if( !subVal.empty() ) {
									v.append( subVal );
									flg = ( subVal.find( "\"" ) == subVal.npos );
								}
							}
						}
					}

					if( flg ) {
						baseCommand.append( FMT( " \"%s\"", v.c_str() ) );
					}
					else {
						baseCommand.append( FMT( " %s", v.c_str() ) );
					}
				}
			}
		}
	}

	for( int i = 0; i < ptrBatchInfo->getNumberOfInputFiles(); i++ ) {
		std::string command = FMT( "%s -in \"%s\" \"%s\""
			, baseCommand.c_str()
			, ptrBatchInfo->getDataTypeIO( i, 0 ).c_str()
			, ptrBatchInfo->getInputFile( i ).c_str() );	// @date 2012/08/29 <Add> OKADA

		for( int j = 0; j < ptrBatchInfo->getNumberOfSaveFunctions(); j++ ) {
			kome::plugin::PluginFunctionItem* item = ptrBatchInfo->getSaveFunction( j );
			if( item != NULL ) {
				command.append( FMT( " -out \"%s\" \"%s\""
					, ptrBatchInfo->getSaveFunctionString( j )	// @date 2013/05/21 <Add> OKADA
					, ptrBatchInfo->getOutputFile( i, j ).c_str() ) );
			}
		}
		fprintf( fp, "%s 2>> \"%s\"\n", command.c_str(), errorFile.c_str() );
	}

	std::string finishStr = FMT( "%s (%s)", FINISHED_STRING, name.c_str() );
	fprintf( fp, "echo %s > \"%s\"\n", finishStr.c_str(), path.c_str() );

	fflush( fp );
	fclose( fp );

	// execute
	std::string processFile = FMT( "%s.process", path.c_str() );
	std::string batchCommand = FMT( "\"%s\"", batchFile.c_str() );
	long pid = wxExecute( batchCommand.c_str(), wxEXEC_ASYNC );
	if( pid <= 0 ) {
		ret = "";
	}
	else {
		ret = name;
		fp = fileopen( processFile.c_str(), "wb" );
		fwrite( &pid, sizeof( long ), 1, fp );
		fflush( fp );
		fclose( fp );
	}

	return ret;
}

// on get status
kome::plugin::BatchService::BatchStatus CommandLineBatchService::onGetStatus( const char* str ) {
	// delete files
	deleteFiles();

	// status
    BatchStatus status = STATUS_WAITING;

	// string
	std::string s = NVL( str, "" );
	if( s.empty() ) {
		status = kome::plugin::BatchService::STATUS_ERROR;
	}
	else {
		std::string commandDir = getCommandDir();
		std::string jobFile = getpath( commandDir.c_str(), s.c_str() );
		std::string batFile = FMT( "%s.bat", jobFile.c_str() );
		std::string errFile = FMT( "%s.error", jobFile.c_str() );
		std::string processFile = FMT( "%s.process", jobFile.c_str() );
		std::string finishStr = FMT( "%s (%s)", FINISHED_STRING, s.c_str() );

		if( fileexists( jobFile.c_str() ) ) {
			FILE* fp = fileopen( jobFile.c_str(), "r" );
			char line[ 4096 ];
			int size = 0;
			bool flg = false;
			while( fgets( line, 4096, fp ) != NULL && !flg ) {
				if( strstr( line, finishStr.c_str() ) != NULL ) {
					flg = true;
				}
			}
			fclose( fp );

			if( flg ) {
				fp = fileopen( errFile.c_str(), "r" );
				int errSize = 0;
				char buff[ 1024 ];
				if( fp != NULL ) {
					int tmpSize = 0;
					while( ( tmpSize = fread( buff, 1, 1024, fp ) ) > 0 ) {
						errSize += tmpSize;
					}
					fclose( fp );
				}

				status = ( errSize > 0 ? STATUS_ERROR : STATUS_COMPLETED );
			}
			else {
				status = STATUS_RUNNING;

				fp = fileopen( processFile.c_str(), "rb" );
				if( fp == NULL ) {
					status = STATUS_ERROR;
				}
				else {
					long pid = -1;
					if( fread( &pid, sizeof( long ), 1, fp ) <= 0 ) {
						pid = -1;
					}
					fclose( fp );

					if( pid < 0 ) {
						status = STATUS_ERROR;
					}
					else {
						if( processexists( pid ) ) {
							status = STATUS_RUNNING;
						}
						else {
							status = STATUS_ERROR;
						}
					}
				}
			}
		}
		else {
			status = STATUS_COMPLETED;
		}
	}

	return status;
}

// get command directory
std::string CommandLineBatchService::getCommandDir() {
	// return value
	std::string dir;

	// tmp dir
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	std::string batchDir = getpath( msppMgr.getConfDir(), "batch" );
	if( !fileexists( batchDir.c_str() ) ) {
		if( !makedirectory( batchDir.c_str() ) ) {
			return dir;
		}
	}

	std::string commandDir = getpath( batchDir.c_str(), "command" );
	if( !fileexists( commandDir.c_str() ) ) {
		if( !makedirectory( commandDir.c_str() ) ) {
			return dir;
		}
	}

	dir = commandDir;
	return dir;
}


//
////// CommandLineBatchServiceManager
//


// constructor
CommandLineBatchServiceManager::CommandLineBatchServiceManager() {
}

// destructor
CommandLineBatchServiceManager::~CommandLineBatchServiceManager() {
}

// get service
CommandLineBatchService* CommandLineBatchServiceManager::getService() {
	return &m_service;
}

// get instance
CommandLineBatchServiceManager& CommandLineBatchServiceManager::getInstance() {
	// make the object (This is the only object.)
	static CommandLineBatchServiceManager mgr;

	return mgr;
}
