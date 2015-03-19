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
#include "SubstanceDialog.h"

#include "SubstanceListCtrl.h"

#include <wx/artprov.h>		// @date 2013/01/30 <Add> OKADA

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

#define ID_MIN_IDENT_SUBSTANCE_BUTTON    2221
#define ID_MAX_IDENT_SUBSTANCE_BUTTON    2240

#define ID_MIN_IDENT_SPECTRUM_BUTTON     2241
#define ID_MAX_IDENT_SPECTRUM_BUTTON     2260

#define TYPE_IDENT_SUBSTANCE	"IDENT_SUBSTANCE"
#define TYPE_IDENT_SPECTRUM		"IDENT_SPECTRUM"

#define STR_STATUS_NAME_RESULT_VIEW_FROM_SEARCH_ENGINE	"result_view_ResultViewFromSearchEngine"
#define STR_STATUS_NAME_SHOW_SUBSTANCES					"result_view_showSubstances"


// for icon
#define CLOSE_FOLDER_ICON_NAME	 "folder-close"
#define OPEN_FOLDER_ICON_NAME	 "folder-open"
#define MS1_ICON_NAME			 "MS1"
#define MS2_ICON_NAME			 "MS2"
#define MS3_ICON_NAME			 "MS3"
#define MS4_ICON_NAME			 "MS4"
#define MS5_ICON_NAME			 "MS5"
#define MS6_ICON_NAME			 "MS6"
#define MS7_ICON_NAME			 "MS7"
#define MSN_ICON_NAME			 "MSn"

#define ROOT_ITEM_NAME			 "root"
        

BEGIN_EVENT_TABLE( SubstanceDialog, kome::window::StandardDialog )
	EVT_BUTTON( ID_SAVE_BUTTON, SubstanceDialog::saveData )
	EVT_BUTTON( ID_DELETE_BUTTON, SubstanceDialog::deleteData )
	EVT_UPDATE_UI( ID_SAVE_BUTTON, SubstanceDialog::updateUI )
	EVT_UPDATE_UI( ID_DELETE_BUTTON, SubstanceDialog::updateUI )
	EVT_TREE_SEL_CHANGED(wxID_ANY, onSelectTree) 
	EVT_COMMAND_RANGE( ID_MIN_IDENT_SUBSTANCE_BUTTON, ID_MAX_IDENT_SUBSTANCE_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, SubstanceDialog::onSubstanceButton )
	EVT_UPDATE_UI_RANGE( ID_MIN_IDENT_SUBSTANCE_BUTTON, ID_MAX_IDENT_SUBSTANCE_BUTTON, SubstanceDialog::onSubstanceUpdateUIButton )
	EVT_COMMAND_RANGE( ID_MIN_IDENT_SPECTRUM_BUTTON, ID_MAX_IDENT_SPECTRUM_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, SubstanceDialog::onSpectrumButton )
	EVT_UPDATE_UI_RANGE( ID_MIN_IDENT_SPECTRUM_BUTTON, ID_MAX_IDENT_SPECTRUM_BUTTON, SubstanceDialog::onSpectrumUpdateUIButton )         
END_EVENT_TABLE()


/** SubstanceDialogを保存 */
SubstanceDialog*	SubstanceDialog::m_dlgSubstance = NULL;

