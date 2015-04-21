/**
 * @file ResourceBundle.cpp
 * @brief implements of ResourceBundle class
 *
 * @author S.Tanaka
 * @date 2009.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ResourceBundle.h"

#include "MsppManager.h"


#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ResourceBundle::ResourceBundle() {
}

// destructor
ResourceBundle::~ResourceBundle() {
}

// search resource
std::string ResourceBundle::searchResource( const char* name, const char* module, const char* ext ) {
	// manager
	MsppManager& msppMgr = MsppManager::getInstance();

	// return object
//	std::string res = searchResource( msppMgr.getMsppDir(), name, module, ext );
	std::string res = searchResource( getplugindir().c_str(),  name, module, ext ); // @date 2013.06.25 <Mod> M.Izumi

	return res;
}

std::string ResourceBundle::searchResource(
		const char* path,
		const char* name,
		const char* module,
		const char* ext
) {
	// return object
	std::string res;

	// create string object
	std::string n = tolowercase( name );
	std::string m = tolowercase( module );
	std::string e = tolowercase( ext );

	if( path == NULL || n.empty() ) {
		return res;
	}

	// path object
	boost::filesystem::path p( path, boost::filesystem::native );
	boost::filesystem::directory_iterator end;

	// check path
	if( !boost::filesystem::exists( p ) ) {
		return res;
	}

	// search
	if( boost::filesystem::is_directory( p ) ) {	// directory
		for( boost::filesystem::directory_iterator it( p ); it != end && res.empty(); it++ ) {
			boost::filesystem::path child = boost::filesystem::absolute( *it );
			res = searchResource( child.string().c_str(), name, module, ext );
		}
	}
	else {	// file
		std::string absPath = boost::filesystem::absolute( p ).string();

		std::string fileName = tolowercase( getfilename( absPath.c_str() ).c_str() );
		std::string fileExt = tolowercase( getext( absPath.c_str() ).c_str() );
		std::string fileDir = tolowercase( getdir( absPath.c_str() ).c_str() );

		// check
		if( fileName.find( n ) != fileName.npos
				&& ( m.empty() || fileDir.find( m ) != fileDir.npos )
				&& ( e.empty() || fileExt.compare( e ) == 0 ) ) {
			res = absPath;
		}
	}

	return res;
}

// get instance
ResourceBundle& ResourceBundle::getInstance() {
	// create instance (This is the only object.)
	static ResourceBundle rb;

	return rb;
}
