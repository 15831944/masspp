/**
 * @file MatrixOverlappingView.cpp
 * @brief implements of MatrixOverlappingView plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.06.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MatrixOverlappingViewFunctions.h"
#include "MatrixOverlappingViewManager.h"

using namespace kome::matrix::overlapping;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




// show the overlapping view
kome::objects::Variant showOverlappingView( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::WINDOW;
	ret.prim.pt = NULL;

	// peak ID
	int peak = kome::plugin::PluginCallTool::getInt( params, "peak", -1 );
	if( peak < 0 ) {
		return ret;
	}

	wxWindow* parent = kome::plugin::PluginCallTool::getParentWindow( params );
	if( parent == NULL ) {
		return ret;
	}
		
	// Spec No.88151 Šg‘åŽ¸”s (Quantiontation - Overlapping View)  @date 2012.10.16 <Mod> M.Izumi
	MatrixOverlappingViewManager& mgr = MatrixOverlappingViewManager::getInstance();
	mgr.createPanel( parent, peak );
		
	// window
	ret.prim.pt = mgr.getPanel();

	return ret;
}
