/**
 * @file AbQstarInputManager.cpp
 * @brief implements of AbQstarInputManager class
 *
 * @author S.Tanaka
 * @date 2007.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "AbQstarInputManager.h"

#include "AbQstarSample.h"

using namespace kome::io::qstar;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PATH_PARAM_NAME "path"


// open raw data
kome::clr::ClrVariant^ AbQstarInputManager::open( kome::clr::ClrParameters^ params ) {
	// logger
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	// log
	logger->trace( "open Applied Biosystems raw data file..." );

	// create return value object
	kome::clr::ClrVariant^ ret = gcnew kome::clr::ClrVariant();

	ret->type = kome::clr::ClrVariant::DataType::SAMPLE_SET;
	ret->obj = nullptr;

	// check parameter
	if( params == nullptr ) {
		logger->errorCode( "The parameter is null.", logger->getErrorCode( "null pointer" ) );
		return ret;
	}

	// get the path
	kome::clr::ClrVariant^ pathValue = params->getValue( PATH_PARAM_NAME );
	if( pathValue == nullptr ) {
		logger->errorCode( "Failed to get path information.", logger->getErrorCode( "othter" ) );
		return ret;
	}
	System::String^ path = (System::String^)pathValue->obj;

	// sample type
	System::Type^ sampleType = System::Type::GetType( "kome.io.qstar.AbQstarSample" );

	// create raw data
	kome::io::ab::AbSampleSet^ sampleSet = gcnew kome::io::ab::AbSampleSet( sampleType );
	ret->obj = sampleSet;

	logger->debug( "Applied Biosystems data file path: " + path );

	return ret;
}
