/**
 * @file DbDialog.cpp
 * @brief implements of DbDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 
 * 補足説明：
 　Version 2.0.0 
 　DB operationとSelect DB処理を1つのダイアログにまとるめた。2011.07.25 <Add> M.Izumi
   DbOperationDialog.h / DbOperationDialog.cpp 削除
 */


#include "stdafx.h"
#include "DbDialog.h"                  
#include "DbTool.h"


using namespace kome::io::db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE							3
#define GRID_GAP							3

#define VERTICAL_GRID_GAP					15
#define BUTTON_WIDTH						150

#define ID_VACUUM_BUTTON					88
#define ID_INIT_BUTTON						89

BEGIN_EVENT_TABLE( DbDialog, kome::window::ParametersAndHelpDialog )
	EVT_BUTTON( ID_VACUUM_BUTTON, DbDialog::onVacuumButton )
	EVT_BUTTON( ID_INIT_BUTTON, DbDialog::onInitButton )
END_EVENT_TABLE()


// constructor
DbDialog::DbDialog( wxWindow* parent ) 
		: kome::window::ParametersAndHelpDialog( parent, "Database" ), m_parameters( NULL, true ) {
	m_dbGetter = NULL;
	m_dbSettings = NULL;
}

// destructor
DbDialog::~DbDialog() {
}

// get DB getter
kome::plugin::PluginFunctionItem* DbDialog::getDbGetter() {
	return m_dbGetter;
}

// get DB settings
kome::objects::SettingParameterValues* DbDialog::getDbSettings() {
	return m_dbSettings;
}

// create top sizer
wxSizer* DbDialog::createMainSizer() {
	// sizer
	wxGridSizer* sizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );

	// labeling
	kome::plugin::SettingsValue* value = m_parameters.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( DB_FUNCTION_TYPE );
	value->setDefaultValue( "sqlite" );
	value->setRequired( true );

	kome::plugin::SettingsParam* param = m_parameters.addParam();
	param->setName( DB_PARAM_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( DB_PARAM_KEY );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Database" );
	form->setDesc( "Select the Mass++ internal database server." ); // @date 2014.01.16 <Add> M.Izumi

	// panel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// create Button Sizer	@date 2011.07.25 <Add> M.Izumi
wxSizer* DbDialog::createButtonSizer(){
	// sizer
	wxGridSizer* sizer = new wxGridSizer( 1, 1, VERTICAL_GRID_GAP, GRID_GAP );

	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );	

	// add OK button
	wxButton* button = new wxButton(
		this,
		wxID_OK,
		wxT( "OK" ),
		wxDefaultPosition,
		wxDefaultSize//wxSize( BUTTON_WIDTH, -1 ) サイズをDefaultに変更 @date 2011.07.25 <Add> M.Izumi
	);	
	buttonSizer->Add( button, 1, wxALL | wxGROW, BORDER_SIZE );

	// add Cancel button
	button = new wxButton(
		this,
		wxID_CANCEL,
		wxT( "Cancel" ),
		wxDefaultPosition,
		wxDefaultSize//wxSize( BUTTON_WIDTH, -1 ) サイズをDefaultに変更 @date 2011.07.25 <Add> M.Izumi
	);
	buttonSizer->Add( button, 1, wxALL | wxGROW, BORDER_SIZE );
	
	sizer->Add( buttonSizer, 1, wxALIGN_CENTER, BORDER_SIZE );

	return sizer;
}

// transfer data from window
bool DbDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// peak detector
	const char* dbGetter = m_settings.getParameterValue( DB_PARAM_NAME );
	m_dbGetter = plgMgr.getFunctionItem( DB_FUNCTION_TYPE, dbGetter );
	m_dbSettings = m_settings.getSubParameterValues( DB_PARAM_NAME, dbGetter );

	return true;
}

// on vacuum button		@date 2011.07.25 <Add> M.Izumi	
void DbDialog::onVacuumButton( wxCommandEvent& evt ) {
	int nResult = wxID_OK;	// @date 2011/12/09 <Add> OKADA

	// get DB
	kome::db::DbManager& dbMgr = kome::db::DbManager::getInstance();
	kome::db::Database* db = dbMgr.getDb();
	if( db == NULL ) {
		kome::window::WindowTool::showError( "There is a problem with the Mass++ results database configuration and a connection cannot be made.\nCheck the database configuration (File -> Database...)." );
		return;
	}

	// vacuume
	const char* msg = "It might take several minutes to vacuum. Continue?";
	if( kome::window::WindowTool::showYesNo( msg, "Vacuum", "Don't vacuum" ) ) {
		wxBeginBusyCursor();
		db->vacuum();
		wxEndBusyCursor();
	}
	else{						// @date 2011/12/09 <Add> OKADA
		nResult = wxID_CANCEL;	// @date 2011/12/09 <Add> OKADA
	}							// @date 2011/12/09 <Add> OKADA

	// end dialog
	EndDialog( nResult );	// @date 2011/12/09 <Mod> OKADA
}

// on initialize button  @date 2011.07.25 <Add> M.Izumi
void DbDialog::onInitButton( wxCommandEvent& evt ) {
	int nResult = wxID_OK;	// @date 2011/12/09 <Add> OKADA

	// get DB
	kome::db::DbManager& dbMgr = kome::db::DbManager::getInstance();
	kome::db::Database* db = dbMgr.getDb();
	if( db == NULL ) {
		kome::window::WindowTool::showError( "There is a problem with the Mass++ results database configuration and a connection cannot be made.\nCheck the database configuration (File -> Database...)." );
		return;
	}

	// vacuume
	const char* msg = "This operation will permanently delete all of the data in the database. Continue?";
	if( kome::window::WindowTool::showYesNo( msg, "Initialize anyway", "Don't initialize" ) ) {
		wxBeginBusyCursor();
		db->initDb();
		wxEndBusyCursor();
	}
	else{						// @date 2011/12/09 <Add> OKADA
		nResult = wxID_CANCEL;	// @date 2011/12/09 <Add> OKADA
	}							// @date 2011/12/09 <Add> OKADA

	// end dialog
	EndDialog( nResult );	// @date 2011/12/09 <Mod> OKADA
}
