/**
 * @file SpecChromWinModePluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.05.21
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SpecChromWinModePluginFunctions.h"

#include "SpecChromWindowManager.h"


#define STATUS_NAME      "WINDOW_MODE"
#define STATUS_VALUE     "SPEC_CHROM"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// on client size
kome::objects::Variant onClientSize( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// event
	kome::evt::SizeEvent* evt = kome::plugin::PluginCallTool::getSizeEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// set
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();
	mgr.setClientSize( evt->width(), evt->height() );
	mgr.layoutChildren();

	return ret;
}

// set window status
kome::objects::Variant setSpecChromWindowStatus( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// managers
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// check status
	const char* s = statusMgr.getValue( STATUS_NAME );
	std::string status = NVL( s, "" );
	if( status.compare( STATUS_VALUE ) == 0 ) {
		return ret;
	}

	// close all window
	while( kome::window::ChildFrame::getNumberOfFrames() > 0 ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( 0 );
		frame->Close();
		frame->Destroy();
		delete frame;
	}
	mgr.init();

	// set status
	statusMgr.setValue( STATUS_NAME, STATUS_VALUE );

	return ret;
}

// open spectrum
kome::objects::Variant openSpectrumFrame( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum ( params );
	if( spec == NULL ) {
		return ret;
	}

	// open
	mgr.openFrame( spec );

	return ret;
}

// open chromatogram
kome::objects::Variant openChromatogramFrame( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();

	// chromatogram
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}

	// open
	mgr.openFrame( chrom );

	return ret;
}

// open data map frame
kome::objects::Variant openDataMapFrame( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();	// return value

	// data map
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	// open
	mgr.openFrame( dataMap );

	return ret;
}

// chromatogram key down
kome::objects::Variant onChromKeyDown( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get parameters
	kome::evt::KeyEvent* evt = kome::plugin::PluginCallTool::getKeyEvent( params );
	if( evt == NULL ) {
		return ret;
	}

	// manager
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();
	if( mgr.isLinkMode() ) {
		ret.prim.boolVal = true;
		if( evt->getCode() == kome::core::key::KEY_RIGHT ) {
			mgr.nextSpectrum();
		}
		else if( evt->getCode() == kome::core::key::KEY_LEFT ) {
			mgr.prevSpectrum();
		}
		else {
			ret.prim.boolVal = false;
		}
	}

	return ret;
}

// close all child frames
kome::objects::Variant closeAllChildFrames( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// close all
	SpecChromWindowManager& mgr = SpecChromWindowManager::getInstance();
	mgr.closeAllChildren();

	return ret;
}
