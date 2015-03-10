/**
 * @file BatchProcessingMethodConfigurationDialog.cpp
 * @brief implements of BatchProcessingMethodConfigurationDialog class
 *
 * @author okada_h
 * @date 2012/02/15
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "BatchProcessingMethodConfigurationDialog.h"
#include "BatchGrid.h"
#include "BatchActionSelectPanel.h"
#include "BatchButtonDef.h"
#include "BatchServicePluginManager.h"
#include "BatchJobListDlg.h"
#include "BatchProcessingSubmitJobDlg.h"
#include "NewBatchMethodDlg.h"
#include "BatchEditNameDlg.h"	// @date 2012/04/19 <Add> OKADA

#include <wx/event.h>
#include <wx/notebook.h>

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




#define BORDER_SIZE	3
#define GRID_GAP	3

#define BATCH_SERVICE_FUNC_TYPE	"GET_BATCH_SERVICE"

#define LABELING_NAME			"Peak Detector"
#define SECTION					"Auto Peak Picking"
#define PEAK_ALGORITHM_KEY		"SPEC_PEAK_ALGORITHM"


BEGIN_EVENT_TABLE( BatchProcessingMethodConfigurationDialog, kome::window::ParametersAndHelpDialog )	// @Date:2014/01/24	<extend help>	A.Ozaki
	EVT_BUTTON( ID_MATRIX_ADD_BUTTON,		BatchProcessingMethodConfigurationDialog::onAdd )
	EVT_BUTTON( ID_MATRIX_EDIT_BUTTON,		BatchProcessingMethodConfigurationDialog::onEdit )
	EVT_BUTTON( ID_MATRIX_SAVE_BUTTON,		BatchProcessingMethodConfigurationDialog::onSave )
	EVT_BUTTON( ID_MATRIX_SAVEAS_BUTTON,	BatchProcessingMethodConfigurationDialog::onSaveAs )
	EVT_BUTTON( ID_MATRIX_DELETE_BUTTON,	BatchProcessingMethodConfigurationDialog::onDelete )
	EVT_BUTTON( ID_BAT_JOB_LIST_BUTTON,		BatchProcessingMethodConfigurationDialog::onBatJobList )
	EVT_BUTTON( wxOK,						BatchProcessingMethodConfigurationDialog::onOkButton )
	
	EVT_BUTTON( wxCANCEL,					BatchProcessingMethodConfigurationDialog::onCancelButton )	// SPEC 85856

	EVT_COLLAPSIBLEPANE_CHANGED( ID_BAT_CONF_CPANE, BatchProcessingMethodConfigurationDialog::onToggleBatConfPane)
	EVT_NOTEBOOK_PAGE_CHANGED( wxID_ANY, onChangePage )
//	EVT_TEXT(ID_COMBOBOX_METHOD, BatchProcessingMethodConfigurationDialog::changeMethodCombobox )	// @date 2013/07/02 <Del> OKADA 不要
	EVT_COMBOBOX(ID_COMBOBOX_METHOD, BatchProcessingMethodConfigurationDialog::changeMethodCombobox )

	EVT_UPDATE_UI( ID_MATRIX_ADD_BUTTON,	BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_MATRIX_EDIT_BUTTON,	BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_MATRIX_SAVE_BUTTON,	BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_MATRIX_SAVEAS_BUTTON,	BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_MATRIX_DELETE_BUTTON,	BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( wxID_OK,  BatchProcessingMethodConfigurationDialog::onUpdateUI )
	EVT_UPDATE_UI( ID_COMBOBOX_METHOD,	BatchProcessingMethodConfigurationDialog::onUpdateUI )	// SPEC 85673
END_EVENT_TABLE()


// constructor
BatchProcessingMethodConfigurationDialog::BatchProcessingMethodConfigurationDialog( wxWindow* parent )
: kome::window::ParametersAndHelpDialog( parent,	"Batch Processing - Method Configuration"), m_parameters( NULL, false )	// @Date:2014/01/24	<extend help>	A.Ozaki
{
	m_pBatConfAddButton		= NULL;
	m_pBatConfEditButton	= NULL;
	m_pBatConfSaveButton	= NULL;
	m_pBatConfSaveAsButton	= NULL;
	m_pBatConfDeleteButton	= NULL;

	m_pBatchList = new BatchGrid( this );

	m_pApplyAllButton = NULL;
	m_pCloseButton = NULL;
	m_pBatchProcessingSubmitJobDlg = NULL;
	m_vect_pBatchActionSelectPanel.clear();

	m_pComboMethod = NULL;
	m_stringsMethod.clear();
	m_bChangeAction = false;	// SPEC 85856

	getInitData();
}

// destructor
BatchProcessingMethodConfigurationDialog::~BatchProcessingMethodConfigurationDialog() {
}

// create top sizer
wxSizer* BatchProcessingMethodConfigurationDialog::createMainSizer() {

	// field sizer
	wxFlexGridSizer* fieldSizer = new wxFlexGridSizer( 4, 1, GRID_GAP, GRID_GAP );
	fieldSizer->AddGrowableCol( 0 );
	fieldSizer->AddGrowableRow( 1 );

	// top sizer
	wxFlexGridSizer* topSizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	topSizer->AddGrowableCol( 0 );


	// create peak sizer
	topSizer->Add(
		createBatchMethodSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
	);

	topSizer->Add(
		createBatchButtonSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
	);
	fieldSizer->Add( topSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	if( m_pBatchList != NULL ) {
		m_pBatchList->create();
	}
	fieldSizer->Add(m_pBatchList, 1, wxEXPAND);
	
	// 隠れるペイン
	wxSizer* sizer = NULL;
	m_pBatConfPane = new kome::window::CollapsiblePane(this, wxT("Batch Method Stage Settings"), ID_BAT_CONF_CPANE);
	fieldSizer->Add(m_pBatConfPane , 0, wxGROW|wxALL, 5);

	// タブ
    wxWindow *win = m_pBatConfPane->GetPane();
    wxSizer *paneSz = new wxBoxSizer(wxVERTICAL);

	// note book
	m_pNoteBook = new wxNotebook(
		win,
		wxID_ANY,
		wxDefaultPosition,
		wxSize(638, 312),
		wxNB_TOP
	);
	
	paneSz->Add( m_pNoteBook, 1, wxALIGN_CENTER | wxALIGN_CENTRE_VERTICAL | wxALL | wxGROW | wxSHAPED, 5 ); // SPEC No.91863 @date 2013.04.01 <Mod> M.Izumi

	BatchActionSelectPanel * winBatchActionSelectPanel0 = new BatchActionSelectPanel( m_pNoteBook, this, 0 );
	BatchActionSelectPanel * winBatchActionSelectPanel1 = new BatchActionSelectPanel( m_pNoteBook, this, 1 );
	BatchActionSelectPanel * winBatchActionSelectPanel2 = new BatchActionSelectPanel( m_pNoteBook, this, 2 );
	BatchActionSelectPanel * winBatchActionSelectPanel3 = new BatchActionSelectPanel( m_pNoteBook, this, 3 );
	BatchActionSelectPanel * winBatchActionSelectPanel4 = new BatchActionSelectPanel( m_pNoteBook, this, 4 );
	BatchActionSelectPanel * winBatchActionSelectPanel5 = new BatchActionSelectPanel( m_pNoteBook, this, 5 );

	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel0 );	// 保存しておく
	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel1 );
	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel2 );
	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel3 );
	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel4 );
	m_vect_pBatchActionSelectPanel.push_back( winBatchActionSelectPanel5 );

	m_pNoteBook->AddPage( winBatchActionSelectPanel0, wxT( g_aszStageString[0] ));	
	m_panels.push_back( winBatchActionSelectPanel0 );
	m_pNoteBook->AddPage( winBatchActionSelectPanel1, wxT( g_aszStageString[1] ));	
	m_panels.push_back( winBatchActionSelectPanel1 );
	m_pNoteBook->AddPage( winBatchActionSelectPanel2, wxT( g_aszStageString[2] ));	
	m_panels.push_back( winBatchActionSelectPanel2 );
	m_pNoteBook->AddPage( winBatchActionSelectPanel3, wxT( g_aszStageString[3] ));	
	m_panels.push_back( winBatchActionSelectPanel3 );
	m_pNoteBook->AddPage( winBatchActionSelectPanel4, wxT( g_aszStageString[4] ));	
	m_panels.push_back( winBatchActionSelectPanel4 );
	m_pNoteBook->AddPage( winBatchActionSelectPanel5, wxT( g_aszStageString[5] ));	
	m_panels.push_back( winBatchActionSelectPanel5 );

	// Init Dialog
	for( unsigned int i = 0; i < m_panels.size(); i++ ) {
// >>>>>>	@Date:2014/02/04	<Modify>	A.Ozaki
// WinXPでは、IniDialog(OnInitDialog)の処理中に例外が発生して、Mass++自体が落ちてしまう
// その回避方法として、OnInitDialogを動かさずに、Widgetを生成させるように変更しました
//
//		m_panels[ i ]->InitDialog();
		m_panels[ i ]->createWidgets();
//
// <<<<<<	@Date:2014/02/04	<Modify>	A.Ozaki
	}

    win->SetSizer(paneSz);
    paneSz->SetSizeHints(win);

	// XMLファイルから読み込んだデータを各タブに書き込む
	{
		BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();

		std::vector<Action>	vectActions = mgr.getVectXmlActions();
		int nCnt = vectActions.size();
		for( int i=0; i<nCnt; i++ ){
			Action actTemp = vectActions[i];	
			m_pBatchList->addAction( actTemp );

			int nnn=m_vect_pBatchActionSelectPanel.size();

			BatchActionGrid* bag;
			switch( actTemp.nStage ){
			case SN_INITIALIZE:		// ①Initialize
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[0]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_FILTER:			// ②
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[1]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_MANIPULATE:		// ③
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[2]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_ANALYZE:		// ④
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[3]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_IDENTIFY:		// ⑤
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[4]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_FILTER_RESULTS:	// ⑥Filter Results
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[5]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			default:
				break;
			}
		}

		// 画面に反映
		m_pBatchList->updateList();
		m_pBatchList->RefreshBatchGrid();

	}

	return fieldSizer;
}


// create button sizer
wxSizer* BatchProcessingMethodConfigurationDialog::createButtonSizer() {
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 4, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableCol( 1 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );
	sizer->AddGrowableRow( 2 );
	sizer->AddGrowableRow( 3 );

	// default button
	wxButton* button = new wxButton( this, ID_BAT_JOB_LIST_BUTTON, wxT( "Batch Job List..." ) );
	sizer->Add( button, 0, wxALIGN_BOTTOM | wxALIGN_LEFT | wxLEFT , BORDER_SIZE );

	// OK, Cancel
	wxSizer* buttonSizer = kome::window::StandardDialog::createButtonSizer();
	sizer->Add( buttonSizer, 2, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );

	return sizer;
}


// create top sizer
wxSizer* BatchProcessingMethodConfigurationDialog::createBatchMethodSizer() {
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );

	sizer->AddGrowableCol( 1 );
	sizer->AddGrowableRow( 0 );

	// "Batch Method"
	wxStaticText* descrName = new wxStaticText( this, wxID_STATIC, wxT( "Batch Method" ), wxDefaultPosition, wxDefaultSize, 0 );
	sizer->Add( descrName, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	// XMLファイルの一覧を取得
	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	std::vector<std::string> vect_strFiles = mgr.getSettingXmlFiles();

	wxArrayString stringsMethod;
	for( unsigned int i=0; i<vect_strFiles.size(); i++ ){
		// 拡張子部分末尾4文字を削除
		std::string strTemp = vect_strFiles[i];
		int nLen = vect_strFiles[i].length();
		if( 4 <= nLen ){
			strTemp = vect_strFiles[i].substr( 0, nLen-4 );
		}

		stringsMethod.Add(wxT( strTemp.c_str() ));
		m_strMethodName = strTemp;
	}

	// 最後に利用したxmlファイルをiniファイルから読み出し
	std::string strXmlFileNameLast = "";
	if( vect_strFiles.size() == 0 ){
		strXmlFileNameLast = "";
	}else{
		strXmlFileNameLast = vect_strFiles[0];	// 初期値
	
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getParameters();

		std::string strXmlFileNameWithoutExt = ini->getString( SECTION_XML, DEFAULT_XML_KEY, DEFAULT_VALUEY );
		std::string strXmlFileName = strXmlFileNameWithoutExt;

		strXmlFileName += ".xml";	// 拡張子を追加
		if( strcmp( DEFAULT_VALUEY, strXmlFileName.c_str() ) == 0 ){
			// 一致、読み取り失敗
		}else{
			// 読み取り成功。→実在するファイルかチェック
			for( unsigned int i=0; i<vect_strFiles.size(); i++ ){
				if( compareignorecase( vect_strFiles[i].c_str(), strXmlFileName.c_str() ) == 0 ){
					// 一致
					strXmlFileNameLast = strXmlFileNameWithoutExt;
					break;
				}
				if( (i+1) == vect_strFiles.size() ){
					// 最後なので不一致
					strXmlFileNameLast = "";
				}
			}
		}
	}

	wxSize wxsSize = wxDefaultSize;	// SPEC 85682
	wxsSize.SetWidth( 320 );		// SPEC 85682
	m_pComboMethod = new wxComboBox(this, ID_COMBOBOX_METHOD, strXmlFileNameLast, wxDefaultPosition, wxsSize, stringsMethod, wxCB_SIMPLE | wxCB_READONLY );	 //　コンボボックスに追加
	// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
	setDescription( m_pComboMethod, "Saved batch methods appears in this drop down and you can select it." );
	// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki
	m_stringsMethod = stringsMethod;

	// 現在のメソッド選択値を更新
	m_strMethodName = m_pComboMethod->GetValue().c_str();	// SPEC 85953	選択済みメソッド名の値を更新	//　@date 2012/05/16 <Add> OKADA

	sizer->Add( m_pComboMethod, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	// Ini ファイルチェック
	if( strXmlFileNameLast == "" ){
		;	// Iniファイル設定無し
	}else{

		// XML
		std::string strXmlFileNameWithExt;
		strXmlFileNameWithExt = strXmlFileNameLast;
		strXmlFileNameWithExt += ".xml";

		if( isabsolutepath( strXmlFileNameWithExt.c_str() ) ){
			;
		}else{
			strXmlFileNameWithExt = getabsolutepath( mgr.getBatchJobPath().c_str(), strXmlFileNameWithExt.c_str() );
		}

		mgr.readXML( strXmlFileNameWithExt.c_str() );	// XMLファイルから読み込み

	}
	// 画面に反映
	m_pBatchList->updateList();
	m_pBatchList->RefreshBatchGrid();

	return sizer;
}


// create top sizer
wxSizer* BatchProcessingMethodConfigurationDialog::createBatchButtonSizer() {
	// create sizer
	wxSizer* sizer = NULL;
	sizer = new wxBoxSizer(wxHORIZONTAL);

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );

	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	m_pBatConfAddButton = new wxBitmapButton( this, ID_MATRIX_ADD_BUTTON, *iconMgr.getIcon("new") );
	m_pBatConfAddButton->SetToolTip("Add new batch method");
	buttonSizer->Add(m_pBatConfAddButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfEditButton = new wxBitmapButton( this, ID_MATRIX_EDIT_BUTTON, *iconMgr.getIcon("edit") );
	m_pBatConfEditButton->SetToolTip("Edit batch method name");
	buttonSizer->Add(m_pBatConfEditButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfSaveButton = new wxBitmapButton( this, ID_MATRIX_SAVE_BUTTON, *iconMgr.getIcon("item_save") );
	m_pBatConfSaveButton->SetToolTip("Save batch method");
	buttonSizer->Add(m_pBatConfSaveButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfSaveAsButton = new wxBitmapButton( this, ID_MATRIX_SAVEAS_BUTTON, *iconMgr.getIcon("item_save_new") );
	m_pBatConfSaveAsButton->SetToolTip("Save as new batch method");
	buttonSizer->Add(m_pBatConfSaveAsButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	m_pBatConfDeleteButton = new wxBitmapButton( this, ID_MATRIX_DELETE_BUTTON, *iconMgr.getIcon("item_del") );
	m_pBatConfDeleteButton->SetToolTip("Delete batch method");
	buttonSizer->Add(m_pBatConfDeleteButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	return sizer;
}


wxSizer* BatchProcessingMethodConfigurationDialog::createBatchPaneSizer() {

	wxSizer* sizer = NULL;
	
	m_pBatConfPane = new kome::window::CollapsiblePane(this, wxT("Peak Matrix Settings"), ID_BAT_CONF_CPANE);

	sizer->Add(m_pBatConfPane , 0, wxGROW|wxALL, 5);

	return sizer;
}


// on toggle the batch configuration collapsible pane
void BatchProcessingMethodConfigurationDialog::onToggleBatConfPane( wxCollapsiblePaneEvent& evt )
{
}

// on add
void BatchProcessingMethodConfigurationDialog::onAdd( wxCommandEvent& evt ) {
	// 現在のメソッド選択値を更新
	m_strMethodName = m_pComboMethod->GetValue().c_str();	// @date 2012/04/19 <Add> OKADA

	// open dialog
	NewBatchMethodDlg dlg(
		kome::window::WindowTool::getMainWindow(), this
	);

	int nRet = dlg.ShowModal();
	if( nRet == wxID_OK )
	{
		// 処理
		int iii = 0;
		NewMethodSetting stNewMethodSetting = dlg.getNewMethodSetting();

		// ラジオボタンが「Method」なら、
		// ・コンボボックスで選択されたMethodを読み込み
		// ・エディットボックスに入力された新しいメソッド名で上書き
		// ラジオボタンが「Template」なら、
		// ラジオボタンが「None」なら、
		// ・全削除
		// ・エディットボックスに入力された新しいメソッド名で上書き

		if( stNewMethodSetting.bMethod ){
			// ラジオボタンが「Method」なら、
			// ・コンボボックスで選択されたMethodを読み込み
			std::string strMethod = stNewMethodSetting.wxstrMethod.c_str();

			// ここで同じ名前がないかチェック
			if( checkSameNameConbobox(stNewMethodSetting.strNewMethodName.c_str()) != 0 ){
				// 同じ名前があった
				return;
			}

			changeMethod( strMethod.c_str() );
			// ・エディットボックスに入力された新しいメソッド名で上書き
			m_pComboMethod->Append( stNewMethodSetting.strNewMethodName );
			m_pComboMethod->SetValue( stNewMethodSetting.strNewMethodName );

			m_stringsMethod.push_back( stNewMethodSetting.strNewMethodName );	// @date 2012/04/19 <Add> OKADA		// SPEC 85666

			copyDefaultOutputFormat( strMethod.c_str(),  stNewMethodSetting.strNewMethodName.c_str() );	//@date 2013/05/23 <Add> OKADA
		}
		else if( stNewMethodSetting.bTemplate ){
			// ラジオボタンが「Template」なら、


			// @date 2013/08/19 <Add> OKADA ------->

			// managers
			kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

			std::string strTemplate = stNewMethodSetting.wxstrTemplate;

			const char* TEMPLATE_TYPE = "batch_set";
			std::string strTemplateDefault;

			unsigned int uNum = plgMgr.getNumberOfFunctionItems( TEMPLATE_TYPE );
			kome::plugin::PluginFunctionItem* item = NULL;
			for( unsigned int i=0; i<uNum; i++ ){
				 item = plgMgr.getFunctionItem( TEMPLATE_TYPE, i );
				if( item != NULL ){
					const char* szLongName = item->getLongName();
					if( strTemplate.compare( szLongName ) ){
						// 不一致
					}else{
						// 一致

						kome::objects::ActiveObjectsManager& actMgr = kome::objects::ActiveObjectsManager::getInstance();
						kome::objects::Sample* acSample = actMgr.getActiveSample();

						kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );

						if( page != NULL ){
							
						}

						kome::objects::SettingParameterValues settings;

						// create parameters
						kome::objects::Parameters params;
						kome::plugin::PluginCallTool::setSample( params, *acSample );

						kome::objects::Variant ret;

						ret = item->getCall()->invoke( &params );

						kome::plugin::BatchInfo* batchInfo = (kome::plugin::BatchInfo*)ret.prim.pt;

						if( batchInfo == NULL ){
							continue;
						}

						// 各タブをクリア
						{
							int nCnt = m_vect_pBatchActionSelectPanel.size();
							for( int i=0; i<nCnt; i++ ){
								BatchActionGrid* bag;
								bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[i]))->getSelectedActions();
								bag->init();	// @date 2013/07/02 <Add> OKADA	// SPEC 93492
							}
						}

						if( batchInfo != NULL ){
							int nNum = batchInfo->getNumberOfBatchFunctions();
							for( int j=0; j<nNum; j++){

								std::string strFunction;
								batchInfo->getBatchProcessingFunction( j, strFunction );
								// strFunction = "chrom_peak_detect" といった文字列が返る

								kome::plugin::PluginFunctionItem* item = batchInfo->getBatchFunction( j );
								if( item != NULL ){
									
									// step 取得
									const char* stepStr = item->getCall()->getProperties().getStringValue( "step", "" );
									// stepStr = 0x02ba912c "identify" といった文字列が取得される

									unsigned short uPrecedence = item->getCall()->getOrder();

									Action actTemp;

									// ステージ番号。①Initialize～⑥FilterResults
									actTemp.nStage = SN_SENTINEL;
									if( compareignorecase( stepStr, "initialize" ) == 0 ) {
										actTemp.nStage = SN_INITIALIZE;
									}
									else if( compareignorecase( stepStr, "input" ) == 0 ) {
										// Inputは、batchでは扱わない
									//	actTemp.nStage = STEP_INPUT;
									}
									else if( compareignorecase( stepStr, "data_set" ) == 0 ) {
										// data_setはFilterになりました
										actTemp.nStage = SN_FILTER;
									}
									else if( compareignorecase( stepStr, "operation" ) == 0 ) {
										// operationは、manipulateになりました
										actTemp.nStage = SN_MANIPULATE;
									}
									else if( compareignorecase( stepStr, "analyze" ) == 0 ) {
										actTemp.nStage = SN_ANALYZE;
									}
									else if( compareignorecase( stepStr, "identify" ) == 0 ) {
										actTemp.nStage = SN_IDENTIFY;
									}
									else if( compareignorecase( stepStr, "filter" ) == 0 ) {
										actTemp.nStage = SN_FILTER;
									}
									else if( compareignorecase( stepStr, "output" ) == 0 ) {
										// Outputは、batchでは扱わない
									//	actTemp.nStage = STEP_OUTPUT;
									}
									else if( compareignorecase( stepStr, "finalize" ) == 0 ) {
										// finalizeは、batchでは扱わない
									//	actTemp.nStage = STEP_FINALIZE;
									}
									// @date 2013/08/02 <Add> OKADA ------->
									else if( compareignorecase( stepStr, "manipulate" ) == 0 ) {
										actTemp.nStage = SN_MANIPULATE;
									}
									else if( compareignorecase( stepStr, "results" ) == 0 ) {
										actTemp.nStage = SN_FILTER_RESULTS;
									}
									if( actTemp.nStage == SN_SENTINEL ){
										// error
										int nnnn = 0;
										continue;	// @date 2013/08/21 <Add> OKADA
									}

									// nPrecedence
									actTemp.nPrecedence = uPrecedence;

									// long name
									actTemp.strName = item->getLongName();

									// short name
									actTemp.strNameShort = strFunction;

									// Plugin ManagerからgetFunctionItem()で読み取る際の番号
									// plgMgr.getFunctionItem( "batch", i );←の「i」に相当
									actTemp.nBatchIndex = -1;
									kome::plugin::PluginFunctionItem* itemTemp = NULL;
									unsigned int uNumTemp = plgMgr.getNumberOfFunctionItems( "batch" );
									for( unsigned int l=0; l<uNumTemp; l++ ){
										itemTemp = plgMgr.getFunctionItem( "batch" , l );
										const char* szLongName = itemTemp->getLongName();
										if( compareignorecase( szLongName, actTemp.strName.c_str() ) == 0 ){
											// 一致
											actTemp.nBatchIndex = l;
											break;
										}
									}
									if( actTemp.nBatchIndex < 0 ){
										// error
										int nnnn = 0;
										continue;	// @date 2013/08/21 <Add> OKADA
									}

									// 1=settings ok
									actTemp.flgSettings = 1;

									// // kome::plugin::SettingsPage * page->getParameterString( settings );	の返り値を保存
									std::string strPatameter;
									batchInfo->getParameterOfBatchProcessingFunction( j, strPatameter );

									actTemp.strSettings = strPatameter;
									
									// パラメータ設定(xmlファイル用) // @date 2013/03/28 <Add> OKADA
									std::vector<std::string> vecParam;
									param2vect( strPatameter.c_str(), vecParam );
									actTemp.vec_strSettings = vecParam;


									// これで、actTempに値がそろった

									// タブへ追加する
									BatchActionGrid* bag;
									int nTabNo=0;
									switch( actTemp.nStage ){
									case SN_INITIALIZE:		nTabNo=0;	break;	// ①Initialize
									case SN_FILTER:			nTabNo=1;	break;	// ②
									case SN_MANIPULATE:		nTabNo=2;	break;	// ③
									case SN_ANALYZE:		nTabNo=3;	break;	// ④
									case SN_IDENTIFY:		nTabNo=4;	break;	// ⑤
									case SN_FILTER_RESULTS:	nTabNo=5;	break;	// ⑥Filter Results
									default:
										// error
										// 通常ない
										continue;	// @date 2013/08/21 <Add> OKADA
										break;
									}

									bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[nTabNo]))->getSelectedActions();
									bag->addAction( actTemp );
									bag->updateList();

								}
							}
						}
						// @date 2013/08/21 <Add> OKADA batchInfo削除 ------->
						if( batchInfo != NULL ){
							delete batchInfo;
							batchInfo = NULL;
						}
						// @date 2013/08/21 <Add> OKADA <-------

					}
				}
			}
			m_strMethodName = stNewMethodSetting.strNewMethodName;

			// 画面に反映
			m_pBatchList->updateList();
			m_pBatchList->RefreshBatchGrid();

			// 画面に反映
			updateBatchList();
			m_pNoteBook->Refresh();

			// ・エディットボックスに入力された新しいメソッド名で上書き
			m_pComboMethod->Append( stNewMethodSetting.strNewMethodName );
			m_pComboMethod->SetValue( stNewMethodSetting.strNewMethodName );

			m_stringsMethod.push_back( stNewMethodSetting.strNewMethodName );	// @date 2012/04/19 <Add> OKADA		// SPEC 85666

			// @date 2013/08/19 <Add> OKADA <-------
		}
		else if( stNewMethodSetting.bBlank ){

			// ここで同じ名前がないかチェック
			if( checkSameNameConbobox(stNewMethodSetting.strNewMethodName.c_str()) != 0 ){
				// 同じ名前があった
				return;
			}

			// ラジオボタンが「None」なら、
			// ・全削除
			changeMethod( "" );
			// ・エディットボックスに入力された新しいメソッド名で上書き
			m_pComboMethod->Append( stNewMethodSetting.strNewMethodName );
			m_pComboMethod->SetValue( stNewMethodSetting.strNewMethodName );

			m_stringsMethod.push_back( stNewMethodSetting.strNewMethodName );	// @date 2012/04/19 <Add> OKADA		// SPEC 85666
		}
		m_strMethodName = stNewMethodSetting.strNewMethodName;

		dlg.Destroy();
	}

	return;
}


// on Edit
void BatchProcessingMethodConfigurationDialog::onEdit( wxCommandEvent& evt ) {

	// 現在のメソッド選択値を更新
	m_strMethodName = m_pComboMethod->GetValue().c_str();	// @date 2012/04/19 <Add> OKADA
		
	BatchEditNameDlg dlgBatch( kome::window::WindowTool::getMainWindow(), this );
	if( dlgBatch.ShowModal() == wxID_OK ){
		std::string strName;
		strName = dlgBatch.getNewMethodName();

		if( strName.length() <= 0 ){
			return;	// cancel
		}

		// ここで同じ名前がないかチェック
		if( checkSameNameConbobox(strName.c_str()) != 0 ){
			// 同じ名前があった
			return;
		}

		if( strName == m_strMethodName ){
			return;	// 変更せずに[OK]押下
		}
	
		// SPEC 85934
		{
			std::string strXmlFileNameWithExt;
			strXmlFileNameWithExt = m_strMethodName.c_str();
			strXmlFileNameWithExt += ".xml";

			// New
			std::string strXmlFileNameWithExtNew;
			strXmlFileNameWithExtNew = strName.c_str();
			strXmlFileNameWithExtNew += ".xml";

			if( isabsolutepath( strXmlFileNameWithExt.c_str() ) ){
				;
			}else{
				// m_strMethodNameのXMLファイルのフルパスを得る
				BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
				strXmlFileNameWithExt = getabsolutepath( mgr.getBatchJobPath().c_str(), strXmlFileNameWithExt.c_str() );
				strXmlFileNameWithExtNew = getabsolutepath( mgr.getBatchJobPath().c_str(), strXmlFileNameWithExtNew.c_str() );
			}
			if( copyfile( strXmlFileNameWithExt.c_str(), strXmlFileNameWithExtNew.c_str() ) ){
				remove( strXmlFileNameWithExt.c_str() );	// 削除
			}

			copyDefaultOutputFormat( m_strMethodName.c_str(), strName.c_str(), true );	// @date 2013/05/23　<Add>　OKADA
		}

		// SPEC 85951
		{
			m_stringsMethod.Remove( m_strMethodName.c_str() );	// m_strMethodNameを削除を削除
			m_stringsMethod.push_back( strName );		// 
		}


		m_strMethodName = strName;

		if( m_pComboMethod != NULL ){
			int n = m_pComboMethod->GetCurrentSelection();
			m_pComboMethod->Delete( n );
			m_pComboMethod->Insert( strName, n );
			m_pComboMethod->SetValue( strName );
			int jjj = 0;
		}
	}

	return;
}

// on save
void BatchProcessingMethodConfigurationDialog::onSave( wxCommandEvent& evt ){
	
	exportBatchXML( m_strMethodName.c_str() );

	m_bChangeAction = false;	// 保存するのでリセット	// SPEC 85856

	return;
}

// on save as
void BatchProcessingMethodConfigurationDialog::onSaveAs( wxCommandEvent& evt ){

	wxString strName = ::wxGetTextFromUser( wxT("Name"), wxT("Save As New Batch Method"),  getCurrentMethod().c_str(), this );  // SPEC 85718	@date 2012/05/15 <Mod> OKADA デフォルト名を現在のメソッド名に

	if( strName != "" ){
		// OK
		// @date 2012/05/15 <Add> OKADA ------->
		// 同じメソッド名が無いか確認
		if( checkSameNameConbobox( strName.c_str() ) != 0 ){	
			// 同じメソッド名があった
			kome::window::WindowTool::showInfo( "A method with that name already exists." );
			return;
		}
		// @date 2012/05/15 <Add> OKADA <-------

		//　SPEC 85953
		m_stringsMethod.push_back( strName );		// 新しい名前を追加

		// [OK]なら、メソッド名(m_strMethodName)更新
		m_strMethodName = strName;

		// [OK]なら、exportBatchXML()で保存
		exportBatchXML( m_strMethodName.c_str() );

		// [OK]なら、コンボボックスへ保存
		m_pComboMethod->Append( strName );
		m_pComboMethod->SetValue( strName );

		m_bChangeAction = false;	// 保存するのでリセット	// SPEC 85856	//　@date 2012/05/15 <Add> OKADA
	}

	return;
}

// on delete
void BatchProcessingMethodConfigurationDialog::onDelete( wxCommandEvent& evt ){

	bool bRet = kome::window::WindowTool::showYesNo( wxT("Are you sure you want to delete the selected batch method?"), "Delete", "Don't delete" );

	if( bRet ){

		wxString wxstrTemp = m_pComboMethod->GetValue();

		int nFindedNo = m_pComboMethod->FindString( wxstrTemp );
		if( wxNOT_FOUND != nFindedNo ){

			// wxstrTempのXMLファイルを削除する
			{
				std::string strXmlFileNameWithExt;
				strXmlFileNameWithExt = wxstrTemp.c_str();
				strXmlFileNameWithExt += ".xml";

				if( isabsolutepath( strXmlFileNameWithExt.c_str() ) ){
					;
				}else{
					// wxstrTempのXMLファイルのフルパスを得る
					BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
					strXmlFileNameWithExt = getabsolutepath( mgr.getBatchJobPath().c_str(), strXmlFileNameWithExt.c_str() );
				}

				remove( strXmlFileNameWithExt.c_str() );	// 削除

				deleteDefaultOutputFormat( wxstrTemp.c_str() );	// @date 2013/05/23 <Add> OKADA
			}

			m_pComboMethod->Delete( nFindedNo );


			m_stringsMethod.RemoveAt( nFindedNo );	// @date 2012/04/19 <Add> OKADA	// SPEC 85666
			

			// 元のデータを全部消す
			int nActSizeTotal = m_pBatchList->getActionSize();
			for( int i=nActSizeTotal-1; 0<=i; i-- ){
				m_pBatchList->deleteAction( i );
			}
			m_pBatchList->clearData( true );

			// 各タブをクリア
			{
				int nCnt = m_vect_pBatchActionSelectPanel.size();
				for( int i=0; i<nCnt; i++ ){
					BatchActionGrid* bag;
					bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[i]))->getSelectedActions();
				}
			}

			// 画面に反映
			updateBatchList();
			m_pNoteBook->Refresh();
			m_strMethodName = "";

			// SPEC 85674
			m_pComboMethod->Select(0);	// 削除時、先頭を選択
			changeMethod( m_pComboMethod->GetValue().c_str() );
		}
	}

	return;
}


// onBatJobList
void BatchProcessingMethodConfigurationDialog::onBatJobList( wxCommandEvent& evt ) {

	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	mgr.dispBatchJobWindow();

	return;
}


// onOkButton
void BatchProcessingMethodConfigurationDialog::onOkButton( wxCommandEvent& evt ) {

	// ここでXMLファイルへ設定データを保存

//	if( m_strMethodName.size() != 0 ){
//		exportBatchXML( m_strMethodName.c_str() );
//	}
	if( m_strMethodName.size() != 0 ){
		int check = exportBatchXML( m_strMethodName.c_str() );  // 出力に成功すると、0が返ってくる
		if (check != 0){
			return;    // 出力に失敗していれば、ここで終了
		}
	}
 
	// Batch Job List 起動

	//　BatchProcessingSubmitJobDlg::onSubmitButton()関数と内容同じ

	if( m_pBatchProcessingSubmitJobDlg != NULL ){
		delete m_pBatchProcessingSubmitJobDlg;
		m_pBatchProcessingSubmitJobDlg = NULL;
	}

	m_pBatchProcessingSubmitJobDlg = new BatchProcessingSubmitJobDlg( kome::window::WindowTool::getMainWindow() );

	int nRet = m_pBatchProcessingSubmitJobDlg->ShowModal();

	if( m_pBatchProcessingSubmitJobDlg != NULL ){
		delete m_pBatchProcessingSubmitJobDlg;
		m_pBatchProcessingSubmitJobDlg = NULL;
	}
	if( nRet == wxID_CANCEL ) {
		// 処理
		int iii = 0;
	}
	else{
		EndDialog( 0 );
	}
	
	return;
}

// on cancel button
void BatchProcessingMethodConfigurationDialog::onCancelButton( wxCommandEvent& evt ) {

	bool bRet = true;	// [OK]=true [Cancel]=false
	if( m_bChangeAction == true ){
		bRet = kome::window::WindowTool::showOkCancelWarning( "There are unsaved changes. If the dialog is canceled, all changes will be lost." );
	}

	if( bRet == true ){
		EndDialog( 0 );
		m_bChangeAction = false;
	}

	return;
}

// get init data
void BatchProcessingMethodConfigurationDialog::getInitData( void ){

	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	mgr.initBatchServicePluginManager();

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	unsigned int uiNum = plgMgr.getNumberOfFunctionItems( "batch" );
	for( unsigned int i=0; i<uiNum; i++ ){
		kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "batch", i );
		unsigned short wOrder = pItem->getCall()->getOrder();

		std::string step = pItem->getCall()->getProperties().getStringValue( "step", "HOGE" );

		std::string strLongName = pItem->getLongName();
		std::string strShortName = pItem->getShortName();


		stage_no nStage = mgr.getStageNoFromStepString( step.c_str() );

		if( nStage == SN_SENTINEL ){
			continue;
		}

		Action actTemp;
		actTemp.nStage = nStage;
		actTemp.nPrecedence = wOrder;
		actTemp.strName = strLongName;
		actTemp.strNameShort = strShortName;
		actTemp.nBatchIndex = i;
		actTemp.flgSettings = 0;

		mgr.setAvailableActions( actTemp );
	}

	return;
}

// on change page
void BatchProcessingMethodConfigurationDialog::onChangePage( wxNotebookEvent& evt ){

	int nSelection = evt.GetSelection();

	return;

};

// update batch list
int BatchProcessingMethodConfigurationDialog::updateBatchList( void ){

	m_bChangeAction = true;	// 書き換えられたのでセット	// SPEC 85856

	BatchActionSelectPanel* pTemp;

	// 元のデータを全部消す
	int nActSizeTotal = m_pBatchList->getActionSize();
	for( int i=nActSizeTotal-1; 0<=i; i-- ){
		m_pBatchList->deleteAction( i );
	}
	m_pBatchList->clearData( true );
	
	int nSize = m_vect_pBatchActionSelectPanel.size();	// タブの枚数
	for( int i=0; i<nSize; i++ ){
		pTemp = (BatchActionSelectPanel*)m_vect_pBatchActionSelectPanel[i];
		int nActSize = pTemp->getSelectedActions()->getActionSize();
		for( int j=0; j<nActSize; j++ ){
			Action actTemp = pTemp->getSelectedActions()->getAction( j );	// BatchActionGrid内のm_vectActions[index]を返す
			m_pBatchList->addAction( actTemp );	// BatchGrid内のm_vectActionsに値を保存
		}
	}

	m_pBatchList->updateList();
	m_pBatchList->RefreshBatchGrid();

	return 0;
}

// export batch XML
int BatchProcessingMethodConfigurationDialog::exportBatchXML( const char* szMethodName ){

	// ここでXMLファイルへ設定データを保存
	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	updateBatchList();                              
	int nSize = m_pBatchList->getActionSize();  

	// XMLに出力すべきアクションがある
	std::vector<Action> vectActions;
	for( int i=0; i<nSize; i++ ){
		Action actTemp;
		actTemp = m_pBatchList->getAction( i );
		if (actTemp.strNameShort == "norm_standard"){       // "norm_standard"に空欄が入ってしまう不具合対策(再現性がないので対処療法的に行う)
			std::string settings = actTemp.strSettings.c_str();
			if (settings == "[file,]"){   // 空欄が入っている場合
				kome::window::WindowTool::showInfo( "normalization standard sample setting is empty. " );
				return 1;
			}
		}

		vectActions.push_back( actTemp );

	}

	mgr.exportBatchXML( szMethodName, vectActions );
	mgr.saveLastRecordXmlFilename( szMethodName );	// Iniファイルへ保存	

	return 0;
}

// change method
void BatchProcessingMethodConfigurationDialog::changeMethod( const char * szMethodName ){
	
	m_strMethodName = szMethodName;

	// XMLの読み直し処理に備え、各タブをクリア
	{
		int nCnt = m_vect_pBatchActionSelectPanel.size();
		for( int i=0; i<nCnt; i++ ){
			BatchActionGrid* bag;
			bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[i]))->getSelectedActions();
			bag->init();	// @date 2013/07/02 <Add> OKADA	// SPEC 93492
		}
	}

	// XMLファイルから読み直し処理
	std::string strXmlFileNameWithExt;
	strXmlFileNameWithExt = szMethodName;
	strXmlFileNameWithExt += ".xml";

	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	if( isabsolutepath( strXmlFileNameWithExt.c_str() ) ){
		;
	}else{
		strXmlFileNameWithExt = getabsolutepath( mgr.getBatchJobPath().c_str(), strXmlFileNameWithExt.c_str() );
	}

	mgr.readXML( strXmlFileNameWithExt.c_str() );	// XMLファイルから読み込み

	// XMLファイルから読み込んだデータを各タブに書き込む
	{
		
		BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();

		std::vector<Action>	vectActions = mgr.getVectXmlActions();
		int nCnt = vectActions.size();
		for( int i=0; i<nCnt; i++ ){
			Action actTemp = vectActions[i];	
			m_pBatchList->addAction( actTemp );


			BatchActionGrid* bag;
			switch( actTemp.nStage ){
			case SN_INITIALIZE:		// ①Initialize
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[0]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_FILTER:			// ②
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[1]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_MANIPULATE:		// ③
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[2]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_ANALYZE:		// ④
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[3]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_IDENTIFY:		// ⑤
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[4]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			case SN_FILTER_RESULTS:	// ⑥Filter Results
				bag = ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[5]))->getSelectedActions();
				bag->addAction( actTemp );
				bag->updateList();
				break;
			default:
				break;
			}
		}

		// 画面に反映
		m_pBatchList->updateList();
		m_pBatchList->RefreshBatchGrid();

	}

	// 画面に反映
	updateBatchList();
	m_pNoteBook->Refresh();

	m_bChangeAction = false;	// xmlを読み直すのでリセット
 // SPEC 85856

}

// change method combobox
void BatchProcessingMethodConfigurationDialog::changeMethodCombobox( wxCommandEvent& evt )
{

	std::string strMethodName = evt.GetString();	// コンボボックスのメソッド名が入る

	changeMethod( strMethodName.c_str() );

	return;
}

// remake method combobox
void BatchProcessingMethodConfigurationDialog::remakeMethodCombobox( std::string strSelectMethod ){
	wxArrayString stringsMethod;
	stringsMethod = m_stringsMethod;
}

// get strings method
wxArrayString BatchProcessingMethodConfigurationDialog::getStringsMethod( void ){
	return m_stringsMethod;
}

// get current method
std::string BatchProcessingMethodConfigurationDialog::getCurrentMethod( void ){
	return m_strMethodName;
}

// get combo method text
std::string BatchProcessingMethodConfigurationDialog::getComboMethodText( void ){
	wxString wxstrTemp = m_pComboMethod->GetValue();

	std::string strTemp = wxstrTemp.c_str();

	return strTemp;
}

// on update UI
void BatchProcessingMethodConfigurationDialog::onUpdateUI( wxUpdateUIEvent& evt ){
	
	int nId = evt.GetId();
	switch( nId ){
	case ID_MATRIX_ADD_BUTTON:
		evt.Enable( true );
		break;
	case ID_MATRIX_EDIT_BUTTON:		// fall-through
	case ID_MATRIX_SAVE_BUTTON:		// fall-through
	case ID_MATRIX_SAVEAS_BUTTON:	// fall-through
	case ID_MATRIX_DELETE_BUTTON:	
		// Batch Methodが空欄でない場合にenable
		if( m_pComboMethod != NULL ){
			wxString wxstrBatchMethod = m_pComboMethod->GetValue();
			std::string strBatchMethod = wxstrBatchMethod.c_str();
			if( 1 <= strBatchMethod.length() ){
				// Batch Method コンボボックスが空欄でない->enable
				evt.Enable( true );
			}else{
				// Batch Method コンボボックスが空欄
				evt.Enable( false );
			}
		}
		break;
	case ID_COMBOBOX_METHOD:		// SPEC 85673
		if( m_pComboMethod != NULL ){
			wxArrayString wxasStrings = m_pComboMethod->GetStrings();
			if( wxasStrings.size() <= 0 ){
				// Batch Method コンボボックスが空
				evt.Enable( false );
			}else{
				// Batch Method コンボボックスが空でない->enable
				evt.Enable( true );
			}
		}
		break;
	case wxID_OK:
		evt.Enable( true );
		break;
	default:
		break;
	}

	return;
}


// for SPEC 85672
int BatchProcessingMethodConfigurationDialog::checkSameNameConbobox( const char * szNewMethodName )
{
	wxArrayString wxasStrings = m_pComboMethod->GetStrings();


	for( unsigned int i=0; i<wxasStrings.size(); i++){	// @date 2013/06/10 <Mod> OKADA
		if( strcmp( wxasStrings[i].c_str(), szNewMethodName ) == 0){
			// 一致
			return 1;
		}
	}
	
	return 0;
}

/** タブのonEditを呼び出します */
void BatchProcessingMethodConfigurationDialog::editBatchActionSelectPanel( int index, int line_no ){
	getBatchActionSelectPanel( index )->editByIndex( line_no );
}

