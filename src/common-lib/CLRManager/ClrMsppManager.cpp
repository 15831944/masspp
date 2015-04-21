/**
 * @file ClrMsppManager.cpp
 * @brief ClrMsppManager class
 * @author T.Okuno
 * @date 2011.08.12
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "ClrMsppManager.h"

#include "ClrObjectTool.h"
#include "ClrIniFile.h"

using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
ClrMsppManager::ClrMsppManager() :
	m_msppManager( kome::core::MsppManager::getInstance() )
{
}

// destructor
ClrMsppManager::~ClrMsppManager()
{
}

// get version
System::String^ ClrMsppManager::getVersion()
{
	return ClrObjectTool::convertString( m_msppManager.getVersion(), NULL );
}

// get Mass++ directory
System::String^ ClrMsppManager::getMsppDir()
{
	return ClrObjectTool::convertString( m_msppManager.getMsppDir(), NULL );
}

// get config directory
System::String^ ClrMsppManager::getConfDir()
{
	return ClrObjectTool::convertString( m_msppManager.getConfDir(), NULL );
}

// get temporary file directory
System::String^ ClrMsppManager::getTmpDir()
{
	return ClrObjectTool::convertString( m_msppManager.getTmpDir(), NULL );
}

// get temporary file name
System::String^ ClrMsppManager::getTmpFileName( System::String^ prefix, System::String^ suffix, System::String^ dir )
{
	std::string		strPrefix	= ClrObjectTool::convertString( prefix );
	std::string		strSuffix	= ClrObjectTool::convertString( suffix );
	std::string		strDir		= ClrObjectTool::convertString( dir );
	std::string		fileName	= m_msppManager.getTmpFileName( strPrefix.c_str(),
																strSuffix.c_str(),
																strDir.c_str() );
	return ClrObjectTool::convertString( fileName.c_str(), NULL );
}

// get ini file
ClrIniFile^ ClrMsppManager::getIniFile()
{
	kome::core::IniFile*	piniOpt = m_msppManager.getIniFile();
	if( piniOpt != NULL ){
		return ClrIniFile::getIniFile( ClrObjectTool::convertString( piniOpt->getFilePath(), NULL ) );
	}
	return nullptr;
}

// get parameters file
ClrIniFile^ ClrMsppManager::getParameters()
{
	kome::core::IniFile*	piniPara = m_msppManager.getParameters();
	if( piniPara != NULL ){
		return ClrIniFile::getIniFile( ClrObjectTool::convertString( piniPara->getFilePath(), NULL ) );
	}
	return nullptr;
}

// get proceess ID
unsigned int ClrMsppManager::getPid()
{
	return m_msppManager.getPid();
}

// get instance
ClrMsppManager^ ClrMsppManager::getInstance()
{
	// create object (This is the only object.)
	if( m_instance == nullptr ){
		LOG_TRACE( FMT( "[DIT-DB] creating ClrMsppManager object" ) );
		m_instance = gcnew ClrMsppManager();
	}

	return m_instance;
}

// >>>>>>	@Date:2013/08/08	<Add>	A.Ozaki
//
// get initialized flag of MATLAB
bool	ClrMsppManager::isInitializedMATLAB( void )
{
	return	m_msppManager.isInitializedMATLAB( );
}

// set initialized flag of MATLAB
void	ClrMsppManager::setInitializedMATLAB( void )
{
	m_msppManager.setInitializedMATLAB( );
	return;
}

// reset initialized flag of MATLAB
void	ClrMsppManager::resetInitializedMATLAB( void )
{
	m_msppManager.resetInitializedMATLAB( );
	return;
}
//
// <<<<<<	@Date:2013/08/08	<Add>	A.Ozaki
