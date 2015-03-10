/**
 * @file OutputDialog.cpp
 * @brief interfaces of OutputDialog class
 *
 * @author M.Izumi
 * @date 2013.08.08
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "OutputDialog.h"
#include "ScriptPluginManager.h"

using namespace kome::script;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP					  3
#define BORDER_SIZE					  3

#define SCRIPT_TYPE_NAME			 "SCRIPT"

BEGIN_EVENT_TABLE( OutputDialog, kome::window::StandardDialog )
END_EVENT_TABLE()

// constructor
OutputDialog::OutputDialog( wxWindow* parent, stScriptInfo stInfo )
	: kome::window::StandardDialog( parent, "Output" ){
			
	m_outText = NULL;
	m_insInfo = stInfo;
}

// destructor
OutputDialog::~OutputDialog(){
}

// create main sizer
wxSizer* OutputDialog::createMainSizer(){
	wxBoxSizer* sizer = new  wxBoxSizer( wxHORIZONTAL );

	m_outText = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( 400, 300 ),
		wxTE_MULTILINE | wxTE_DONTWRAP | wxTE_READONLY
	);

	sizer->Add( m_outText, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// create button sizer
wxSizer* OutputDialog::createButtonSizer(){
	wxBoxSizer*	pBoxSizer = new wxBoxSizer( wxVERTICAL );

	pBoxSizer->Add( new wxButton( this, wxID_CANCEL, "Close" ), 0, (wxALL & ~wxLEFT) | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return pBoxSizer;
}

// transfer data to window
bool OutputDialog::TransferDataToWindow(){
	// manager
	ScriptPluginManager& mgr = ScriptPluginManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// script
	kome::objects::Script* sc = mgr.getScript( plgMgr.getFunctionItem( SCRIPT_TYPE_NAME, m_insInfo.strFuncItemName.c_str() ) );
	if( sc == NULL ){
		kome::window::WindowTool::showError( "Script Console cannot be run because the script language engine cannot be found.\nDownload and install the script language." );
		return false;
	}

	// message
	time_t t = time( NULL );
	struct tm* now = localtime( &t );
	std::string date = datetostring( now );
	std::string msg = FMT( "\n=== Run the script [%s] ===\n", date.c_str() );

	m_outText->AppendText( wxT( msg.c_str() ) );
			
	std::string script = mgr.getScriptVal( m_insInfo.strPath.c_str() );

	std::string result = mgr.scriptRun( sc, script.c_str(), NULL );

	msg.append( result );
	msg.append( "\n== End ==\n\n" );
	m_outText->AppendText( msg.c_str() );

	// default
	return kome::window::StandardDialog::TransferDataToWindow();
}