// constructor
SubstanceDialog::SubstanceDialog( wxWindow* parent, const char* title, const bool canDelete )
		: StandardDialog( parent, title ) {
	m_list = NULL;
	m_llSearchID = -1;

	m_itemIdSelected = NULL;

    // manager
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	/** Substance Button */
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	const unsigned int nNumSubstance = plgMgr.getNumberOfFunctionItems( TYPE_IDENT_SUBSTANCE );
	if( 0 < nNumSubstance ) {
		m_substanceButton.resize( nNumSubstance );
	}
	for( unsigned int i = 0 ; i < nNumSubstance; i++ ) {
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( TYPE_IDENT_SUBSTANCE, i );
		m_substanceButton[ i ].fun = item;
		m_substanceButton[ i ].button = NULL;
		m_substanceButton[ i ].dialog = NULL;
		m_substanceButton[ i ].pos = -1;

		kome::plugin::PluginIcon* icon = item->getCall()->getIcon();
		if( icon != NULL ) {
			iconMgr.loadBitmap( &( m_substanceButton[ i ].icon0 ), icon );
		}

		wxBitmap bitmap( 16, 16 );
		wxMemoryDC memDC( bitmap );
		memDC.SetBackground( *wxGREY_BRUSH );
		memDC.Clear();
		memDC.DrawBitmap( m_substanceButton[ i ].icon0, 1, 0 );

		memDC.SelectObject( wxNullBitmap );

		m_substanceButton[ i ].icon1.CopyFromBitmap( bitmap );
	}

	/** Spectrum Button */
	const unsigned int nNumSpectrum = plgMgr.getNumberOfFunctionItems( TYPE_IDENT_SPECTRUM );
	if( 0 < nNumSpectrum ) {
		m_spectrumButton.resize( nNumSpectrum );
	}
	for( unsigned int i = 0 ; i < nNumSpectrum; i++ ) {
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( TYPE_IDENT_SPECTRUM, i );
		m_spectrumButton[ i ].fun = item;
		m_spectrumButton[ i ].button = NULL;
		m_spectrumButton[ i ].dialog = NULL;
		m_spectrumButton[ i ].pos = -1;

		kome::plugin::PluginIcon* icon = item->getCall()->getIcon();
		if( icon != NULL ) {
			iconMgr.loadBitmap( &( m_spectrumButton[ i ].icon0 ), icon );
		}

		wxBitmap bitmap( 16, 16 );
		wxMemoryDC memDC( bitmap );
		memDC.SetBackground( *wxGREY_BRUSH );
		memDC.Clear();
		memDC.DrawBitmap( m_spectrumButton[ i ].icon0, 1, 0 );

		memDC.SelectObject( wxNullBitmap );

		m_spectrumButton[ i ].icon1.CopyFromBitmap( bitmap );
	}

}

// destructor
SubstanceDialog::~SubstanceDialog() {
	// ボタンを初期化
	clearStatus();
}

