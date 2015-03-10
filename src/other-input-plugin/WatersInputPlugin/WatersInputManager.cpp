/**
 * @file WatersInputManager.cpp
 * @brief implements of WatersInputManager class
 *
 * @author S.Tanaka
 * @date 2006.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "WatersInputManager.h"

#include "WatersSampleSet.h"
#include "WatersSample.h"


using namespace kome::io::waters;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PATH_PARAM_NAME		  "path"

#define SECTION				  "Waters"
#define DEFAULT_DIR_KEY		  "DEFAULT_DIR"

#define SAMPLE_SET_PARAM_NAME	"sample"
#define PATH_PARAM_NAME		  "path"


// constructor
WatersInputManager::WatersInputManager() {
	m_list = gcnew System::Collections::ArrayList();
}

// destructor
WatersInputManager::~WatersInputManager() {
}

// add sample
void WatersInputManager::addSampleSet( WatersSampleSet^ sampleSet ) {
	m_list->Add( sampleSet );
}

// remove sample
void WatersInputManager::removeSampleSet( WatersSampleSet^ sampleSet ) {
	m_list->Remove( sampleSet );
}

// oepn file
kome::clr::ClrVariant^ WatersInputManager::open( kome::clr::ClrParameters^ params ) {
	// return value
	kome::clr::ClrVariant^ ret = gcnew kome::clr::ClrVariant();	
	ret->type = kome::clr::ClrVariant::DataType::SAMPLE_SET;

	// path
	kome::clr::ClrVariant^ pathVal = params->getValue( PATH_PARAM_NAME );
	System::String^ path = (System::String^)pathVal->obj;

	// file
	WatersSampleSet^ sampleSet = gcnew WatersSampleSet();
	ret->obj = sampleSet;

	return ret;
}

// check the folder
kome::clr::ClrVariant^ WatersInputManager::isMicromassFolder( kome::clr::ClrParameters^ params ) {
	// return value
	kome::clr::ClrVariant^ ret = gcnew kome::clr::ClrVariant();
	ret->type = kome::clr::ClrVariant::DataType::BOOL;
	ret->obj = (System::Object^)false;

	// path
	kome::clr::ClrVariant^ pathVal = params->getValue( PATH_PARAM_NAME );
	System::String^ path = nullptr;
	if( pathVal != nullptr ) {
		path = (System::String^)pathVal->obj;
	}

	// check
	if( path->ToLower()->EndsWith( ".raw" ) ) {
		System::IO::DirectoryInfo^ dir = gcnew System::IO::DirectoryInfo( path );
		ret->obj = dir->Exists;
	}

	return ret;
}

// get instance
WatersInputManager^ WatersInputManager::getInstance() {
	if( m_instance == nullptr ) {
		m_instance = gcnew WatersInputManager();
	}

	return m_instance;
}
