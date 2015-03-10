/**
 * @file BatchProcessingSubmitJobDlg.cpp
 * @brief implementsation of BatchProcessingSubmitJobDlg class
 *
 * @author OKADA, H
 * @date 2012-02-29
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "BatchProcessingSubmitJobDlg.h"
#include "BatchButtonDef.h"
#include "BatchServicePluginFunctions.h"	// SPEC 85678

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP	3
#define BORDER_SIZE		3

// OutputFolder
#define INI_OUTPUT_FOLDER_SECTION	"BatchProcessingSubmitJob"
#define INI_OUTPUT_FOLDER_KEY		"DEFAULT_DIR"
#define INI_OUTPUT_DEFAULT_DATA_TYPE	"DEFAULT_DATA_TYPE"	// @date 2013/08/27 <Add> OKADA

// Combobox（Output Folder)
#define FORMAT                      "Output Format"				// @date 2013/05/17 <Add> OKADA
#define SECTION                     "file_IO_Batch"				// @date 2013/05/17 <Add> OKADA
#define FILE_IO_KEY                 "file_IO_key_Batch"			// @date 2013/05/17 <Add> OKADA

BEGIN_EVENT_TABLE( BatchProcessingSubmitJobDlg,			kome::window::ParametersAndHelpDialog )	// @Date;2014/01/24	<extend help>	A.Ozaki
	EVT_BUTTON( ID_SUBMIT_JOB_SUBMIT_BUTTON,			BatchProcessingSubmitJobDlg::onSubmitButton )
	EVT_BUTTON( ID_SUBMIT_JOB_ADD_BUTTON,				BatchProcessingSubmitJobDlg::onAddButton )
	EVT_BUTTON( ID_SUBMIT_JOB_DELETE_BUTTON,			BatchProcessingSubmitJobDlg::onDeleteButton )
	EVT_BUTTON( ID_SUBMIT_JOB_UP_BUTTON,				BatchProcessingSubmitJobDlg::onUpButton )
	EVT_BUTTON( ID_SUBMIT_JOB_DOWN_BUTTON,				BatchProcessingSubmitJobDlg::onDownButton )
	EVT_BUTTON( ID_SUBMIT_JOB_CANCEL_BUTTON,			BatchProcessingSubmitJobDlg::onCancelButton )
	EVT_BUTTON( ID_SUBMIT_JOB_FOLDER_OPEN_BUTTON,		BatchProcessingSubmitJobDlg::onOpenFolder )	// @date 2012/08/03 <Add> OKADA
	EVT_COMBOBOX( ID_COMBOBOX_DATA_TYPE, BatchProcessingSubmitJobDlg::onChangeComboDataType)		// @date 2012/08/06 <Add> OKADA
	EVT_KOME_SETTINGS_PANEL( wxID_ANY, onSelectComboOutputFormat )	// @date 2013/05/17 <Add> OKADA
	EVT_UPDATE_UI( ID_SUBMIT_JOB_DELETE_BUTTON,			BatchProcessingSubmitJobDlg::onUpdateUI )
	EVT_UPDATE_UI( ID_SUBMIT_JOB_UP_BUTTON,				BatchProcessingSubmitJobDlg::onUpdateUI )
	EVT_UPDATE_UI( ID_SUBMIT_JOB_DOWN_BUTTON,			BatchProcessingSubmitJobDlg::onUpdateUI )
	EVT_UPDATE_UI( ID_SUBMIT_JOB_SUBMIT_BUTTON,			BatchProcessingSubmitJobDlg::onUpdateUI )
	EVT_UPDATE_UI( ID_SUBMIT_JOB_FOLDER_OPEN_BUTTON,	BatchProcessingSubmitJobDlg::onUpdateUI )	// @date 2012/08/03 <Add> OKADA
END_EVENT_TABLE()

// constructor
BatchProcessingSubmitJobDlg::BatchProcessingSubmitJobDlg( wxWindow* parent )
     : kome::window::ParametersAndHelpDialog( parent, "Batch Processing - Submit Job"), m_parameters( NULL, false )	// @Date;2014/01/24	<extend help>	A.Ozaki
{
	m_pBatchProcessingSubmitJobList = new BatchProcessingSubmitJobGrid(this);

	/** name */
	m_nameCtrl = NULL;

	/** comment */
	m_textCtrl = NULL;

	m_pCombo = NULL;

	m_pComboDataType = NULL;	// @date 2012/08/02 <Add> OKADA

	m_stringDefaultSelectMethod = "";
}

// destructor
BatchProcessingSubmitJobDlg::~BatchProcessingSubmitJobDlg() {
}

