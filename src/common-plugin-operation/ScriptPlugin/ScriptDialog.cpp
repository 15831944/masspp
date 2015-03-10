/**
 * @file ScriptDialog.cpp
 * @brief implements of ScriptDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ScriptDialog.h"
#include "VariablesListCtrl.h"
#include "ScriptListDialog.h"
#include "ScriptListCtrl.h"	
#include "ScriptPluginManager.h"

#include <wx/splitter.h>
#include <wx/tglbtn.h>


using namespace kome::script;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP					  3
#define BORDER_SIZE					  3
	
#define ID_COMBO					 62
#define ID_LOAD_BUTTON				 63
#define ID_SAVE_BUTTON				 64
#define ID_SAVE_AS_BUTTON			 65
#define ID_RUN_BUTTON				 66
#define ID_VARIABLE_BUTTON			 67
#define ID_NAME_TEXT				 68
#define ID_EVT_COMBO				 69
#define ID_IN_TEXT					 70
#define ID_OUT_TEXT					 71


#define SCRIPT_TYPE_NAME			 "SCRIPT"
#define SCRIPT_NAME				     "script"

#define SECTION						 "Script Console"
#define SCRIPT_KEY					 "DEFAULT_ENGINE"

BEGIN_EVENT_TABLE( ScriptDialog, kome::window::ParametersAndHelpDialog )	// @Date:2014/01/24	<extend help>	A.Ozaki
	EVT_COMBOBOX( ID_COMBO, ScriptDialog::onCombo )
	EVT_BUTTON( ID_LOAD_BUTTON, ScriptDialog::onLoad )
	EVT_BUTTON( ID_SAVE_BUTTON, ScriptDialog::onSave )
	EVT_BUTTON( ID_SAVE_AS_BUTTON, ScriptDialog::onSaveAs )
	EVT_BUTTON( ID_RUN_BUTTON, ScriptDialog::onRun )
	EVT_TEXT( ID_IN_TEXT, ScriptDialog::onText )
	EVT_TEXT( ID_OUT_TEXT, ScriptDialog::onText )
	EVT_BUTTON( ID_VARIABLE_BUTTON, ScriptDialog::onVariable )
	EVT_BUTTON( wxID_OK, ScriptDialog::onSaveScriptList )
END_EVENT_TABLE()


// static member
ScriptDialog* ScriptDialog::m_dlg = NULL;


// constructor
ScriptDialog::ScriptDialog( wxWindow* parent, bool bEdit ) 
		: kome::window::ParametersAndHelpDialog( parent, "Script Console", NULL, -1, -1, 500, 500 ), m_bEdit(bEdit) {
	// initialize
	m_item = NULL;
	m_combo = NULL;
	m_inText = NULL;
	m_outText = NULL;
	m_edited = false;
	m_varDlg = NULL;
	m_nameText = NULL;
	m_evtCombo = NULL;
}

// destructor
ScriptDialog::~ScriptDialog() {
}

// insert text
void ScriptDialog::insertText( const char* s ) {
	// string object
	std::string text = NVL( s, "" );

	// insert
	if( m_inText != NULL ) {
		m_inText->WriteText( wxT( text.c_str() ) );
	}
}

// on combo
void ScriptDialog::onCombo() {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::script::ScriptPluginManager& mgr = kome::script::ScriptPluginManager::getInstance();

	// selection
	int sel = m_combo->GetSelection();

	m_item = plgMgr.getFunctionItem( SCRIPT_TYPE_NAME, sel );

	// initialize
	m_path = "";
	kome::objects::Script* sc = mgr.getScript( m_item );	// @Date:2013/08/06	<Modify>	A.Ozaki
	std::string t;
	if( sc != NULL ) {
		t = sc->getTemplate();
	}

	if( !m_bEdit ){
		m_inText->SetValue( t.c_str() );
	}
	m_edited = false;
	setDialogTitle();

	// close dialog
	if( m_varDlg != NULL ) {
		delete m_varDlg;
		m_varDlg = NULL;
	}
}

// on load
void ScriptDialog::onLoad() {
	// check the item
	if( m_item == NULL ) {
		kome::window::WindowTool::showInfo( "No item is selected." );
		return;
	}

	// extension
	std::string ext = getExt();
	if( ext.empty() ) {
		ext = "*";
	}

	//  dialog
	std::string wildcard = FMT( "%s (*.%s)|*.%s", m_item->getLongName(), ext.c_str(), ext.c_str() );
	wxFileDialog dlg( this, "Select File", wxEmptyString, wxEmptyString, wxT( wildcard.c_str() ), wxFD_OPEN | wxFD_FILE_MUST_EXIST );

	if( dlg.ShowModal() == wxID_OK ) {
		std::string path = dlg.GetPath().c_str();

		setInTextVal( path.c_str() );
			
		m_path = path;
	}
}

// on save
void ScriptDialog::onSave() {
	// check the path
	if( m_path.empty() ) {
		onSaveAs();
	}

	// check the item
	if( m_item == NULL ) {
		kome::window::WindowTool::showInfo( "No item is selected." );
		return;
	}

	// extension
	std::string ext = getExt();
	if( ext.empty() ) {
		ext = "*";
	}

	if( !m_path.empty() )
	{
		// save
		saveScript( m_path.c_str() );
		m_edited = false;
	}
}

// on save as
void ScriptDialog::onSaveAs() {
	// check the item
	if( m_item == NULL ) {
		kome::window::WindowTool::showInfo( "No item is selected." );
		return;
	}

	// extension
	std::string ext = getExt();
	if( ext.empty() ) {
		ext = "*";
	}

	//  dialog
	std::string wildcard = FMT( "%s (*.%s)|*.%s", m_item->getLongName(), ext.c_str(), ext.c_str() );
	wxFileDialog dlg( this, "Save As", wxEmptyString, wxEmptyString, wxT( wildcard.c_str() ), wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

	if( dlg.ShowModal() == wxID_OK ) {
		std::string path = dlg.GetPath().c_str();
		saveScript( path.c_str() );
		m_path = path;
		m_edited = false;
	}
}

// on run
void ScriptDialog::onRun() {
	// manager
	kome::script::ScriptPluginManager& mgr = kome::script::ScriptPluginManager::getInstance();
	
	// script
	kome::objects::Script* sc = mgr.getScript( m_item );	// @Date:2013/08/06	<Modify>	A.Ozaki
	if( sc == NULL ) {
		kome::window::WindowTool::showError( "Script Console cannot be run because the script language engine cannot be found.\nDownload and install the script language." );
		return;
	}

	// message
	time_t t = time( NULL );
	struct tm* now = localtime( &t );
	std::string date = datetostring( now );
	std::string msg = FMT( "\n=== Run the script [%s] ===\n", date.c_str() );
	m_outText->AppendText( wxT( msg.c_str() ) );

	// execute
	std::string script = m_inText->GetValue().c_str();
// @Date:2013/08/06	<Modify>	A.Ozaki
// ‰¼‚Å‡‚í‚¹‚Ä‚¢‚Ü‚·
//	std::string result = sc->run( script.c_str() );
//	kome::objects::Variant	var;
//	std::string result = sc->run( script.c_str(), var, NULL );

	std::string result = mgr.scriptRun( sc, script.c_str(), NULL );
//^^^^^
	msg.append( result );
	msg.append( "\n== End ==\n\n" );
	m_outText->AppendText( msg.c_str() );
}

// get extension
std::string ScriptDialog::getExt() {
	// return value
	std::string ret;

	// extension
	if( m_item != NULL ) {
		kome::plugin::PluginCall* call = m_item->getCall();

		ret = call->getProperties().getStringValue( "ext", "" );
	}

	return ret;
}

// save script
void ScriptDialog::saveScript( const char* path ) {
	// file open
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		std::string msg = FMT( "Failed to open the file for writing. Check folder permissions. [%s]", NVL( path, "" ) );
		kome::window::WindowTool::showError( msg.c_str() );
	}
	else
	{
		// script
		std::string script = m_inText->GetValue().c_str();
		fprintf( fp, "%s", script.c_str() );

		fflush( fp );
		fclose( fp );
	}
}

// set the dialog title
void ScriptDialog::setDialogTitle() {
	// title
	std::string title = "Script Console";

	if( m_path.empty() ) {
		title.append( " - [Untitled]" );
	}
	else {
		std::string fileName = getfilename( m_path.c_str() );
		title.append( FMT( " - [%s]", fileName.c_str() ) );
	}
	if( m_edited ) {
		title.append( "*" );
	}

	SetTitle( wxT( title.c_str() ) );

	// file path
	kome::objects::Script::setScriptFilePath( m_path.c_str() );	// @Date:2013/08/06	<Modify>	A.Ozaki
}

// create main sizer
wxSizer* ScriptDialog::createMainSizer() {
	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::script::ScriptPluginManager& mgr = kome::script::ScriptPluginManager::getInstance();

	// controls
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 1, GRID_GAP, GRID_GAP );
	
	// Text’Ç‰Á	
	wxSizer* nameSizer = new wxBoxSizer( wxHORIZONTAL );
	nameSizer->Add( new wxStaticText( this, wxID_ANY, wxT( "Name" ) ) );
	
	m_nameText = new wxTextCtrl(
		this,
		ID_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxSize(300,-1), 
		0
	);
	nameSizer->AddSpacer( 20 );
	nameSizer->Add( m_nameText, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	sizer->Add( nameSizer, 1, wxGROW | wxALL , BORDER_SIZE );
		
	// ini
	kome::core::IniFile* ini = msppMgr.getIniFile();
	// combobox
	std::string defaultScript;
	if( ini != NULL ) {
		defaultScript = ini->getString( SECTION, SCRIPT_KEY, "" );
	}

	std::vector< wxString > items;
	unsigned int num = plgMgr.getNumberOfFunctionItems( SCRIPT_TYPE_NAME );
	int sel = 0;
	for( unsigned int i = 0; i < num; i++ ) {
		kome::plugin::PluginFunctionItem* tmp = plgMgr.getFunctionItem( SCRIPT_TYPE_NAME, i );
		if( defaultScript.compare( tmp->getShortName() ) == 0 ) {
			sel = (int)i;
		}

		items.push_back( wxT( tmp->getLongName() ) );
	}

	// script engne combo
	wxSizer* comboSizer = new wxBoxSizer( wxHORIZONTAL );
	comboSizer->Add( new wxStaticText( this, wxID_ANY, wxT( "Script Engine" ) )  );
	m_combo = new wxComboBox(
		this,
		ID_COMBO,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		items.size(),
		( items.size() == 0 ? NULL : &( items[ 0 ] ) ),
		wxCB_SIMPLE | wxCB_READONLY
	);

	m_combo->Enable( items.size() > 0 );
	if( items.size() > 0 ) {
		m_combo->Select( sel );
		m_item = plgMgr.getFunctionItem( SCRIPT_TYPE_NAME, sel );
	}

	comboSizer->AddSpacer( 20 );
	comboSizer->Add( m_combo,  0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	sizer->Add( comboSizer, 1, wxGROW | wxALL , BORDER_SIZE );


	// get call type
	std::vector< wxString > calls;
	calls.push_back( "" );

	for(unsigned int i =0; i < plgMgr.getNumberOfPlugins(); i++) {
		kome::plugin::PluginInfo* plginfo = plgMgr.getPlugin(i);
		for (unsigned int j = 0; j < plginfo->getNumberOfCalls(); j++) {
			kome::plugin::PluginCall* plgcall = plginfo->getCall(j);
			wxString name = plgcall->getType();
			std::vector< wxString >::iterator it = std::find( calls.begin(), calls.end(), name );
			
			if( it == calls.end() ){
				calls.push_back( name.c_str() );
			}						
		}
	}
	// sort
	std::sort( calls.begin(), calls.end() );

	// event combo
	wxSizer* evtComboSizer = new wxBoxSizer( wxHORIZONTAL );
	evtComboSizer->Add( new wxStaticText( this, wxID_ANY, wxT( "Event" ) ) );
	m_evtCombo = new wxComboBox(
		this,
		ID_EVT_COMBO,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		calls.size(),
		( calls.size() == 0 ? NULL : &( calls[ 0 ] ) ),
		wxCB_READONLY
	);

	m_evtCombo->Enable( items.size() > 0 );

	evtComboSizer->AddSpacer( 20 );
	evtComboSizer->Add( m_evtCombo, 0, wxALIGN_CENTER_VERTICAL , BORDER_SIZE );
	
	sizer->Add( evtComboSizer, 1, wxGROW | wxALL  , BORDER_SIZE );

	// splitter
	wxSplitterWindow* splitter = new wxSplitterWindow(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxSize( 600, 400 ),
		wxSP_BORDER
	);

	// text
	m_inText = new wxTextCtrl(
		splitter,
		ID_IN_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( 600, 250 ),
		wxTE_MULTILINE | wxTE_DONTWRAP
	);

	m_outText = new wxTextCtrl(
		splitter,
		ID_OUT_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_MULTILINE | wxTE_DONTWRAP | wxTE_READONLY
	);

	splitter->SplitHorizontally( m_inText, m_outText );
	splitter->SetSashGravity( 0.75 );
	splitter->SetMinimumPaneSize( 150 );

	sizer->Add( splitter, 1, wxGROW | wxALL, BORDER_SIZE );

// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
	setDescription( m_nameText, "Input the script name." );
	setDescription( m_combo, "Select the script engine." );
	setDescription( m_evtCombo, "If you want to run the script by event, select it." );
	setDescription( m_inText, "Write the script." );
// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki

	onCombo();

	return sizer;
}

// create button sizer
wxSizer* ScriptDialog::createButtonSizer() {
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 4, GRID_GAP, 20 );
	sizer->Add( new wxButton( this, ID_VARIABLE_BUTTON, "Variables" ), 0, wxALIGN_LEFT );
	
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	buttonSizer->Add( new wxButton( this, ID_LOAD_BUTTON, wxT( "Load..." ) ) );
	buttonSizer->Add( new wxButton( this, ID_SAVE_BUTTON, wxT( "Save" ) ) );
	buttonSizer->Add( new wxButton( this, ID_SAVE_AS_BUTTON, wxT( "Save as..." ) ) );
	sizer->Add( buttonSizer, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_LEFT );

	sizer->Add( new wxButton( this, ID_RUN_BUTTON, wxT( "Run" ) ) );

	wxSizer* buttonSizer2 = new wxBoxSizer( wxHORIZONTAL );
	buttonSizer2->Add( new wxButton( this, wxID_OK, wxT( "OK" ) ) );
	buttonSizer2->Add( new wxButton( this, wxID_CANCEL, wxT( "Cancel" ) ) );
	sizer->Add( buttonSizer2, 3, wxALIGN_RIGHT );

	return sizer;
}

// on cancel
bool ScriptDialog::onCancel() {
	// default
	if( !kome::window::StandardDialog::onCancel() ) {
		return false;
	}

	// check the edited flag
	bool ret = true;
	if( m_edited ) {
		ret	= kome::window::WindowTool::showOkCancel(
			"There are unsaved changes.\nIf the dialog is closed, all changes will be lost."
		);
	}

	return ret;
}

// on combo
void ScriptDialog::onCombo( wxCommandEvent& evt ) {
	onCombo();
}

// on load button
void ScriptDialog::onLoad( wxCommandEvent& evt ) {
	onLoad();
	setDialogTitle();
}

// on save button
void ScriptDialog::onSave( wxCommandEvent& evt ) {
	onSave();
	setDialogTitle();
}

// on save as button
void ScriptDialog::onSaveAs( wxCommandEvent& evt ) {
	onSaveAs();
	setDialogTitle();
}

// on run button
void ScriptDialog::onRun( wxCommandEvent& evt ) {
	onRun();
}

// on text
void ScriptDialog::onText( wxCommandEvent& evt ) {
	m_edited = true;

	setDialogTitle();
}

// on variable button
void ScriptDialog::onVariable( wxCommandEvent& evt ) {
	// delete
	if( m_varDlg != NULL ) {
		delete m_varDlg;
		m_varDlg = NULL;
	}

	m_varDlg = new kome::window::GridListDialog( this, "Variables" );
	VariablesListCtrl* listCtrl = new VariablesListCtrl( m_varDlg, this );
	m_varDlg->Show();

	// manager
	kome::script::ScriptPluginManager& mgr = kome::script::ScriptPluginManager::getInstance();
	kome::objects::Script* sc = mgr.getScript( m_item );	// @Date:2013/08/06	<Modify>	A.Ozaki
	listCtrl->setScript( sc );
}

// on ok
void ScriptDialog::onSaveScriptList( wxCommandEvent& evt ) {
	if( m_nameText->GetValue().empty() ){
		kome::window::WindowTool::showWarning( "Name cannot be empty." );
		return;
	}

	bool bflg = false;
	if( m_edited ){
		if( !kome::window::WindowTool::showOkCancel( "There are unsaved changes in the script program.\nIf the dialog is closed, all changes will be lost." ) ){
			return;
		}else{
			bflg = true;
		}
	}
			
	TransferDataFromWindow();

	// >>>>>>	@Date:2013/09/12	<Modified>	A.Ozaki
	if  ( !bflg )
	{
		if  ( (ScriptListDialog *)NULL != ScriptListDialog::m_pDlg )
		{
			ScriptListCtrl* list = ScriptListDialog::m_pDlg->getList();
			stScriptInfo	insInfo;
			insInfo.nCnt = list->getNumberOfScripts();
			insInfo.strName = m_nameText->GetValue();
			insInfo.strEngine = m_combo->GetValue();
			insInfo.strEvent = m_evtCombo->GetValue();
			insInfo.strPath = m_path.c_str();
			insInfo.strFuncItemName = ( m_item != NULL ? m_item->getShortName() : "" );

			// edit
			if  ( m_bEdit )
			{
				int index = list->getSelectedIndex();
				list->setScript( insInfo, index );

			// add
			}
			else
			{
				// manager
				ScriptPluginManager& mgr = ScriptPluginManager::getInstance();
				ScriptPluginCall* call = mgr.getScriptPluginCall( insInfo );
				if( call != NULL ){
					kome::window::WindowTool::showError( "There are objects and events matching names. Please change the name." );
					return;
				}
			
				list->addScriptInfo( insInfo, false );

				list->updateList();
			}				
		}
		SetReturnCode( wxID_OK );
	}
	else
	{
		SetReturnCode( wxID_CANCEL );
	}
	// <<<<<<	@Date:2013/09/12	<Modified>	A.Ozaki
	
	Show( false );
}

// transfer data from window
bool ScriptDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}
		
	return true;
}

// transfer data to window
bool ScriptDialog::TransferDataToWindow() {
	
	if( m_bEdit ){
		if( (ScriptListDialog *)NULL != ScriptListDialog::m_pDlg ){
			ScriptListCtrl* list = ScriptListDialog::m_pDlg->getList();
			stScriptInfo insInfo = list->getScript( list->getSelectedIndex() );

			m_nameText->SetValue( insInfo.strName.c_str() );
			m_combo->SetValue( insInfo.strEngine.c_str() );
			m_evtCombo->SetValue( insInfo.strEvent.c_str() );
			m_path = insInfo.strPath.c_str();		

			if( !m_path.empty() ){
				setInTextVal( m_path.c_str() );
			}else{
				
				// manager
				kome::script::ScriptPluginManager& mgr = kome::script::ScriptPluginManager::getInstance();
				kome::objects::Script* sc = mgr.getScript( m_item );	// @Date:2013/08/06	<Modify>	A.Ozaki
				std::string t;
				if( sc != NULL ) {
					t = sc->getTemplate();
				}
				m_inText->SetValue( t.c_str() );
				m_edited = false;
				setDialogTitle();
			}
		}
	}

	// default
	return kome::window::StandardDialog::TransferDataToWindow();
}

// set in Text value
void ScriptDialog::setInTextVal( const char* path ) {

	// manager
	kome::script::ScriptPluginManager& mgr = kome::script::ScriptPluginManager::getInstance();
	std::string s = mgr.getScriptVal( path );

	m_inText->SetValue( s.c_str() );
	m_edited = false;
	setDialogTitle();
}
