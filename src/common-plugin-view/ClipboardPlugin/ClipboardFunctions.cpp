/**
 * @file ClipboardFunctions.cpp
 * @brief implements of Clipboard plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.05.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd, Laboratory of Core Technology All rights reserved.
 */


#include "stdafx.h"
#include "ClipboardFunctions.h"
#include "ClipboardManager.h"

using namespace kome::clipboard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// copy text data
kome::objects::Variant copyTextData( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	if( spec != NULL && chrom != NULL ) {
		LOG_WARN( FMT( "Failed to get the active 2D data object." ) );
		return ret;
	}

	kome::core::XYData* xyData = NULL;
	if( spec != NULL ) {
		xyData = spec->getXYData();
	}
	else if( chrom != NULL ) {
		xyData = chrom->getXYData();
	}

	// copy
	ClipboardManager& mgr = ClipboardManager::getInstance();
	if( xyData != NULL ) {
		mgr.copyXyDataText( *xyData );
	}

	ret.prim.boolVal = true;
	return ret;
}

// copy bitmap data
kome::objects::Variant copyBitmapData( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// active canvas
	ClipboardManager& mgr = ClipboardManager::getInstance();
	kome::window::Canvas* canvas = mgr.getActiveCanvas();
	if( canvas == NULL ) {
		LOG_WARN( FMT( "Failed to get the active data object." ) );
		return ret;
	}

	// copy
	mgr.copyBitmap( *canvas );

	ret.prim.boolVal = true;
	return ret;
}

// copy metafile data
kome::objects::Variant copyMetafileData( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

#ifdef __unix
	LOG_ERROR( FMT( "This function is not implemented under UNIX." ) );
#else
	// active canvas
	ClipboardManager& mgr = ClipboardManager::getInstance();
	kome::window::Canvas* canvas = mgr.getActiveCanvas();
	if( canvas == NULL ) {
		LOG_WARN( FMT( "Failed to get the active data object." ) );
		return ret;
	}

	// copy
	mgr.copyMetafile( *canvas );

	ret.prim.boolVal = true;
#endif	// __unix

	return ret;
}

// @date 2011.08.23 <Add> M.Izumi ->
// 3D Viewは次期バージョン以降でクリップボードにBitmapコピー機能追加
// 2.0.0では、可視で対応
// SPEC 82135:Cannot copy the 3D display data to the clipboard 

// check the visibility Bitmap
kome::objects::Variant isVisibleBitmap( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ){
		kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );
		if( canvas != NULL ){
			ret.prim.boolVal = true;
		}
	}
	
	return ret;
}

// check the visibility Metafile
kome::objects::Variant isVisibleMetafile( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ){
		kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );
		if( canvas != NULL ){
			ret.prim.boolVal = true;
		}
	}
	
	return ret;
}

// @date 2011.08.23 <Add> M.Izumi <-
