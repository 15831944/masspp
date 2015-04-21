/**
 * @file DisplayProgressWrapper.cpp
 * @brief interfaces of DisplayProgressWrapper class
 *
 * @author S.Tanaka
 * @date 2013.10.24
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "DisplayProgressWrapper.h"

#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DisplayProgressWrapper::DisplayProgressWrapper( System::String^ title )
		: ProgressWrapper( *( new kome::core::DisplayProgress( ClrObjectTool::convertString( title ).c_str() ) ) ) {
}

// destructor
DisplayProgressWrapper::~DisplayProgressWrapper() {
		delete &m_progress;
}
