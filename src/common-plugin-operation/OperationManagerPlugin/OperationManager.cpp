/**
 * @file OperationManager.cpp
 * @brief implements of OperationManager class
 *
 * @author S.Tanaka
 * @date 2007.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "OperationManager.h"
#include "OperationManagerDialog.h"


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
OperationManager::OperationManager() {
}

// destructor
OperationManager::~OperationManager() {
}

// clear
bool OperationManager::clear(){	
	// manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

	// check active objects
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();

	kome::objects::MsDataVariant obj;
	if( frame != NULL ) {
		obj.setVariant( frame->getActiveObject() );
	}

	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	if( spec == NULL && chrom == NULL ) {
		LOG_WARN( FMT( "This operation requires an active 2D data window." ) );
		return false;
	}

	// clear
	if( spec != NULL ) {
		ptMgr.clearOperations( spec );
	}
	if( chrom != NULL ) {
		ptMgr.clearOperations( chrom );
	}

	// refresh
	kome::window::WindowTool::refresh();

	return true;
}

// open dialog
bool OperationManager::openDialog() {
	// manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

	// check active objects
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();

	kome::objects::MsDataVariant obj;
	if( frame != NULL ) {
		obj.setVariant( frame->getActiveObject() );
	}

	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	if( spec == NULL && chrom == NULL ) {
		LOG_WARN( FMT( "An xy data object is not active." ) );
		return false;
	}

	// dialog
	OperationManagerDialog dlg( kome::window::WindowTool::getMainWindow() );
	if( spec != NULL ) {
		dlg.setSpectrum( spec );
	}
	if( chrom != NULL ) {
		dlg.setChromatogram( chrom );
	}

	if( dlg.ShowModal() == wxID_OK ) {	// @date 2011.08.25<Add> M.Izumi
		// delete
		for( unsigned int i = 0; i < dlg.getNumberOfDeleteOperations(); i++ ) {
			if( spec != NULL ) {
				ptMgr.removeOperation( spec, dlg.getDeleteOperation( i ) );
			}
			if( chrom != NULL ) {
				ptMgr.removeOperation( chrom, dlg.getDeleteOperation( i ) );
			}
		}		
	}

	// update
	if( spec != NULL ) {
		kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
	}
	if( chrom != NULL ) {
		kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
	}

	kome::window::WindowTool::refresh();

	return true;
}

// get instance
OperationManager& OperationManager::getInstance() {
	// create object (This is the only object.)
	static OperationManager mgr;

	return mgr;
}