// clear Default Output Format
void BatchProcessingMethodConfigurationDialog::deleteDefaultOutputFormat( const char* szBatchMethod )
{
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();

	ini->deleteValue( SECTION_METHOD_FMT, szBatchMethod );

	std::string strKey = szBatchMethod;
	strKey.append( "_PARAM" );
	ini->deleteValue( SECTION_METHOD_FMT, strKey.c_str() );

	return;
}

// copy Default Output Format
void BatchProcessingMethodConfigurationDialog::copyDefaultOutputFormat( const char* szBatchMethod, const char* szNewBatchMethod, bool flgRename )
{
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();

	std::string strKey = szBatchMethod;
	strKey.append( "_PARAM" );

	const char* szSetVal = ini->getString( SECTION_METHOD_FMT, szBatchMethod, "" );
	std::string strSetVal = szSetVal;

	const char* szSetParamVal = ini->getString( SECTION_METHOD_FMT, strKey.c_str(), "" );
	std::string strSetParamVal = szSetParamVal;

	std::string strNewKey = szNewBatchMethod;
	strNewKey.append( "_PARAM" );

	ini->setString( SECTION_METHOD_FMT, szNewBatchMethod, strSetVal.c_str() );
	ini->setString( SECTION_METHOD_FMT, strNewKey.c_str(), strSetParamVal.c_str() );

	if( flgRename ){
		// delete original setting
		deleteDefaultOutputFormat( szBatchMethod );
	}

	return;
}

