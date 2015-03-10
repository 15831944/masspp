/**
 * @file AlignmentDialog.cpp
 * @brief implements of AlignmentDialog class
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "AlignmentDialog.h" 
#include "AlignmentManager.h"
#include "GraphPanel.h"
#include "CorrectionPanel.h"
#include "ResultPanel.h"

using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP				    3
#define BORDER_SIZE					3

#define ID_CONTROL_GROUP_COMBO		1
#define ID_CONTROL_CHROM_COMBO		2
#define ID_TREATMENT_GROUP_COMBO	3
#define ID_TREATMENT_CHROM_COMBO	4

#define ID_ALIGNMENT_START_BUTTON	5

// <-- @date 2013.03.13 <Add> FUJITA
#define SCALING_NAME				"scaling"
#define SCALING_TYPE_NAME		    "spectrum_scaling_for_rt_alignment"

#define SCALING_SECTION				"Scaling"
#define SCALING_KEY_NAME			"spectrum_scaling_key"

// -->

#define ALIGNMENT_NAME				"alignment"
#define ALIGNMENT_TYPE_NAME		    "ALIGNMENT"

#define SECTION					    "Alignment"
#define ALIGN_KEY_NAME				"ALIGNMENT_ALGORITHM"

#define PARAM_NAME                  "m/z_range"    // 各pluginのsettings.xml内で、パラメータの名前として記述した文字列：<param-name>m/z_range</param-name> 


BEGIN_EVENT_TABLE( AlignmentDialog, kome::window::ParametersAndHelpDialog )
	EVT_BUTTON( ID_ALIGNMENT_START_BUTTON, AlignmentDialog::startAlignment )
	EVT_BUTTON( wxID_CLOSE, AlignmentDialog::onOK )		// @date 2011.02.07 <Add> M.Izumi
END_EVENT_TABLE()


// constructor
AlignmentDialog::AlignmentDialog( wxWindow* parent, kome::objects::SettingParameterValues* settings )
		: kome::window::ParametersAndHelpDialog( parent, "RT Align Sample" ), m_params( NULL, false ) {
	// initialize
	m_book = NULL;
}

// destructor
AlignmentDialog::~AlignmentDialog() {
}

// get parameters
kome::objects::SettingParameterValues& AlignmentDialog::getParameters() {
	return m_settings;
}

// create main sizer
wxSizer* AlignmentDialog::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableCol( 0 );

	// notebook
	m_book = new wxNotebook(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxNB_TOP
	);
	sizer->Add( m_book, 1, wxALL | wxGROW, BORDER_SIZE );

	// graph panel
	GraphPanel* orgPanel = new GraphPanel( m_book );
	m_book->AddPage( orgPanel, wxT( "Chromatogram (Original)" ) );
	m_panels.push_back( orgPanel );

	// correction panel
	CorrectionPanel* corrPanel = new CorrectionPanel( m_book );
	m_book->AddPage( corrPanel, wxT( "Correction" ) );
	m_panels.push_back( corrPanel );

	// result panel
	ResultPanel* resultPanel = new ResultPanel( m_book );
	m_book->AddPage( resultPanel, wxT( "Result" ) );
	m_panels.push_back( resultPanel );

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

	// add chromatogram selection sizer
	m_helpGetterPanel = new AlignmentHelpGetterPanel( this );
	sizer->Add( m_helpGetterPanel, 1, wxGROW | wxALL, BORDER_SIZE );

	// add correction of scaling sizer                                        // スケーリングのコンボボックスを追加　@date 2013/3/13 <Add> FUJITA
	wxSizer* corrSizerScaling = createCorrectionSizerScaling();
	sizer->Add( corrSizerScaling, 1, wxGROW | wxALL, BORDER_SIZE );

	// add correction sizer
	wxSizer* corrSizer = createCorrectionSizerAlignment();
	sizer->Add( corrSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// create button sizer
wxSizer* AlignmentDialog::createButtonSizer() {
	// Override this function so that OK and Cancel buttons are not included.
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
		
	wxButton* pCloseButton = new wxButton(this, wxID_CLOSE, "Close");
//	buttonSizer->Add(pCloseButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	sizer->Add( pCloseButton, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5 );

	return sizer;
}

// create correction of scaling sizer									@date 2013/3/13 <Add> FUJITA
wxSizer* AlignmentDialog::createCorrectionSizerScaling() {
	// sizer
//	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );    // 元々こっち
	wxStaticBoxSizer* sizer = new wxStaticBoxSizer( wxHORIZONTAL, this, wxT( "Scaling method for TIC pattern" ) );
	
	// pageの作成
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );               // m_params....SettingParameter   パラメータを設定するpageを設定

	// pageの設定
	kome::plugin::ParameterSettings setting_params( NULL, true );
	kome::plugin::SettingsValue* value = m_params.addValue();      // 値の定義   
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( SCALING_TYPE_NAME );
	value->setRequired( false );    // true...必ず選択　　false...選択肢に「(None)」が表れる 
	   
	      
	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setValue( value );
	param->setName( SCALING_NAME );
	param->setSection( SCALING_SECTION );
	param->setKey( SCALING_KEY_NAME );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setTitle( "" );
	form->setParam( param );

	page->setParameters( m_settings );

	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel(
		this,
		page,
		false,
		&m_settings,
		true
	);

	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;      
}

// create correction sizer
wxSizer* AlignmentDialog::createCorrectionSizerAlignment() {
	// sizer
	wxStaticBoxSizer* sizer = new wxStaticBoxSizer( wxHORIZONTAL, this, wxT( "Alignment" ) );    // "Alignment"コンボボックス生成

	// panel
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );               // 「Parameters...」ボタン生成("page"はボタンを指す)
	// pageの設定
	kome::plugin::SettingsValue* value = m_params.addValue();   // 値の定義
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );   // プラグイン一覧から選択するという宣言
	value->setEnumKey( ALIGNMENT_TYPE_NAME );						// プラグインタイプが"ALIGNMENT_TYPE_NAME"であるプラグイン一覧
	value->setRequired( true );                                     // true...必ず選択
	
	kome::plugin::SettingsParam* param = m_params.addParam();      // m_paramsをvalue型とする
	param->setValue( value );                                      // m_paramsをvalueに、実際に設定 
	param->setName( ALIGNMENT_NAME );
	param->setSection( SECTION );                                  // 必須ではない
	param->setKey( ALIGN_KEY_NAME );                               // 必須ではない
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setTitle( "" );
	form->setParam( param );
	form->setDesc( "Select the alignment method." );

	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel(
		this,
		page,
		false,
		&m_settings
	);

	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );

	// add button
	wxButton* button = new wxButton( this, ID_ALIGNMENT_START_BUTTON, "Align", wxDefaultPosition, wxSize(140, 24) );
	sizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, BORDER_SIZE );
	button->Enable( m_helpGetterPanel->ctrlGrpIsEnable() );

	return sizer;
}


// initialize note
void AlignmentDialog::initNote() {
	// delete note
	for( int i = (int)( m_book->GetPageCount() - 1 ); i >= (int)m_panels.size(); i-- ) {
		m_book->DeletePage( i );
	}
}

// start alignment
void AlignmentDialog::startAlignment( wxCommandEvent& evt ) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();


    // init note
	initNote();
	
	// set functions
	const char* scaling = m_settings.getParameterValue( SCALING_NAME );   // パネル上の「SCALING_NAME」名のコントロール上の文字列(log or vast scaling)を読んでくる。
	kome::plugin::PluginFunctionItem* item_scaling = plgMgr.getFunctionItem( SCALING_TYPE_NAME, scaling );  //その文字列が表すプラグインを使えるようにする。

	// control group
	kome::objects::DataGroupNode* ctrlGroup = NULL;
	ctrlGroup = m_helpGetterPanel->getSelGroup( ID_CONTROL_GROUP_COMBO );
	
	kome::objects::DataGroupNode* trmtGroup = NULL;
	// treatment group
	trmtGroup = m_helpGetterPanel->getSelGroup( ID_TREATMENT_GROUP_COMBO );

	// @date 2011.02.07 <Mod> M.Izumi
	// transfer
	if( !TransferDataFromWindow() ){
		return;
	}
	    
	// init note
	initNote();

	// alignment item
	const char* alignment = m_settings.getParameterValue( ALIGNMENT_NAME );
	kome::plugin::PluginFunctionItem* alignment_item = plgMgr.getFunctionItem( ALIGNMENT_TYPE_NAME, alignment );
	if( alignment_item != NULL ) {
		// progress
		kome::window::DialogProgress progress( this, "RT Align Sample" );

		// scaling item     // @date 2013.04.04 <Add> Fujita
		const char* scaling = m_settings.getParameterValue( SCALING_NAME );   // パネル上の「SCALING_NAME」名のコントロール上の文字列(log or vast scaling)を読んでくる。
		kome::plugin::PluginFunctionItem* scaling_item = plgMgr.getFunctionItem( SCALING_TYPE_NAME, scaling );  //その文字列が表すプラグインを使えるようにする。

		// alignment settings
		kome::objects::SettingParameterValues* alignment_settings = m_settings.getSubParameterValues( ALIGNMENT_NAME, alignment );

		// scaling settings
		kome::objects::SettingParameterValues* scaling_settings = m_settings.getSubParameterValues( SCALING_NAME, scaling );    // "Log Scaling"のパラメータ設定ダイアログで設定されたパラメータ群を丸ごと読んでくる

		// perform alignment
		mgr.selectCtrlChrom( ctrlGroup->getChromatogram( 0 ) );
		mgr.selectTrmtChrom( trmtGroup->getChromatogram( 0 ) );
		if( mgr.prepareAlignment( *alignment_item, alignment_settings, *scaling_item, scaling_settings, progress ) ) {    // このif文の条件文中でアライメント実行  // @date 2013.04.04 <Add> Fujita  scaling用のitemとsettingsを追加
			Refresh();

			// Select the result tab.
			m_book->ChangeSelection(2);
		}
		else if( !progress.isStopped() ) {
			kome::window::WindowTool::showError( getlasterrormessage() );
		}
	}
}

// SPEC No.79986 Data alignment でエラーダイアログが出ない不具合修正 @date 2011.02.07 <Add> M.Izumi
// data from window 
bool AlignmentDialog::TransferDataFromWindow() {
	// default
	if( !StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// group
	kome::objects::DataGroupNode* ctrlGroup = NULL;
	ctrlGroup = m_helpGetterPanel->getSelGroup(ID_CONTROL_GROUP_COMBO);
	
	if( ctrlGroup == NULL ) {
		kome::window::WindowTool::showInfo( "Select a Control Group." );
		return false;
	}

	kome::objects::DataGroupNode* trmtGroup = NULL;
	trmtGroup = m_helpGetterPanel->getSelGroup( ID_TREATMENT_GROUP_COMBO );
	
	if( trmtGroup == NULL ) {
		kome::window::WindowTool::showInfo( "Select a Treatment Group." );
		return false;
	}

	if( ctrlGroup == trmtGroup ) {
		kome::window::WindowTool::showInfo( "Control and Treatment groups must be different. Change one of the selected groups." );
		return false;
	}

	return true;
}

// on OK @date 2011.02.07 <Add> M.Izumi
void AlignmentDialog::onOK( wxCommandEvent& evt ) {
	SetReturnCode( wxOK );
	Show( false );
}

// --------------------------------------------------------------
//  class Alignment HelpGetter Panel
// --------------------------------------------------------------
BEGIN_EVENT_TABLE( AlignmentHelpGetterPanel, kome::window::HelpGetterPanel )
	EVT_COMBOBOX( ID_CONTROL_GROUP_COMBO, AlignmentHelpGetterPanel::onGroupCombo )
	EVT_COMBOBOX( ID_CONTROL_CHROM_COMBO, AlignmentHelpGetterPanel::onChromCombo )
	EVT_COMBOBOX( ID_TREATMENT_GROUP_COMBO, AlignmentHelpGetterPanel::onGroupCombo )
	EVT_COMBOBOX( ID_TREATMENT_CHROM_COMBO, AlignmentHelpGetterPanel::onChromCombo )
END_EVENT_TABLE()

AlignmentHelpGetterPanel::AlignmentHelpGetterPanel( wxWindow* parent ) 
	: kome::window::HelpGetterPanel( parent ){
	m_ctrlGrpCombo = NULL;
	m_ctrlChromCombo = NULL;
	m_trmtGrpCombo = NULL;
	m_trmtChromCombo = NULL;

	createControls();
}

AlignmentHelpGetterPanel::~AlignmentHelpGetterPanel(){
}

wxSizer* AlignmentHelpGetterPanel::createMainSizer(){
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableCol( 0 );
	
	sizer->Add( createChromSizer(), 1, wxGROW | wxALL, BORDER_SIZE );
	
	return sizer;
}

// create chromatogram sizer
wxSizer* AlignmentHelpGetterPanel::createChromSizer() {
	// sizer
	wxGridSizer* sizer = new wxGridSizer( 2, 1, GRID_GAP, GRID_GAP );

	// control
	wxStaticBoxSizer* staticSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Control" ) );
	sizer->Add( staticSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	wxFlexGridSizer* gridSizer = new wxFlexGridSizer( 2, 2, GRID_GAP, GRID_GAP );
	gridSizer->AddGrowableCol( 1 );
	staticSizer->Add( gridSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	gridSizer->Add( 
		new wxStaticText( this, wxID_ANY, wxT( "Group" ) ),
		0,
		wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	m_ctrlGrpCombo = createGroupCombo( ID_CONTROL_GROUP_COMBO );

	wxSizer* comboSizer = new wxBoxSizer( wxHORIZONTAL );
	comboSizer->Add( m_ctrlGrpCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	gridSizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	gridSizer->Add( 
		new wxStaticText( this, wxID_ANY, wxT( "Chromatogram" ) ),
		0,
		wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	m_ctrlChromCombo = new wxComboBox(
		this,
		ID_CONTROL_CHROM_COMBO,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxCB_SIMPLE | wxCB_READONLY
	);
	m_ctrlChromCombo->SetClientData( NULL );

	comboSizer = new wxBoxSizer( wxHORIZONTAL );
	comboSizer->Add( m_ctrlChromCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	gridSizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	// treatment
	staticSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Treatment" ) );
	sizer->Add( staticSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	gridSizer = new wxFlexGridSizer( 2, 2, GRID_GAP, GRID_GAP );
	gridSizer->AddGrowableCol( 1 );
	staticSizer->Add( gridSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	gridSizer->Add( 
		new wxStaticText( this, wxID_ANY, wxT( "Group" ) ),
		0,
		wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	m_trmtGrpCombo = createGroupCombo( ID_TREATMENT_GROUP_COMBO );

	comboSizer = new wxBoxSizer( wxHORIZONTAL );
	comboSizer->Add( m_trmtGrpCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	gridSizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	gridSizer->Add( 
		new wxStaticText( this, wxID_ANY, wxT( "Chromatogram" ) ),
		0,
		wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	m_trmtChromCombo = new wxComboBox(
		this,
		ID_CONTROL_CHROM_COMBO,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxCB_SIMPLE | wxCB_READONLY
	);
	m_trmtChromCombo->SetClientData( NULL );

	comboSizer = new wxBoxSizer( wxHORIZONTAL );
	comboSizer->Add( m_trmtChromCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	gridSizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	// update chromatogram combo boxes
	updateChromCombos();
		
	return sizer;
}

// create group combo
wxComboBox* AlignmentHelpGetterPanel::createGroupCombo( const int id ) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// groups
	std::vector< kome::objects::DataGroupNode* > groups;
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		for( unsigned int j = 0; j < sample->getNumberOfGroups(); j++ ) {
			kome::objects::DataGroupNode* group = sample->getGroup( j );
			if( group->getNumberOfChromatograms() > 0 ) {
				groups.push_back( group );
			}
		}
	}

	// Spec No.86959 @date 2012.08.22 <Mod> M.Izumi
	wxString* groupNames = NULL;
	groupNames = new wxString[ groups.size() + 1 ];
	groupNames[0] = wxT("Select File");
	if( groups.size() > 0 ) {
		for( unsigned int i = 0; i < groups.size(); i++ ) {
			kome::objects::DataGroupNode* group = groups[ i ];
			std::string name;
			while( group != NULL ) {
				if( !name.empty() ) {
					name.append( " : " );
				}
				name.append( group->getName() );
				group = group->getParentGroup();
			}

			groupNames[ i + 1 ] = wxT(name.c_str());
		}
	}

	// combo box
	wxComboBox* combo = new wxComboBox(
		this,
		id,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		groups.size() + 1,
		groupNames,
		wxCB_SIMPLE | wxCB_READONLY
	);
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		combo->SetClientData( i + 1, groups[ i ] );
	}

	if( groups.size() == 0 ) {
		combo->Enable( false );
	}
	else {
		combo->SetSelection( 0 );
	}

	// delete array
	if( groupNames != NULL ) {
		delete[] groupNames;
	}

	return combo;
}

// on group combo
void AlignmentHelpGetterPanel::onGroupCombo( wxCommandEvent& evt ) {
	updateChromCombos();
}

// on chromatogram combo
void AlignmentHelpGetterPanel::onChromCombo( wxCommandEvent& evt ) {
	selectChromatograms();
}

kome::objects::DataGroupNode* AlignmentHelpGetterPanel::getSelGroup( const int id ){
	int sel = -1;
	if( id == ID_CONTROL_GROUP_COMBO ){
		sel =  m_ctrlGrpCombo->GetSelection();
		if( sel >= 1 ) {
			return (kome::objects::DataGroupNode*)m_ctrlGrpCombo->GetClientData( sel );
		}
	}else if( id == ID_TREATMENT_GROUP_COMBO ){
		sel = m_trmtGrpCombo->GetSelection();
		if( sel >= 1 ) {
			return (kome::objects::DataGroupNode*)m_trmtGrpCombo->GetClientData( sel );
		}
	}

	return NULL;
}

// update chromatogram combos
void AlignmentHelpGetterPanel::updateChromCombos() {
	// control group
	kome::objects::DataGroupNode* ctrlGrp = NULL;
	if( m_ctrlGrpCombo->GetCount() > 1 ) {
		int sel = m_ctrlGrpCombo->GetSelection();

		if (sel > 0)
		{
			ctrlGrp = (kome::objects::DataGroupNode*)m_ctrlGrpCombo->GetClientData( sel );
		}
	}

	// set control chromatogram combo
	kome::objects::DataGroupNode* tmp = (kome::objects::DataGroupNode*)m_ctrlChromCombo->GetClientData();
	if( ctrlGrp == NULL ) {
		m_ctrlChromCombo->Clear();
		m_ctrlChromCombo->SetClientData( NULL );
	}
	else if( ctrlGrp != tmp ) {
		m_ctrlChromCombo->Clear();
		for( unsigned int i = 0; i < ctrlGrp->getNumberOfChromatograms(); i++ ) {
			kome::objects::Chromatogram* chrom = ctrlGrp->getChromatogram( i );
			m_ctrlChromCombo->Append( wxT( chrom->getName() ), chrom );
		}
		m_ctrlChromCombo->SetClientData( ctrlGrp );

		if( m_ctrlChromCombo->GetCount() > 0 ) {
			m_ctrlChromCombo->Select( 0 );
		}
	}
	m_ctrlChromCombo->Enable( m_ctrlChromCombo->GetCount() > 0 );

	// treatment group
	kome::objects::DataGroupNode* trmtGrp = NULL;
	if( m_trmtGrpCombo->GetCount() > 1 ) {
		int sel = m_trmtGrpCombo->GetSelection();

		if (sel > 0)
		{
			trmtGrp = (kome::objects::DataGroupNode*)m_trmtGrpCombo->GetClientData( sel );
		}
	}

	// set treatment chromaotgram combo
	tmp = (kome::objects::DataGroupNode*)m_trmtChromCombo->GetClientData();
	if( trmtGrp == NULL ) {
		m_trmtChromCombo->Clear();
		m_trmtChromCombo->SetClientData( NULL );
	}
	else if( trmtGrp != tmp ) {
		m_trmtChromCombo->Clear();
		for( unsigned int i = 0; i < trmtGrp->getNumberOfChromatograms(); i++ ) {
			kome::objects::Chromatogram* chrom = trmtGrp->getChromatogram( i );
			m_trmtChromCombo->Append( wxT( chrom->getName() ), chrom );
		}
		m_trmtChromCombo->SetClientData( trmtGrp );

		if( m_trmtChromCombo->GetCount() > 0 ) {
			m_trmtChromCombo->Select( 0 );
		}
	}
	m_trmtChromCombo->Enable( m_trmtChromCombo->GetCount() > 0 );

	// select chromatograms
	selectChromatograms();
}

// select chromatograms
void AlignmentHelpGetterPanel::selectChromatograms() {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	// control chromatogram
	kome::objects::Chromatogram* chrom = NULL;
	if( m_ctrlChromCombo->GetCount() > 0 ) {
		int sel = m_ctrlChromCombo->GetSelection();
		chrom = (kome::objects::Chromatogram*)m_ctrlChromCombo->GetClientData( sel );
	}
	mgr.selectCtrlChrom( chrom );

	// treatment chromatogram
	chrom = NULL;
	if( m_trmtChromCombo->GetCount() > 0 ) {
		int sel = m_trmtChromCombo->GetSelection();
		chrom = (kome::objects::Chromatogram*)m_trmtChromCombo->GetClientData( sel );
	}
	mgr.selectTrmtChrom( chrom );

	// refresh
	kome::window::WindowTool::refresh();
}

std::string AlignmentHelpGetterPanel::getDescription( wxWindow* wnd ){
	std::string s;

	if( wnd == m_ctrlGrpCombo ){
		s = "Select the control group from opened samples.";
	}else if( wnd == m_ctrlChromCombo ){
		s = "Select the chromatogram to be displayed.";
	}else if( wnd == m_trmtGrpCombo ){
		s = "Select the treatmnet group from opened samples.";
	}else if( wnd == m_trmtChromCombo ){
		s = "Select the chromatogram to be displayed.";
	}
	return s;
}
