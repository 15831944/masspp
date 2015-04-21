/**
 * @file FileFunctions.cpp
 * @brief implements of file system function.
 *
 * @author S.Tanaka
 * @date 2006.06.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"


#include "FileFunctions.h"

#include "LogFunctions.h"
#include "StringFunctions.h"
#include "CoreMacros.h"
#include "ErrorCode.h"

#include <vector>
#include <list>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>

#ifdef __unix
	#include <sys/stat.h>
#else
	#include <direct.h>
#endif //__unix


#ifndef _LARGEFILE_SOURCE
	#define _LARGEFILE_SOURCE
#endif //  _LARGEFILE_SOURCE


/** the array of arguments */
std::vector<std::string> g_arguments;

/** module file path */
std::string g_moduleFile;

/** home directory */
std::string g_homeDir;


// judge where file exists.
bool fileexists( const char* path ) {
	// check parameter
	if( path == NULL ) {
		return false;
	}

	// create path object
	boost::filesystem::path p( path, boost::filesystem::native );

	// judge
	bool ret = false;
	try{
		ret = boost::filesystem::exists( p );
	}
	catch( ... ) {
		ret = false;
	}

	return ret;
}

// judge where path is directory.
bool isdirectory( const char* path ) {
	// check parameter
	if( !fileexists( path ) ) {
		return false;
	}

	// create path object
	boost::filesystem::path p( path, boost::filesystem::native );

	// check existing
	if( !boost::filesystem::exists( p ) ) {
		return false;
	}

	// judgement
	return boost::filesystem::is_directory( p );
}

// check file before open
bool checkfile( const char* path ) {
	// check parameter
	if( path == NULL ) {
		LOG_ERROR( FMT( "The file path is null." ) );
		return false;
	}

	//
	if( !fileexists( path ) ) {
		LOG_ERROR_CODE( FMT( "The file does not exist. [%s]", path ), ERR_FILE_NOT_FOUND );
		return false;
	}

	if( isdirectory( path ) ){
		LOG_ERROR_CODE( FMT( "The path is a directory. A file should be specified. [%s]", path ), ERR_PATH_IS_DIRECTORY );
		return false;
	}

	return true;
}

// get file name
std::string getfilename( const char* path ) {
	// check parameter
	if( path == NULL ) {
		return "";
	}

	// create path object
	boost::filesystem::path p( path, boost::filesystem::native );

	// leaf
	std::string name = p.leaf().string();
	return name;
}

// get dir name
std::string getdir( const char* path ) {
	// check parameter
	if( path == NULL ) {
		return "";
	}

	// create path object
	boost::filesystem::path p( path, boost::filesystem::native );

	// directory
	if( p.has_branch_path() ) {
		p = p.branch_path();
		return p.string();
	}

	return std::string( "" );
}

// get path
std::string getpath( const char* dir, const char* file ) {
	// check parameter
	if( dir == NULL && file == NULL ) {
		return std::string( "" );
	}
	if( dir == NULL ) {
		return std::string( file );
	}
	if( file == NULL ) {
		return std::string( dir );
	}

	// create path object
	boost::filesystem::path filePath( file, boost::filesystem::native );

	std::string mergeFileName = FMT( "%s%s%s", dir, DIR_SEPARATOR, file );
	boost::filesystem::path mergePath( mergeFileName.c_str(), boost::filesystem::native );

	// return path
	boost::filesystem::path p = filePath.is_complete() ? filePath : mergePath;		
	p.normalize();
	std::string ret = p.string();
	ret = replacestring( ret.c_str(), "/", DIR_SEPARATOR );
	ret = replacestring( ret.c_str(), "\\", DIR_SEPARATOR );
	return ret;
}

// get absolute path
std::string absolutepath( const char* path ) {
	// check parameter
	if( path == NULL ) {
		return "";
	}

	// create path object
	boost::filesystem::path p( path, boost::filesystem::native );

	// get absolute path
	boost::filesystem::path absPath = boost::filesystem::absolute( p );

	return absPath.string();
}

// get absolute path
std::string getabsolutepath( const char* dir, const char* file ) {
	// create path object
	boost::filesystem::path p( getpath( dir, file ), boost::filesystem::native );
	p.normalize();
	std::string ret = p.string();
	ret = replacestring( ret.c_str(), "/", DIR_SEPARATOR );

	// return path
	if( p.is_complete() ) {	// dir has root
		return ret;
	}

	return absolutepath( ret.c_str() );
}

// judge absolute file path or not
bool isabsolutepath( const char* file ) {
	// create path object
	boost::filesystem::path p( file, boost::filesystem::native );
	p.normalize();

	// judge
	return p.is_complete();
}

