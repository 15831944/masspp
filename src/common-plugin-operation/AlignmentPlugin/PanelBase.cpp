/**
 * @file PanelBase.cpp
 * @brief implements of PanelBase class
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PanelBase.h"


using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( PanelBase, kome::window::StandardPanel )
END_EVENT_TABLE()


// constructor
PanelBase::PanelBase( wxWindow* parent ) : kome::window::StandardPanel( parent ) {
}

// destructor
PanelBase::~PanelBase() {
}

// >>>>>>	@Date:2014/02/04	<Add>	A.Ozaki
//
void	PanelBase::createWidgets( void )
{
	createControls( );

	return;
}
//
// <<<<<<	@Date:2014/02/04	<Add>	A.Ozaki
