/**
 * @file PluginComponent.cpp
 * @brief implements of PluginComponet class
 *
 * @author A.Ozaki
 * @date 2006.07.19
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "PluginComponent.h"

#include "PluginCall.h"

#include <list>
#include <boost/bind.hpp>


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PluginComponent::PluginComponent( )
{
	m_strName = std::string( "" );
	m_strDescription = std::string( "" );
	m_vpCalls.clear( );
	m_vstrPackages.clear( );

	m_bEnable = false;

	return;
}

// destructor
PluginComponent::~PluginComponent( )
{
	m_vpCalls.clear( );
	m_vstrPackages.clear( );

	return;
}

// set component name
void	PluginComponent::setName( const char* lpstrName )
{
	m_strName = std::string( NVL( lpstrName, "" ) );

	return;
}

// get component name
const char	*PluginComponent::getName( void )
{
	return	m_strName.c_str( );
}

// set component description 
void	PluginComponent::setDesc( const char* lpstrDescription )
{
	m_strDescription = std::string( NVL( lpstrDescription, "" ) );

	return;
}

// get component description
const char	*PluginComponent::getDesc( void )
{
	return	m_strDescription.c_str( );
}

// add PluginCall
void	PluginComponent::addCall( PluginCall *pCall )
{
	// add
	m_vpCalls.push_back( pCall );

	return;
}

// get the number of call
unsigned int PluginComponent::getNumberOfCalls( void )
{
	return	m_vpCalls.size( );
}

// get PluginCall
PluginCall	*PluginComponent::getCall( const unsigned int unIndex )
{
	if  ( m_vpCalls.size( ) <= unIndex )
	{
		return	(PluginCall *)NULL;
	}

	return	m_vpCalls[unIndex];
}

// add package
void	PluginComponent::addPackage( const char *lpstrPackage )
{
	if  ( (char *)NULL == lpstrPackage )
	{
		return;
	}

	std::string	strPackage = lpstrPackage;

	m_vstrPackages.push_back( strPackage );

	return;
}

// has package
bool	PluginComponent::hasPackage( const char *lpstrPackage )
{
	std::string	strPackage = std::string( NVL( lpstrPackage, "" ) );
	std::vector< std::string >::iterator	insIter = find( m_vstrPackages.begin( ), m_vstrPackages.end( ) , strPackage );

	bool	bRet = false;
	if  ( m_vstrPackages.end( ) != insIter )
	{
		// exist package name
		bRet = true;
	}

	return	bRet;
}

// >>>>>>	@Date:2013/08/21	<Add>	A.Ozaki
//
// set enable component
void	PluginComponent::setEnable( void )
{
	m_bEnable = true;
}

// reset enable component
void	PluginComponent::resetEnable( void )
{
	m_bEnable = false;
}

// enable component?
bool	PluginComponent::isEnable( void )
{
	return	m_bEnable;
}

//
// <<<<<<	@Date:2013/08/21	<Add>	A.Ozaki
