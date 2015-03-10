/**
 * @file MatrixGroupPlot.cpp
 * @brief implements of MatrixGroupPlot plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.07.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "MatrixGroupPlotFunctions.h"
#include "MatrixGroupPlotManager.h"

using namespace kome::matrix::groupplot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// show the groupplot view
kome::objects::Variant showGroupPlot( kome::objects::Parameters* params ) {
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
	MatrixGroupPlotManager& mgr = MatrixGroupPlotManager::getInstance();
	mgr.createPanel( parent, peak );

	// window
	ret.prim.pt = mgr.getPanel();

	return ret;
}