// update data
void SubstanceDialog::updateDialog() {
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
wxSizer* SubstanceDialog::createMainSizer() { 
	
	wxBeginBusyCursor();

	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// hits
	std::vector< int > hits;
	std::vector< int > substances;
	identMgr.getHitsFromSearch( hits, m_llSearchID );

	for( unsigned int i = 0; i < hits.size(); i++ ) {
		kome::ident::HitInfo hitInfo;
		identMgr.getHit( hits[ i ], hitInfo );
		int substance = hitInfo.substance;
		if( m_hitsMap.find( substance ) == m_hitsMap.end() ) {
			substances.push_back( substance );
		}
		m_hitsMap[ substance ].push_back( hitInfo );
	}

	// 区切りをつけて見やすく
	sizer->Add( -1, 10 );

	wxSize treeSize = wxSize( 200, 300 );

	// あとで使う場合はヘッダに「m_tree_ctrl」といった形で保存
	m_tree = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxSize( 500, 350 ),
		wxTR_SINGLE			// 一度に一項目だけを選択できることを明示するための便宜的なフラグ
		| wxTR_ROW_LINES	// 行の間に境界線を表示
		| wxTR_HAS_BUTTONS	// 親となる項目の左側に + や - のボタンを表示
		| wxTR_HIDE_ROOT	// ルートノードを表示させない
	);
	m_root = m_tree->AddRoot( wxEmptyString );


	// MSxアイコンの読み込み
	// get icon
	wxIcon* wxIcons[8];
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();
	{
		const char* iconNames[] = {
			MS1_ICON_NAME,		
			MS2_ICON_NAME,
			MS3_ICON_NAME,
			MS4_ICON_NAME,
			MS5_ICON_NAME,
			MS6_ICON_NAME,
			MS7_ICON_NAME,
			MSN_ICON_NAME
		};

		for( int i=0; i<8; i++ ){
			std::string iconName = iconNames[ i ];

			// get icon
			int icon = iconMgr.getIconIndex( iconName.c_str() );

			wxIcons[i] = iconMgr.getIcon( icon );
		}
	}

	const char* szIconPath = getIconPath();
	// .pngをロードしてアイコンにする。
	wxIcon icon;
	icon.CopyFromBitmap(wxBitmap( wxString(szIconPath), wxBITMAP_TYPE_ANY));

	wxImageList* imageList = new wxImageList(16, 16);
	imageList->Add(wxIcon( wxString( szIconPath ), wxBITMAP_TYPE_ANY ) );
	for( int i=0; i<8; i++ ){
		imageList->Add( *wxIcons[i] );
	}

	wxBitmap idx1 = wxArtProvider::GetBitmap(wxART_FOLDER,wxART_OTHER );	// for debug
	wxBitmap idx2 = wxArtProvider::GetBitmap(wxART_NEW,wxART_OTHER);
	imageList->Add(idx1);
	imageList->Add(idx2);

	m_tree->AssignImageList(imageList);

	for( unsigned int i = 0; i < substances.size(); i++ ) {
		makeTree( substances[ i ] );
	}
	sizer->Add( m_tree, 1, wxALL | wxGROW, BORDER_SIZE );
    
	// list control
	m_list = new SubstanceListCtrl( this );
	sizer->Add( m_list, 1, wxALL | wxGROW, BORDER_SIZE );

	// ボタンを表示(Result View From Search Engine ボタン、Substancesボタン等)
	/** Substance Button */
	wxBoxSizer* GroupsOpsButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	for( unsigned int i = 0; i < m_substanceButton.size(); i++ ) {
		kome::plugin::PluginFunctionItem* item = m_substanceButton[ i ].fun;
		kome::plugin::PluginIcon* icon = item->getCall()->getIcon();
		std::string tooltip = FMT( "%s", item->getLongName() );	// @date 2013/02/06 <Add> OKADA

		wxBitmapButton* button = new wxBitmapButton(
			this,
			ID_MIN_IDENT_SUBSTANCE_BUTTON + i,
			m_substanceButton[ i ].icon0
		);

		m_substanceButton[ i ].button = button;
		button->SetToolTip( wxT( tooltip.c_str() ) );
		GroupsOpsButtonSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );
	}

	//////////
	/** Spectrum Button */
	for( unsigned int i = 0; i < m_spectrumButton.size(); i++ ) {
		kome::plugin::PluginFunctionItem* item = m_spectrumButton[ i ].fun;
		kome::plugin::PluginIcon* icon = item->getCall()->getIcon();
		std::string tooltip = FMT( "%s", item->getLongName() );

		wxBitmapButton* button = new wxBitmapButton(
			this,
			ID_MIN_IDENT_SPECTRUM_BUTTON + i,
			m_spectrumButton[ i ].icon0
		);

		m_spectrumButton[ i ].button = button;
		button->SetToolTip( wxT( tooltip.c_str() ) );
		
		GroupsOpsButtonSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5 );
	}

	sizer->Add(GroupsOpsButtonSizer, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	wxEndBusyCursor();

	return sizer;
}

// create button sizer
wxSizer* SubstanceDialog::createButtonSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// button
	wxButton* button = new wxButton( this, wxID_OK, wxT( "Close" ) );
	sizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );

	return sizer;
}


// save data
void SubstanceDialog::saveData( wxCommandEvent& evt ) {
	m_list->exportData();
}

// delete data
void SubstanceDialog::deleteData( wxCommandEvent& evt ) {
	// confirm
	if( !kome::window::WindowTool::showYesNo( "Are you sure you want to delete the selected item?", "Delete", "Don't delete" ) ) {
		return;
	}

	// selected item
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( m_list, items );

	// delete
	wxBeginBusyCursor();
	for( unsigned int i = 0; i < items.size(); i++ ) {
		const long long data = m_list->getData( items[ i ] );
		m_list->deleteData( data, false );
	}
	wxEndBusyCursor();

	m_list->updateList();

	m_list->Refresh();
}

// update UI
void SubstanceDialog::updateUI( wxUpdateUIEvent& evt ) {
	bool enable = true;

	if( evt.GetId() == ID_DELETE_BUTTON ) {
		std::vector< int > items;
		kome::window::ListTool::getSelectedItems( m_list, items );

		enable = ( items.size() > 0 );
	}

	evt.Enable( enable );
}

