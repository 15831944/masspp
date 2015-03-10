/**
 * @file SettingsDialogBase.cpp
 * @brief implements of SettingsDialogBase class
 *
 * @author S.Tanaka
 * @date 2013.02.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsDialogBase.h"

#include <wx/treebook.h>


#define GRID_GAP				   3
#define BORDER_SIZE				   3

#define ID_APPLY_BUTTON            7


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( SettingsDialogBase, kome::window::ParametersAndHelpDialog )
    EVT_BUTTON( ID_APPLY_BUTTON, SettingsDialogBase::onApply )
END_EVENT_TABLE()


// constructor
SettingsDialogBase::SettingsDialogBase( wxWindow* parent, const char* title )
		: kome::window::ParametersAndHelpDialog( parent, title ) {
	// initialize
	m_book = NULL;

	// add to the manager
	SettingsManager::getInstance().setDialog( this );
}

// destructor
SettingsDialogBase::~SettingsDialogBase() {
	// unset 
	SettingsManager::getInstance().unsetDialog( this );
}

// add panels
void SettingsDialogBase::addPanels() {
	// This method should be implemented in sub class.
}

// update settings
void SettingsDialogBase::updateSettings() {
	// window -> data
	TransferDataFromWindow();

	// settings
	wxBeginBusyCursor();
	kome::plugin::PluginCallTool::onSettings();
	wxEndBusyCursor();

	// refresh
	kome::window::WindowTool::refresh();
}

// create main sizer
wxSizer* SettingsDialogBase::createMainSizer() {
	// sizer
	wxGridSizer* sizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	// tree book
	m_book = new wxTreebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );
	sizer->Add( m_book, 1, wxALL | wxGROW, BORDER_SIZE );
	
	// add panels
	addPanels();

	return sizer;
}

// create button sizer
wxSizer* SettingsDialogBase::createButtonSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// inner sizer
	wxSizer* innerSizer = new wxBoxSizer( wxHORIZONTAL );

	// button
	wxButton* button = new wxButton( this, ID_APPLY_BUTTON, wxT( "Apply" ) );
	innerSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// default sizer
	wxSizer* dfltSizer = kome::window::StandardDialog::createButtonSizer();
	if( dfltSizer != NULL ) {
		innerSizer->Add( dfltSizer, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	sizer->Add( innerSizer, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return sizer;
}

// on OK
bool SettingsDialogBase::onOK() {
	// update settings
	updateSettings();
   
	// default
	if( !kome::window::StandardDialog::onOK() ) {
		return false;
	}

	return true;
}

// on Cancel
bool SettingsDialogBase::onCancel() {
	return kome::window::StandardDialog::onCancel();
}

// on apply 
void SettingsDialogBase::onApply( wxCommandEvent& evt ) {
	updateSettings();
}


//
// Settings Manager
//

// constructor
SettingsManager::SettingsManager() {
	m_dlg = NULL;
}

// destructor
SettingsManager::~SettingsManager() {
}

// set dialog
void SettingsManager::setDialog( SettingsDialogBase* dlg ) {
	// check the member
	if( dlg == m_dlg ) {
		return;
	}

	// delete
	if( m_dlg != NULL ) {
		delete m_dlg;
	}

	// set
	m_dlg = dlg;
}

// unset dialog
void SettingsManager::unsetDialog( SettingsDialogBase* dlg ) {
	if( m_dlg == dlg ) {
		m_dlg = NULL;
	}
}

// get instance
SettingsManager& SettingsManager::getInstance() {
	static SettingsManager mgr;

	return mgr;
}
