/**
 * @file VerticalBoxCanvasPluginFunctions.cpp
 * @brief VerticalBoxCanvas Plugin Functions
 * @author OKADA, H.
 * @date 2010.11
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 * 
 */


#include	"stdafx.h"
#include	"VerticalBoxCanvasPluginFunctions.h"
#include	"VerticalBoxSettingsViewManager.h"		// support delete function	@Date:2013/05/23	<Add>	A.Ozaki
#include	"VerticalBoxChromatogramCanvas.h"
#include	"VerticalBoxSpectrumCanvas.h"
#include	"StackedCanvasOperaton.h"

// @date 2011.05.10 <Mod> M.Izumi ->
#define STATUS_PARAM_NAME		"WINDOW"
#define STATUS_PARAM_VALUE		"vertical"
// @date 2011.05.10 <Mod> M.Izumi <-

// @date 2011.09.14 <Add> M.Izumi ->
#define STATUSBAR_DISP			"DispMode"
#define MODE_DISP				"Display Mode: Stacked"
// <-

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// create spectrum canvas
kome::objects::Variant getSpectrumVerticalBox( kome::objects::Parameters* params ){
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::WINDOW;
	
	// get objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}
	wxWindow* parent = kome::plugin::PluginCallTool::getParentWindow( params );
	
	// add operation
	stackCanvasOperation* opt = new stackCanvasOperation();
	opt->setShortName( "spec_stack" );
	opt->setOperationName( "Spectrum Stacked Canvas" );
	opt->setTargetSample( spec->getSample() );
	opt->setOperationType( kome::operation::Operation::TYPE_GUI);
	opt->setSpec( spec );
	opt->setParentWindow( parent );


	// manager
	VerticalBoxSettingsViewManager&	mgr = VerticalBoxSettingsViewManager::getInstance( );
	
	kome::window::VerticalBoxChromatogramCanvas* chromCanvas = NULL;
	if( opt->execute() ){
		ret.prim.pt = opt->getSpecCanvas();

		chromCanvas = opt->getChromCanvas();
		if( chromCanvas == NULL && !mgr.isVisibleVerticalBoxSettingsView() ){
			toggleVerticalBoxDialog( params );
		}
	}else{
		delete opt;
	}

	return	ret;
	// <<<<<< support delete function	@Date:2013/05/23	<Refactor>	A.Ozaki
}

// create chromatgram canvas
kome::objects::Variant getChromatogramVerticalBox( kome::objects::Parameters* params ){
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::WINDOW;
	
	// get objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );

	if( chrom == NULL ) {
		return ret;
	}
	wxWindow* parent = kome::plugin::PluginCallTool::getParentWindow( params );

	// add operation
	stackCanvasOperation* opt = new stackCanvasOperation();
	opt->setShortName( "chrom_stack" );
	opt->setOperationName( "Chromatogram Stacked Canvas" );
	opt->setTargetSample( chrom->getSample() );
	opt->setOperationType( kome::operation::Operation::TYPE_GUI );
	opt->setChrom( chrom );
	opt->setParentWindow( parent );

	kome::window::VerticalBoxSpectrumCanvas* specCanvas = NULL;

	// manager
	VerticalBoxSettingsViewManager&	mgr = VerticalBoxSettingsViewManager::getInstance( );

	if( opt->execute() ){
		ret.prim.pt = opt->getChromCanvas();

		specCanvas = opt->getSpecCanvas();
		if( specCanvas == NULL && !mgr.isVisibleVerticalBoxSettingsView( ) ){
			toggleVerticalBoxDialog( params );
		}
	}else{
		delete opt;
	}
	
	//// <<<<<< support delete function	@Date:2013/05/23	<Refactor>	A.Ozaki
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

kome::objects::Variant updateSettings( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get canvas
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		wxWindow* canvas = kome::window::ChildFrame::getFrame( i )->getCanvas();

		kome::window::VerticalBoxSpectrumCanvas* specCanvas
			= dynamic_cast< kome::window::VerticalBoxSpectrumCanvas* >( canvas );
		if( specCanvas != NULL ) {
			specCanvas->refreshHeight();	// support delete function	@Date:2013/05/23	<Changed>	A.Ozaki
		}

		kome::window::VerticalBoxChromatogramCanvas* chromCanvas
			= dynamic_cast< kome::window::VerticalBoxChromatogramCanvas* >( canvas );
		if( chromCanvas != NULL ) {
			chromCanvas->refreshHeight();	// support delete function	@Date:2013/05/23	<Changed>	A.Ozaki
		}
	}
	
	return ret;
}

