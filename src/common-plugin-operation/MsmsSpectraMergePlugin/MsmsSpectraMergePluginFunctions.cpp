/**
 * @file MsmsSpectraMergePluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.01.16
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MsmsSpectraMergePluginFunctions.h"
#include "MsmsMerge.h"


using namespace kome::operation::msmsmerge;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// merge MS/MS spectra
kome::objects::Variant mergeMs2Spectra( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// merge object
	MsmsMerge& merge = MsmsMerge::getInstance();

	// sample
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Sample* sample = obj.getSample();
	if( sample == NULL ) {
		LOG_ERROR( FMT( "A sample is not active." ) );
		return ret;
	}

	ret.prim.boolVal = true;

	// dialog
	double rtTol = double();
	double mzTol = double();
	if( !merge.openParametersDialog( &rtTol, &mzTol ) ) {
		return ret;
	}

	// merge
	kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Merge MS/MS Spectra" );
	merge.mergeSpectra( sample, rtTol, mzTol, progress );

	kome::plugin::PluginCallTool::onUpdateSample( *sample );
	kome::plugin::PluginCallTool::onUpdateGroup( *( sample->getRootDataGroupNode() ) );
	kome::plugin::PluginCallTool::onUpdateDataSet( *( sample->getRootDataGroupNode() ) );

	return ret;
}
