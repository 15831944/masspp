/**
 * @file DetailsViewFunctions.cpp
 * @brief implements of DetailsView plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.02.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "DetailsViewFunctions.h"
#include "DetailsViewManager.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// init file details view
kome::objects::Variant initPropertiesView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// create view
	DetailsViewManager& mgr = DetailsViewManager::getInstance();
	mgr.createPropertiesView();

	return ret;
}

// on activate
kome::objects::Variant changeActiveObject( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// active data
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	kome::objects::DataGroupNode* group = obj.getGroup();

	// select
	DetailsViewManager& mgr = DetailsViewManager::getInstance();
	if( spec != NULL ) {
		mgr.selectSpec( spec );
	}
	else if( chrom != NULL ) {
		mgr.selectChrom( chrom );
	}
	else {
		mgr.selectGroup( group );
	}

	return ret;
}

// update spectrum
kome::objects::Variant updateSpecDetails( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// update
	DetailsViewManager& mgr = DetailsViewManager::getInstance();
	mgr.updateSpectrum( spec );

	return ret;
}

// update chromatogram
kome::objects::Variant updateChromDetails( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get chromatogram
	kome::objects::Chromatogram* chro = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chro == NULL ) {
		return ret;
	}

	// update
	DetailsViewManager& mgr = DetailsViewManager::getInstance();
	mgr.updateChromatogram( chro );

	return ret;
}

// toggle visibility of the file details view
kome::objects::Variant togglePropertiesView( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// toggle visibility
	DetailsViewManager& mgr = DetailsViewManager::getInstance();
	mgr.togglePropertiesVisibility();

	return ret;
}

// check the visibility of the file details view
kome::objects::Variant isVisibleProperties( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// check the visibility
	DetailsViewManager& mgr = DetailsViewManager::getInstance();
	ret.prim.boolVal = mgr.isVisiblePropertiesView();

	return ret;
}

// exit GUI
kome::objects::Variant exitGui( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// release view
	DetailsViewManager& mgr = DetailsViewManager::getInstance();
	mgr.releaseView();

	return ret;
}
