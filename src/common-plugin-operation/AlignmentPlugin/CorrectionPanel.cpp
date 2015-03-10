/**
 * @file CorrectionPanel.cpp
 * @brief implements of CorrectionPanel class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CorrectionPanel.h"
#include "CorrectionCanvas.h"


using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP				3
#define BORDER_SIZE			 3



BEGIN_EVENT_TABLE( CorrectionPanel, PanelBase )
END_EVENT_TABLE()


// constructor
CorrectionPanel::CorrectionPanel( wxWindow* parent ) : PanelBase( parent ) {
	m_canvas = NULL;
}

// destructor
CorrectionPanel::~CorrectionPanel() {
}

// create main sizer
wxSizer* CorrectionPanel::createMainSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// canvas
	m_canvas = new CorrectionCanvas( this );
	sizer->Add( m_canvas, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}
