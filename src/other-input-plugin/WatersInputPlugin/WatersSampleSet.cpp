/**
 * @file WatersSampleSet.cpp
 * @brief implements of WatersSampleSet class
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "WatersSampleSet.h"

#include "WatersSample.h"
#include "WatersInputManager.h"


using namespace kome::io::waters;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
WatersSampleSet::WatersSampleSet() {
}

// destructor
WatersSampleSet::~WatersSampleSet() {
}

// get path
System::String^ WatersSampleSet::getPath() {
	return m_path;
}

// on open file
bool WatersSampleSet::onOpenFile( System::String^ path ) {
	// logger
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	// add sample set
	WatersInputManager^ mgr = WatersInputManager::getInstance();
	mgr->addSampleSet( this );

	// path
	m_path = path;	
	if( m_path->EndsWith( "\\" ) ) {
		m_path = m_path->Substring( 0, path->Length - 1 );
	}

	// function object
	DACSERVERLib::DACFunctionInfoClass fun;

	// add samples
	int funcNum = fun.GetNumFunctions( path );
	logger->info( funcNum.ToString() + " Functions" );
	for( int i = 1; i <= funcNum; i++ ) {
		// get functino info
		fun.GetFunctionInfo( path, i );

		// sample name
		System::String^ funcName = "Function" + i + " [" + fun.FunctionType + "]";

		// sample
		WatersSample^ sample = gcnew WatersSample( this, i );
		sample->setName( funcName );
		addSample( sample );
	}

	return true;
}

// on close file
bool WatersSampleSet::onCloseFile() {
	// remove sample set
	WatersInputManager^ mgr = WatersInputManager::getInstance();
	mgr->removeSampleSet( this );

	return true;
}

// on open sample
bool WatersSampleSet::onOpenSample( kome::clr::SampleWrapper^ sample ) {
	return true;
}

// on close sample
bool WatersSampleSet::onCloseSample( kome::clr::SampleWrapper^ sample ) {
	return true;
}
