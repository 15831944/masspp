/**
 * @file GraphPanel.cpp
 * @brief implement of GraphPanel class
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "GraphPanel.h"
#include "ChromatogramCanvas.h"


using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP			3
#define BORDER_SIZE			3

#define GRAPH_WIDTH		  480
#define GRAPH_HEIGHT	  150


BEGIN_EVENT_TABLE( GraphPanel, PanelBase )
END_EVENT_TABLE()


// constructor
GraphPanel::GraphPanel( wxWindow* parent ) : PanelBase( parent ) {
	m_controlCanvas = NULL;
	m_sampleCanvas = NULL;
}

// destructor
GraphPanel::~GraphPanel() {
}

// create main sizer
wxSizer* GraphPanel::createMainSizer() {
	// create sizer
	wxGridSizer* sizer = new wxGridSizer( 2, 1, GRID_GAP, GRID_GAP );

	// control canvas
	m_controlCanvas = new ChromatogramCanvas( this, GRAPH_WIDTH, GRAPH_HEIGHT, true );
	sizer->Add( m_controlCanvas, 1, wxGROW | wxALL, BORDER_SIZE );

	// sample canvas
	m_sampleCanvas = new ChromatogramCanvas( this, GRAPH_WIDTH, GRAPH_HEIGHT, false );
	sizer->Add( m_sampleCanvas, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}
