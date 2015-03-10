/**
 * @file MatrixPeaksMergeFunction.cpp
 * @brief interfaces of MatrixPeaksMergeFunction plug-in functions
 *
 * @author M.Izumi
 * @date 2013.01.15
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "MatrixPeaksMergeFunction.h"
#include "MatrixPeaksMergePanel.h"

using namespace kome::mergepeaks;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// open peak mage panel
kome::objects::Variant getMergePeakPanel( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	wxWindow* parent = kome::plugin::PluginCallTool::getWindow( params );
	if( parent == NULL ){
		return ret;
	}

	std::string strName;
	strName = kome::plugin::PluginCallTool::getString( params, "panelName", "" );
	
	// panel
	wxWizard* pParent = dynamic_cast<wxWizard*>( parent );

	PeakMagePanel* pane = new PeakMagePanel( pParent, strName.c_str() );

	ret.prim.pt = (kome::window::PeakMatrixSettingsPageBase*)pane;

	return ret;
}


