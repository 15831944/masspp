/**
 * @file SpectraGridViewPluginFunctions.cpp
 * @brief implements of plug-in functions
 *
 * @author okada
 * @date 2011.03.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SpectraGridViewPluginFunctions.h"
#include "SpectraGridViewManager.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// init spectra view
kome::objects::Variant initGridView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// create view
	SpectraGridViewManager& mgr = SpectraGridViewManager::getInstance();
	mgr.createSpectraGridView();

	return ret;
}

// change active object
kome::objects::Variant changeActiveObject( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// active group
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Sample* sample = obj.getSample();

	// select
	SpectraGridViewManager& mgr = SpectraGridViewManager::getInstance();
	mgr.selectSample( sample );

	return ret;
}

// unset sample
kome::objects::Variant unsetSample( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get sample
	kome::objects::Sample* sample = kome::plugin::PluginCallTool::getSample( params );
	if( sample == NULL ) {
		return ret;
	}

	// current sample
	SpectraGridViewManager& mgr = SpectraGridViewManager::getInstance();
	kome::objects::Sample* currentSample = mgr.getCurrentSample();

	// unset	
	if( currentSample == sample ) {
		mgr.selectSample( NULL );
	}

	return ret;
}

// update spectra
kome::objects::Variant updateSpectra( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// spectrum
	kome::objects::DataSet* dataSet = kome::plugin::PluginCallTool::getDataSet( params );
	if( dataSet == NULL ) {
		return ret;
	}

	// update
	SpectraGridViewManager& mgr = SpectraGridViewManager::getInstance();
	mgr.updateView( *dataSet );

	return ret;
}

// update sample
kome::objects::Variant updateSample( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// sample
	kome::objects::Sample* sample = kome::plugin::PluginCallTool::getSample( params );
	if( sample == NULL ) {
		return ret;
	}

	// data set
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::objects::DataSet* dataSet = aoMgr.getFilteredDataSet( sample );
	dataSet->clearSpectra();
	dataSet->clearChromatograms();
	sample->getRootDataGroupNode()->getDataSet( dataSet );

	// update
	SpectraGridViewManager& mgr = SpectraGridViewManager::getInstance();
	mgr.updateView( *dataSet );

	return ret;
}

// toggle the visibility of the spectra view
kome::objects::Variant toggleView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// toggle visibility
	SpectraGridViewManager& mgr = SpectraGridViewManager::getInstance();
	mgr.toggleSpectraGridViewVisibility();

	return ret;
}

// check the visibility of the spectra view
kome::objects::Variant isVisibleView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// check the visibility
	SpectraGridViewManager& mgr = SpectraGridViewManager::getInstance();
	ret.prim.boolVal = mgr.isVisibleSpectraGridView();

	return ret;
}

// exit GUI
kome::objects::Variant exitGui( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// release view
	SpectraGridViewManager& mgr = SpectraGridViewManager::getInstance();
	mgr.releaseView();

	return ret;
}