// set arguments
void setarguments( int argc, char** argv ) {
	// initialize the array
	g_arguments.clear();

	// keep arguments
	if( argc > 0 ){
		g_arguments.reserve( argc );
	}
	for( int i = 0; i < argc; i++ ){
		g_arguments.push_back( std::string( argv[i] ) );
	}
}

// get number of arguments
int getargc() {
	// check variable
	if( g_arguments.size() <= 0 ) {
		LOG_WARN( FMT( "\"setarguments\" function has not been called yet." ) );
	}

	return (int)g_arguments.size();
}

// get argument
const char* getargv( int index ) {
	// check variable
	if( g_arguments.size() <= 0 ) {
		LOG_WARN( FMT( "\"setarguments\" function has not been called yet." ) );
	}

	// check index
	if( index < 0 || index >= (int)g_arguments.size() ) {
		return NULL;
	}

	return g_arguments[index].c_str();
}

// get extension
std::string getext( const char* path ) {
	// check parameter
	if( path == NULL ) {
		return std::string( "" );
	}

	// find '.'
	std::string p( path );
	size_t pos = p.find_last_of( "." );
	if( pos == p.npos ) {
		return "";
	}
	
	return p.substr( pos + 1 );
}

// get environment variable
std::string getenvariable( const char* name ) {
	// check parameter
	if( name == NULL ) {
		return std::string( "" );
	}

	// get environment variable
	std::string value;

#ifdef _MSC_VER
	size_t size = 0;
	char* v = NULL;
	_dupenv_s( &v, &size, name );
	if( v != NULL ) {
		value = std::string( v );
		delete[] v;
	}
#else
	char* v = getenv( name );
	if( v != NULL ) {
		value = std::string( v );
	}

#endif	// _MSC_VER

	return value;
}

// get module file path
std::string getmodulefile() {
	// check global variables
	if( !g_moduleFile.empty() ) {
		return g_moduleFile;
	}
	if( g_arguments.size() <= 0 ) {
		LOG_WARN( FMT( "\"setarguments\" function has not been called yet." ) );
		return std::string( "" );
	}

	// get file name
	std::string program = getfilename( g_arguments[0].c_str() );
	std::string fileName = program;

#ifndef __unix		// Windows

	std::string ext = getext( fileName.c_str() );
	if( compareignorecase( ext.c_str(), "exe" ) != 0 ) {
		fileName.append( ".exe" );
	}

#endif	// __unix

	if( strcmp( program.c_str(), g_arguments[0].c_str() ) == 0 ) {	// search PATH
		// get environment variable "PATH"
#ifdef __unix		// unix
		std::string p = getenvariable( "PATH" );
#else		// windows
		std::string p = getenvariable( "Path" );
#endif		// __unix

		// creating search path list
		std::vector<std::string> paths; 
		stringtoken(
			p.c_str(),
			PATH_SEPARATOR, 
				paths
		);

#ifndef __unix		// Windows
		paths.insert(paths.begin(), std::string("."));
#endif	// __unix

		// search path
		for( unsigned int xx = 0; g_moduleFile.empty() && xx < paths.size(); xx++) {

			if( !paths[xx].empty() ) {
				try {
					std::string pgPath = getabsolutepath( paths[xx].c_str(), fileName.c_str() );
					if( fileexists( pgPath.c_str() ) ) {
								g_moduleFile = pgPath;
					}
				}
				catch( ... ) {
				}
			}
		}
	}
	else {	// specified directory
		g_moduleFile = getabsolutepath( getdir( g_arguments[0].c_str() ).c_str(), fileName.c_str() );
	}

	return g_moduleFile;
}

// get module file dir
std::string getmoduledir() {
	return getdir( getmodulefile().c_str() );
}

// get home directory
std::string gethomedir() {
	// check global variable
	if( !g_homeDir.empty() ) {
		return g_homeDir;
	}

	// get home dir
	std::string homeDir;

#ifdef __unix
	homeDir = getenvariable( "HOME" );
	if( homeDir.empty() ) {		
		homeDir = "~";
	}
#else
	std::string homeDrive = getenvariable( "HOMEDRIVE" );
	std::string homePath = getenvariable( "HOMEPATH" );
	homeDir = homeDrive + homePath;
#endif  // __unix
	g_homeDir = absolutepath( homeDir.c_str() );
	
	return g_homeDir;
}

// file open
FILE* fileopen( const char* path, const char* mode ) {
	// file descriptor
	FILE* fp = NULL;

#ifdef _MSC_VER
	errno_t ret = fopen_s( &fp, path, mode );
	if( ret != 0 ) {
		fp = NULL;
	}
#else
	fp = fopen( path, mode );
#endif	// MSC_VER

	return fp;
}

