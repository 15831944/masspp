/**
 * @file RTAlignmentPanel.cpp
 * @brief interfaces of RTAlignmentPanel class
 *
 * @author M.Izumi
 * @date 2013.01.24
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "RTAlignmentPanel.h"
#include "CreatePeakMatrixDialog.h"

#include "matrix_flow04.xpm"

#include <sstream>

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP						3
#define BORDER_SIZE						3

#define ALIGNMENT_NAME				"alignment"
#define ALIGNMENT_TYPE_NAME		    "ALIGNMENT"

#define SECTION					    "Alignment"
#define ALIGN_KEY_NAME				"ALIGNMENT_ALGORITHM"

// <-- @date 2013.05.28 <Add> FUJITA
#define SCALING_NAME				"scaling"
#define SCALING_TYPE_NAME		    "spectrum_scaling_for_rt_alignment"
#define SCALING_SECTION				"Scaling"
#define SCALING_KEY_NAME			"spectrum_log_scaling"
#define SPECTRUM_LOG_SCALING_FUNC_TYPE	"SPECTRUM_LOG_SCALING"
#define ALIGNMENT_LONG_NAME	  "AB3D Alignment (TIC Pattern)"  // RT アライメントとしてこれが選択されている場合のみスペクトルのスケーリングを行う。
// -->


BEGIN_EVENT_TABLE( RTAlignmentPanel, kome::window::PeakMatrixSettingsPageBase )
	EVT_KOME_SETTINGS_PANEL(wxID_ANY, RTAlignmentPanel::onSettingChanged)
END_EVENT_TABLE()

RTAlignmentPanel::RTAlignmentPanel( wxWizard* pParent )
	: kome::window::PeakMatrixSettingsPageBase( pParent, "RT Alignment" ){

	m_helpPanel = NULL;
	m_book = NULL;

	m_pParentDlg = dynamic_cast<CreatePeakMatrixDialog*>( pParent );

	createControls();
}

RTAlignmentPanel::~RTAlignmentPanel(){
}

// Validate panel settings 
bool RTAlignmentPanel::IsValid()
{
	return m_helpPanel->Validate();
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool RTAlignmentPanel::PreApplyValidationCheck(std::string& sError)
{
	bool bSuccess = true;

	std::ostringstream errorString;
		
	sError = errorString.str();

	return bSuccess;
}

// save panel settings
bool RTAlignmentPanel::SavePanelSettings(kome::core::Progress *pProgress)
{
	if( !TransferDataFromWindow() ) {
		return false;
	}
	
	bool bSuccess = false;
	
	if( m_helpPanel == NULL || m_book->GetSelection() != 0 ){
		return true;
	}

	if( pProgress == NULL ){
		pProgress = &kome::core::Progress::getIgnoringProgress();
	}
	// progress
	pProgress->createSubProgresses(0);
	
	// スケーリング実行 
	kome::plugin::PluginFunctionItem* scaling_item = m_helpPanel->getScaling();                      // 選択されているスケーリング手法の名前を読んでくる
	kome::objects::SettingParameterValues* scaling_settings = m_helpPanel->getScalingSettings();     // パラメータの値を読んでくる
	
	
	if (scaling_item != NULL ){    // (None)が選択されていれば、スケーリングを行わない。

		// groups   NormalizationPanel::SavePanelSettings からコピー
		kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
		// selected project
		int project = GetActiveMatrix();
		// groups   NormalizationPanel::SavePanelSettings のl248からコピー
		std::vector< int > matrixGroups;
		std::vector< int > samples;
		identMgr.getGroups( matrixGroups, project );
		for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
			identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
		}

		// PeakMatrixManager::Normalizations　からコピー(l777付近) したものを拡張
		for( unsigned int i = 0; i < samples.size(); i++ ) {
			// sample
			int matrixSampleId = samples[ i ];
			int sampleId = identMgr.getMatrixSampleSample( matrixSampleId );

			kome::ident::SampleReadInfo readInfo;
			kome::objects::Sample* sample = identMgr.openSample( sampleId, readInfo );
			// 下記を追加
			
			if ( sample != NULL ) {   // エラー処理

				kome::objects::DataGroupNode* sampleGroup = sample->getGroup(0);           // 0で決め打ち
		
				kome::objects::Parameters params;
	
				kome::plugin::PluginCallTool::setSample( params, *sample );           // sampleの情報をparamsに入れる  
				kome::plugin::PluginCallTool::setSettingValues( params, *scaling_settings ); 
	
				// TICアライメントならば、スケーリングを行う
				kome::plugin::PluginFunctionItem* alignment_item = m_helpPanel->getAlignment();
				const char* alignment_name = alignment_item->getLongName();

				kome::plugin::PluginCallTool::setGroup( params, *sampleGroup );
				if ( 0 == strcmp( alignment_name, ALIGNMENT_LONG_NAME) ){         // TICアライメント       
					kome::objects::Variant var = scaling_item->getCall()->invoke( &params );	     // スケーリング実行
					sample = (kome::objects::Sample*)var.prim.pt;
				}
			}   // エラー処理用
		}
	}
		
	// アライメント実行 
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	kome::plugin::PluginFunctionItem* item = m_helpPanel->getAlignment();
	kome::objects::SettingParameterValues* settings = m_helpPanel->getAlignmentSettings();

	int matrix = identMgr.getSelectedMatrix();

	const char* name = (item != NULL ? item->getShortName() : "" );
	kome::plugin::SettingsPage* page = (item != NULL ? item->getSettingsPage() : NULL );
	std::string strParam;
	if( page != NULL ){
		strParam = page->getParameterString( *settings );
	}

	// set Alignment	
	identMgr.setAlignment( matrix, name, strParam.c_str() );
	
	// progress
	if( !pProgress->isStopped() ) {
		pProgress->fill();
	}
	bSuccess = true;

	return (bSuccess);
}

// Update standard samples on this panel
void RTAlignmentPanel::updateStandardSamples(bool bReadFromINIfile){
}

// on a setting changing in the settings panel
void RTAlignmentPanel::onSettingChanged(kome::window::SettingsPanelEvent& evt)
{
	SetEdited();
}

// Refresh panel contents for matrix
void RTAlignmentPanel::RefreshPanelContentsForMatrix()
{
	updateStandardSamples();

	updateSettings();
}

// transfer data from window
bool RTAlignmentPanel::TransferDataFromWindow(){
	// default
	if( !PeakMatrixSettingsPageBase::TransferDataFromWindow() ) {
		return false;
	}

	if( m_helpPanel == NULL || m_book->GetSelection() != 0 ){
		m_helpPanel->setAlignment( NULL );
		m_helpPanel->setAligmentSettings( NULL );
	}else{

		kome::plugin::PluginFunctionItem* item = m_helpPanel->getAlignment();
		std::string s = FMT( "- %s(%s)\n", m_sPanelName.c_str(), (item != NULL ? item->getLongName() : "" ) );
		setSettingParam( s.c_str() );
	}
	
	return true;
}

// create inner sizer
void RTAlignmentPanel::createInnerSizer(wxStaticBoxSizer* InnerSizer){
	// set bitmap
	wxBitmap addBmp( matrix_flow04_xpm );
	
	wxBoxSizer* bmtSizer = new wxBoxSizer( wxVERTICAL );
	bmtSizer->Add(  new wxStaticBitmap( this, wxID_ANY, addBmp ), 1, (wxALL & ~wxRIGHT) | wxGROW, BORDER_SIZE );

	wxSizer* gHoriznSizer = new wxBoxSizer( wxHORIZONTAL);
	gHoriznSizer->Add( bmtSizer, 1, (wxALL & ~wxRIGHT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	
	InnerSizer->Add( gHoriznSizer );

	m_book = new kome::window::RadioBoxBook( this );
	
	m_helpPanel = new RTAlignmentHelpPanel( m_book, GetINISectionNameForMatrix() );
	m_book->AddPage( m_helpPanel, wxT("Executing RT Alignment") );
	
	// panel 2
	wxPanel* panel2 = new wxPanel( m_book );
	m_book->AddPage( panel2, wxT( "Not Executing RT Alignment" ) );

	InnerSizer->Add( m_book, 1, wxALL | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

void RTAlignmentPanel::updateSettings(){
	// Load values from the INI file
	// Update the settings in the UI
	TransferDataToWindow();
}


// -------------------------------------------------------------------------------------
// class RT Alignment help pnale
// -------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( RTAlignmentHelpPanel, kome::window::ParametersAndHelpPanel )	
END_EVENT_TABLE()

// constructor
RTAlignmentHelpPanel::RTAlignmentHelpPanel( wxWindow* parent, const char* section )
	: kome::window::ParametersAndHelpPanel( parent ), m_params( NULL, false ){

	m_section = section;
	m_scaling = NULL;
	m_scalingSettings = NULL;
	m_alignment = NULL;
	m_alignmentSettings = NULL;

	// create controls
	createControls();
}

// destructor
RTAlignmentHelpPanel::~RTAlignmentHelpPanel(){
}

// create main sizer
wxSizer* RTAlignmentHelpPanel::createMainSizer(){
	wxBoxSizer* pVerticalSizer = new wxBoxSizer( wxVERTICAL );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	
    // スケーリング関数選択コンボボックス 
	wxSizer* corrSizerScaling = createCorrectionSizerScaling();
	pVerticalSizer->Add( corrSizerScaling, 1, wxGROW | wxALL, BORDER_SIZE );
	
	// RT alignment関数選択コンボボックス 
	kome::plugin::SettingsValue* value = m_params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( "alignment" );
	value->setRequired( true );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setValue( value );
	param->setName( ALIGNMENT_NAME );
	param->setSection( m_section.c_str() );
	param->setKey( ALIGN_KEY_NAME );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "RT Alignment" );
	form->setDesc( "Select RT Alignment algorithm" );

	// panel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
		
	pVerticalSizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );     // パネル上に、上記で色々設定を行ったRT alignment選択コンボボックスを追加

	return pVerticalSizer;
}

bool RTAlignmentHelpPanel::TransferDataFromWindow(){
	// default
	if( !kome::window::ParametersAndHelpPanel::TransferDataFromWindow() ) {
		return false;
	}

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// スケーリングの設定　　　　　　　　　　　　　　　　　　　　　　　// @date 2013.05.30 <Add> Fujita 
	const char* scaling = m_settings.getParameterValue( SCALING_NAME );

	m_scaling = plgMgr.getFunctionItem( SCALING_TYPE_NAME, scaling );
	m_scalingSettings = m_settings.getSubParameterValues( SCALING_NAME, scaling );


	// アライメントの設定
	
	const char* alignment = m_settings.getParameterValue( "alignment" );

	m_alignment = plgMgr.getFunctionItem( "alignment", alignment );
	m_alignmentSettings = m_settings.getSubParameterValues( "alignment", alignment );

	return true;
}

// create correction of scaling sizer									@date 2013/3/13 <Add> FUJITA
wxSizer* RTAlignmentHelpPanel::createCorrectionSizerScaling() {
	// sizer
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
