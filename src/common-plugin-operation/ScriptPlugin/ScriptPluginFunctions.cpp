/**
 * @file ScriptPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author tanaka-s
 * @date 2012.07.10
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ScriptPluginFunctions.h"
#include "ScriptListDialog.h"	// @Date:2013/08/06	<Add>	A.Ozaki
#include "ScriptPluginManager.h"
#include "ScriptPluginCall.h"

using namespace kome::script;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SECTION						 "Script List"
#define SCRIPT_KEY_COUNT			 "COUNT"
#define SCRIPT_KEY_NAME				 "NAME"
#define SCRIPT_KEY_ENGINE			 "ENGINE"
#define SCRIPT_KEY_EVENT			 "EVENT"
#define SCRIPT_KEY_PATH				 "PATH"
#define SECTION_COUNT				 "Script List Count"
#define LIST_NUM_KEY				 "COUNT"
#define ENGIN_SHORT_NAME			 "SHORT_NAME"

// opens script dialog
kome::objects::Variant openScriptDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;
	
	if  ( (ScriptListDialog *)NULL != ScriptListDialog::m_pDlg )
	{
		delete	ScriptListDialog::m_pDlg;
	}

	ScriptListDialog::m_pDlg = new ScriptListDialog( kome::window::WindowTool::getMainWindow( ) );
	ScriptListDialog::m_pDlg->Show( true );

	return ret;
}

// init
kome::objects::Variant initScriptPlugin( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	// managers
    kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	kome::script::ScriptPluginManager& mgr = kome::script::ScriptPluginManager::getInstance();

	int inum = ini->getInt( SECTION_COUNT, LIST_NUM_KEY, -1 );
	for( int i=0; i < inum; i++ ){		
		std::string strSection = FMT( "%s:%d", SECTION, i );

		stScriptInfo	insInfo;
		
		insInfo.nCnt = ini->getInt( strSection.c_str(), SCRIPT_KEY_COUNT, -1 );
		insInfo.strName = ini->getString( strSection.c_str(), SCRIPT_KEY_NAME, "" );
		insInfo.strEngine = ini->getString( strSection.c_str(), SCRIPT_KEY_ENGINE, "" );
		insInfo.strEvent = ini->getString( strSection.c_str(), SCRIPT_KEY_EVENT, "" );
		insInfo.strPath = ini->getString( strSection.c_str(), SCRIPT_KEY_PATH, "" );
		insInfo.strFuncItemName = ini->getString( strSection.c_str(), ENGIN_SHORT_NAME, "" );

		// >>>>>>	@Date:2013/09/12	<Add>	A.Ozaki
		// check script path
		if  ( 0 != _access_s( insInfo.strPath.c_str( ), 4 ) )
		{
			// ファイルが読み込めない場合は、スクリプトとして認めません
			continue;
		}
		// <<<<<<	@Date:2013/09/12	<Add>	A.Ozaki

		// add plugin info
		ScriptPluginCall* call = mgr.getScriptPluginCall( insInfo );
		if( call == NULL ){
			mgr.addPluginInfo( insInfo );
		}
	}

	return ret;
}