// file seek
void fileseek( FILE* fp, long long offset, int origin ) {

#ifdef _MSC_VER
	_fseeki64( fp, offset, origin );
#else
	fseeko64( fp, offset, origin );
#endif
}

// file tell
long long filetell( FILE* fp ) {
	long long pos = 0;
#ifdef _MSC_VER
	pos = _ftelli64( fp );
#else
	pos = ftello64( fp );
#endif

	return pos;
}

// copy file
bool copyfile( const char* src, const char* dst ) {
	// buffer
	char buff[ 0x10000 ];

	// file open
	FILE* srcFp = fileopen( src, "rb" );
	if( srcFp == NULL ) {
		return false;
	}

	FILE* dstFp = fileopen( dst, "wb" );
	if( dstFp == NULL ) {
		return false;
	}

	// copy
	size_t len = int();
	while( ( len = fread( buff, 1, 0x10000, srcFp ) ) > 0 ) {
		fwrite( buff, 1, len, dstFp );
	}

	// close
	fflush( dstFp );
	fclose( dstFp );
	fclose( srcFp );

	return true;
}

// remove file
bool removefile( const char* path ) {
	// result
	int ret = 0;

#ifdef _MSC_VER
	ret = _unlink( path );
#else
	ret = unlink( path );
#endif

	if( ret != 0 ) {
		LOG_ERROR( FMT( "Failed to remove the file. [%s]", NVL( path, "" ) ) );
	}

	return ( ret == 0 );
}

// make directory
bool makedirectory( const char* path ) {
	// result
	int ret = 0;

#ifdef  _MSC_VER
	ret = _mkdir( path );
#else
	ret = mkdir( path, 0755 );
#endif

	if( ret != 0 ) {
		LOG_ERROR( FMT( "Failed to create the directory. [%s]", NVL( path, "" ) ) );
	}

	return ( ret == 0 );
}

// remove directories
bool removedirs( const char* path ) {
	// path object
	boost::filesystem::path p( path, boost::filesystem::native );
	boost::filesystem::directory_iterator end;

	// check
	if( !boost::filesystem::exists( p ) ) {
		LOG_WARN( FMT( "%s does not exist.", path ) );
		return false;
	}

	// remove
	bool ret = true;
	if( boost::filesystem::is_directory( p ) ) {	// directory
		for( boost::filesystem::directory_iterator it( p ); it != end; it++ ) {
			boost::filesystem::path child = boost::filesystem::absolute( *it );
			if( !removedirs( child.string().c_str() ) ) {
				ret = false;
			}
		}

#ifdef _MSC_VER
		_rmdir( path );
#else
		rmdir( path );
#endif
	}
	else {  // file
		if( !removefile( path ) ) {
			ret = false;
		}
	}

	return ret;
}

// make directories
bool makedirs( const char* path ) {
	// path object
	boost::filesystem::path p( path, boost::filesystem::native );
	p = boost::filesystem::absolute( p );

	// check
	if( boost::filesystem::exists( p ) ) {
		return false;
	}

	// branch
	bool ret = true;
	boost::filesystem::path branche = p.branch_path();
	if( !boost::filesystem::exists( branche ) ) {
		if( !makedirs( branche.string().c_str() ) ) {
			ret = false;
		}		
	}

	// make directory
	if( ret ) {
		ret = makedirectory( p.string().c_str() );
	}

	return ret;
}

// get mspp plugin dir
std::string getplugindir(){
	std::string strDir;
	strDir = getenvariable( "MSPP_PLUGIN_DIR" );
	return strDir;
}

// file size
unsigned long long filesize( const char* path ) {
	// path object
	boost::filesystem::path p( path, boost::filesystem::native );

	// size
	unsigned long long size = (unsigned long long)boost::filesystem::file_size( p );

	return size;
}

// search file
std::string searchfile( const char* dir, const char* fileName, const bool recursive ) {
	// return value
	std::string filePath;

	// search files
	std::vector< std::string > dirs;

	boost::filesystem::path p( dir, boost::filesystem::native );
	if( !boost::filesystem::exists( p ) ) {
		return filePath;
	}

	boost::filesystem::directory_iterator end;
	for( boost::filesystem::directory_iterator it( p ); it != end && filePath.empty(); it++ ) {
		std::string childName = (*it).path().leaf().string();
		std::string child = getpath( dir, (*it).path().leaf().string().c_str() );

		if( childName.compare( fileName ) == 0 ) {
			filePath = child;
		}
		else if( isdirectory( child.c_str() ) ) {
			dirs.push_back( child );
		}
	}

	// search folders
	if( recursive ) {
		for( unsigned int i = 0; i < dirs.size() && filePath.empty(); i++ ) {
			filePath = searchfile( dirs[ i ].c_str(), fileName, true );
		}
	}

	return filePath;

}
