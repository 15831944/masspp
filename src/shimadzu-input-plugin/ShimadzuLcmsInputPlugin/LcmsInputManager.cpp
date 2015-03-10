/**
 * @file LcmsInputManager.cpp
 * @brief implements of LcmsInputManager class
 *
 * @author S.Tanaka
 * @date 2008.02.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LcmsInputManager.h"

#include "LcmsSampleSet.h"


#define PATH_PARAM_NAME "path"


using namespace kome::io::lcms;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// open the file
kome::clr::ClrVariant^ LcmsInputManager::open( kome::clr::ClrParameters^ params ) {
	// logger
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	// log
	logger->trace( "open LCMS data file..." );

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
		logger->errorCode( "Failed to get the path information." , logger->getErrorCode( "other" ) );
		return ret;
	}
	System::String^ path = (System::String^)pathValue->obj;

	// create raw data
	LcmsSampleSet^ sampleSet = gcnew LcmsSampleSet();
	ret->obj = sampleSet;

	logger->debug( "LCMS data file path: " + path );

	return ret;
}
