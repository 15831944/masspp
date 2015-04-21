/**
 * @file MsppManager.h
 * @brief implements of MsppManager class
 *
 * @author S.Tanaka
 * @date 2008.12.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MsppManager.h"

#include <time.h>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define APP_NAME              "Mass++"
#define REG_NAME              "Mass++2"
#define MSPP_VERSION		  "2.7.5"

#define TO_STR( s ) TO_STR_( s )
#define TO_STR_( s ) #s


// constructor
MsppManager::MsppManager() {
	// initialize
	m_ini = NULL;
	m_params = NULL;

	// set path
	setPath();

	// set mspp plugin dir
	setPluginDir();

	// delete tmp files
	deleteTmpFiles();

	// version
	m_app = APP_NAME;
	m_reg = REG_NAME;
	m_version = MSPP_VERSION;

#if defined(_WIN64) || defined(__amd64__)
	m_platform = "64-bit";
#else
    m_platform = "32-bit";
#endif    // _WIN64 || __amd64__

	/** process ID */
	m_pid = getprocessid();

	m_bInitializedMATLAB = false;	// @Date:2013/08/08	<Add>	A.Ozaki
}

// destructor
MsppManager::~MsppManager() {
}

// get the application name
const char* MsppManager::getAppName() {
	return m_app.c_str();
}

// get the platform name
const char* MsppManager::getPlatformName() {
	return m_platform.c_str();
}

// get the registory name
const char* MsppManager::getRegName() {
	return m_reg.c_str();
}

// get version
const char* MsppManager::getVersion() {
	return m_version.c_str();
}

// set file path
void MsppManager::setPath() {
	// Mass++ dir
#ifdef MSPP_INSTALL_DIR
	m_msppDir = TO_STR( MSPP_INSTALL_DIR );
#else
	std::string moduleDir = getmoduledir();
	m_msppDir = getabsolutepath( moduleDir.c_str(), ".." );
#endif  // MSPP_INSTALL_DIR

	// conf dir
	std::string homeDir = gethomedir();
	m_confDir = getabsolutepath( homeDir.c_str(), ".mspp" );
	if( !fileexists( m_confDir.c_str() ) ) {
		makedirectory( m_confDir.c_str() );
	}

	// tmp dir
	const char* envariables[] = {
		"TMP", "TEMP", "tmp", "temp"
	};
	unsigned int num = sizeof( envariables ) / sizeof( const char* );

	std::string tmpDir;
	for( unsigned int i = 0; i < num && tmpDir.empty(); i++ ) {
		tmpDir = getenvariable( envariables[ i ] );
	}

	// cut by path separator
	std::string::size_type pos = tmpDir.find( PATH_SEPARATOR );
	if( pos != tmpDir.npos ) {
		tmpDir = tmpDir.substr( 0, pos );
	}

	// default directory
	if( tmpDir.empty() ) {
#ifdef __unix
		tmpDir = "/tmp";
#else
		tmpDir = "C:\\TEMP";
#endif		// __unix
	}

	m_tmpDir = getabsolutepath( tmpDir.c_str(), "MassPlusPlus_Tmp" );

	// make directory
	if( !fileexists( m_tmpDir.c_str() ) ) {
		makedirectory( m_tmpDir.c_str() );
	}
}

// delete tmp files
void MsppManager::deleteTmpFiles() {
	// check directory
	if( m_tmpDir.empty() ) {
		return;
	}

	// delete files
	boost::filesystem::path p( m_tmpDir.c_str(), boost::filesystem::native );
	boost::filesystem::directory_iterator end;

	for( boost::filesystem::directory_iterator it( p ); it != end; it++ ) {
		boost::filesystem::path child = boost::filesystem::absolute( *it );

		std::string fileName = getfilename( child.string().c_str() );
		int pid = toint( fileName.c_str(), 16, m_pid );

		if( pid == m_pid || !processexists( pid ) ) {
			removedirs( child.string().c_str() );
		}
	}
}

// get Mass++ directory
const char* MsppManager::getMsppDir() {
	return m_msppDir.c_str();
}

