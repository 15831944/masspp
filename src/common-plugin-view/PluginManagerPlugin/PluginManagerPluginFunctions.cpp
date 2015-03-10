/**
 * @file PluginManagerPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author okada_h
 * @date 2013.02.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "PluginManagerPluginFunctions.h"
#include "PluginManagerPluginManager.h"

using namespace kome::pluginmanager::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// pluginManagerView
kome::objects::Variant openPluginManagerView( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// @date 2013/05/27 <Add> OKADA ------->
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();	  
	bool flgActiveEnable = plgMgr.getActiveEnable();
	plgMgr.setActiveEnable( false );	// プラグイン管理画面表示中はactiveなものに限らず扱う
	// @date 2013/05/27 <Add> OKADA <-------

	PluginManagerView& mgr = PluginManagerView::getInstance();

	int nRet = mgr.openDialog();

	if( nRet == wxID_APPLY ){
		ret.prim.boolVal = true;
	}

	plgMgr.setActiveEnable( flgActiveEnable );	// 設定を元に戻す	// @date 2013/05/27 <Add> OKADA

	return ret;
}

