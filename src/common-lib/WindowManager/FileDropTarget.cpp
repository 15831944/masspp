/**
 * @file FileDropTarget.cpp
 * @brief implements of FileDropTarget class
 *
 * @author S.Tanaka
 * @date 2013.01.10
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "FileDropTarget.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
FileDropTarget::FileDropTarget() {
}

// destructor
FileDropTarget::~FileDropTarget() {
}

// on drop files
bool FileDropTarget::OnDropFiles( wxCoord x, wxCoord y, const wxArrayString& filenames ) {
	// plugin manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// call
	for( unsigned int i = 0; i < filenames.size(); i++ ) {
		kome::plugin::PluginCallTool::openFile( filenames[ i ].c_str() );
	}

	return true;
}
