/**
 * @file GcmsSampleSet.h
 * @brief interface of AIASampleSet class
 *
 * @author Y.Fujita
 * @date 2012.07.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#include "stdafx.h"
#include "AIAManager.h"

#include "AIASampleSet.h"
#include "AIASample.h"


using namespace kome::io::aia;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DLL_NAME	   "QGAiaCvt.dll"
#define FUNC_NAME	  "GCMSAiaCvt"

#define PLUGIN_NAME	"Shimadzu GCMS Input"


// constructor
AIAManager::AIAManager() {
	// coinitialize
	CoInitialize( NULL );

/*	// plug-in information
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginInfo* plugin = plgMgr.getPlugin( PLUGIN_NAME );

	// get dll path
	std::string dllFile = DLL_NAME;
	if( plugin != NULL ) {
		dllFile = getpath( plugin->getDir(), DLL_NAME );
	}

	// load library
	m_module = LoadLibraryEx( dllFile.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
	LOG_INFO( dllFile );

	// get function
	m_fun = NULL;
	if( m_module != NULL ) {
		m_fun = (ConvertFun)GetProcAddress( m_module, FUNC_NAME );
	}

	// error message
	if( m_fun == NULL ) {
		// error code
		DWORD err = GetLastError();

		// error message
		LPVOID errBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)( &errBuf ),
			0,
			NULL
		);

		// log
		std::string msg;
		if( m_module == NULL ) {
			msg = FMT( "It failed to open dll. [%s]", DLL_NAME );
		}
		else {
			msg = FMT( "It failed to get function. [%s]", FUNC_NAME );
		}
		LOG_ERROR(
			FMT(
				"%s  : Error(code=%x): %s",
				msg.c_str(),
				err,
				errBuf
			)
		);

		// free
		LocalFree( errBuf );
	}  */
}

// destructor
AIAManager::~AIAManager() {
	if( m_module != NULL ) {
		FreeLibrary( m_module );
	}

	// co uninitialize
	CoUninitialize();
}

// convert
void AIAManager::convert( AIASampleSet* sampleSet ) {
	// check members
//	if( m_module == NULL ) {
//		return;
//	}
//	if( m_fun == NULL ) {
//		return;
//	}

	// get tmp file path
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	std::string cdfName = msppMgr.getTmpFileName( sampleSet->getFileName(), ".cdf" );
	std::string cdfPath = getpath( msppMgr.getTmpDir(), cdfName.c_str() );
	LOG_DEBUG( FMT( "GCMS Convert [%s -> %s]", sampleSet->getFilePath(), cdfPath.c_str() ) );
	
/*	// convert
	if( (*m_fun)( const_cast<char*>( sampleSet->getFilePath() ), const_cast<char*>( cdfPath.c_str() ) ) != 0 ) {
		LOG_ERROR( FMT( "It failed to convert QGD file to CDF format." ) );
		return;
	}
*/
	// set file path
	sampleSet->setCdfFilePath( cdfPath.c_str() );
}

// get object
AIAManager& AIAManager::getInstance() {
	// create object (This is the only object.)
	static AIAManager mgr;

	return mgr;
}
