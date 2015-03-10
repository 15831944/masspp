/**
 * @file XTandemManager.cpp
 * @brief implements of XTandemManager class
 *
 * @author N.Tashiro
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "XTandemManager.h"
#include "XTandemXMLManager.h"
#include "XTandemXMLHandler.h"

#include <string>
#include <fstream>
#include <boost/filesystem.hpp>


using namespace kome::xtandem;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::ident;


// constructor
XTandemManager::XTandemManager() {
}

// destructor
XTandemManager::~XTandemManager() {
}

// get the X! Tandem search engine
TandemSearchEngine* XTandemManager::getEngine() {
	return &m_engine;
}

// get result directory
const char* XTandemManager::getResultDir() {
	// make the directory
	if( m_resultDir.empty() ) {
		// manager
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

		std::string identDir = getpath( msppMgr.getConfDir(), "search_engine" );
		if( !fileexists( identDir.c_str() ) ) {
			makedirectory( identDir.c_str() );
		}

		m_resultDir = getpath( identDir.c_str(), "tandem" );
		if( !fileexists( m_resultDir.c_str() ) ) {
			makedirectory( m_resultDir.c_str() );
		}
	}

	return m_resultDir.c_str();
}

// get residues
void XTandemManager::getResiduesFiles( std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// residues files
	std::set< std::string > fileSet;
	for( unsigned int i = 0; i < identMgr.getNumberOfLocalDbs(); i++ ) {
		std::string path = identMgr.getResiduesFile( i );
		if( fileSet.find( path ) == fileSet.end() ) {
			fileSet.insert( path );
			enumeration->resize( enumeration->size() + 1 );
			enumeration->back().name = getfilename( path.c_str() );
			enumeration->back().value = path;
		}
	}		
}

// get instance
XTandemManager& XTandemManager::getInstance() {
	// create object (This is the only object.)
	static XTandemManager mgr;

	return mgr;
}
