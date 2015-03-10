/**
 * @file AgilentInputManager.cpp
 * @brief implements of AgilentInputManager class
 *
 * @author S.Tanaka
 * @date 2009.05.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "AgilentInputManager.h"

#include "AgilentSampleSet.h"
#include "AgilentSample.h"

using namespace kome::io::agilent;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PATH_PARAM_NAME		  "path"

#define SECTION				  "Agilent"
#define DEFAULT_DIR_KEY		  "DEFAULT_DIR"

#define SAMPLE_SET_PARAM_NAME	"sample"
#define PATH_PARAM_NAME		  "path"


// constructor
AgilentInputManager::AgilentInputManager() {
	m_list = gcnew System::Collections::ArrayList();
}

// destructor
AgilentInputManager::~AgilentInputManager() {
}

// add sample
void AgilentInputManager::addSampleSet( AgilentSampleSet^ sampleSet ) {
	m_list->Add( sampleSet );
}

// remove sample
void AgilentInputManager::removeSampleSet( AgilentSampleSet^ sampleSet ) {
	m_list->Remove( sampleSet );
}

// oepn file
kome::clr::ClrVariant^ AgilentInputManager::open(
	kome::clr::ClrParameters^ params
) {
	// return value
	kome::clr::ClrVariant^ ret = gcnew kome::clr::ClrVariant();	
	ret->type = kome::clr::ClrVariant::DataType::SAMPLE_SET;

	// path
	kome::clr::ClrVariant^ pathVal = params->getValue( PATH_PARAM_NAME );
	System::String^ path = (System::String^)pathVal->obj;

	// file
	AgilentSampleSet^ sampleSet = gcnew AgilentSampleSet();
	ret->obj = sampleSet;

	return ret;
}

// check the folder
kome::clr::ClrVariant^ AgilentInputManager::isAgilentFolder(
	kome::clr::ClrParameters^ params
) {
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();
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
	if( path->ToLower()->EndsWith( ".d" ) ) {
		System::IO::DirectoryInfo^ dir = gcnew System::IO::DirectoryInfo( path );
		ret->obj = dir->Exists;
	}

	return ret;
}

// get instance
AgilentInputManager^ AgilentInputManager::getInstance() {
	if( m_instance == nullptr ) {
		m_instance = gcnew AgilentInputManager();
	}

	return m_instance;
}
