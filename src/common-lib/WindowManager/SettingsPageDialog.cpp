/**
 * @file SettingsPageDialog.cpp
 * @brief implements of SettingsPageDialog class
 *
 * @author S.Tanaka
 * @date 2007.08.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsPageDialog.h"

#include "SettingsHelpPanel.h"
#include "SettingsPanel.h"
#include "WindowTool.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( SettingsPageDialog, StandardDialog )
END_EVENT_TABLE()


// open dialog
bool openSettingsPageDialog(
		kome::plugin::SettingsPage* page,
		kome::objects::SettingParameterValues* settings,
		const bool saveVal,
		const char* prefix,
		const bool dfltBtnFlg,
		const char* commitBtnName
) {
	// dialog
	SettingsPageDialog dlg(
		WindowTool::getMainWindow(),
		page,
		settings,
		saveVal,
		prefix,
		dfltBtnFlg,
		commitBtnName
	);

	return ( dlg.ShowModal() == wxID_OK );
}

// constructor
SettingsPageDialog::SettingsPageDialog(
		wxWindow* parent,
		kome::plugin::SettingsPage* page,
		kome::objects::SettingParameterValues* settings,
		const bool saveVal,
		const char* prefix,
		const bool dfltBtnFlg,		// [DEFAULT] button flag  true:display  false:not dislpay
		const char* commitBtnName
) : StandardDialog( parent, page == NULL ? "" : page->getTitle(), commitBtnName ), m_saveVal( saveVal ) {
	// initialize
	m_page = page;
	m_settings = settings;
	m_prefix = NVL( prefix, "" );
	m_edited = false;
	m_dfltBtnFlg = dfltBtnFlg;
	m_panel = NULL;
}

// destructor
SettingsPageDialog::~SettingsPageDialog() {
}

// set edited flag
void SettingsPageDialog::setEdited( const bool edited ) {
	m_edited = edited;
}

// get edited flag
bool SettingsPageDialog::isEdited() {
	return m_edited;
}

// main sizer
wxSizer* SettingsPageDialog::createMainSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// panel
	m_page->setCommon( false );
	m_panel = new SettingsHelpPanel(
		this,
		m_page,
		m_dfltBtnFlg,
		m_settings,
		m_saveVal
	);

	sizer->Add( m_panel, 1, wxALL | wxGROW );

	return sizer;
}

// window -> value
bool SettingsPageDialog::TransferDataFromWindow() {
	// default
	if( !StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// edited
	m_edited = m_panel->getSettingsPanel()->isEdited();

	return true;
}
