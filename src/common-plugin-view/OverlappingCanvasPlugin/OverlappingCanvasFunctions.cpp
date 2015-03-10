/**
 * @file OverlappingCanvasFunctions.cpp
 * @brief implements of functions of OverlappingCanvas plug-in
 *
 * @author S.Tanaka
 * @date 2008.03.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "OverlappingCanvasFunctions.h"
#include "OverlappingSettingsViewManager.h"
#include "OverlappingSpectraCanvasEx.h"
#include "OverlappingChromatogramsCanvasEx.h"
#include "OverlappingCanvasOperation.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// @date 2011.05.10 <Mod> M.Izumi ->
#define STATUS_PARAM_NAME	  "WINDOW"
#define STATUS_PARAM_VALUE	  "Overlapping"
// <-

// @date 2011.09.14 <Add> M.Izumi ->
#define STATUSBAR_DISP		"DispMode"
#define MODE_DISP			"Display Mode: Overlapping"
// <-

// init view
kome::objects::Variant initGui( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	mgr.createOverlappingSettingsView();

	return ret;
}

// change active object
kome::objects::Variant changeActiveObject( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* canvas = ( frame == NULL ? NULL : frame->getCanvas() );

	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	mgr.selectCanvas( dynamic_cast< kome::window::CanvasBase* >(canvas) );

	return ret;
}

// create spectrum canvas
kome::objects::Variant getSpectrumCanvas( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::WINDOW;

	// manager
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();

	// get objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}
	wxWindow* parent = kome::plugin::PluginCallTool::getParentWindow( params );

	// add operation
	OverlappingCanvasOperation* opt = new OverlappingCanvasOperation();
	opt->setShortName("spec_overlap");
	opt->setOperationName("Spectrum Overlapping Canvas");
	opt->setTargetSample( spec->getSample() );
	opt->setOperationType( kome::operation::Operation::TYPE_GUI );
	opt->setSpec( spec );
	opt->setParentWindow( parent );

	kome::window::OverlappingChromatogramsCanvasEx* chromCanvas = NULL;
	if( opt->execute() ){

		ret.prim.pt = opt->getSpecCanvas();

		chromCanvas = opt->getChromCanvas();
		if( chromCanvas == NULL && !mgr.isVisibleOverlappingSettingsView() ) {
			// SPEC No. 85758 @date 2012.04.19 <Mod> M.Izumi
			toggleOverlappingDialog( params );
		}
	}else{
		delete opt;
	}
	
	return ret;
}

// create chromatogram canvas
kome::objects::Variant getChromatogramCanvas( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::WINDOW;

	// manager
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();

	// get objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}
	wxWindow* parent = kome::plugin::PluginCallTool::getParentWindow( params );
		
	// add operation
	OverlappingCanvasOperation* opt = new OverlappingCanvasOperation();
	opt->setShortName("chrom_overlap");
	opt->setOperationName("Chromatogram Overlapping Canvas");
	opt->setTargetSample( chrom->getSample() );
	opt->setOperationType( kome::operation::Operation::TYPE_GUI );
	opt->setChrom( chrom );
	opt->setParentWindow( parent );

	kome::window::OverlappingSpectraCanvasEx* specCanvas = NULL;
	if( opt->execute() ){
		ret.prim.pt = opt->getChromCanvas();	
		
		specCanvas = opt->getSpecCanvas();
		if( specCanvas == NULL && !mgr.isVisibleOverlappingSettingsView() ) {
			// SPEC No. 85758 @date 2012.04.19 <Mod> M.Izumi
			toggleOverlappingDialog( params );
		}
	}else{
		delete opt;
	}
		
	return ret;
}

// chromatogram,specturumのset status関数を1つにしました @date 2011.05.10 <Mod> M.Izumi
// set status
kome::objects::Variant setSpecAndChromStatus( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// set window status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	std::string currentStatus = statusMgr.getValue( STATUS_PARAM_NAME );
	if( currentStatus.compare( STATUS_PARAM_VALUE ) != 0 ) {
		statusMgr.setValue( STATUS_PARAM_NAME, STATUS_PARAM_VALUE );

		std::string message = " This menu item is for just setting the display mode.\n";
		message.append( " If you want to display a spectrum/chromatogram with the specified mode,\n" );
		message.append( "open a spectrum/chromatogram (Double-click an item in the sample tree view)\n" );
		message.append( "after setting the display mode." );

		kome::window::MessageCheckDialog::openDialogAfterChecking(
			kome::window::WindowTool::getMainWindow(),
			"Information",
			message.c_str(),
			"SPEC_CHROM_CANVAS_MODE"
		);
	}
	
	// ステータスバーに状態をセット　Display Mode @date 2011.09.15 <Add> M.Izumi
	kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance(); 
	statusBar.setFlgDisp( STATUSBAR_DISP, 1);
	statusBar.setStatusMsg(STATUSBAR_DISP, MODE_DISP);
	
	return ret;
}

// toggle the visibility of the overlapping dialog
kome::objects::Variant toggleOverlappingDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// toggle visibility
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	mgr.toggleOverlappingSettingsViewVisibility();

	return ret;
}

// judge whether the overlapping dialog is shown
kome::objects::Variant checkOverlappingDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// check the visibility
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	ret.prim.boolVal = mgr.isVisibleOverlappingSettingsView();

	return ret;
}

// exit GUI
kome::objects::Variant exitGui( kome::objects::Parameters* ) {

	// return value
	kome::objects::Variant ret;

	// toggle visibility
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	mgr.releaseView();

	return ret;
}

// キャンバスで選択されたスペクトルを取得  @date 2011.10.26 <Add> M.Izumi
kome::objects::Variant SelectCanvasOfSpectrum( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	mgr.setSelectionItem( spec );
	
	return ret;
}

// キャンバスで選択されたクロマトグラムを取得  @date 2011.10.26 <Add> M.Izumi
kome::objects::Variant SelectCanvasOfChromatogram( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	mgr.setSelectionItem( chrom );
	

	return ret;
}

// @date 2012/12/06 <Add> OKADA
kome::objects::Variant onSpecKeyUp( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get parameters
	kome::evt::KeyEvent* keyEvent = kome::plugin::PluginCallTool::getKeyEvent( params );
	if( keyEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return ret;
	}

	// on button up
	// mgrの処理を呼び出す
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	mgr.onSpecKeyUpMgr( spectrum );
	
	return ret;
}

// get spec overlap canvas operation
kome::objects::Variant getSpecOverlapCanvasOperation( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new OverlappingCanvasOperation();

	return ret;
}
	
// get chrom oberlap canvas operation
kome::objects::Variant getChromOverlapCanvasOperation( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new OverlappingCanvasOperation();

	return ret;
}
