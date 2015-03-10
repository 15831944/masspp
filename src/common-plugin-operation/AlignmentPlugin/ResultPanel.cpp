/**
 * @file ResultPanel.cpp
 * @brief implements of ResultPanel class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ResultPanel.h"
#include "ResultCanvas.h"


using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP				3
#define BORDER_SIZE			 3



BEGIN_EVENT_TABLE( ResultPanel, PanelBase )
END_EVENT_TABLE()


// constructor
ResultPanel::ResultPanel( wxWindow* parent ) : PanelBase( parent ) {
	m_canvas = NULL;
}

// destructor
ResultPanel::~ResultPanel() {
}

// create main sizer
wxSizer* ResultPanel::createMainSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// canvas
	m_canvas = new ResultCanvas( this );
	sizer->Add( m_canvas, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}
