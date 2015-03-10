/**
 * @file TppPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "TppPluginFunctions.h"

#include "TppDialog.h"
#include "TppResultDialog.h"

using namespace kome::Tpp;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// opens Tools > Tpp dialog
kome::objects::Variant openTppDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	if( TppDialog::m_dlg != NULL ) {
		delete TppDialog::m_dlg;
	}

	TppDialog::m_dlg = new TppDialog( kome::window::WindowTool::getMainWindow() );
	TppDialog::m_dlg->ShowModal();

	return ret;
}

// opens View > Results > Tpp dialog
kome::objects::Variant openTppResultDialog( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	if( TppResultDialog::m_dlg != NULL ) {
		delete TppResultDialog::m_dlg;
	}

	TppResultDialog::m_dlg = new TppResultDialog( kome::window::WindowTool::getMainWindow() );
	TppResultDialog::m_dlg->ShowModal();

	return ret;
}
