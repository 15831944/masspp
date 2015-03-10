/**
 * @file ScriptListDialog.cpp
 * @brief implements of ScriptListDialog class
 *
 * @author A.Ozaki
 * @date 2013.08.08
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "ScriptListDialog.h"
#include "ScriptListCtrl.h"
#include "ScriptDialog.h"
#include "OutputDialog.h"
#include "ScriptPluginManager.h"
#include "ScriptPluginCall.h"

#include <wx/splitter.h>
#include <wx/tglbtn.h>

#include "images\save_icon16x16.xpm"

using namespace	kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::script;


#define GRID_GAP					  3
#define BORDER_SIZE					  3
	
#define ID_LIST						 72
#define ID_BUTTON_RUN				 73
#define ID_BUTTON_ADD				 74
#define ID_BUTTON_EDIT				 75
#define ID_BUTTON_DELETE			 76
#define ID_BUTTON_CLOSE				 77
#define ID_BUTTON_SAVE				 78

#define SCRIPT_TYPE_NAME			 "SCRIPT"
#define SCRIPT_NAME				     "script"

#define SECTION						 "Script List"
#define SCRIPT_KEY_COUNT			 "COUNT"
#define SCRIPT_KEY_NAME				 "NAME"
#define SCRIPT_KEY_ENGINE			 "ENGINE"
#define SCRIPT_KEY_EVENT			 "EVENT"
#define SCRIPT_KEY_PATH				 "PATH"
#define SECTION_COUNT				 "Script List Count"
#define LIST_NUM_KEY				 "COUNT"
#define ENGIN_SHORT_NAME			 "SHORT_NAME"


BEGIN_EVENT_TABLE( ScriptListDialog, kome::window::StandardDialog )
	EVT_BUTTON( ID_BUTTON_RUN, ScriptListDialog::onRun )
	EVT_BUTTON( ID_BUTTON_ADD, ScriptListDialog::onAdd )
	EVT_BUTTON( ID_BUTTON_EDIT, ScriptListDialog::onEdit )
	EVT_BUTTON( ID_BUTTON_DELETE, ScriptListDialog::onDelete )
	EVT_BUTTON( ID_BUTTON_CLOSE, ScriptListDialog::onClose )
	EVT_BUTTON( ID_BUTTON_SAVE, ScriptListDialog::onSave )

	EVT_UPDATE_UI( ID_BUTTON_RUN, ScriptListDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_BUTTON_ADD, ScriptListDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_BUTTON_EDIT, ScriptListDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_BUTTON_DELETE, ScriptListDialog::onUpdateUI )
		
END_EVENT_TABLE()


// static member
ScriptListDialog	*ScriptListDialog::m_pDlg = (ScriptListDialog *)NULL;


// constructor
ScriptListDialog::ScriptListDialog( wxWindow* parent )
	: kome::window::StandardDialog( parent,
								"Script List",
								NULL, -1, -1, 500, 500 )
{
	// initialize
	m_pList = (ScriptListCtrl *)NULL;
	m_pBitmapButtonRun = (wxBitmapButton *)NULL;
	m_pBitmapButtonAdd = (wxBitmapButton *)NULL;
	m_pBitmapButtonEdit = (wxBitmapButton *)NULL;
	m_pBitmapButtonDelete = (wxBitmapButton *)NULL;
	m_pBitmapButtonSave = (wxBitmapButton *)NULL;
		
	return;
}

// destructor
ScriptListDialog::~ScriptListDialog( )
{
	return;
}

// create main sizer
wxSizer* ScriptListDialog::createMainSizer( void )
{
	// managers
	kome::core::MsppManager&	insMsppMgr = kome::core::MsppManager::getInstance( );
	kome::plugin::PluginManager&	insPluginMgr = kome::plugin::PluginManager::getInstance( );

	// ini
	kome::core::IniFile	*pIni = insMsppMgr.getIniFile( );

	// controls
	wxFlexGridSizer	*pSizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	pSizer->AddGrowableCol( 0 );
	pSizer->AddGrowableRow( 1 );

	// list box
	m_pList = new ScriptListCtrl( this, 400, 400 );
	if  ( (kome::window::GridListCtrl *)NULL == m_pList )
	{
		delete	pSizer;
		return	(wxSizer *)NULL;
	}
		
	pSizer->Add( m_pList, 0, wxALL | wxGROW, BORDER_SIZE );
	
	return	pSizer;
}

// create button sizer
wxSizer* ScriptListDialog::createButtonSizer( void )
{
	wxBoxSizer*	pBoxSizer = new wxBoxSizer( wxVERTICAL );

	wxSizer	*pButtonSizer = new wxBoxSizer( wxHORIZONTAL );

	IconManager&	iconMgr = IconManager::getInstance( );

	if  ( (wxBoxSizer *)NULL == pBoxSizer || (wxBoxSizer *)NULL == pButtonSizer )
	{
		return	(wxSizer *)NULL;
	}

	// save button 
	m_pBitmapButtonSave = new wxBitmapButton( this, ID_BUTTON_SAVE, wxImage( save_icon16x16_xpm ) );
	if  ( (wxBitmapButton *)NULL != m_pBitmapButtonSave )
	{
		m_pBitmapButtonSave->SetToolTip("Save script");
		pBoxSizer->Add( m_pBitmapButtonSave, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, BORDER_SIZE );
	}
		
	// run button
	m_pBitmapButtonRun = new wxBitmapButton( this, ID_BUTTON_RUN, *iconMgr.getIcon( "run" ) );
	if  ( (wxBitmapButton *)NULL != m_pBitmapButtonRun )
	{
		m_pBitmapButtonRun->SetToolTip( "Run script" );
		m_pBitmapButtonRun->SetBitmapDisabled( *iconMgr.getIcon( "run_disabled" ) );
		m_pBitmapButtonRun->Enable( false );
		pButtonSizer->Add( m_pBitmapButtonRun, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );
	}

	// add button
	m_pBitmapButtonAdd = new wxBitmapButton( this, ID_BUTTON_ADD, *iconMgr.getIcon( "add" ) );
	if  ( (wxBitmapButton *)NULL != m_pBitmapButtonAdd )
	{
		m_pBitmapButtonAdd->SetToolTip( "Add script" );
		m_pBitmapButtonAdd->SetBitmapDisabled( *iconMgr.getIcon( "add_disabled" ) );
		pButtonSizer->Add( m_pBitmapButtonAdd, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );
	}

	// edit button
	m_pBitmapButtonEdit = new wxBitmapButton( this, ID_BUTTON_EDIT, *iconMgr.getIcon( "edit" ) );
	if  ( (wxBitmapButton *)NULL != m_pBitmapButtonEdit )
	{
		m_pBitmapButtonEdit->SetToolTip( "Edit script" );
		m_pBitmapButtonEdit->SetBitmapDisabled( *iconMgr.getIcon( "edit_disabled" ) );
		m_pBitmapButtonEdit->Enable( false ); 
		pButtonSizer->Add( m_pBitmapButtonEdit, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );
	}

	// delete button
	m_pBitmapButtonDelete = new wxBitmapButton( this, ID_BUTTON_DELETE, *iconMgr.getIcon( "delete" ) );
	if  ( (wxBitmapButton *)NULL != m_pBitmapButtonDelete )
	{
		m_pBitmapButtonDelete->SetToolTip( "Delete script" );
		m_pBitmapButtonDelete->SetBitmapDisabled( *iconMgr.getIcon( "delete_disabled" ) );
		m_pBitmapButtonDelete->Enable( false );
		pButtonSizer->Add( m_pBitmapButtonDelete, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );
	}
	
	pBoxSizer->Add( pButtonSizer, 0, (wxALL & ~wxLEFT) | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	
	// close button
	wxSizer	*pCloseButtonSizer = new wxBoxSizer( wxHORIZONTAL );

	pCloseButtonSizer->Add( new wxButton( this, ID_BUTTON_CLOSE, wxT( "Close" ) ) );

	pBoxSizer->Add( pCloseButtonSizer, 0, (wxALL & ~wxLEFT) | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return	pBoxSizer;
}

// for Window Event
// on run button
void	ScriptListDialog::onRun( wxCommandEvent& evt )
{
	onRun( );

	return;
}

// on add button
void	ScriptListDialog::onAdd( wxCommandEvent& evt )
{
	onAdd( );

	return;
}

// on edit button
void	ScriptListDialog::onEdit( wxCommandEvent& evt )
{
	onEdit( );

	return;
}

// on delete button
void	ScriptListDialog::onDelete( wxCommandEvent& evt )
{
	onDelete( );

	return;
}

// on close button
void	ScriptListDialog::onClose( wxCommandEvent& evt )
{
	TransferDataFromWindow();

	this->Close( true );

	// ここでは、このダイアログのインスタンスは破棄しません
	// 起動元で破棄します

	SetReturnCode( wxID_CANCEL );

	return;
}

// on update ui
void	ScriptListDialog::onUpdateUI( wxUpdateUIEvent& evt ){
	
	onSelect( );

}

// save data
void	ScriptListDialog::onSave( wxCommandEvent& evt ) {
	m_pList->exportData();
}


// for processing

// on select
void ScriptListDialog::onSelect( void )
{
	m_pList->OnSelect( );
	UpdateButtonControls( );
}

// on cancel
bool	ScriptListDialog::onCancel( void )
{
	return	true;
}

// run
void	ScriptListDialog::onRun( void )
{	
	int idx = m_pList->getSelectedIndex( );
	if  ( -1 == idx )
	{
		return;
	}
	
	stScriptInfo stInfo = m_pList->getScript( idx );
	
	// output dialog
	OutputDialog dlg( this, stInfo  );
	dlg.ShowModal();

	return;
}

// add
void	ScriptListDialog::onAdd( void )
{
	if( ScriptDialog::m_dlg != NULL ) {
		delete ScriptDialog::m_dlg;
	}

	ScriptDialog::m_dlg = new ScriptDialog( kome::window::WindowTool::getMainWindow(), false );

	// okボタン押下時
	if( ScriptDialog::m_dlg->ShowModal() == wxID_OK ){
		
		// manager
		if  ( 0 < m_pList->getNumberOfScripts( ) )		// 
		{
			ScriptPluginManager& mgr = ScriptPluginManager::getInstance();
		
			stScriptInfo stInfo = m_pList->getScript( m_pList->getNumberOfScripts()-1 ); 
		
			mgr.addPluginInfo( stInfo ); // PluginManagerに追加する処理
		}
	}

	return;
}

// edit
void	ScriptListDialog::onEdit( void )
{
	if( ScriptDialog::m_dlg != NULL ) {
		delete ScriptDialog::m_dlg;
	}

	ScriptDialog::m_dlg = new ScriptDialog( kome::window::WindowTool::getMainWindow(), true );

	// show
	if( ScriptDialog::m_dlg->ShowModal() == wxID_OK ){
		TransferDataFromWindow();
		// manager
		ScriptPluginManager& mgr = ScriptPluginManager::getInstance();

		stScriptInfo stInfo = m_pList->getScript( m_pList->getSelectedIndex() ); 
		
		mgr.addPluginInfo( stInfo ); // PluginManagerに追加する処理
	}

	return;
}

// delete
void	ScriptListDialog::onDelete( void )
{
	if( kome::window::WindowTool::showOkCancel( "Are you sure you want to delete the specified script?" ) ){
		
		// >>>>>>	SPEC94873	@Date:2013/09/11	<Modified>	A.Ozaki
		//
		int index =  m_pList->getSelectedIndex();

		// manager
		ScriptPluginManager& mgr = ScriptPluginManager::getInstance();

		stScriptInfo stInfo = m_pList->getScript( index ); 

		mgr.deletePluginInfo( stInfo );

		m_pList->deleteScriptInfo( index, true );

		//		
		// <<<<<<	SPEC94873	@Date:2013/09/11	<Modified>	A.Ozaki
	}
	
	return;
}

// update button controls
void	ScriptListDialog::UpdateButtonControls( )
{
	if  ( -1 == m_pList->getSelectedIndex( ) )
	{
		m_pBitmapButtonRun->Enable( false );
		m_pBitmapButtonEdit->Enable( false ); 
		m_pBitmapButtonDelete->Enable( false );
	}
	else
	{
		m_pBitmapButtonRun->Enable( true );
		m_pBitmapButtonEdit->Enable( true ); 
		m_pBitmapButtonDelete->Enable( true );
	}

	return;
}

// transfer data from window
bool ScriptListDialog::TransferDataFromWindow()
{
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}
		
	// managers
    kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	
	ini->setInt( SECTION_COUNT, LIST_NUM_KEY, m_pList->getNumberOfScripts() );

	for( unsigned int i=0; i < m_pList->getNumberOfScripts(); i++ ){
		stScriptInfo info = m_pList->getScript( i );
		std::string strSection =  FMT( "%s:%d", SECTION, i );

		ini->setInt( strSection.c_str(), SCRIPT_KEY_COUNT, info.nCnt );
		ini->setString( strSection.c_str(), SCRIPT_KEY_NAME, info.strName.c_str() );
		ini->setString( strSection.c_str(), SCRIPT_KEY_ENGINE, info.strEngine.c_str() );
		ini->setString( strSection.c_str(), SCRIPT_KEY_EVENT, info.strEvent.c_str() );
		ini->setString( strSection.c_str(), SCRIPT_KEY_PATH, info.strPath.c_str() );
		ini->setString( strSection.c_str(), ENGIN_SHORT_NAME, info.strFuncItemName.c_str() );
	}
	
	return true;
}

// transfer data to window
bool ScriptListDialog::TransferDataToWindow(){

	// managers
    kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	kome::script::ScriptPluginManager& mgr = kome::script::ScriptPluginManager::getInstance();
	
	int inum = ini->getInt( SECTION_COUNT, LIST_NUM_KEY, -1 );
	for( int i=0; i < inum; i++ ){		
		std::string strSection = FMT( "%s:%d", SECTION, i );

		stScriptInfo	insInfo;
		
		insInfo.nCnt = ini->getInt( strSection.c_str(), SCRIPT_KEY_COUNT, -1 );
		insInfo.strName = ini->getString( strSection.c_str(), SCRIPT_KEY_NAME, "" );
		insInfo.strEngine = ini->getString( strSection.c_str(), SCRIPT_KEY_ENGINE, "" );
		insInfo.strEvent = ini->getString( strSection.c_str(), SCRIPT_KEY_EVENT, "" );
		insInfo.strPath = ini->getString( strSection.c_str(), SCRIPT_KEY_PATH, "" );
		insInfo.strFuncItemName = ini->getString( strSection.c_str(), ENGIN_SHORT_NAME, "" );

		// >>>>>>	@Date:2013/09/12	<Add>	A.Ozaki
		// check script path
		if  ( 0 != _access_s( insInfo.strPath.c_str( ), 4 ) )
		{
			// ファイルが読み込めない場合は、スクリプトとして認めません
			continue;
		}
		// <<<<<<	@Date:2013/09/12	<Add>	A.Ozaki

		m_pList->addScriptInfo( insInfo, true );

		// add plugin info
		ScriptPluginCall* call = mgr.getScriptPluginCall( insInfo );
		if( call == NULL ){
			mgr.addPluginInfo( insInfo );
		}
	}
		
	// default
	return kome::window::StandardDialog::TransferDataToWindow();
}
