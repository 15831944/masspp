/**
 * @file ProfileCanvasFunctions.cpp
 * @brief implements of functions of ProfileCanvas plug-in
 *
 * @author S.Tanaka
 * @date 2007.09.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ProfileCanvasFunctions.h"
#include "ProfileCanvasOperation.h"
#include "ProfileCanvasManager.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define STATUS_PARAM_NAME	  "WINDOW"
#define STATUS_PARAM_VALUE	  "Canvas"

#define STATUSBAR_DISP		  "DispMode"
#define MODE_DISP			  "Display Mode: Single"

// create spectrum canvas
kome::objects::Variant createSpectrumCanvas( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::WINDOW;

	// get objects
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}
	wxWindow* parent = kome::plugin::PluginCallTool::getParentWindow( params );

	// ---------------------------
	// Operaton Log追加
	// ---------------------------
	ProfileCanvasOperation* opt = new ProfileCanvasOperation();
	opt->setShortName( "spec_profile" );
	opt->setOperationName( "Spectrum Profile Canvas" );
	opt->setOperationType( kome::operation::Operation::TYPE_GUI );
	opt->setSpec( spec );
	opt->setParentWindow( parent );
	opt->setTargetSample( spec->getSample() );

	if( opt->execute() ){
		ret.prim.pt = opt->getSpecCanvas();
	}else{
		delete opt;
	}

	//ret.prim.pt = canvas;
	return ret;
}

// create chromatogram canvas
kome::objects::Variant createChromatogramCanvas( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::WINDOW;

	// get objects
	kome::objects::Chromatogram* chrom = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chrom == NULL ) {
		return ret;
	}
	wxWindow* parent = kome::plugin::PluginCallTool::getParentWindow( params );

	// ---------------------------
	// Operaton Log追加
	// ---------------------------
	ProfileCanvasOperation* opt = new ProfileCanvasOperation();
	opt->setShortName( "chrom_profile" );
	opt->setOperationName( "Chromatogram Profile Canvas" );
	opt->setOperationType( kome::operation::Operation::TYPE_GUI );
	opt->setChrom( chrom );
	opt->setParentWindow( parent );
	opt->setTargetSample( chrom->getSample() );

	if( opt->execute() ){
		ret.prim.pt = opt->getChromCanvas();
	}else{
		delete opt;
	}

	//ret.prim.pt = canvas;
	return ret;
}

// get spec profile canvas operation
kome::objects::Variant getSpecProfileCanvasOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new ProfileCanvasOperation();
	
	return ret;
}

// get chrom profile canvas operation
kome::objects::Variant getChromProfileCanvasOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new ProfileCanvasOperation();
	
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
		message.append( "open a spectrum/chromatogram (double-click an item in the sample tree view)\n" );
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

