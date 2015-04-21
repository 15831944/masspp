/**
 * @file CommonParameterManager.cpp
 * @brief interfaces of common parameter manager class
 *
 * @author M.Izumi
 * @date 2012.02.22
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "CommonParameterManager.h"
#include "MsppManager.h"
#include "IniFile.h"

#include <iostream>
#include <boost/regex.hpp>
#include <boost/algorithm/string/case_conv.hpp>


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define SECTION "Common Parameters"

// constructor
CommonParameterManager::CommonParameterManager(){
	m_paramInfo.clear();
}

// destructor
CommonParameterManager::~CommonParameterManager(){
}

// get number of parameters
int CommonParameterManager::getNumberOfParameters(){
	return m_paramInfo.size();
}

// get parameter name
std::string CommonParameterManager::getParameterName( unsigned int index ){	
	if( index >= m_paramInfo.size() ) {
		return NULL;
	}

	return m_paramInfo[ index ].paramName;
}

// set parameter
void CommonParameterManager::setParameter( std::string paramName, std::string paramValue, bool tempFlg ){
	for( unsigned int i=0; i < m_paramInfo.size(); i++ ){
		if( paramName.compare( m_paramInfo[i].paramName ) == 0 ){
			return;
		}		
	}

	ParamInfo info;
	info.paramName = paramName;
	info.paramValue = paramValue;
	info.tempFlg = tempFlg;

	m_paramInfo.push_back( info );

	if( !tempFlg ){
		setIniString( paramName, paramValue );		
	}
}

// set ini strign
void CommonParameterManager::setIniString( std::string strKey, std::string strValue ){
	// ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
		
	const char* strRegex = "([^0-9a-zA-Z])";
	boost::regex  reg_exp(strRegex);
		
	std::string replace = "_";
	std::string result = "";

	// replace
	result =  boost::regex_replace( strKey, reg_exp, replace, boost::format_all );
	// Converted to uppercase.
	boost::algorithm::to_upper(result);

	// set string 
	ini->setString( SECTION, result.c_str(), strValue.c_str() );
}

// get parameter
std::string CommonParameterManager::getParameter( std::string paramName, std::string defaultName ){
	std::string str = defaultName;
	for( unsigned int i=0; i < m_paramInfo.size(); i++ ){
		if(  paramName.compare( m_paramInfo[i].paramName ) == 0 ){
			if( !m_paramInfo[i].paramValue.empty() ){
				str = m_paramInfo[i].paramValue;
			}
			break;
		}
	}
	
	return str;
}

// is temporary parameter
bool CommonParameterManager::isTemporaryParameter( unsigned int index ){
	if( index >= m_paramInfo.size() ) {
		return true;
	}

	return m_paramInfo[ index ].tempFlg;
}

// get instance
CommonParameterManager& CommonParameterManager::getInstance(){
	// create object (This is the only object.)
	static CommonParameterManager mgr;

	return mgr;
}
