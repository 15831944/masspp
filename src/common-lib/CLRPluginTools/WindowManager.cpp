/**
 * @file WindowManager.cpp
 * @brief implements of WindowManager class
 *
 * @author S.Tanaka
 * @date 2013.05.14
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "WindowManager.h"

#include "SettingsPageWrapper.h"

#include "windows.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
WindowManager::WindowManager() {
}

// destructor
WindowManager::~WindowManager() {
}

// open settings page dialog
bool WindowManager::openSettingsPageDialog(
		SettingsPageWrapper^ page,
		SettingParameterValuesWrapper^ settings,
		bool saveVal,
		System::String^ prefix,
		bool dfltBtnFlg,
		System::String^ commitName
) {
	// check parameters
	if( page == nullptr ) {
		return false;
	}

	// logger

	// folder
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	std::string msppDir = msppMgr.getMsppDir();
	std::string binDir = getpath( msppDir.c_str(), "bin" );
	std::string dllFile = getpath( binDir.c_str(), "WindowManager.dll" );

	// open
	HMODULE module = LoadLibraryEx( dllFile.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
	if( module == NULL ) {
		LOG_ERROR( FMT( "Failed to open the dll file. [%s]", dllFile.c_str() ) );
		return false;
	}

	// get function
	typedef bool (*OpenSettingsDialogFun)(
			kome::plugin::SettingsPage*,
			kome::objects::SettingParameterValues*,
			const bool,
			const char*,
			const bool,
			const char*
	);

	OpenSettingsDialogFun fun = (OpenSettingsDialogFun)GetProcAddress( module, "openSettingsPageDialog" );
	if( fun == NULL ) {
		LOG_ERROR( FMT( "Failed to get the function address. [openSettingsPageDialog]" ) );
		return false;
	}

	// parametes
	kome::plugin::SettingsPage* p = NULL;
	if( page != nullptr ) {
		p = &( page->getSettingsPage() );
	}

	kome::objects::SettingParameterValues* v = NULL;
	if( settings != nullptr ) {
		v = ClrObjectTool::getSettings( settings );
	}

	// call
	return (*fun)(
		p,
		v,
		saveVal,
		ClrObjectTool::convertString( prefix ).c_str(),
		dfltBtnFlg,
		ClrObjectTool::convertString( commitName ).c_str()
	);
}

// get window manager
WindowManager^ WindowManager::getInstance() {
	if( m_mgr == nullptr ) {
		m_mgr = gcnew WindowManager();
	}

	return m_mgr;
}
