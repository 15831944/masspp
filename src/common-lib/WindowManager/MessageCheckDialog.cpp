/**
 * @file MessageCheckDialog.cpp
 * @brief implements of MessageCheckDialog class
 *
 * @author S.Tanaka
 * @date 2013.02.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#include "stdafx.h"
#include "MessageCheckDialog.h"

#include "CollapsiblePane.h"

#include <wx/artprov.h>
		

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						   3
#define GRID_GAP						   3

#define ID_CHECKBOX                        9

#define SECTION                            "MessageBoxCheck"



BEGIN_EVENT_TABLE( MessageCheckDialog, MessageDialog )
END_EVENT_TABLE()


// constructor
MessageCheckDialog::MessageCheckDialog(
		wxWindow* parent,
		const char* title,
		const char* message,
		const char* key,
		const IconType icon,
		int buttonStyle) : MessageDialog( parent, title, message, icon, buttonStyle, "Information", NULL, NULL, NULL ) {
    // initialize
	m_key = NVL( key, "" );
}

// destructor
MessageCheckDialog::~MessageCheckDialog() {
}

// create main sizer
wxSizer* MessageCheckDialog::createMainSizer() {
	return MessageDialog::createMainSizer();
}

// create button sizer
wxSizer* MessageCheckDialog::createButtonSizer() {
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableCol( 1 );

	// check box
	wxCheckBox* check = new wxCheckBox( this, ID_CHECKBOX, wxT( "Do not show again." ) );
	sizer->Add( check, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// button
	sizer->Add(
		MessageDialog::createButtonSizer(),
		0,
		wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	return sizer;
}

// on ok
bool MessageCheckDialog::onOK() {
	// default
	if( !MessageDialog::onOK() ) {
		return false;
	}

	// checkbox
	wxCheckBox* check = dynamic_cast< wxCheckBox* >( FindWindow( ID_CHECKBOX ) );
	if( check == NULL ) {
		return true;
	}

	// save
	if( check->GetValue() ) {
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();
		if( ini != NULL ) {
			ini->setBool( SECTION, m_key.c_str(), true );
		}
	}

	return true;
}

// open dialog
void MessageCheckDialog::openDialogAfterChecking(
		wxWindow* parent,
		const char* title,
		const char* message,
		const char* key,
		const IconType icon,
		int buttonStyle
) {
	// ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// key
	std::string k = NVL( key, "" );

	// check the flag
	bool flg = false;
	if( ini != NULL ) {
		flg = ini->getBool( SECTION, k.c_str(), false );
	}
	if( flg ) {
		return;
	}

	// dialog
	MessageCheckDialog dlg( parent, title, message, key, icon, buttonStyle );
	dlg.ShowModal();
}
