/**
 * @file BackgroundSubtractFunctions.cpp
 * @brief implements of BackgroundSubtract plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.12.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BackgroundSubtractFunctions.h"
#include "BackgroundSubtractManager.h"
#include "BackgroundSubtractDialog.h"
#include "BackgroundOperation.h"
#include "BackgroundSubtraction.h"

using namespace kome::background;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// subtract background
kome::objects::Variant openBackgroundDialog( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();

	// active spectrum
	kome::objects::Variant acObj = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( acObj );
	kome::objects::Spectrum* spec = obj.getSpectrum();

	if( spec == NULL ) {
		LOG_ERROR( FMT( "A spectrum is not active." ) );
		return ret;
	}

	// set background spectrum
	mgr.setBackground( spec );
	mgr.openDialog();

	ret.prim.boolVal = true;
	return ret;
}

// draw background label
kome::objects::Variant drawBackgroundLabel( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// graphics
	kome::img::Graphics* g = kome::plugin::PluginCallTool::getGraphics( params );
	if( g == NULL ) {
		return ret;
	}

	// draw
	mgr.drawLabel( *spec, *g );

	return ret;
}

// open spectrum canvas		@date 2011.07.14 <Add> M.Izumi
kome::objects::Variant openSpectrumCanvas( kome::objects::Parameters* params ){
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
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	mgr.AddSpectrumCanvas(  );

	return ret;
}

// close spectrums canvas	@date 2011.07.14 <Add> M.Izumi
kome::objects::Variant closeSpectrumCanvas( kome::objects::Parameters* params ){
	kome::objects::Variant ret;
	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ){
		return ret;
	}

	// close
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	mgr.closeSpecCanvas( spec );
	
	return ret;
}

// Main Frame(Mass++) ‚ð•Â‚¶‚½Žž
kome::objects::Variant closeAllSpectrumCanvas( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	// unset background spectrum
	BackgroundSubtractManager& mgr = BackgroundSubtractManager::getInstance();
	mgr.closeMainFrame();
	
	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}

// get background operation
kome::objects::Variant getBackgroundOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new BackgroundOperation();
	
	return ret;
}

// get spec xydata operation ( Background Subtraction )
kome::objects::Variant getSpecXYDataOperation( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// get settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}
		
	std::string strName = settings->getParameterName( 0 );
	std::string val = settings->getParameterValue( strName.c_str() );
	
	std::vector< std::string > tmp;
	stringtoken( val.c_str(), ":", tmp );

	double min = todouble( tmp[0].c_str(), -1.0 );
	double max = todouble( tmp[1].c_str(), -1.0 );

	if( min < -1.0 || max < -1.0 ){
		return ret;
	}
			
	// background subtraction object
	BackgroundSubtraction* background = new BackgroundSubtraction();
	background->setRt( min, max );
	
	// get xyDataOperation
	ret.prim.pt = background;

	return ret;
}
