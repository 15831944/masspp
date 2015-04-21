/**
 * @file ClrSearchEngineBase.cpp
 * @brief implements of ClrSearchEngineBase class
 *
 * @author S.Tanaka
 * @date 2012.11.07
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ClrSearchEngineBase.h"

#include "ClrSearchEngine.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrSearchEngineBase::ClrSearchEngineBase( System::String^ name, System::String^ version, bool multipleSpec )
		: SearchEngineWrapper(
			ClrSearchEngineManager::getInstance().createSearchEngine(
				ClrObjectTool::convertString( name ).c_str(),
				ClrObjectTool::convertString( version ).c_str(),
				multipleSpec
			)
		)
{
	( (ClrSearchEngine&)m_engine ).setBaseSearchEngine( this );
}

// destructor
ClrSearchEngineBase::~ClrSearchEngineBase() {
	ClrSearchEngineManager::getInstance().destroySearchEngine( (ClrSearchEngine&)m_engine );
}
