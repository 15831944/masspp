/**
 * @file SettingsHelpPanel.cpp
 * @brief implements of SettingsHelpPanel class
 *
 * @author S.Tanaka
 * @date 2013.01.09
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SettingsHelpPanel.h"

#include "SettingsPanel.h"

#include "SettingsPanel.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE                     3


BEGIN_EVENT_TABLE( SettingsHelpPanel, ParametersAndHelpPanel )
END_EVENT_TABLE()


// constructor
SettingsHelpPanel::SettingsHelpPanel(
		wxWindow* parent,
		kome::plugin::SettingsPage* page,
		const bool dfltBtn,
		kome::objects::SettingParameterValues* settings,
		const bool saveVal
) : ParametersAndHelpPanel( parent ) {
	// initialize
	m_page = page;
	m_dfltBtn = dfltBtn;
	m_settings = settings;
	m_saveVal = saveVal;
	m_panel = NULL;

	// create controls
	createControls();
}

// destructor
SettingsHelpPanel::~SettingsHelpPanel() {
}

// get panel
SettingsPanel* SettingsHelpPanel::getSettingsPanel() {
	return m_panel;
}

// create top sizer
wxSizer* SettingsHelpPanel::createMainSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// panel
	m_panel = new SettingsPanel( this, m_page, m_dfltBtn, m_settings, m_saveVal );
	sizer->Add( m_panel, 1, wxALL | wxGROW );

	return sizer;
}
