/**
 * @file MzmlIOPluginFunctions.cpp
 * @brief implements of MzmlIO plug-in function
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzmlIOPluginFunctions.h"
#include "MzmlSample.h"
#include "MzmlManager.h"
#include "MzmlSampleSet.h"

#define DATA_STATUS		 "data_status"


using namespace kome::io::mzml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// open file
kome::objects::Variant openMzml( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::SAMPLE_SET;
	ret.prim.pt = NULL;

	// get path
	const char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		return ret;
	}

	// open
	MzmlSampleSet* sampleSet = new MzmlSampleSet();

	// set file
	ret.prim.pt = sampleSet;

	return ret;
}
                         
// save file
kome::objects::Variant saveMzml( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

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
       
	// write
	MzmlManager& mgr = MzmlManager::getInstance();
	ret.prim.boolVal = mgr.exportDataSet( path, *dataSet, *progress, saveSettings );
	return ret;
}