// create top sizer
wxSizer* BatchProcessingSubmitJobDlg::createMainSizer() {

	wxFlexGridSizer* fieldSizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	fieldSizer->AddGrowableCol( 0 );
	fieldSizer->AddGrowableRow( 0 );
	fieldSizer->AddGrowableRow( 1 );
	fieldSizer->AddGrowableRow( 2 );

	// Name, Server, Comment
	wxFlexGridSizer* topSizer = new wxFlexGridSizer( 3, 2, GRID_GAP, GRID_GAP );
	topSizer->AddGrowableCol( 1 );
	topSizer->AddGrowableRow( 0 );
	topSizer->AddGrowableRow( 1 );
	topSizer->AddGrowableRow( 2 );

	wxStaticText* descrName = new wxStaticText( this, wxID_STATIC, wxT( "Name" ), wxDefaultPosition, wxDefaultSize, 0 );
	topSizer->Add(descrName, 0, wxALIGN_LEFT|wxALL, 5);

	// A text control for the batch name
	m_nameCtrl = new wxTextCtrl ( this, ID_NAME_TEXT, wxT( "" ), wxDefaultPosition, wxDefaultSize, 0 );	// SPEC 85680
	topSizer->Add(m_nameCtrl, 0, wxGROW|wxALL, 5);

	// Server
	wxStaticText* descrDummy3 = new wxStaticText( this, wxID_STATIC, wxT( "Server" ), wxDefaultPosition, wxDefaultSize, 0 );
	topSizer->Add(descrDummy3, 0, wxALIGN_LEFT|wxALL, 5);

	wxArrayString strings;
	std::vector<std::string> vstrServerNameLong;
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	unsigned int uiNum = plgMgr.getNumberOfFunctionItems( "GET_BATCH_SERVICE" );
	for( unsigned int i=0; i<uiNum; i++ ){
		kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "GET_BATCH_SERVICE", i );
		kome::plugin::SettingsPage * spage = NULL;
		if( pItem != NULL ){
			spage = pItem->getSettingsPage();
			std::string strNameLong = pItem->getLongName();
			strings.Add(strNameLong.c_str());
		}
	}

	if( strings.size() <= 0 ){
		m_pCombo = new wxComboBox( this, ID_BATCH_SRV_COMBOBOX, wxT( "" ), wxDefaultPosition, wxDefaultSize, strings, wxCB_SIMPLE | wxCB_READONLY);

	}else{
		m_pCombo = new wxComboBox( this, ID_BATCH_SRV_COMBOBOX, wxT( strings[0].c_str() ), wxDefaultPosition, wxDefaultSize, strings, wxCB_SIMPLE | wxCB_READONLY);
	}

	topSizer->Add( m_pCombo, 0, wxALIGN_LEFT|wxALL, 5);
	// Comment
	wxStaticText* descrDummy5 = new wxStaticText( this, wxID_STATIC, wxT( "Comment" ), wxDefaultPosition, wxDefaultSize, 0 );
	topSizer->Add(descrDummy5, 0, wxALIGN_LEFT|wxALL, 5);

	m_textCtrl = new wxTextCtrl( this, ID_COMMENT_NAME_TEXT, wxEmptyString, wxDefaultPosition, wxSize(318, 22), wxTE_LEFT );

	topSizer->Add( m_textCtrl, 0, wxALIGN_LEFT|wxALL, 5);
	fieldSizer->Add( topSizer, 1, wxGROW | wxALL, BORDER_SIZE );
	
	// Output Format	// @date 2013/05/17 <Add> OKADA ------->	
	// コンボボックス設定
	wxSizer* comboSizer = new wxBoxSizer( wxHORIZONTAL );

	// page	                                                                                //@date 2012/8/6 <Add> FUJITA
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );      //@date 2012/8/6 <Add> FUJITA

	kome::plugin::SettingsValue* value = m_parameters.addValue();    
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );  
	value->setEnumKey( FILE_SAVE_FUNC_TYPE_NAME );                       
	value->setRequired( true );                                      

	kome::plugin::SettingsParam* param = m_parameters.addParam();  
	param->setValue( value );
	param->setName( FORMAT );  

	param->setSection( SECTION );          
	param->setKey( FILE_IO_KEY );          

	kome::plugin::SettingsForm* form = page->addForm();   
	form->setParam( param );
	form->setTitle( FORMAT );
	form->setDesc( "Select the output format and set parameters." );	// @Date:2014/01/24	<extend help>	A.Ozaki
	
	//settingpanel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel(this,page,false,&m_settings,true);
	comboSizer->Add( panel, 0, wxALL | wxGROW, BORDER_SIZE );
	fieldSizer->Add( comboSizer, 1, wxALL | wxEXPAND);
			
	const char* szSaveFormat = this->m_settings.getParameterValue( FORMAT );
	
	// @date 2013.09.30 <Mod> M.Izumi ->
	kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "save_file", szSaveFormat );
	const char * pszExt = pItem->getCall()->getProperties().getStringValue( "ext", "" );

	m_pBatchProcessingSubmitJobList->setFormatExt( pszExt );
	// @date 2013.09.30 <Mod> M.Izumi <-
	
	// @date 2013/05/17 <Add> OKADA <-------
	
	// Input / Output リスト
	wxFlexGridSizer* centerSizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	centerSizer->AddGrowableCol( 0 );
	centerSizer->AddGrowableRow( 0 );
	centerSizer->AddGrowableRow( 1 );

	wxStaticText* staticText = new wxStaticText( this
		, wxID_STATIC
		, wxT( "Input / Output Settings" )
		, wxDefaultPosition
		, wxDefaultSize
		, wxALIGN_LEFT);
	centerSizer->Add( staticText, 1, wxALL | wxEXPAND);

	if( m_pBatchProcessingSubmitJobList != NULL ) {	// list
		m_pBatchProcessingSubmitJobList->create();
	}
	centerSizer->Add(m_pBatchProcessingSubmitJobList, 1, wxEXPAND);
	fieldSizer->Add( centerSizer, 1, wxALL | wxEXPAND);

	// ボタン
	wxFlexGridSizer* bottomSizer = new wxFlexGridSizer( 1, 1, GRID_GAP, GRID_GAP );
	bottomSizer->AddGrowableCol( 0 );
	bottomSizer->AddGrowableRow( 0 );

	bottomSizer->Add(
		createSubmitBatchButtonSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
	);
	fieldSizer->Add( bottomSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	// >>>>>>	@Date:2014/01/24	<extend help>	A.Ozaki
	setDescription( m_nameCtrl, "Batch processing job name. This name is shown in the Batch Jpb Results dialog." );
	setDescription( m_pCombo, "Batch processing job server." );
	setDescription( m_textCtrl, "The comment of the batch processing job. this comment is shown in the Batch Job Result dialog." );
	// <<<<<<	@Date:2014/01/24	<extend help>	A.Ozaki

	return fieldSizer;
}