// param to vect
int BatchProcessingMethodConfigurationDialog::param2vect( const char* szParam, std::vector<std::string>& vecParam )
{
	int nLen = strlen( szParam );

	if( nLen <= 1 ){
		// error
	}

	int nCutStart = 0;
	int nCutEnd = nLen-1;

	if( szParam[0] =='[' ){
		nCutStart=1;
	}
	if( szParam[nCutEnd] ==']' ){
		nCutEnd--;
	}

	if( nCutStart >=  nCutEnd ){
		// error
	}

	char szDestParam[4096];
	int nLenDestParam = nCutEnd-nCutStart+1;
	if( sizeof(szDestParam) < nLenDestParam ){
		// error
	}
	strncpy( szDestParam, &szParam[nCutStart], nLenDestParam );
	
	char szTemp[10] = "";
	std::string strTemp = "";
	std::string strTemp2;
	std::vector<std::string> vecParamLocal;
	int nKakkoDepth = 0;

	for( int i=0; i<nLenDestParam; i++ ){
		if( szDestParam[i] == ',' ){
			if( nKakkoDepth == 0 ){
				vecParamLocal.push_back( strTemp );
				strTemp.clear();
			}else{
				szTemp[0] = szDestParam[i];
				szTemp[1] = '\0';
				strTemp2 = strTemp + szTemp;
				strTemp = strTemp2;
			}
		}else if( szDestParam[i] == '[' ){
			nKakkoDepth++;
			szTemp[0] = szDestParam[i];
			szTemp[1] = '\0';
			strTemp2 = strTemp + szTemp;
			strTemp = strTemp2;
		}else if( szDestParam[i] == ']' ) {
			nKakkoDepth--;
			szTemp[0] = szDestParam[i];
			szTemp[1] = '\0';
			strTemp2 = strTemp + szTemp;
			strTemp = strTemp2;
		}else{
			szTemp[0] = szDestParam[i];
			szTemp[1] = '\0';
			strTemp2 = strTemp + szTemp;
			strTemp = strTemp2;
		}
	}
	vecParamLocal.push_back( strTemp );
	strTemp.clear();

	vecParam = vecParamLocal;

	return 0;
}