// get config directory
const char* MsppManager::getConfDir() {
	return m_confDir.c_str();
}

// get temporary file directory
const char* MsppManager::getTmpDir() {
	// check the member
	if( m_pidTmpDir.empty() ) {
		std::string pidStr = FMT( "%04x", m_pid );
		m_pidTmpDir = getpath( m_tmpDir.c_str(), pidStr.c_str() );

		// make directory
		if( !fileexists( m_pidTmpDir.c_str() ) ) {
			makedirs( m_pidTmpDir.c_str() );
		}
	}

	return m_pidTmpDir.c_str();
}

// get temporary file name
std::string MsppManager::getTmpFileName( const char* prefix, const char* suffix, const char* dir ) {
	// file path
	std::string path;
	std::string fileName;
	std::string d = NVL( dir, "" );

	// check directory
	std::string tmpDir = getTmpDir();
	if( tmpDir.empty() ) {
		return path;
	}

	// check directory
	if( d.empty() ) {
		d = tmpDir;
	}
	else if( !isabsolutepath( d.c_str() ) ) {
		d = getpath( tmpDir.c_str(), d.c_str() );
		makedirs( d.c_str() );
	}		

	// date
	struct tm date;
	time_t now;
	
	time( &now );

#ifdef _MSC_VER
	localtime_s( &date, &now );
#else
	date = *( localtime( &now ) );
#endif

	// tempolary file path
	long count = 0;
	while( path.empty() && count >= 0 ) {
		// file
		fileName = FMT(
			"%s%02d%02d%02d%04x%s",
			prefix,
			date.tm_mon + 1,
			date.tm_mday,
			date.tm_year % 100,
			count,
			suffix
		);
		for( unsigned int i = 0; i < fileName.length(); i++ ) {
			char c = fileName[ i ];
			if( c == '/' || c == '\\' || c == ':' || c == '?'
					|| c == '>' || c == '<' || c == '|' || c == ' '
					|| c == '\r' || c == '\t' || c == '\n' || c == ',' ) {
				fileName[ i ] = '_';
			}
		}
		std::string p = getabsolutepath( d.c_str(), fileName.c_str() );

		// check file
		if( !fileexists( p.c_str() ) ) {
			path = p;
		}

		count++;
	}

	LOG_TRACE( FMT( "Temporary File [%s]", path.c_str() ) );

	return fileName;
}

// set ini file
void MsppManager::setIniFile( IniFile* ini ) {
	m_ini = ini;
}

// get ini file
IniFile* MsppManager::getIniFile() {
	return m_ini;
}

// set parameters file
void MsppManager::setParameters( IniFile* params ) {
	m_params = params;
}

// get parameters file
IniFile* MsppManager::getParameters() {
	return m_params;
}

// get proceess ID
unsigned int MsppManager::getPid() {
	return m_pid;
}

// get instance
MsppManager& MsppManager::getInstance() {
	// create object (This is the only object.)
	static MsppManager mgr;

	return mgr;
}

// @date 2011.10.21 <Add> M.Izumi ->
//set mspp plugin dir
void MsppManager::setPluginDir(){
	std::string str = getplugindir();
	stringtoken( str.c_str(), PATH_SEPARATOR, m_pluginPaths );
}

// get mspp plugin dir
std::vector< std::string > MsppManager::getPluginDir(){
	return m_pluginPaths;
}
//<-

// >>>>>>	@Date:2013/08/08	<Add>	A.Ozaki
//
// get initialized flag of MATLAB
bool	MsppManager::isInitializedMATLAB( void )
{
	return	m_bInitializedMATLAB;
}

// set initialized flag of MATLAB
void	MsppManager::setInitializedMATLAB( void )
{
	m_bInitializedMATLAB = true;
	return;
}

// reset initialized flag of MATLAB
void	MsppManager::resetInitializedMATLAB( void )
{
	m_bInitializedMATLAB = false;
	return;
}

//
// <<<<<<	@Date:2013/08/08	<Add>	A.Ozaki