// create Button sizer
wxSizer* BatchProcessingSubmitJobDlg::createButtonSizer() 
{
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableCol( 1 );
	sizer->AddGrowableRow( 0 );

	// default button
	wxButton* button = new wxButton( this, ID_SUBMIT_JOB_SUBMIT_BUTTON, wxT( "Submit" ) );
	sizer->Add( button, 0, wxALIGN_BOTTOM | wxALIGN_RIGHT | wxRIGHT , BORDER_SIZE );

	wxButton* buttonCancel = new wxButton( this, ID_SUBMIT_JOB_CANCEL_BUTTON, wxT( "Cancel" ) );
	sizer->Add( buttonCancel, 0, wxALIGN_BOTTOM | wxALIGN_RIGHT | wxRIGHT , BORDER_SIZE );

	return sizer;
}

// create submit batch button sizer
wxSizer* BatchProcessingSubmitJobDlg::createSubmitBatchButtonSizer(){

	wxSizer* sizer = NULL;
	sizer = new wxBoxSizer(wxHORIZONTAL);

	// page
	// ボタンの枠
	wxStaticBoxSizer* pDataTypeWaku = new wxStaticBoxSizer( wxHORIZONTAL, this, wxT( "" ) );

	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	m_pSubmitJobAddButton = new wxBitmapButton( this, ID_SUBMIT_JOB_ADD_BUTTON, *iconMgr.getIcon("sample_add") );
	m_pSubmitJobAddButton->SetToolTip("Add sample");

	// コンボボックス
	//	DataType コンボボックスでは GET_FILE_READER 関数の一覧を取得し long name を表示する。

	wxArrayString stringsMethod;
	{
		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

		unsigned int num = plgMgr.getNumberOfFunctionItems( wxT("get_file_reader") );
		kome::plugin::PluginFunctionItem* pItem;
	
		for( unsigned int i=0; i<num; i++ ){
			pItem = plgMgr.getFunctionItem( wxT("get_file_reader"), (unsigned int)i );
			const char * pszLongName = pItem->getLongName();
			if( pszLongName != NULL ){
				stringsMethod.Add( pszLongName );
			}
		}
	}

	// デフォルト選択を決める。"Raw Data File", "DIT-FP"等
#if 1
	{	// SPEC 94692	// @date 2013/08/27 <Add> OKADA ------->
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();	// kome.ini
		const char* DEFAULT_STR="";
		std::string strDataType="";
		const char* pszDataTypeIni = ini->getString( INI_OUTPUT_FOLDER_SECTION, INI_OUTPUT_DEFAULT_DATA_TYPE, DEFAULT_STR );	
		if( 0 == strcmp( pszDataTypeIni, DEFAULT_STR ) ){
			strDataType = "Raw Data File";	// デフォルトは"Raw Data File"
		}else{
			strDataType = pszDataTypeIni;	// pszDataTypeIni = "Raw Data File", "DIT-FP" 等が入る
		}
		wxString wxstrDefaultSelectMethod = "";
		wxString wxstrDataTypeIni = strDataType;
		for( unsigned int i=0; i<stringsMethod.size(); i++ ){
			wxstrDefaultSelectMethod = stringsMethod.Item(i);
			int nRet = wxstrDefaultSelectMethod.compare( strDataType.c_str() );
			if( 0 == nRet ){
				break;	// 一致
			}
		}	// 一致するものがなければコンボボックスの最後の項目がデフォルト選択になります
		m_stringDefaultSelectMethod = wxstrDefaultSelectMethod;
		m_pComboDataType = new wxComboBox(this, ID_COMBOBOX_DATA_TYPE, m_stringDefaultSelectMethod, wxDefaultPosition, wxDefaultSize, stringsMethod, wxCB_SIMPLE | wxCB_READONLY );	 // コンボボックスに追加

	}	// SPEC 94692	// @date 2013/08/27 <Add> OKADA <-------
#else
	wxString stringDefaultSelectMethod = "";
	if( 1 <= stringsMethod.size() ){
		stringDefaultSelectMethod = stringsMethod.Item(0);
		m_stringDefaultSelectMethod = stringDefaultSelectMethod;

	}
	m_pComboDataType = new wxComboBox(this, ID_COMBOBOX_DATA_TYPE, stringDefaultSelectMethod, wxDefaultPosition, wxDefaultSize, stringsMethod, wxCB_SIMPLE | wxCB_READONLY );	 // コンボボックスに追加
#endif

	pDataTypeWaku->Add(m_pComboDataType, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	pDataTypeWaku->Add(m_pSubmitJobAddButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 
	buttonSizer->Add(pDataTypeWaku, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	// Delete
	m_pSubmitJobDeleteButton = new wxBitmapButton( this, ID_SUBMIT_JOB_DELETE_BUTTON, *iconMgr.getIcon("sample_del") );
	m_pSubmitJobDeleteButton->SetToolTip("Delete selected sample");
	buttonSizer->Add(m_pSubmitJobDeleteButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 
	// Up
	m_pSubmitJobUpButton = new wxBitmapButton( this, ID_SUBMIT_JOB_UP_BUTTON, *iconMgr.getIcon("item_up") );
	m_pSubmitJobUpButton->SetToolTip("Move selected sample up");
	buttonSizer->Add(m_pSubmitJobUpButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 
	// Down
	m_pSubmitJobDownButton = new wxBitmapButton( this, ID_SUBMIT_JOB_DOWN_BUTTON, *iconMgr.getIcon("item_down") );
	m_pSubmitJobDownButton->SetToolTip("Move selected sample down");
	buttonSizer->Add(m_pSubmitJobDownButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 
	// (blank)
	wxStaticText* staticText = new wxStaticText( this
		, wxID_STATIC
		, wxT( "     " )	// blank
		, wxDefaultPosition
		, wxDefaultSize
		, wxALIGN_LEFT);

	buttonSizer->Add( staticText, 1, wxALL | wxEXPAND);


	// ここでOutputFolderボタン描画

	// @date 2012/08/03 <Add> ------>
	m_pSubmitJobFolderOpenButton = new wxBitmapButton( this, ID_SUBMIT_JOB_FOLDER_OPEN_BUTTON, *iconMgr.getIcon("folder-open") );
	m_pSubmitJobFolderOpenButton->SetToolTip("Output Folder");
	buttonSizer->Add(m_pSubmitJobFolderOpenButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 
	// @date 2012/08/03 <Add> <------

	sizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	return sizer;
}


// ID_SUBMIT_JOB_SUBMIT_BUTTON
void BatchProcessingSubmitJobDlg::onSubmitButton( wxCommandEvent& evt ) {

	// 保存先のdirectoryを読み込み、保存先が設定されていなければエラーメッセージ	// @date 2012/08/09 <Add> OKADA
	{
		// default directory
		std::string path;
		// INIファイルからDefaultのディレクトリを読み込み
		BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
		path = mgr.getDirBatchProcessingSubmitJob();
		if( path.empty() ){
			// Default directoryが設定されていない！
			kome::window::WindowTool::showError( "Default folder is empty. Push Output Folder button and select default folder. " );
			return;
		}
	}

	// SPEC 94692		// @date 2013/08/27 <Add> OKADA ------->
	{
		wxString stringSelectedType = m_pComboDataType->GetValue();	// 現在選択されているData Type
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();	// kome.ini
		ini->setString( INI_OUTPUT_FOLDER_SECTION, INI_OUTPUT_DEFAULT_DATA_TYPE, stringSelectedType.c_str() );
	}
	// @date 2013/08/27 <Add> OKADA <-------

	// Submit処理
	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	kome::plugin::BatchInfo* bi = &( mgr.getBatchiInfo() );

	wxString wxsName = m_nameCtrl->GetValue();		// Name取得
	wxString wxsComment = m_textCtrl->GetValue();	// Comment取得

	bi->setName( wxsName.c_str() );			// Nameをセット
	bi->setComment( wxsComment.c_str() );	// Commentをセット
	
	// 入出力情報をセット(m_vectstrPaths
	int nSizeInput = m_stIoInf.vectstrPaths.size();

	const char* szSaveFormat = this->m_settings.getParameterValue( FORMAT );
	
	// @date 2013.09.30 <Add> M.Izumi ->
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "save_file", szSaveFormat );
	const char * pszExt = pItem->getCall()->getProperties().getStringValue( "ext", "" );
	kome::objects::SettingParameterValues *pSettingSub = this->m_settings.getSubParameterValues( FORMAT );
	kome::plugin::SettingsPage* page = pItem->getSettingsPage();
	std::string strParaSub = "";
	if( page ){
		strParaSub = page->getParameterString( *pSettingSub );
	}
	// @date 2013.09.30 <Add> M.Izumi <-

	for( int nIndexOfInputFile=0; nIndexOfInputFile<nSizeInput; nIndexOfInputFile++ ){
		
		bi->addInputFile( m_stIoInf.vectstrPaths[nIndexOfInputFile] );	// m_vectstrPaths[i]はフルパス

		std::string strOutput = mgr.getOutputFineName( m_stIoInf.vectstrPaths[nIndexOfInputFile], m_stIoInf.vectstrOutputFilenameInfo[nIndexOfInputFile], pszExt, nIndexOfInputFile, 0 );   // @2012/08/09 <Mod> OKADA
		bi->setOutputFile( nIndexOfInputFile, 0, strOutput );
		bi->setDataTypeIO( nIndexOfInputFile, 0, m_stIoInf.vectstrTypeName[nIndexOfInputFile] );	// @date 2012/08/29 <Add> OKADA
			
	}

	// サーバの関数にbatchinfo入れる
	wxString wxstrServer = m_pCombo->GetValue();	// サーバのlong nameを得る
	std::string strServerName = wxstrServer.c_str();

	if( strServerName.length() <= 0 ){
		// エラー
		return;
	}
	
	unsigned int uiNum = plgMgr.getNumberOfFunctionItems( "GET_BATCH_SERVICE" );
	for( unsigned int i=0; i<uiNum; i++ ){
		kome::plugin::PluginFunctionItem* pServer = plgMgr.getFunctionItem( "GET_BATCH_SERVICE", i );
		if( pServer != NULL ){
			std::string strNameLong = pServer->getLongName();

			if( strNameLong == strServerName ){
				// 一致した
				kome::plugin::BatchService* service = (kome::plugin::BatchService*)pServer->getCall()->invoke( NULL ).prim.pt;
			
				bi->addSaveFunction( pItem->getShortName() + strParaSub );

				// バッチ実行
				std::string strId = service->execute( bi );
					
				// 投げ終わったのでregisterで登録
				mgr.registerBatchInfo( *bi, pServer, strId.c_str() );
				bi->clearFiles();
			}
		}
	}
	
	// executeするとstrが返る。投げ終わったのでregisterで登録（batchinfo, 関数, 文字列）

	// Batch Job List 起動
	mgr.dispBatchJobWindow( true );

	EndDialog( 0 );		// SPEC 85678

	return;
}

// on cancel
void BatchProcessingSubmitJobDlg::onCancelButton( wxCommandEvent& evt ) {

	SetReturnCode( wxID_CANCEL );	// SPEC 85678
	EndDialog( wxID_CANCEL );		// SPEC 85678

	(this)->Close( true );

	(this)->Destroy();

	return;
}

// split text
void BatchProcessingSubmitJobDlg::splitText( std::vector<std::string>& result, const std::string& src, const char * separator )
{
	result.clear();
	if (src.empty()){ 
		return;
	}
	if( separator == NULL ){
		return;
	}

	std::string::size_type pos = 0;
	std::string::size_type new_pos;
	while ((new_pos = src.find( separator, pos )) != std::string::npos){
		result.push_back(src.substr(pos, new_pos - pos));
		pos = new_pos + 1;
	}
	result.push_back(src.substr(pos));
}


// ID_SUBMIT_JOB_ADD_BUTTON
void BatchProcessingSubmitJobDlg::onAddButton( wxCommandEvent& evt ) {
	// Batch Processing
	//   Add ボタンを押した際は SampleReader オブジェクトの selectKeys メソッドを呼び出し、キーを選択する。
	//   選択した キー を リストの Input 列に、選択された GET_FILE_READER の short name を Type 列に追加する。

	wxString stringSelectedType = m_pComboDataType->GetValue();	// 現在選択されているData Type

	const char * pszSelectedType = stringSelectedType.c_str();

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	unsigned int num = plgMgr.getNumberOfFunctionItems( wxT("get_file_reader") );
	kome::plugin::PluginFunctionItem* pItem = NULL;
	
	for( unsigned int i=0; i<num; i++ ){
		pItem = plgMgr.getFunctionItem( wxT("get_file_reader"), (unsigned int)i );
		const char * pszLongName = pItem->getLongName();
		if( pszLongName != NULL ){

			if( strcmp( pszSelectedType, pszLongName ) == 0 ){
				// 一致した
				break;
			}else{
				pItem = NULL;
			}
		}
	}

	// 入力ファイル追加
	std::vector< std::string > paths;
	std::string strKeys="";

	if( pItem != NULL ){
		kome::objects::SampleReader* sample_reader = (kome::objects::SampleReader*)pItem->getCall()->invoke( NULL ).prim.pt;

		if( sample_reader != NULL ){
			strKeys = sample_reader->selectKeys();
			// strKeysには、選択されたキー（ファイル名）が STR_FILE_STRINGS_SEPARATOR で区切られた形式で入る
			splitText( paths, strKeys, STR_FILE_STRINGS_SEPARATOR );

			delete sample_reader;	// @date 2013/06/05 <Add> OKADA for memory leak
		}
		
	}

	// @date 2012/12/14 <Add> OKADA
	for( unsigned int i=0; i<paths.size(); i++ ){
		std::string strTarget = paths[i];

		std::vector< std::string >::iterator cIter = std::find( m_stIoInf.vectstrPaths.begin(), m_stIoInf.vectstrPaths.end(), strTarget );
		if( cIter != m_stIoInf.vectstrPaths.end() ){
			//printf( "存在してます" );
			;
		}else{
			// 存在していないので追加
			m_stIoInf.vectstrPaths.push_back( strTarget );

			// 年月日時分秒の文字列を作成
			SYSTEMTIME stNowTime;
			GetLocalTime( &stNowTime );
			std::string strNowTime = strfmt( "%04d%02d%02d_%02d%02d%02d"
				, stNowTime.wYear
				, stNowTime.wMonth
				, stNowTime.wDay
				, stNowTime.wHour
				, stNowTime.wMinute
				, stNowTime.wSecond
			);
			m_stIoInf.vectstrOutputFilenameInfo.push_back( strNowTime );

			// Short Name をType列に追加
			const char * pszShortName = pItem->getShortName();		//  @date 2012/08/08 <Add> OKADA
			m_stIoInf.vectstrTypeName.push_back( pszShortName );	//  @date 2012/08/08 <Add> OKADA
		}
	}

	// ここでGridにセット
	m_pBatchProcessingSubmitJobList->setVectStrPaths( m_stIoInf );
	m_pBatchProcessingSubmitJobList->doCreate();
	m_pBatchProcessingSubmitJobList->updateList();

	return;
}


// ID_SUBMIT_JOB_DELETE_BUTTON
void BatchProcessingSubmitJobDlg::onDeleteButton( wxCommandEvent& evt ) {

// 参考：void BatchActionSelectPanel::onDelete

	bool bRet = kome::window::WindowTool::showYesNo( wxT("Are you sure you want to delete the selected input sample?"), "Delete", "Don't delete" );

	if( bRet ){
	
		int nSelected = m_pBatchProcessingSubmitJobList->GetSelectedItemCount();
		if( nSelected <= 0 ){
			// NG
		}else{

			std::vector< int > items;
			kome::window::ListTool::getSelectedItems( m_pBatchProcessingSubmitJobList, items );

			if( items.size() <= 0 ){
				// 選択データ無し
			}else{
				// 選択データあり
				for( long i=(((long)items.size())-1); 0<=i; i-- ){
					int nSize = m_pBatchProcessingSubmitJobList->getCountOfVectPaths();
					if( items[i] < nSize ){
						int nRet=-1;

						// m_vectstrPaths の items[i] 個目の要素を削除
						std::vector<std::string>::iterator it  = m_stIoInf.vectstrPaths.erase(m_stIoInf.vectstrPaths.begin() + items[i]);
						std::vector<std::string>::iterator it2 = m_stIoInf.vectstrOutputFilenameInfo.erase(m_stIoInf.vectstrOutputFilenameInfo.begin() + items[i]);
						std::vector<std::string>::iterator it3 = m_stIoInf.vectstrTypeName.erase(m_stIoInf.vectstrTypeName.begin() + items[i]);	// @date 2013/01/21 <Add> OKADA
					}
				}

				// ここでGridにセット
				m_pBatchProcessingSubmitJobList->setVectStrPaths( m_stIoInf );
				m_pBatchProcessingSubmitJobList->doCreate();
				m_pBatchProcessingSubmitJobList->updateList();
			}
		}
	}

	return;
}


//ID_SUBMIT_JOB_UP_BUTTON
void BatchProcessingSubmitJobDlg::onUpButton( wxCommandEvent& evt ) {

	int nSelected = m_pBatchProcessingSubmitJobList->GetSelectedItemCount();
	if( nSelected <= 0 ){
		// NG
	}else{

		std::vector< int > items;
		kome::window::ListTool::getSelectedItems( m_pBatchProcessingSubmitJobList, items );

		if( items.size() <= 0 ){
			// 選択データ無し
		}else{
			// 選択データあり
			bool flgReDraw = 0;
			for( unsigned int i=0; i<items.size(); i++ ){
				int nSize = m_pBatchProcessingSubmitJobList->getCountOfVectPaths();
				if( items[i] < nSize ){

					// UPの場合
					if( items[i] <= 0 ){
						// 一番上なので、動かせない
					}else{
						// 移動先（items[i]-1）が選択セル( items[i])なら、移動させない ※選択セルの順番を入れ替えない為
						int flgSelected = 0;
						for( unsigned int j=0; j<items.size(); j++ ){	// SPEC 85683
							if( (items[i]-1) == items[j] ){
								flgSelected = 1;	// 移動先は選択セル
							}
						}
						if( flgSelected ){
							continue;
						}

						// items[i], items[i]-1番目を入れ替える
						std::swap(	m_stIoInf.vectstrPaths[items[i]], 
									m_stIoInf.vectstrPaths[items[i]-1] );

						std::swap(	m_stIoInf.vectstrOutputFilenameInfo[items[i]], 
									m_stIoInf.vectstrOutputFilenameInfo[items[i]-1] );

						items[i]--;	// @date 2012/04/20 <Add> OKADA

						flgReDraw = 1;
					}
				}
			}

			if( flgReDraw ){
				// ここでGridにセット
				m_pBatchProcessingSubmitJobList->setVectStrPaths( m_stIoInf );
				m_pBatchProcessingSubmitJobList->doCreate();
				m_pBatchProcessingSubmitJobList->updateList();

				// ここで、選択していた物を選択し直す
				for( unsigned int i=0; i<items.size(); i++ ){
					kome::window::ListTool::selectItem( m_pBatchProcessingSubmitJobList, items[i] );		
				}
			}
		}
	}

	return;
}


// ID_SUBMIT_JOB_DOWN_BUTTON
void BatchProcessingSubmitJobDlg::onDownButton( wxCommandEvent& evt ) {

	int nSelected = m_pBatchProcessingSubmitJobList->GetSelectedItemCount();
	if( nSelected <= 0 ){
		// NG
	}else{

		std::vector< int > items;
		kome::window::ListTool::getSelectedItems( m_pBatchProcessingSubmitJobList, items );

		if( items.size() <= 0 ){
			// 選択データ無し
		}else{
			// 選択データあり
			bool flgReDraw = 0;
			for( long i=(((long)items.size())-1); 0<=i; i-- )
			{
				int nSize = m_pBatchProcessingSubmitJobList->getCountOfVectPaths();
				if( items[i] < nSize ){

					// DOWN の場合
					if( nSize <= (items[i]+1) ){
						// 一番下なので、動かせない
					//	break;// SPEC 85683
					}else{

						// 移動先（items[i]+1）が選択セル( items[i])なら、移動させない ※選択セルの順番を入れ替えない為
						int flgSelected = 0;
						for( unsigned int j=0; j<items.size(); j++ ){	// SPEC 85683
							if( (items[i]+1) == items[j] ){
								flgSelected = 1;	// 移動先は選択セル
							}
						}
						if( flgSelected ){
							continue;
						}

						// items[i], items[i]+1番目を入れ替える
						std::swap(	m_stIoInf.vectstrPaths[items[i]], 
									m_stIoInf.vectstrPaths[items[i]+1] );
						std::swap(	m_stIoInf.vectstrOutputFilenameInfo[items[i]], 
									m_stIoInf.vectstrOutputFilenameInfo[items[i]+1] );

						items[i]++;	// @date 2012/04/20 <Add> OKADA

						flgReDraw = 1;
					}
				}
			}

			if( flgReDraw ){
				// ここでGridにセット
				m_pBatchProcessingSubmitJobList->setVectStrPaths( m_stIoInf );
				m_pBatchProcessingSubmitJobList->doCreate();
				m_pBatchProcessingSubmitJobList->updateList();

				// ここで、選択していた物を選択し直す
				int nSize = m_pBatchProcessingSubmitJobList->getCountOfVectPaths();
				for( unsigned int i=0; i<items.size(); i++ ){
					kome::window::ListTool::selectItem( m_pBatchProcessingSubmitJobList, items[i] );		

				}
			}
		}
	}
	return;
}

// @date 2012/08/03 <Add> OKADA
void BatchProcessingSubmitJobDlg::onOpenFolder( wxCommandEvent& evt ) {

	// INIファイルからDefaultのディレクトリを読み込み

	// default directory
	std::string path;
	// INIファイルからDefaultのディレクトリを読み込み
	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	path = mgr.getDirBatchProcessingSubmitJob();

	
	// @date 2012/12/18 <Add> OKADA テスト用
	// SettingsPanel.cppから引用
	// dialog
	wxDirDialog dlg( this, "Choose a folder", path.c_str() );
	if( dlg.ShowModal() == wxID_OK ) {
		 wxString wxsTemp = dlg.GetPath();
		// INIファイルのDefaultのディレクトリに設定（書き込み）
		 std::string strDir = wxsTemp;

		strDir.append( "\\" );	// @date 2012/12/18 <Add> OKADA	// 「C:\temp」といった文字列をgetdir()で処理すると「C:\」になってしまう為。

		path = getdir(strDir.c_str()).c_str();
	}

	// ディレクトリ文字列をメンバ変数に格納
	mgr.setDirBatchProcessingSubmitJob( path );

	return;
}


// @date 2012/08/06 <Add> OKADA
void BatchProcessingSubmitJobDlg::onChangeComboDataType( wxCommandEvent& evt ) {

	wxString stringSelectedType = m_pComboDataType->GetValue();

	m_stringSelectedMethod = stringSelectedType;

	return;
}

void BatchProcessingSubmitJobDlg::onUpdateUI( wxUpdateUIEvent& evt ){
	
	int nId = evt.GetId();
	switch( nId ){
	case ID_SUBMIT_JOB_DELETE_BUTTON:
		{
			// セルが選択されている -> enable
			int nSelected = m_pBatchProcessingSubmitJobList->GetSelectedItemCount();
			if( nSelected <= 0 ){
				// NG
				evt.Enable( false );
			}else{
				evt.Enable( true );
			}
		}
		break;
	case ID_SUBMIT_JOB_UP_BUTTON:
		{
			// 表が選択されていて、かつ選択された何れかのセルの上に選択されていないセルがある場合 -> enable


			// 表が選択されていて、かつ一番上が選択されていない場合 -> enable
			bool flgEnable = false;	// SPEC 85683

			int nSelected = m_pBatchProcessingSubmitJobList->GetSelectedItemCount();
			if( nSelected <= 0 ){
				// NG
				flgEnable = false;
			}else{

				std::vector< int > items;
				kome::window::ListTool::getSelectedItems( m_pBatchProcessingSubmitJobList, items );

				if( items.size() <= 0 ){
					// 選択データ無し
					flgEnable = false;
				}else{
					// 選択データあり
					bool flgReDraw = 0;
					for( unsigned int i=0; i<items.size(); i++ ){
						int nSize = m_pBatchProcessingSubmitJobList->getCountOfVectPaths();
						if( items[i] < nSize ){


							// UPの場合
							if( items[i] <= 0 ){
								// 一番上なので、動かせない
							}else{
								// 一番上ではない
								int flgSelected = 0;
								for( unsigned int j=0; j<items.size(); j++ ){
									if( (items[i]-1) == items[j] ){
										flgSelected = 1;	// 移動先は選択セル
										 break;
									}
								}
								if( flgSelected ){
									continue;
								}
								flgEnable = true;
							}
						}
					}
				}
			}

			if( flgEnable == true ){
				evt.Enable( true );
			}else{
				evt.Enable( false );
			}
		}
		break;
	case ID_SUBMIT_JOB_DOWN_BUTTON:
		{
			// 表が選択されていて、かつかつ選択された何れかのセルの下に選択されていないセルがある場合 -> enable

			// 表が選択されていて、かつ一番下が選択されていない場合 -> enable
			bool flgEnable = false;	// SPEC 85683

			// セルが選択されている?
			int nSelected = m_pBatchProcessingSubmitJobList->GetSelectedItemCount();
			if( nSelected <= 0 ){
				// NG
				flgEnable = false;
			}else{

				std::vector< int > items;
				kome::window::ListTool::getSelectedItems( m_pBatchProcessingSubmitJobList, items );

				if( items.size() <= 0 ){
					// 選択データ無し
					flgEnable = false;
				}else{
					// 選択データあり
					bool flgReDraw = 0;
					for( long i=(((long)items.size())-1); 0<=i; i-- )
					{
						int nSize = m_pBatchProcessingSubmitJobList->getCountOfVectPaths();
						if( items[i] < nSize ){

							// DOWNの場合
							if( nSize <= (items[i]+1) ){
								// 一番下なので、動かせない
							}else{
								// 一番下ではない
								int flgSelected = 0;
								for( unsigned int j=0; j<items.size(); j++ ){
									if( (items[i]+1) == items[j] ){
										flgSelected = 1;	// 移動先は選択セル
										 break;
									}
								}
								if( flgSelected ){
									continue;
								}
								flgEnable = true;
							}
						}
					}
				}
			}

			if( flgEnable == true ){
				evt.Enable( true );
			}else{
				evt.Enable( false );
			}
		}
		break;
	case ID_SUBMIT_JOB_SUBMIT_BUTTON:
		{
			// Input/Output Settingsが1件以上登録されている AND Nameが登録されている AND Serverが登録されている -> enable
			// Input/Output Settingsが1件も登録されていない OR  Nameが登録されてない OR  Serverが登録されてない -> disable
			 bool flgEnable = true;

			int nSize = m_pBatchProcessingSubmitJobList->getCountOfVectPaths();
			if( nSize <= 0 ){	
				// Input/Output Settingsが1件も登録されていない
				flgEnable = false;
			}

			wxString wxsName = m_nameCtrl->GetValue();		// Name取得
			if( wxsName.length() <= 0 ){
				// Nameが登録されてない
				flgEnable = false;
			}
			
			wxString wxstrServer = m_pCombo->GetValue();	// サーバのlong nameを得る
			if( wxstrServer.length() <= 0 ){
				// サーバが登録されてない
				flgEnable = false;
			}

			if( flgEnable == true ){
				evt.Enable( true );
			}else{
				evt.Enable( false );
			}
		}
	default:
		break;
	}

	return;
}

void BatchProcessingSubmitJobDlg::onSelectComboOutputFormat( kome::window::SettingsPanelEvent& evt)
{
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return;
	}

	const char* szSaveFormat = this->m_settings.getParameterValue( FORMAT );
	
	// @date 2013.09.30 <Mod> M.Izumi ->
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "save_file", szSaveFormat );
	const char * pszExt = pItem->getCall()->getProperties().getStringValue( "ext", "" );

	m_pBatchProcessingSubmitJobList->setFormatExt( pszExt );
	// @date 2013.09.30 <Mod> M.Izumi <-

	m_pBatchProcessingSubmitJobList->doCreate();
	m_pBatchProcessingSubmitJobList->updateList();

	return;	
}
