/**
 * @file BatchActionSelectPanel.cpp
 * @brief interfaces of BatchActionSelectPanel class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "BatchActionSelectPanel.h"
#include "BatchProcessingMethodConfigurationDialog.h"
#include "BatchActionGrid.h"
#include "BatchButtonDef.h"

#define ID_EDIT_BUTTON                    67
#define ID_DEL_BUTTON                     68
#define ID_UP_BUTTON                      69
#define ID_DOWN_BUTTON                    70

#define IDENTIFICATION_FUNC_TYPE     "IDENTIFICATION"

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( BatchActionSelectPanel, BatchBase )	
	EVT_MENU( ID_EDIT_BUTTON, BatchActionSelectPanel::onEdit )
	EVT_MENU( ID_UP_BUTTON, BatchActionSelectPanel::onUp )
	EVT_MENU( ID_DOWN_BUTTON, BatchActionSelectPanel::onDown )

	EVT_BUTTON( ID_BAT_SELECT_BUTTON,	BatchActionSelectPanel::onAdd )
	EVT_BUTTON( ID_BAT_EDIT_BUTTON,		BatchActionSelectPanel::onEdit )
	EVT_BUTTON( ID_BAT_DELETE_BUTTON,	BatchActionSelectPanel::onDelete )
	EVT_BUTTON( ID_BAT_UP_BUTTON,		BatchActionSelectPanel::onUp )
	EVT_BUTTON( ID_BAT_DOWN_BUTTON,		BatchActionSelectPanel::onDown )

	EVT_UPDATE_UI( ID_BAT_SELECT_BUTTON,	BatchActionSelectPanel::onUpdateUI )
	EVT_UPDATE_UI( ID_BAT_EDIT_BUTTON,		BatchActionSelectPanel::onUpdateUI )
	EVT_UPDATE_UI( ID_BAT_DELETE_BUTTON,	BatchActionSelectPanel::onUpdateUI )
	EVT_UPDATE_UI( ID_BAT_UP_BUTTON,		BatchActionSelectPanel::onUpdateUI )
	EVT_UPDATE_UI( ID_BAT_DOWN_BUTTON,		BatchActionSelectPanel::onUpdateUI )

END_EVENT_TABLE()

#define LIST_SELECTED
#define CENTER_BUTTON
#define LIST_AVAILABLE

#define GRID_GAP	3

// constructor
BatchActionSelectPanel::BatchActionSelectPanel( wxWindow* pParent, BatchProcessingMethodConfigurationDialog* pParentDlg, int nPanelNo ) : BatchBase(pParent, pParentDlg, "Get Peak Positions"), m_params( NULL, false )
{
	m_pParentDlg = pParentDlg;

	m_pBatchListSelected = NULL;
	m_pBatchListAvailable = NULL;

#ifdef LIST_SELECTED
	m_pBatchListSelected  = new BatchActionGrid((wxWindow*)this, nPanelNo, 0 );

#endif
#ifdef LIST_AVAILABLE
	m_pBatchListAvailable = new BatchActionGrid((wxWindow*)this, nPanelNo, 1 );
#endif
	
	m_nPanelNo = nPanelNo;
}

// destructor
BatchActionSelectPanel::~BatchActionSelectPanel(void)
{
}


void BatchActionSelectPanel::createInnerSizer(wxStaticBoxSizer* pInnerSizer)
{
	static int flg = 0;
	if( flg ){
		return;
	}
	// SPEC No.91863 @date 2013.04.01 <Mod> M.Izumi ->
	// manager
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();
		
	wxSizer* gHoriznSizer = new wxBoxSizer( wxHORIZONTAL);
	wxBoxSizer* LeftListSizer = new wxBoxSizer( wxVERTICAL );

	//　左側の表
	wxStaticText* staticText = new wxStaticText( this,
		wxID_STATIC,
		wxT("Selected Actions" ),
		wxDefaultPosition,
		wxDefaultSize,
		wxALIGN_LEFT );

	LeftListSizer->Add( staticText, 0, wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );
	if( m_pBatchListSelected != NULL ) {
		m_pBatchListSelected->create();
		LeftListSizer->Add( m_pBatchListSelected, 1, wxALL | wxSHAPED, BORDER_SIZE );
	}
	
	// [←]Button
	wxBoxSizer* buttonSizerDs = new wxBoxSizer(wxHORIZONTAL);

	m_pBatConfEditButton = new wxBitmapButton( this, ID_BAT_EDIT_BUTTON, *iconMgr.getIcon("edit") );
	m_pBatConfEditButton->SetToolTip("Edit action");
	buttonSizerDs->Add(m_pBatConfEditButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfDeleteButton = new wxBitmapButton( this, ID_BAT_DELETE_BUTTON, *iconMgr.getIcon("item_del") );
	m_pBatConfDeleteButton->SetToolTip("Delete action");
	buttonSizerDs->Add(m_pBatConfDeleteButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfUpButton = new wxBitmapButton( this, ID_BAT_UP_BUTTON, *iconMgr.getIcon("item_up") );
	m_pBatConfUpButton->SetToolTip("Move action up");
	buttonSizerDs->Add(m_pBatConfUpButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfDownButton = new wxBitmapButton( this, ID_BAT_DOWN_BUTTON, *iconMgr.getIcon("item_down") );
	m_pBatConfDownButton->SetToolTip("Move action down");
	buttonSizerDs->Add(m_pBatConfDownButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	LeftListSizer->Add( buttonSizerDs, 0, (wxALL & ~wxLEFT) | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	gHoriznSizer->Add( LeftListSizer, 1,  wxALL | wxEXPAND, BORDER_SIZE);

	// [←]ボタン
	wxBoxSizer* btnSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pBatConfSelectActionButton = new wxBitmapButton( this, ID_BAT_SELECT_BUTTON, *iconMgr.getIcon( "batch_item_add" ) );
	m_pBatConfSelectActionButton->SetToolTip( "Select action" );

	btnSizer->Add( m_pBatConfSelectActionButton, 2, (wxALL & ~wxLEFT) | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	gHoriznSizer->Add( btnSizer, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// right list sizer
	wxBoxSizer* RightListSizer = new wxBoxSizer( wxVERTICAL );
	// 右側の表
	wxStaticText* staticText2 = new wxStaticText( this,
		wxID_STATIC,
		wxT( "Available Actions" ),
		wxDefaultPosition,
		wxDefaultSize,
		wxALIGN_LEFT );
	RightListSizer->Add( staticText2, 0, wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );

	if( m_pBatchListAvailable != NULL ) {
		m_pBatchListAvailable->create();
		RightListSizer->Add( m_pBatchListAvailable, 1, wxALL | wxSHAPED, BORDER_SIZE );
	}

	gHoriznSizer->Add( RightListSizer, 1,  wxALL | wxSHAPED, BORDER_SIZE);
	
	pInnerSizer->Add( gHoriznSizer, 1, wxSHAPED , BORDER_SIZE );
	// @date 2013.04.01 <Mod> M.Izumi <-
}


// ID_BAT_SELECT_BUTTON
void BatchActionSelectPanel::onAdd( wxCommandEvent& evt ){

	int nSelected = m_pBatchListAvailable->GetSelectedItemCount();
	if( nSelected <= 0 ){
		// NG
	}else{
		std::vector< int > items;
		kome::window::ListTool::getSelectedItems( m_pBatchListAvailable, items );

		if( items.size() <= 0 ){
			// 選択データ無し
		}else{
			// 選択データあり
			for( unsigned int nIndex=0; nIndex<items.size(); nIndex++ ){
				int nSize = m_pBatchListAvailable->getCountOfVectActions();
				if( items[nIndex] < nSize ){
					Action actTemp;
					actTemp.vec_strSettings.clear();	// @date 2013/03/28 <Add> OKADA
					actTemp = m_pBatchListAvailable->getVectActions( items[nIndex] );
					actTemp.flgSettings = 0;

					kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
					kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "batch", actTemp.strNameShort.c_str() );	 // 2012/03/27

					kome::plugin::SettingsPage * spage = NULL;
					if( pItem != NULL ){
						spage = pItem->getSettingsPage();
					}

					kome::objects::SettingParameterValues settings;

					if( spage != NULL ){

						// @date 2013/04/24 <Add> OKADA ------->
						// SPEC 92361対応
						if ( m_nPanelNo == 4 ){	// 4 = 「5.Identify タブ」
							bool bRet = prepare();
							if( bRet == false ){
								return;
							}
						}
						// @date 2013/04/24 <Add> OKADA <-------

						// @date 2013/09/10 <Add> OKADA ------->
						if( 3 == m_nPanelNo ){
							int nRet = actTemp.strNameShort.compare( "comparative_ident" );
							if( 0 == nRet ){
								bool bRet = prepare();
								if( bRet == false ){
									return;
								}
							}
						}
						// @date 2013/09/10 <Add> OKADA <-------


						kome::window::SettingsPageDialog dlg(
							kome::window::WindowTool::getMainWindow(),
							spage,
							&settings,
							true
						);
						if( dlg.ShowModal() == wxID_OK ) {

							unsigned int uiNum = settings.getNumberOfParameters();

							for( unsigned int i=0; i<uiNum; i++ ){
								const char* n = settings.getParameterName( i );
								const char* v = settings.getParameterValue( i );

								bool bVal = settings.hasSubParameterValues( n, v );
								if( bVal ){
									kome::objects::SettingParameterValues* pSettingsSub = settings.getSubParameterValues( n, v );
									unsigned int uiNumSub  = pSettingsSub->getNumberOfParameters();

									for( unsigned int j=0; j<uiNumSub; j++ ){
										const char* pszNSub = pSettingsSub->getParameterName( j );
										const char* pszVSub = pSettingsSub->getParameterValue( j );
									}
								}
							}
							
							actTemp.flgSettings = 1;
							
							// @date 2013/03/28 <Add> OKADA ------->
							if( spage != NULL ){
								std::string strSettings = spage->getParameterString( settings );
								actTemp.strSettings = strSettings;	// パラメータ保存用の[○,○,○,○]形式の文字列を得る
							}

							// @date 2013/03/28 <Add> OKADA ------->
							// サブParameterがあるか確認
							bool flgSubParam = false;
							for( unsigned int i=0; i<uiNum; i++ ){	// @date 2013/06/10 <Mod> OKADA
								flgSubParam = false;
								const char* n = settings.getParameterName( i );
								const char* v = settings.getParameterValue( i );
								std::string strActStr = "";
								if(	( spage->getForm( i )->getParam()->getValue()->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) )
								{
									// サブParameterがある可能性がある
									
									const char* szType = spage->getForm( i )->getParam()->getValue()->getEnumKey();
									const char* szName = actTemp.strNameShort.c_str();

									kome::plugin::PluginFunctionItem* pItemSub = plgMgr.getFunctionItem( szType, v );	 // 2013/03/28
									if( pItemSub == NULL ){
										// サブParameterは無い
									}else{
										kome::plugin::SettingsPage * sSubPage = pItemSub->getSettingsPage();

										if( sSubPage != NULL ){
											kome::objects::SettingParameterValues* pSettingsSub = settings.getSubParameterValues( n, v );
											if( pSettingsSub != NULL ){
												// サブParameterがある
												flgSubParam =true;
												std::string strSettings = sSubPage->getParameterString( *pSettingsSub );

												strActStr = v;
												strActStr.append( strSettings );
											}
										}
									}
								}

								if( flgSubParam == false ){
									actTemp.vec_strSettings.push_back( v );
								}else{
									actTemp.vec_strSettings.push_back( strActStr );
								}
							}
							// @date 2013/03/28 <Add> OKADA <-------							
						}
						else{
							// dlg.ShowModal()でcancel
							return;
						}
					}

					m_pBatchListSelected->clearData( true );

					m_pBatchListSelected->addAction( actTemp );

					m_pBatchListSelected->updateList();
					m_pBatchListSelected->RefreshBatchActionGrid();

					// refresh
					kome::window::WindowTool::refresh();

					volatile int jjj=0;
				}else{
					// 範囲外
				}
			}
		}

		unsigned int uiSelectedRowLine = nSelected;
		long long llData = m_pBatchListAvailable->getData( uiSelectedRowLine );

		volatile int iii=0;
	}

	// Batch Processing Method Configurationの上の表を更新する
	m_pParentDlg->updateBatchList();

	return;
}

// ID_BAT_EDIT_BUTTON
void BatchActionSelectPanel::onEdit( wxCommandEvent& evt ) {

	int nSelected = m_pBatchListSelected->GetSelectedItemCount();
	if( nSelected <= 0 ){
		// NG
	}else{
		std::vector< int > items;
		kome::window::ListTool::getSelectedItems( m_pBatchListSelected, items );

		if( items.size() <= 0 ){
			// 選択データ無し
		}else{
			// 選択データあり
			for( unsigned int i=0; i<items.size(); i++ ){
				int nSize = m_pBatchListSelected->getCountOfVectActions();
				if( items[i] < nSize ){

					if( editByIndex(items[i]) == 1 ){
						return;
					}

				}
			}
		}
	}

	// Batch Processing Method Configurationの上の表を更新する
	m_pParentDlg->updateBatchList();

	return;
}

// [Edit]ボタン押下時（onEdit()処理）
int BatchActionSelectPanel::editByIndex( int nIndex )
{
	Action actTemp;
	actTemp = m_pBatchListSelected->getVectActions( nIndex );

	if( actTemp.nBatchIndex < 0 ){	// 通常あり得ない
		return 0;
	}

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "batch", actTemp.strNameShort.c_str() );	 // 2012/03/27

	kome::plugin::SettingsPage * spage = NULL;
	if( pItem != NULL ){
		spage = pItem->getSettingsPage();
	}

	kome::objects::SettingParameterValues settings;

	if( spage != NULL ){			// @date 2013/03/28 <Add> OKADA
		spage->setParameterString( settings, actTemp.strSettings.c_str() );
	}								
	
	if( spage != NULL ){
		kome::window::SettingsPageDialog dlg(
			kome::window::WindowTool::getMainWindow(),
			spage,
			&settings,
			true
		);

		std::string strOldPramValue = "";	// SPEC 85856
		std::string strNewPramValue = "";	// SPEC 85856

		// SPEC 85856 ------->
		if( spage != NULL ){
			std::string strSettings = spage->getParameterString( settings );
			strOldPramValue = strSettings;	// 以前の文字列を保存
		}// <------ SPEC 85856

		if( dlg.ShowModal() == wxID_OK ) {

			if( spage != NULL ){
				std::string strSettings = spage->getParameterString( settings );
				actTemp.strSettings = strSettings;	// パラメータ保存用の[○,○,○,○]形式の文字列を得る
				strNewPramValue = strSettings;
			}

			if( strOldPramValue == strNewPramValue ){	// SPEC 85856 関連
				// 書き換えられていない
				return 1;
			}

			actTemp.flgSettings = 1;

			unsigned int uiNum = settings.getNumberOfParameters();

			for( unsigned int i=0; i<uiNum; i++ ){
				const char* n = settings.getParameterName( i );
				const char* v = settings.getParameterValue( i );

				bool bVal = settings.hasSubParameterValues( n, v );
				if( bVal ){
					kome::objects::SettingParameterValues* pSettingsSub = settings.getSubParameterValues( n, v );
					unsigned int uiNumSub  = pSettingsSub->getNumberOfParameters();

					for( unsigned int j=0; j<uiNumSub; j++ ){
						const char* pszNSub = pSettingsSub->getParameterName( j );
						const char* pszVSub = pSettingsSub->getParameterValue( j );
					}
				}
			}

			// @date 2013/03/29 <Add> OKADA ------->
			// サブParameterがあるか確認
			bool flgSubParam = false;
			for( unsigned int i=0; i<uiNum; i++ ){	// @date 2013/06/10 <Mod> OKADA
				flgSubParam = false;
				const char* n = settings.getParameterName( i );
				const char* v = settings.getParameterValue( i );
				std::string strActStr = "";
				if(	( spage->getForm( i )->getParam()->getValue()->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN )	)
				{
					// サブParameterがある可能性がある
									
					const char* szType = spage->getForm( i )->getParam()->getValue()->getEnumKey();
					const char* szName = actTemp.strNameShort.c_str();

					kome::plugin::PluginFunctionItem* pItemSub = plgMgr.getFunctionItem( szType, v );	 // 2013/03/28
					if( pItemSub == NULL ){
						// サブParameterは無い
					}else{
						kome::plugin::SettingsPage * sSubPage = pItemSub->getSettingsPage();
			
						if( sSubPage != NULL ){
							kome::objects::SettingParameterValues* pSettingsSub = settings.getSubParameterValues( n, v );
							if( pSettingsSub != NULL ){
								// サブParameterがある
								flgSubParam =true;
								std::string strSettings = sSubPage->getParameterString( *pSettingsSub );

								strActStr = v;
								strActStr.append( strSettings );
							}
						}
					}
				}

				if( i == 0 ){
					actTemp.vec_strSettings.clear();
				}

				if( flgSubParam == false ){
					actTemp.vec_strSettings.push_back( v );
				}else{
					actTemp.vec_strSettings.push_back( strActStr );
				}
							
			}
			// @date 2013/03/29 <Add> OKADA <-------

		}
		else{
			// 書き換えられていない	// SPEC 85856 関連
			return 1;
		}
	}

	m_pBatchListSelected->clearData( false );
	m_pBatchListSelected->updateAction( nIndex, actTemp );
	m_pBatchListSelected->RefreshBatchActionGrid();
	m_pBatchListSelected->updateList();

	// refresh
	kome::window::WindowTool::refresh();

	return 0;
}


// ID_BAT_DELETE_BUTTON
void BatchActionSelectPanel::onDelete( wxCommandEvent& evt ) {

	int nSelected = m_pBatchListSelected->GetSelectedItemCount();
	if( nSelected <= 0 ){
		// NG
	}else{
		std::vector< int > items;
		kome::window::ListTool::getSelectedItems( m_pBatchListSelected, items );

		if( items.size() <= 0 ){
			// 選択データ無し
		}else{
			// 選択データあり
			for( long i=(((long)items.size())-1); 0<=i; i-- ){
				int nSize = m_pBatchListSelected->getCountOfVectActions();
				if( items[i] < nSize ){
					int nRet=-1;

					onDeleteLine( items[i] );

				}
			}
		}
	}

	// Batch Processing Method Configurationの上の表を更新する
	m_pParentDlg->updateBatchList();

	return;
}


// on delete line
int BatchActionSelectPanel::onDeleteLine( int index, bool flgUpdate ){
	int nRet = m_pBatchListSelected->deleteAction( index );
	if( nRet == 0 ){
		m_pBatchListSelected->clearData( false );
		m_pBatchListSelected->RefreshBatchActionGrid();
		m_pBatchListSelected->updateList();
	}

	if( flgUpdate ){
		// Batch Processing Method Configurationの上の表を更新する
		m_pParentDlg->updateBatchList();
	}

	return nRet;
}


// ID_BAT_UP_BUTTON
void BatchActionSelectPanel::onUp( wxCommandEvent& evt ) {
	int nSelected = m_pBatchListSelected->GetSelectedItemCount();
	if( nSelected <= 0 ){
		// NG
	}else{

		std::vector< int > items;
		kome::window::ListTool::getSelectedItems( m_pBatchListSelected, items );

		if( items.size() <= 0 ){
			// 選択データ無し
		}else{
			// 選択データあり
			for( unsigned int i=0; i<items.size(); i++ ){
				int nSize = m_pBatchListSelected->getCountOfVectActions();
				if( items[i] < nSize ){
					Action actTemp;
					actTemp = m_pBatchListSelected->getVectActions( items[i] );

					int nRet=-1;

					// UPの場合
					if( items[i] <= 0 ){
						// 一番上なので、動かせない
						// break;
						continue;
					}else{

						// 移動先（items[i]-1）が選択セル( items[i])なら、移動させない ※選択セルの順番を入れ替えない為
						int nTarget = (items[i]-1);
						for( unsigned int j=0; j<items.size(); j++ ){
							int nSelectedCell = items[j];
							if( nTarget == nSelectedCell ){
								continue;	// 移動先は選択セル
							}
						}
						nRet = m_pBatchListSelected->swapAction( items[i], items[i]-1 );
					}

					if( nRet == 1 ){
						m_pBatchListSelected->clearData( false );
						m_pBatchListSelected->RefreshBatchActionGrid();

						bool flgSelect = kome::window::ListTool::isSelected( m_pBatchListSelected, items[i]-1 );
						kome::window::ListTool::selectItem( m_pBatchListSelected, items[i]-1 );
						if( flgSelect ){
							kome::window::ListTool::selectItem( m_pBatchListSelected, items[i] );
						}else{
							kome::window::ListTool::unselectItem( m_pBatchListSelected, items[i] );
						}

						m_pBatchListSelected->Refresh();

						// refresh
						kome::window::WindowTool::refresh();
					}
				}
			}
		}
	}

	// Batch Processing Method Configurationの上の表を更新する
	m_pParentDlg->updateBatchList();

	return;
}


// ID_BAT_DOWN_BUTTON
void BatchActionSelectPanel::onDown( wxCommandEvent& evt ) {

	int nSelected = m_pBatchListSelected->GetSelectedItemCount();
	if( nSelected <= 0 ){
		// NG
	}else{

		std::vector< int > items;
		kome::window::ListTool::getSelectedItems( m_pBatchListSelected, items );

		if( items.size() <= 0 ){
			// 選択データ無し
		}else{
			// 選択データあり
			for( long i=(((long)items.size())-1); 0<=i; i-- )
			{
				int nSize = m_pBatchListSelected->getCountOfVectActions();
				if( items[i] < nSize ){
					Action actTemp;
					actTemp = m_pBatchListSelected->getVectActions( items[i] );

					int nRet=-1;

					// DOWN の場合
					if( nSize <= (items[i]+1) ){
						// 一番下なので、動かせない
						// break;
						continue;
					}else{
						// 移動先（items[i]-1）が選択セル( items[i])なら、移動させない ※選択セルの順番を入れ替えない為
						for( unsigned int j=0; j<items.size(); j++ ){
							if(  items[i]+1 ==  items[j] ){
								continue;	// 移動先は選択セル
							}
						}
						nRet = m_pBatchListSelected->swapAction( items[i], items[i]+1 );
					}

					if( nRet == 1 ){
						m_pBatchListSelected->clearData( false );
						m_pBatchListSelected->RefreshBatchActionGrid();

						bool flgSelect = kome::window::ListTool::isSelected( m_pBatchListSelected, items[i]+1 );
						kome::window::ListTool::selectItem( m_pBatchListSelected, items[i]+1 );
						if( flgSelect ){
							kome::window::ListTool::selectItem( m_pBatchListSelected, items[i] );
						}else{
							kome::window::ListTool::unselectItem( m_pBatchListSelected, items[i] );
						}

						m_pBatchListSelected->Refresh();
						// refresh
						kome::window::WindowTool::refresh();
					}
				}
			}
		}
	}

	// Batch Processing Method Configurationの上の表を更新する
	m_pParentDlg->updateBatchList();

	return;
}


void BatchActionSelectPanel::onUpdateUI( wxUpdateUIEvent& evt ){
	
	int nId = evt.GetId();
	switch( nId ){
	case ID_BAT_SELECT_BUTTON:
		{
			std::string strTemp = m_pParentDlg->getComboMethodText();
			int nAvailable = m_pBatchListAvailable->GetSelectedItemCount();
			if(	( 0 < strTemp.length() ) && ( 0 < nAvailable ) ) {	// @date 2012/05/08 <Mod> OKADA	// SPEC 85902
				// Batch Methodが空欄でない AND Availableセルが選択されている
				evt.Enable( true );
			}else{
				evt.Enable( false );
			}
		}
		break;
	case ID_BAT_EDIT_BUTTON:
		{
			// セルが選択されている AND そのセルのActionはパラメータを持っている -> enable
			bool flgEnable = false;

			int nSelected = m_pBatchListSelected->GetSelectedItemCount();
			if( nSelected <= 0 ){
				// NG
			}else{
				std::vector< int > items;
				kome::window::ListTool::getSelectedItems( m_pBatchListSelected, items );
				
				if( items.size() <= 0 ){
					// 選択データ無し
				}else{
					// 選択データあり
					for( unsigned int i=0; i<items.size(); i++ ){
						int nSize = m_pBatchListSelected->getCountOfVectActions();
						if( items[i] < nSize ){
							Action actTemp;
							actTemp = m_pBatchListSelected->getVectActions( items[i] );

							if( actTemp.nBatchIndex < 0 ){	// 通常あり得ない
								continue;
							}

							kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
							kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "batch", actTemp.strNameShort.c_str() );

							kome::plugin::SettingsPage * spage = NULL;
							if( pItem != NULL ){
								spage = pItem->getSettingsPage();
							}

							if( spage != NULL ){
								// そのセルのActionはパラメータを持っている
								flgEnable = true;
								break;
							}
						}
					}
				}
			}
			if( flgEnable ){
				evt.Enable( true );
			}else{
				evt.Enable( false );
			}
		}
		break;
	case ID_BAT_DELETE_BUTTON:
		{
			int nSelected = m_pBatchListSelected->GetSelectedItemCount();
			if( nSelected <= 0 ){
				// NG
				evt.Enable( false );
			}else{
				// セルが選択されている->enable
				evt.Enable( true );
			}
		}
		break;
	case ID_BAT_UP_BUTTON:
		{
			// 仕様変更

			// セルが選択されている AND 一番上のセルが選択されていない AND 選択されているどれかのセルでそのセルと1つ上のセルのPrecedenceが同じ -> enable
			// セルが選択されてない OR  一番上のセルが選択されている   OR  選択されているセルのうち全てのセルで1つ上のセルのPrecedenceが違う -> disable

			bool flgEnable = true;

			int nSelected = m_pBatchListSelected->GetSelectedItemCount();
			if( nSelected <= 0 ){
				// NG
				flgEnable = false;
			}else{
				// セルが選択されている
				std::vector< int > items;
				kome::window::ListTool::getSelectedItems( m_pBatchListSelected, items );
				
				if( items.size() <= 0 ){
					// 選択データ無し
				}else{
					// 選択データあり
					int flgAllCheck = 1;	// SPEC 85677
					for( unsigned int i=0; i<items.size(); i++ ){
						if( items[i] <= 0 ){
							if( items.size() <= 0 ){
								flgEnable = false;
								break;
							}
						}else{
							// 一番上ではない

							int nSize = m_pBatchListSelected->getCountOfVectActions();
							if( items[i] < nSize ){
								Action actTemp;
								actTemp = m_pBatchListSelected->getVectActions( items[i] );
								Action actTempUp;
								actTempUp = m_pBatchListSelected->getVectActions( items[i]-1 );

								int flgSelected = 0;
								for( unsigned int j=0; j<items.size(); j++ ){
									if(  items[i]-1 ==  items[j] ){
										 flgSelected = 1;	// 移動先は選択セル
										 break;
									}
								}

								if( flgSelected ){
									continue;
								}
								if(actTemp.nPrecedence == actTempUp.nPrecedence){
									// 同じ->enable
									flgAllCheck = 0;	// SPEC 85677
									break;
								}else{
									// 違う
									continue;	
								}
							}
						}
					}
					if( flgAllCheck == 1 ){
						flgEnable = false;
					}
				}
			}

			if( flgEnable ){
				evt.Enable( true );
			}else{
				evt.Enable( false );
			}
		}
		break;
	case ID_BAT_DOWN_BUTTON:
		{
			// セルが選択されている AND 一番下のセルが選択されていない AND 選択されている全てのセルでそのセルと1つ下のセルのPrecedenceが同じ -> enable
			// セルが選択されてない OR  一番下のセルが選択されている   OR  選択されているセルのうちどれか１つで1つ下のセルのPrecedenceが違う -> disable
			bool flgEnable = true;

			int nSelected = m_pBatchListSelected->GetSelectedItemCount();
			if( nSelected <= 0 ){
				// NG
				flgEnable = false;
			}else{
				// セルが選択されている
				std::vector< int > items;
				kome::window::ListTool::getSelectedItems( m_pBatchListSelected, items );
				
				if( items.size() <= 0 ){
					// 選択データ無し
				}else{
					// 選択データあり
					int flgAllCheck = 1;	// SPEC 85677
					for( unsigned int i=0; i<items.size(); i++ ){
						int nBottomCnt = m_pBatchListSelected->getActionSize();
						if(( nBottomCnt-1) <= items[i] ){
							if( items.size() <= 0 ){
								flgEnable = false;
								break;
							}
						}else{
							// 一番下ではない

							int nSize = m_pBatchListSelected->getCountOfVectActions();
							if( items[i] < nSize ){
								Action actTemp;
								actTemp = m_pBatchListSelected->getVectActions( items[i] );
								Action actTempUp;
								actTempUp = m_pBatchListSelected->getVectActions( items[i]+1 );

								int flgSelected = 0;
								for( unsigned int j=0; j<items.size(); j++ ){
									if(  items[i]+1 ==  items[j] ){
										 flgSelected = 1;	// 移動先は選択セル
										 break;
									}
								}
								if( flgSelected ){
									continue;
								}

								if(actTemp.nPrecedence == actTempUp.nPrecedence){
									// 同じ->enable
									flgAllCheck = 0;	// SPEC 85677
									break;
								}else{
									// 違う
									continue;	
								}
							}
						}
					}
					if( flgAllCheck == 1 ){
						flgEnable = false;
					}
				}
			}

			if( flgEnable ){
				evt.Enable( true );
			}else{
				evt.Enable( false );
			}
		}
		break;
	default:
		break;
	}

	return;
}


kome::batchservice::BatchProcessingMethodConfigurationDialog* BatchActionSelectPanel::getParentDlg( void ){
	return m_pParentDlg;
}

// @date 2013/04/24 <Add> OKADA ------->
// prepare for the DB search
bool BatchActionSelectPanel::prepare() {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// dialog progress
	kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Identification" );

	// search engines
	const unsigned int num = plgMgr.getNumberOfFunctionItems( IDENTIFICATION_FUNC_TYPE );
	if( num == 0 ) {
		kome::window::WindowTool::showError( "No search engine plug-ins are not installed.\nExist Mass++ and install at least one search engine plug-in." );
		return false;
	}
	progress.createSubProgresses( num );

	bool flg = true;
	for( unsigned int i = 0; i < num && !progress.isStopped(); i++ ) {
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( IDENTIFICATION_FUNC_TYPE, i );
		kome::core::Progress* prgs = progress.getSubProgress( i );

		if( item != NULL ) {
			progress.setStatus( FMT( "Preparation for %s ... [%d/%d]", item->getLongName(), ( i + 1 ), num ).c_str() );
			kome::ident::SearchEngine* engine = (kome::ident::SearchEngine*)( item->getCall()->invoke( NULL ).prim.pt );
			flg = engine->prepareSearch( *prgs );
			if( !flg ) {
				kome::window::WindowTool::showError( getlasterrormessage() );
			}
		}
		prgs->fill();
	}
	
	progress.fill();
	
	return !( progress.isStopped() );
}

// @date 2013/04/24 <Add> OKADA <-------
