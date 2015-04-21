/**
 * @file HelpPanel.cpp
 * @brief implements of HelpPanel class
 *
 * @author S.Tanaka
 * @date 2014.01.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "HelpPanel.h"

#include "BitmapCanvas.h"

#include <wx/splitter.h>
#include <wx/statline.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						3
#define GRID_GAP						3


BEGIN_EVENT_TABLE( HelpPanel, ParametersAndHelpPanel )
END_EVENT_TABLE()


// constructor
HelpPanel::HelpPanel( wxWindow* parent, const int id ) : ParametersAndHelpPanel( parent, id ) {
	createControls();
}

// destructor
HelpPanel::~HelpPanel() {
}

// main sizer
wxSizer* HelpPanel::createMainSizer() {
	return new wxBoxSizer( wxVERTICAL );
}
