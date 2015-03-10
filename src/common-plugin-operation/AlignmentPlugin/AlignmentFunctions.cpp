/**
 * @file AlignmentFunctions.cpp
 * @brief implements of Filter plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"

#include "AlignmentManager.h"
#include "AlignmentDialog.h"
#include "AlignmentOperation.h"

#include <wx/wx.h>
#include <wx/utils.h>

using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// subtract alignment
kome::objects::Variant openAlignmentDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// initialize
	mgr.clearChromatograms();
	mgr.clearRange();

	// dialog
	kome::objects::SettingParameterValues settings;
	AlignmentDialog dlg( kome::window::WindowTool::getMainWindow(), &settings );
	dlg.ShowModal();

	// clear chromatogram
	mgr.clearChromatograms();
	mgr.ReinstateTicValue();    // @date 2013.05.14 <Add> Fujita   controlÅAtreatmentÇÃscaled TICÇraw TICÇ…íuÇ´ä∑Ç¶

	ret.prim.boolVal = true;
	return ret;
}

// get alignment operation
kome::objects::Variant getAlignmentOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new AlignmentOperation();
	
	return ret;
}