// get the status name
std::string SubstanceDialog::getStatusNameSubstance( const unsigned int idx ) {
	// string
	std::string s;

	// check the index
	if( idx >= m_substanceButton.size() ) {
		return s;
	}

	// name
	s = FMT( "ident_substance_%s", m_substanceButton[ idx ].fun->getShortName() );

	return s;
}

// get the status name
std::string SubstanceDialog::getStatusNameSpectrum( const unsigned int idx ) {
	// string
	std::string s;

	// check the index
	if( idx >= m_substanceButton.size() ) {
		return s;
	}

	// name
	s = FMT( "ident_spectrum_%s", m_substanceButton[ idx ].fun->getShortName() );

	return s;
}

// on peak button
void SubstanceDialog::onSubstanceButton( wxCommandEvent& evt ) {
	// index
	int idx = evt.GetId() - ID_MIN_IDENT_SUBSTANCE_BUTTON;
	if( idx < 0 || idx >= (int)m_substanceButton.size() ) {
		return;
	}

	// TPPプラグインが実装された後、タンパク情報を表示するウインドウを開く機能アリ

	// substance ID
	int nSubstanceID = -1;
	if( m_itemIdSelected->m_type == TreeData::TYPE_SUBSTANCE ) {
		nSubstanceID = m_itemIdSelected->m_id;
	}
	if( nSubstanceID < 0 ) {
		return;
	}

	// 関数呼び出し
	wxBeginBusyCursor();	// busy cursor

	kome::plugin::PluginFunctionItem* item = m_substanceButton[ idx ].fun;
	if( item != NULL ){
		kome::objects::Parameters params;

		// nSubstanceID
		kome::plugin::PluginCallTool::setSubstanceID( params, nSubstanceID );

		// search ID
		kome::plugin::PluginCallTool::setSearchID( params, m_llSearchID );

		// wxWindow*
		kome::plugin::PluginCallTool::setCanvas( params, this );

		item->getCall()->invoke( &params );
	}

	wxEndBusyCursor();	// busy cursor

}

// on update peak button
void SubstanceDialog::onSubstanceUpdateUIButton( wxUpdateUIEvent& evt ) {
	evt.Enable( m_itemIdSelected != NULL && m_itemIdSelected->m_type == TreeData::TYPE_SUBSTANCE );
}

// on Spectrum Button
void SubstanceDialog::onSpectrumButton( wxCommandEvent& evt ) {
	// index
	int idx = evt.GetId() - ID_MIN_IDENT_SPECTRUM_BUTTON;
	if( idx < 0 || idx >= (int)m_spectrumButton.size() ) {
		return;
	}

	// Spectrum ID
	int nSpectrumID = -1;
	if( m_itemIdSelected->m_type == TreeData::TYPE_SPECTRUM ) {
		nSpectrumID = m_itemIdSelected->m_id;
	}
	if( nSpectrumID < 0 ) {
		return;
	}

	// 関数呼び出し

	wxBeginBusyCursor();	// busy cursor

	// IdentificationManager
	kome::ident::IdentificationManager& identificationMgr = kome::ident::IdentificationManager::getInstance();

	// SearchID
	const int nSearchID = m_llSearchID;

	// search Spectrum ID
	int nSearchSpectrumID = identificationMgr.getSearchSpectrum( nSearchID, nSpectrumID );

	kome::plugin::PluginFunctionItem* item = m_spectrumButton[ idx ].fun;
	if( item != NULL ){
		kome::objects::Parameters params;

		// search Spectrum ID
		kome::plugin::PluginCallTool::setSearchSpectrumID( params, nSearchSpectrumID );

		// wxWindow*
		kome::plugin::PluginCallTool::setCanvas( params, this );

		item->getCall()->invoke( &params );
	}

	wxEndBusyCursor();	// busy cursor
}

// on Spectrum Update UI Button
void SubstanceDialog::onSpectrumUpdateUIButton( wxUpdateUIEvent& evt ) {
	evt.Enable( m_itemIdSelected != NULL && m_itemIdSelected->m_type == TreeData::TYPE_SPECTRUM );
}