// >>>>>> support delete function	@Date:2013/05/23	<Add>	A.Ozaki
//
// init view
kome::objects::Variant	initGui( kome::objects::Parameters* )
{
	// return value
	kome::objects::Variant	ret;

	VerticalBoxSettingsViewManager&	mgr = VerticalBoxSettingsViewManager::getInstance( );
	mgr.createVerticalBoxSettingsView( );

	return	ret;
}

// change active object
kome::objects::Variant	changeActiveObject( kome::objects::Parameters* params )
{
	// return value
	kome::objects::Variant	ret;

	kome::window::ChildFrame*	frame = kome::window::ChildFrame::getActiveFrame( );
	wxWindow*	canvas = ( frame == NULL ? NULL : frame->getCanvas( ) );

	VerticalBoxSettingsViewManager&	mgr = VerticalBoxSettingsViewManager::getInstance( );
	mgr.selectCanvas( dynamic_cast< kome::window::CanvasBase* >( canvas ) );

	return	ret;
}

// exit GUI
kome::objects::Variant	exitGui( kome::objects::Parameters* )
{
	// return value
	kome::objects::Variant	ret;

	// toggle visibility
	VerticalBoxSettingsViewManager&	mgr = VerticalBoxSettingsViewManager::getInstance( );
	mgr.releaseView( );

	return	ret;
}

// toggle the visibility of the vertical box dialog
kome::objects::Variant	toggleVerticalBoxDialog( kome::objects::Parameters* )
{
	// return value
	kome::objects::Variant	ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// toggle visibility
	VerticalBoxSettingsViewManager& mgr = VerticalBoxSettingsViewManager::getInstance( );
	mgr.toggleVerticalBoxSettingsViewVisibility( );

	return	ret;
}

// judge whether the vertical box dialog is shown
kome::objects::Variant	checkVerticalBoxDialog( kome::objects::Parameters* )
{
	// return value
	kome::objects::Variant	ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// check the visibility
	VerticalBoxSettingsViewManager&	mgr = VerticalBoxSettingsViewManager::getInstance( );
	ret.prim.boolVal = mgr.isVisibleVerticalBoxSettingsView( );
	
	return	ret;
}

// キャンバスで選択されたスペクトルを取得
kome::objects::Variant	SelectCanvasOfSpectrum( kome::objects::Parameters* pParams )
{
	// return value
	kome::objects::Variant	ret;
	
	kome::objects::Spectrum*	spec = kome::plugin::PluginCallTool::getSpectrum( pParams );
	
	VerticalBoxSettingsViewManager&	mgr = VerticalBoxSettingsViewManager::getInstance( );

	mgr.setSelectionItem( spec );
	
	return	ret;
}

// キャンバスで選択されたクロマトグラムを取得
kome::objects::Variant	SelectCanvasOfChromatogram( kome::objects::Parameters* pParams )
{
	// return value
	kome::objects::Variant	ret;
	
	kome::objects::Chromatogram*	chrom = kome::plugin::PluginCallTool::getChromatogram( pParams );
	
	VerticalBoxSettingsViewManager&	mgr = VerticalBoxSettingsViewManager::getInstance( );

	mgr.setSelectionItem( chrom );

	return	ret;
}

// <<<<<< support delete function	@Date:2013/05/23	<Add>	A.Ozaki

// get spectrum stacked canvas operation
kome::objects::Variant getSpecStackedCanvasOperation( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new stackCanvasOperation();

	return ret;
}

// get chromatogram staked canvas operation
kome::objects::Variant getChromStackedCanvasOperation( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new stackCanvasOperation();

	return ret;
}
