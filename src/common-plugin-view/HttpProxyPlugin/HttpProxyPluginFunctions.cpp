/**
 * @file HttpProxyPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.03.13
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "HttpProxyPluginFunctions.h"


#define SECTION           "Proxy"
#define SERVER_KEY        "SERVER"
#define PASS_KEY          "PASS"


// set proxy information
kome::objects::Variant setProxyInfo( kome::objects::Parameters* ) {
	kome::objects::Variant ret;

	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::net::NetworkManager& netMgr = kome::net::NetworkManager::getInstance();

	// server
	kome::core::IniFile* ini = msppMgr.getIniFile();
	std::string server;
	std::string pass;
	if( ini != NULL ) {
		server = ini->getString( SECTION, SERVER_KEY, "" );
		pass = ini->getString( SECTION, PASS_KEY, "" );
	}

	// set
	netMgr.setProxyServer( server.c_str() );
	netMgr.setProxyPass( pass.c_str() );

	return ret;
}
