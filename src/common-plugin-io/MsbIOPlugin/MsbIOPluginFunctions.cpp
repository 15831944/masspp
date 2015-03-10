/**
 * @file MsbIOPluginFunctions.cpp
 * @brief implements of MsbIO plug-in function
 *
 * @author S.Tanaka
 * @date 2006.10.05
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */
 
#include "stdafx.h"
#include "MsbIOPluginFunctions.h"

#include "MsbManager.h"
#include "MsbSampleSet.h"
#include "MsbSample.h"

#include "MsbIO0202.h"

#define DATA_STATUS		 "data_status"


using namespace kome::io::msb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// open file
kome::objects::Variant openMsb( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get path
	char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		return ret;
	}

	// create file
	kome::objects::SampleSet* sampleSet = new MsbSampleSet();

	// set file
	ret.type = kome::objects::Variant::SAMPLE_SET;
	ret.prim.pt = sampleSet;

	return ret;
}

// save file
kome::objects::Variant saveMsb( kome::objects::Parameters* params ) {
	// create object
	MsbIO0202 msb;

	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// gets parameters
	kome::objects::DataSet* dataSet = kome::plugin::PluginCallTool::getDataSet( params );
	if( dataSet == NULL ) {
		return ret;
	}

	char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		return ret;
	}

	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	kome::objects::SettingParameterValues* saveSettings = kome::plugin::PluginCallTool::getSettingValues( params );  // @date 2012/08/21 <Add> FUJITA
      

	// @date 2012/09/25 <Add> OKADA ------->
	bool op = false;	
	if( saveSettings == NULL ){
		;
	}else{
		op = saveSettings->getBoolValue( DATA_STATUS,false );
	}
	// @date 2012/09/25 <Add> OKADA <-------

	// write
//	ret.prim.boolVal = msb.writeMsb( path, *dataSet, true, *progress );												 // @date 2012/08/21 <Del> FUJITA
	ret.prim.boolVal = msb.writeMsb( path, *dataSet, op, *progress );											     // @date 2012/08/21 <Add> FUJITA

	return ret;
}
