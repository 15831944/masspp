/**
 * @file SampleTreeViewFunctions.cpp
 * @brief SampleTreeView plug-in function implements
 *
 * @author S.Tanaka
 * @date 2008.01.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SampleTreeViewFunctions.h"
#include "SampleTreeManager.h"

#include <wx/treectrl.h>
#include <wx/notebook.h>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define RAW_TREE_STATUS_NAME	"RAW_TREE"


// initialize tree view
kome::objects::Variant initView( kome::objects::Parameters* ) {
	// return value;
	kome::objects::Variant ret;

	// get managers
	SampleTreeManager& treeMgr = SampleTreeManager::getInstance();
	kome::objects::StatusManager&  statusMgr = kome::objects::StatusManager::getInstance();

	// set status
	statusMgr.setValue(		RAW_TREE_STATUS_NAME, "on" );
	
	// @date 2011.05.10 <Add> M.Izumi
	treeMgr.createNoteBook();	
	wxNotebook* book = treeMgr.getNoteBook();

	// add dock
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame != NULL ) {
		mainFrame->appendPane( book, "Sample Tree", kome::window::WindowTool::DOCK_ALIGN_LEFT );
	}

	return ret;
}

// finalize view
kome::objects::Variant finalizeView( kome::objects::Parameters* ) {
	// return value;
	kome::objects::Variant ret;

	// get tree manager object
	SampleTreeManager& treeMgr = SampleTreeManager::getInstance();

	// set status
	kome::objects::StatusManager&  statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue(	RAW_TREE_STATUS_NAME, "on" );

	// release
	treeMgr.releaseView();

	return ret;
}

// toggle visibility
kome::objects::Variant toggleTreeView( kome::objects::Parameters* ) {
	// return value;
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get tree manager object
	SampleTreeManager& treeMgr = SampleTreeManager::getInstance();

	// init view
	if( !treeMgr.hasView() ) {
		initView( NULL );
		return ret;
	}

	// @date 2011.04.25 <Mod> M.Izumi
	wxNotebook* book = treeMgr.getNoteBook();
	if( book ==NULL ){
		return ret;
	}

	// toggle visibility
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame != NULL ) {
		// @date 2011.04.25 <Mod> M.Izumi
		mainFrame->toggleVisibleBar( book );
	}

	return ret;
}

// check visibility
kome::objects::Variant isVisibleTreeView( kome::objects::Parameters* params ) {
	// return value;
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// check the visibility
	SampleTreeManager& treeMgr = SampleTreeManager::getInstance();
	// @date 2011.04.25 <Mod> M.Izumi
	wxNotebook* book = treeMgr.getNoteBook();

	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	// @date 2011.04.25 <Mod> M.Izumi
	if( mainFrame == NULL || book == NULL ) {
		ret.prim.boolVal = false;
	}
	else {
		// @date 2011.04.25 <Mod> M.Izumi
		ret.prim.boolVal = mainFrame->isVisibleBar( book );
	}

	return ret;
}

// add sample node
kome::objects::Variant addSampleNode( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get tree manager object
	SampleTreeManager& treeMgr = SampleTreeManager::getInstance();

	// get sample
	kome::objects::Sample* sample = kome::plugin::PluginCallTool::getSample( params );
	if( sample == NULL ) {
		return ret;
	}
	
	
	// open sample tree view
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( kome::window::WindowTool::getMainWindow() );
	wxNotebook* book = treeMgr.getNoteBook();
	if( mainFrame != NULL && book != NULL ) {
		// @date 2011.04.25 <Mod> M.Izumi
		treeMgr.createTreeCtrl( sample );

		mainFrame->showBar( book );
	}

	return ret;
}

// update group
kome::objects::Variant updateGroup( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	SampleTreeManager& treeMgr = SampleTreeManager::getInstance();

	// group
	kome::objects::DataGroupNode* group = kome::plugin::PluginCallTool::getGroup( params );
	if( group == NULL ) {
		return ret;
	}

	// update nodes
	treeMgr.updateNodes( group );

	return ret;
}

// update sample
kome::objects::Variant updateSample( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	SampleTreeManager& treeMgr = SampleTreeManager::getInstance();

	// sample
	kome::objects::Sample* sample = kome::plugin::PluginCallTool::getSample( params );
	if( sample == NULL ) {
		return ret;
	}

	// update sample node
	treeMgr.updateSampleNode( sample );

	return ret;
}

// select sample page
kome::objects::Variant selectSamplePage( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	SampleTreeManager& treeMgr = SampleTreeManager::getInstance();

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	kome::objects::Sample* sample = obj.getSample();
	if( sample != NULL ) {
		treeMgr.selectSample( sample );
	}

	return ret;
}

// navigates to the spectrum on open
kome::objects::Variant updateSpectrum( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// manager
	SampleTreeManager& treeMgr = SampleTreeManager::getInstance();

	treeMgr.selectSpectrum( spec );

	return ret;
}

// on activate
kome::objects::Variant onActivateObject( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	SampleTreeManager& mgr = SampleTreeManager::getInstance();

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	// select
	if( chrom != NULL ) {
		mgr.selectChromatogram( chrom );
	}
	else {
		mgr.selectSpectrum( spec );
	}

	return ret;
}
