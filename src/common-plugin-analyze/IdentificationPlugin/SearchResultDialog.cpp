/**
 * @file SearchResultDialog.cpp
 * @brief implements of GridListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.09.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "SearchResultDialog.h"
#include "SubstanceListCtrl.h"	
#include "SearchResultListCtrl.h"	// @date 2013.06.24 <Add> M.Izumi

#include "images\delete_icon16x16.xpm"
#include "images\save_icon16x16.xpm"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP						 3
#define BORDER_SIZE						 3

#define ID_SAVE_BUTTON					 10001
#define ID_DELETE_BUTTON                 10002

#define ID_MIN_IDENT_RESULT_BUTTON       2201
#define ID_MAX_IDENT_RESULT_BUTTON       2220

#define STR_STATUS_NAME_RESULT_VIEW_FROM_SEARCH_ENGINE	"result_view_ResultViewFromSearchEngine"
#define STR_STATUS_NAME_SHOW_SUBSTANCES					"result_view_showSubstances"
                                 
#define SAFETY_DELETE(OBJECTX)	if(OBJECTX!=NULL){delete OBJECTX;OBJECTX=NULL;}	// @date 2013/02/06 <Add> OKADA

BEGIN_EVENT_TABLE( SearchResultDialog, kome::window::StandardDialog )
	EVT_BUTTON( ID_SAVE_BUTTON, SearchResultDialog::saveData )
	EVT_BUTTON( ID_DELETE_BUTTON, SearchResultDialog::deleteData )
	EVT_UPDATE_UI( ID_SAVE_BUTTON, SearchResultDialog::updateUI )
	EVT_UPDATE_UI( ID_DELETE_BUTTON, SearchResultDialog::updateUI )

	EVT_COMMAND_RANGE( ID_MIN_IDENT_RESULT_BUTTON, ID_MAX_IDENT_RESULT_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, SearchResultDialog::onResultButton )
	EVT_UPDATE_UI_RANGE( ID_MIN_IDENT_RESULT_BUTTON, ID_MAX_IDENT_RESULT_BUTTON, SearchResultDialog::onResultUpdateUIButton )
	EVT_BUTTON( wxID_CANCEL, SearchResultDialog::onClose )
END_EVENT_TABLE()


// constructor
SearchResultDialog::SearchResultDialog( wxWindow* parent, const char* title, const bool canDelete )
		: StandardDialog( parent, title ) {
	m_list = NULL;
	m_deleteFlg = canDelete;

    // manager
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// ボタンを表示する準備
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	const unsigned int num = plgMgr.getNumberOfFunctionItems( "IDENT_RESULT" );
	if( num > 0 ) {
		m_resultViews.resize( num );
	}
	for( unsigned int i = 0 ; i < num; i++ ) {
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "IDENT_RESULT", i );
		m_resultViews[ i ].fun = item;
		m_resultViews[ i ].button = NULL;
		m_resultViews[ i ].pos = -1;

		kome::plugin::PluginIcon* icon = item->getCall()->getIcon();
		if( icon != NULL ) {
			iconMgr.loadBitmap( &( m_resultViews[ i ].icon0 ), icon );
		}

		wxBitmap bitmap( 16, 16 );
		wxMemoryDC memDC( bitmap );
		memDC.SetBackground( *wxGREY_BRUSH );
		memDC.Clear();
		memDC.DrawBitmap( m_resultViews[ i ].icon0, 1, 0 );

		memDC.SelectObject( wxNullBitmap );

		m_resultViews[ i ].icon1.CopyFromBitmap( bitmap );
	}
	SubstanceDialog::m_dlgSubstance = NULL;
}

// destructor
SearchResultDialog::~SearchResultDialog() {

	if( SubstanceDialog::m_dlgSubstance != NULL ){
		if( SubstanceDialog::m_dlgSubstance->IsShown() ){
			SubstanceDialog::m_dlgSubstance->Hide();
		}
	}
	
	SAFETY_DELETE( SubstanceDialog::m_dlgSubstance );

	SearchResultListCtrl::m_dlg = NULL;	 // @date 2013.06.24 <Add> M.Izumi
}

// set grid list control
void SearchResultDialog::setGridListCtrl( kome::window::GridListCtrl* listCtrl ) {
	m_list = listCtrl;
}

// update data
void SearchResultDialog::updateDialog() {
	// update list
	if( m_list != NULL ) {
		m_list->create();
	}

	// refresh
	if( IsShown() ) {
		Refresh();
	}
}

// create main sizer
wxSizer* SearchResultDialog::createMainSizer() { 
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// ボタンを表示(Result View From Search Engine ボタン、Substancesボタン等)
	wxBoxSizer* GroupsOpsButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	for( unsigned int i = 0; i < m_resultViews.size(); i++ ) {
		kome::plugin::PluginFunctionItem* item = m_resultViews[ i ].fun;
		kome::plugin::PluginIcon* icon = item->getCall()->getIcon();
		std::string tooltip = FMT( "Toggle %s on / off", item->getLongName() );

		wxBitmapButton* button = new wxBitmapButton(
			this,
			ID_MIN_IDENT_RESULT_BUTTON + i,
			m_resultViews[ i ].icon0		 
		);
		m_resultViews[ i ].button = button;
		button->SetToolTip( wxT( tooltip.c_str() ) );

		std::string sName = getStatusName( i );
		const char* s0 = statusMgr.getValue( sName.c_str() );
		std::string s1 = NVL( s0, "" );

		if( s1.compare( "on" ) == 0 ) {
			button->SetBitmapLabel( m_resultViews[ i ].icon1 );
		}
		else {
			button->SetBitmapLabel( m_resultViews[ i ].icon0 );
		}
		
		GroupsOpsButtonSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );
	}

	sizer->Add(GroupsOpsButtonSizer, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	// list control
	if( m_list != NULL ) {
		m_list->create();
		sizer->Add( m_list, 1, wxALL | wxGROW, BORDER_SIZE );
	}

	// butotn sizer
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBitmapButton* button = new wxBitmapButton( this, ID_SAVE_BUTTON, wxImage( save_icon16x16_xpm ) );
	button->SetToolTip("Save list to text file");
	buttonSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	if( m_deleteFlg ) {
		button = new wxBitmapButton( this, ID_DELETE_BUTTON, wxImage( delete_icon16x16_xpm ) );
		button->SetToolTip("Delete selected item");
		buttonSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, BORDER_SIZE );

	return sizer;
}

// create button sizer
wxSizer* SearchResultDialog::createButtonSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// button	
	wxButton* button = new wxButton( this, wxID_CANCEL, wxT( "Close" ) );
	sizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );

	return sizer;
}

// save data
void SearchResultDialog::saveData( wxCommandEvent& evt ) {
	m_list->exportData();
}

// delete data
void SearchResultDialog::deleteData( wxCommandEvent& evt ) {

	// selected item
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( m_list, items );

	// delete
	wxBeginBusyCursor();
	
	for( unsigned int i = 0; i < items.size(); i++ ) {
		m_list->deleteData( items[ i ], false );
	}
	
	wxEndBusyCursor();

	m_list->updateList();
	m_list->Refresh();
}

// update UI
void SearchResultDialog::updateUI( wxUpdateUIEvent& evt ) {
	bool enable = true;

	if( evt.GetId() == ID_DELETE_BUTTON ) {
		std::vector< int > items;
		kome::window::ListTool::getSelectedItems( m_list, items );

		enable = ( items.size() > 0 );
	}

	evt.Enable( enable );
}

// get the status name
std::string SearchResultDialog::getStatusName( const unsigned int idx ) {
	// string
	std::string s;

	// check the index
	if( idx >= m_resultViews.size() ) {
		return s;
	}

	// name
	s = FMT( "result_view_%s", m_resultViews[ idx ].fun->getShortName() );

	return s;
}

// on peak button
void SearchResultDialog::onResultButton( wxCommandEvent& evt ) {
	// index
	int idx = evt.GetId() - ID_MIN_IDENT_RESULT_BUTTON;
	if( idx < 0 || idx >= (int)m_resultViews.size() ) {
		return;
	}

	// satatus manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// button
	wxBitmapButton* button = m_resultViews[ idx ].button;

	// status
	std::string name = getStatusName( idx );
	if( name.empty() ) {
		return;
	}

	const char* s = statusMgr.getValue( name.c_str() );
	std::string status = NVL( s, "" );


	// open / close
	if( status.compare( "on" ) == 0 ) {    // close
		// いまonなので、closeする
		statusMgr.setValue( name.c_str(), "" );
		button->SetBitmapLabel( m_resultViews[ idx ].icon0 );

		if( isVisibleDialog( idx ) ) {
			SubstanceDialog::m_dlgSubstance->Hide();
			SAFETY_DELETE(SubstanceDialog::m_dlgSubstance);	// @date 2013/02/06 <Add> OKADA
		}
	}
	else {    // open

		// いまopenしてないので、openする
		statusMgr.setValue( name.c_str(), "on" );
		button->SetBitmapLabel( m_resultViews[ idx ].icon1 );

		// 関数呼び出し
		kome::plugin::PluginFunctionItem* item = m_resultViews[ idx ].fun;
		if( item != NULL ){
			kome::objects::Parameters params;

			// search ID
			int nSearchID = getSearchID();	// -1; // 選択されていない
			if( nSearchID == -1 ){
				return;	// 選択されていない
			}
			kome::plugin::PluginCallTool::setSearchID( params, nSearchID );

			// wxWindow*
			kome::plugin::PluginCallTool::setCanvas( params, this );

			item->getCall()->invoke( &params );
		}
	}
}

// on update peak button
void SearchResultDialog::onResultUpdateUIButton( wxUpdateUIEvent& evt ) {
	// index
	int idx = evt.GetId() - ID_MIN_IDENT_RESULT_BUTTON;
	if( idx < 0 || idx >= (int)m_resultViews.size() ) {
		return;
	}

	// satatus manager
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	// status
	std::string name = getStatusName( idx );
	if( name.empty() ) {
		return;
	}

	const char* s = statusMgr.getValue( name.c_str() );
	std::string status = NVL( s, "" );

	evt.Check( status.compare( "on" ) == 0 );
}

// judges the specified dialog is visible or not
bool SearchResultDialog::isVisibleDialog( const unsigned int idx ) {

	// check the parameters
	if( idx >= m_resultViews.size() ) {
		return false;
	}

	std::string name = getStatusName( idx );
	if( name.empty() ) {
		return false;
	}
	if( name.compare( STR_STATUS_NAME_SHOW_SUBSTANCES ) == 0 ){
		if( SubstanceDialog::m_dlgSubstance == NULL ){
			return false;
		}else{
			return SubstanceDialog::m_dlgSubstance->IsShown();
		}
	}

	return false;
}

// on close button
void SearchResultDialog::onClose( wxCommandEvent& evt ){
	Destroy();
}

// clear status
void SearchResultDialog::clearStatus( const char szInitStr[] ){
	// ボタンを初期化
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	for( unsigned int i = 0; i < m_resultViews.size(); i++ ) {

		// status
		std::string name = getStatusName( i );
		if( name.empty() ) {
			continue;
		}

		statusMgr.setValue( name.c_str(), szInitStr );
	}

	return;
}

// search ID
int SearchResultDialog::getSearchID() {
	int searchId = ( (SearchResultListCtrl*)m_list )->getSelectedSearch();
	return searchId;
}

// get List
kome::window::GridListCtrl* SearchResultDialog::getList( void ){
	return m_list;
}

// on Double Click At List
void SearchResultDialog::onDoubleClickAtList( void ){
	// 各ボタンが押されているか調べて、押されているなら、押されているボタンを押したときの処理を実行する

	int nSize = m_resultViews.size();

	for( int i=0; i<nSize; i++){

		// status
		std::string name = getStatusName( i );
		if( name.empty() ) {
			continue;
		}

		// satatus manager
		kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

		const char* s = statusMgr.getValue( name.c_str() );
		std::string status = NVL( s, "" );

		if( status.compare( "on" ) == 0 ) { 
			// ボタン押されてます

			// 関数呼び出し
			kome::plugin::PluginFunctionItem* item = m_resultViews[ i ].fun;
			if( item != NULL ){
				kome::objects::Parameters params;

				// search ID
				int nSearchID = getSearchID();
				kome::plugin::PluginCallTool::setSearchID( params, nSearchID );

				// wxWindow*
				kome::plugin::PluginCallTool::setCanvas( params, this );

				item->getCall()->invoke( &params );
			}
		}
	}

	return;
}