// clear status
void SubstanceDialog::clearStatus( const char szInitStr[] ){
	// ボタンを初期化

	/** Substance Button */
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	for( unsigned int i = 0; i < m_substanceButton.size(); i++ ) {

		// status
		std::string name = getStatusNameSubstance( i );
		if( name.empty() ) {
			continue;
		}

		statusMgr.setValue( name.c_str(), szInitStr );
	}

	/** Spectrum Button */
	for( unsigned int i = 0; i < m_spectrumButton.size(); i++ ) {

		// status
		std::string name = getStatusNameSpectrum( i );
		if( name.empty() ) {
			continue;
		}

		statusMgr.setValue( name.c_str(), szInitStr );
	}

	return;
}

// set Search ID
void SubstanceDialog::setSearchID( long long llSearchID )
{
	m_llSearchID = llSearchID;
}

void SubstanceDialog::onSelectTree(  wxTreeEvent& evt ) {
	wxTreeItemId item = evt.GetItem();
	m_itemIdSelected = (TreeData*)m_tree->GetItemData( item );

	// clear
	m_list->clearValues();

	for( unsigned int i = 0; i < m_itemIdSelected->m_props.getNumberOfProperties(); i++ ) {
		m_list->addValue( m_itemIdSelected->m_props.getKey( i ), m_itemIdSelected->m_props.getValue( i ) );
	}

	// listを再描画m
	m_list->create();
	m_list->Refresh();
}

// make tree
void SubstanceDialog::makeTree( const int substance ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// add substance node
	wxTreeItemId treeItem = addSubstanceNode( substance, m_root );

	// add spectrum node
	std::vector< kome::ident::HitInfo >& hits = m_hitsMap[ substance ];

	for( unsigned int i = 0; i < hits.size(); i++ ) {
		kome::ident::HitInfo& hitInfo = hits[ i ];
		for( unsigned int j = 0; j < hitInfo.spectra.size(); j++ ) {
			int searchSpecId = hitInfo.spectra.at( j );
			kome::ident::SpecInfo specInfo;
			if( m_specMap.find( searchSpecId ) == m_specMap.end() ) {
				double precursor = 0.0;
				int specId = 0;
				identMgr.getSearchSpectrum( searchSpecId, &specId, NULL, NULL, &precursor );
				identMgr.getSpectrum( specId, specInfo );
				specInfo.precursor = precursor;
				
				m_specMap[ searchSpecId ] = specInfo;
			}
			else {
				specInfo = m_specMap[ searchSpecId ];
			}
			
			// add node
			std::string name;
			if( specInfo.precursor >= 0.0 ) {
				name = FMT( "%s (%.2f) [%.2f]", specInfo.name.c_str(), specInfo.retentionTime, specInfo.precursor );
			}
			else {
				name = FMT( "%s (%.2f)", specInfo.name.c_str(), specInfo.retentionTime );
			}
			
			int img = CLAMP( specInfo.stage, 1, 8 );
			TreeData* data = new TreeData( TreeData::TYPE_SPECTRUM );
			data->m_id = specInfo.id;
			data->m_props.setValue( "Sample", specInfo.sampleName.c_str() );
			data->m_props.setValue( "Path", specInfo.path.c_str() );
			data->m_props.setIntValue( "Spectrum ID", specInfo.id );
			data->m_props.setValue( "Spectrum Name", specInfo.name.c_str() );
			data->m_props.setIntValue( "MS Stage", specInfo.stage );
			data->m_props.setDoubleValue( "Precursor", specInfo.precursor );

			for( unsigned int k = 0; k < hitInfo.props.size(); k++ ) {
				data->m_props.setValue( hitInfo.props.at( k ).name.c_str(), hitInfo.props.at( k ).value.c_str() );
			}

			m_tree->AppendItem( treeItem, wxT( name.c_str() ), img, img, data );
		}
	}

	// add alter protein node
	for( unsigned int i = 0; i < hits.size(); i++ ) {    // each hits
		kome::ident::HitInfo& hitInfo = hits[ i ];
		for( unsigned int j = 0; j < hitInfo.altSubstances.size(); j++ ) {    // alternative proteins
			kome::ident::AltSubstance& altSub = hitInfo.altSubstances.at( j );
			wxTreeItemId altNodeId = treeItem;

			altNodeId = addSubstanceNode( altSub.id, treeItem );
			TreeData* data = (TreeData*)m_tree->GetItemData( altNodeId );
			data->m_props.setIntValue( "Start", altSub.start );
			data->m_props.setIntValue( "End", altSub.end );
			data->m_props.setValue( "Residue Before", altSub.before.c_str() );
			data->m_props.setValue( "Residue After", altSub.after.c_str() );

			for( unsigned int k = 0; k < hitInfo.spectra.size(); k++ ) {				
				int searchSpecId = hitInfo.spectra.at( k );
				kome::ident::SpecInfo specInfo = m_specMap[ searchSpecId ];

				std::string name;
				if( specInfo.precursor >= 0.0 ) {
					name = FMT( "%s (%.2f) [%.2f]", specInfo.name.c_str(), specInfo.retentionTime, specInfo.precursor );
				}
				else {
					name = FMT( "%s (%.2f)", specInfo.name.c_str(), specInfo.retentionTime );
				}

				int img = CLAMP( specInfo.stage, 1, 8 );
				TreeData* data = new TreeData( TreeData::TYPE_SPECTRUM );
				data->m_id = specInfo.id;
				data->m_props.setValue( "Sample", specInfo.sampleName.c_str() );
				data->m_props.setValue( "Path", specInfo.path.c_str() );
				data->m_props.setIntValue( "Spectrum ID", specInfo.id );
				data->m_props.setValue( "Spectrum Name", specInfo.name.c_str() );
				data->m_props.setIntValue( "MS Stage", specInfo.stage );
				data->m_props.setDoubleValue( "Precursor", specInfo.precursor );
				
				for( unsigned int k = 0; k < hitInfo.props.size(); k++ ) {
					data->m_props.setValue( hitInfo.props.at( k ).name.c_str(), hitInfo.props.at( k ).value.c_str() );
				}

				m_tree->AppendItem( altNodeId, wxT( name.c_str() ), img, img, data );
			}
		}
	}
}

// add substance node
wxTreeItemId SubstanceDialog::addSubstanceNode( const int substance, wxTreeItemId parent ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// check the map
	if( m_substanceIdMap.find( substance ) != m_substanceIdMap.end() ) {
		return m_substanceIdMap[ substance ];
	}

	// substance
	kome::ident::SubstanceInfo substanceInfo;
	identMgr.getSubstance( substance, substanceInfo );
	m_substanceMap[ substance ] = substanceInfo;

	// name
	std::string name = substanceInfo.name;
	if( !substanceInfo.accession.empty() ) {
		name = FMT( "%s : %s", substanceInfo.accession.c_str(), substanceInfo.name.c_str() );
	}

	// add
	wxTreeItemId id;
	TreeData* data = new TreeData( TreeData::TYPE_SUBSTANCE );
	data->m_id = substance;
	data->m_props.setValue( "Accession", substanceInfo.accession.c_str() );
	data->m_props.setValue( "Name", substanceInfo.name.c_str() );

	for( unsigned int i = 0; i < substanceInfo.props.getNumberOfProperties(); i++ ) {
		data->m_props.setValue( substanceInfo.props.getKey( i ), substanceInfo.props.getValue( i ) );
	}

	if( substanceInfo.parentId < 0 ) {
		id = m_tree->AppendItem( parent, wxT( name.c_str() ), 0, 0, data );
	}
	else {
		parent = addSubstanceNode( substanceInfo.parentId, parent );
		id = m_tree->AppendItem( parent, wxT( name.c_str() ), 0, 0, data );
	}

	m_substanceIdMap[ substance ] = id;

	return id;
}

// "component-r"のアイコンのパスを求める
const char* SubstanceDialog::getIconPath( void ){

    // manager
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// ボタンを表示する準備
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	const unsigned int num = plgMgr.getNumberOfFunctionItems( "IDENT_RESULT" );
	for( unsigned int i = 0 ; i < num; i++ ) {
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "IDENT_RESULT", i );
		kome::plugin::PluginIcon* icon = item->getCall()->getIcon();
		if( icon != NULL ) {
			
			const char *pszIconName = icon->getName();

			if( strcmp( "component-r", pszIconName ) == 0 ){
				return icon->getPath();
			}
		}
	}

	return "";
}


//
// TreeData
//

// constructor
TreeData::TreeData( NodeType type ) {
	m_type = type;
}

// destructor
TreeData::~TreeData() {
}
