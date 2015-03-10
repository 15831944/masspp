/**
 * @file PeakAnalysisPanel.cpp
 * @brief implementation of PeakAnalysisPanel class
 *
 * @author S.Tanaka
 * @date 2012.03.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "CreatePeakMatrixDialog.h"
#include "PeakAnalysisPanel.h"
#include "PeakMatrixManager.h"
#include "NormalizationPanel.h"
#include "MatrixViewManager.h"

#include "matrix_flow06.xpm"

#include <sstream>

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						3

#define ANALYSIS_PLUGIN_TYPE            "MATRIX_PEAK_ANALYSIS"
#define PM_STANDARD_SAMPLE_PARAM_KEY	"PEAK_ANALYSIS_STANDARD_SAMPLE"
#define PM_ANALYSIS_PARAM_KEY           "ANALYSIS_LIST"

#define ANALYSIS_PARAM_NAME             "Analysis"

#define SCALING_PLUGIN_TYPE				"MATRIX_PEAK_SCALING"
#define SCALING_PARAM_NAME				"Scaling"
#define PM_SCALING_PARAM_KEY			"SCALING_LIST"

#define using_multivariate_analysis    // これが定義されていると、多変量解析プラグインが使える。

#ifdef _WIN64
	#undef using_multivariate_analysis
#endif

#ifdef using_multivariate_analysis

	#define MULTIVARIATE_ANALYSIS_PARAM_NAME "Multivariate Analysis"  
	#define MULTIVARIATE_ANALYSIS_PLUGIN_TYPE "MATRIX_MULTIVARIATE_ANALYSIS"  
	#define PM_MULTIVARIATE_ANALYSIS_PARAM_KEY "MULTIVARIATE_ANALYSIS_LIST"  

#endif




BEGIN_EVENT_TABLE( PeakAnalysisPanel, kome::window::PeakMatrixSettingsPageBase )
	EVT_KOME_SETTINGS_PANEL(wxID_ANY, PeakAnalysisPanel::onSettingChanged)
END_EVENT_TABLE()

// constructor
PeakAnalysisPanel::PeakAnalysisPanel( wxWizard* parent )
		: kome::window::PeakMatrixSettingsPageBase( parent, "Peak Analysis" )
{
	// initialize
	m_stdId = -1;
	m_helpPanel = NULL;

	m_pParentDlg = dynamic_cast<CreatePeakMatrixDialog*>( parent );

	createControls();
}

// destructor
PeakAnalysisPanel::~PeakAnalysisPanel(void)
{
	clearScaling();
	clearAnalysis();
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool PeakAnalysisPanel::PreApplyValidationCheck(std::string& sError)
{
	bool bSuccess = true;

	std::ostringstream errorString;

	if( m_pParentDlg->GetTotalNumberOfSamples() == 0 ) {
		errorString << "There are no samples specified in the current matrix. Add at least one sample on the Group Settings tab.";
		bSuccess = false;
	}

	if( m_analysis.size() == 0 ) {
		errorString << "No methods for peak analysis have been specified. Add at least one.";
	}

	#ifdef using_multivariate_analysis
		if( m_multivariate_analysis.size() == 0 ) {  
			errorString << "No methods for multivariate analysis have been specified. Add at least one."; 
		}
	#endif



	sError = errorString.str();

	return bSuccess;
}

// save panel settings
bool PeakAnalysisPanel::SavePanelSettings(kome::core::Progress *pProgress)
{
	if( !TransferDataFromWindow() ) {
		return false;
	}

	pProgress->setStatus(GetPanelName().c_str());

	std::ostringstream infoString;

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = true;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();	
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();
	
	// selected project
	int project = GetActiveMatrix();
	
	// clear scaling
	clearScaling();
	
	const char* scaling = m_helpPanel->getScalingParam();
	std::vector< std::string > scalingArr;
	stringtoken( scaling, LIST_VALUES_DELIMITER, scalingArr );

	for( unsigned int i=0; i < scalingArr.size(); i++ ){
		std::string scalingName = scalingArr[ i ];
		kome::objects::SettingParameterValues* subSettings = new kome::objects::SettingParameterValues();
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( SCALING_PLUGIN_TYPE, scalingName.c_str(), subSettings );
		if( item != NULL ){
			m_scaling.push_back( item );
			m_scalingSettings.push_back( subSettings );
			infoString << scalingName << "\n";
		}
	}
	
	// analysis
	clearAnalysis();

	// analysis
	const char* analysis = m_helpPanel->getAnalysisParam();
	std::vector< std::string > analysisArr;
	stringtoken( analysis, LIST_VALUES_DELIMITER, analysisArr );
	
	for( unsigned int i = 0; i < analysisArr.size(); i++ ) {
		std::string analysisName = analysisArr[ i ];
		kome::objects::SettingParameterValues* subSettings = new kome::objects::SettingParameterValues();
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( ANALYSIS_PLUGIN_TYPE, analysisName.c_str(), subSettings );
		if( item != NULL ) {
			m_analysis.push_back( item );
			m_analysisSettings.push_back( subSettings );
			infoString << analysisName << "\n";
		}			
	}

	#ifdef using_multivariate_analysis  
		// multivariate analyisis
		clearMultivariateAnalysis();

		const char* multivariate_analysis = m_helpPanel->getMultivariateAnalysisParam();
		std::vector< std::string > multivariate_analysisArr;
		stringtoken( multivariate_analysis, LIST_VALUES_DELIMITER, multivariate_analysisArr );
	
		for( unsigned int i = 0; i < multivariate_analysisArr.size(); i++ ) {
			std::string multivariate_analysisName = multivariate_analysisArr[ i ];
			kome::objects::SettingParameterValues* subSettings = new kome::objects::SettingParameterValues();
			kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( MULTIVARIATE_ANALYSIS_PLUGIN_TYPE, multivariate_analysisName.c_str(), subSettings );
			if( item != NULL ) {
				m_multivariate_analysis.push_back( item );
				m_multivariate_analysisSettings.push_back( subSettings );
				infoString << multivariate_analysisName << "\n";
			}			
		}
	#endif

	m_pParentDlg->GetDialogProgressPtr()->setInformation(GetPanelName().c_str(), infoString.str().c_str());
		
	// groups
	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, project );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}

	// selected groups
	for( unsigned int i = 0; i < samples.size(); i++ ) {
		m_samples.push_back( samples[ i ] );
	}

	// standard group ID	
	m_stdId = -1;
	m_stdId = identMgr.getStandardSample( project );

	// scaling search
	if( m_scaling.size() > 0 && m_scalingSettings.size() > 0 ){
		bSuccess = mgr.scalingPeaks(
			&( m_scaling[ 0 ] ),
			&( m_scalingSettings[ 0 ] ),
			m_scaling.size(),
			m_stdId,
			pProgress
		);
		if (!bSuccess ){
			LOG_ERROR( FMT("scalingPeaks function failure") );
		}
	
	}

	// analysis search
	if( m_analysis.size() > 0 && m_analysisSettings.size() > 0 ) {
		bSuccess = mgr.analyzePeaks(
			&( m_analysis[ 0 ] ),
			&( m_analysisSettings[ 0 ] ),
			m_analysis.size(),
			m_stdId,
			pProgress
		);	

		if (!bSuccess ){
			LOG_ERROR( FMT("analyzePeaks function failure") );
		}
	}

	#ifdef using_multivariate_analysis 
	// multivariate analysis 
	if( m_multivariate_analysis.size() > 0 && m_multivariate_analysisSettings.size() > 0 ) {   

		bSuccess = mgr.multivariate_analysisPeaks(  
			&( m_multivariate_analysis[ 0 ] ),   
			&( m_multivariate_analysisSettings[ 0 ] ),   
			m_multivariate_analysis.size(),   
			m_stdId,   
			pProgress   
		);

		if (!bSuccess ){
			LOG_ERROR( FMT("multivariate_analysisPeaks function failure") );
		}
	}   
	#endif 

	if( !pProgress->isStopped() && bSuccess ) {
		pProgress->fill();
		MatrixViewManager& viewMgr = MatrixViewManager::getInstance();
		viewMgr.updateMatrixList();
	}

	if (!bSuccess && !pProgress->isStopped())
	{
		// After calling getlasterrormessage once, the error is deleted.
		// We want to use it twice, so make a copy first.
		std::string lasterrormessage = getlasterrormessage();
		if (lasterrormessage.size() > 0)
		{
			std::string strError = "An error occurred while analyzing matrix peaks:\n";
			strError += lasterrormessage;
			kome::window::WindowTool::showError( strError.c_str() );
		}
		else
		{
			LOG_ERROR( FMT("An error occurred while analyzing matrix peaks.") );
		}
	}

	return (bSuccess) && !pProgress->isStopped();
}

// create inner sizer
void PeakAnalysisPanel::createInnerSizer(wxStaticBoxSizer* pInnerSizer)
{
	// set bitmap
	wxBitmap addBmp( matrix_flow06_xpm );
	
	wxBoxSizer* bmtSizer = new wxBoxSizer( wxVERTICAL );
	bmtSizer->Add(  new wxStaticBitmap( this, wxID_ANY, addBmp ), 1, (wxALL & ~wxRIGHT) | wxGROW, BORDER_SIZE );

	wxSizer* gHoriznSizer = new wxBoxSizer( wxHORIZONTAL);
	gHoriznSizer->Add( bmtSizer, 1, (wxALL & ~wxRIGHT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	pInnerSizer->Add( gHoriznSizer );
		
	m_helpPanel = new PeakAnalysisHelpPanel( this, GetINISectionNameForMatrix() );
	
	pInnerSizer->Add( m_helpPanel, 1, wxALL | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// transfer data from window
bool PeakAnalysisPanel::TransferDataFromWindow() {
	// default
	if( !PeakMatrixSettingsPageBase::TransferDataFromWindow() ) {
		return false;
	}
	
	std::string s = "";
	s.append( FMT( "- %s", m_sPanelName.c_str() ) );

	std::string strPeakScaling = m_helpPanel->getScalingParam();
	if( !strPeakScaling.empty() ){
		s.append( FMT( "(%s)", strPeakScaling.c_str() ) );
	}

	std::string strPeakAnalysis = m_helpPanel->getAnalysisParam();
	if( !strPeakAnalysis.empty() ){
		s.append( FMT( "(%s)", strPeakAnalysis.c_str() ) );
	}

	#ifdef using_multivariate_analysis   
		std::string strMultivariateAnalysis = m_helpPanel->getMultivariateAnalysisParam();   
		if( !strMultivariateAnalysis.empty() ){   
			s.append( FMT( "(%s)", strMultivariateAnalysis.c_str() ) );   
		}
	#endif

	s.append("\n");
	setSettingParam( s.c_str() );
	
	return true;
}

// Validate panel settings 
bool PeakAnalysisPanel::IsValid()
{
	//return m_pSettingsPanel->Validate();
	return m_pParentDlg->Validate();
}

// clear analysis
void PeakAnalysisPanel::clearAnalysis() {
	m_analysis.clear();
	for( unsigned int i = 0; i < m_analysisSettings.size(); i++ ) {
		delete m_analysisSettings[ i ];
	}
	m_analysisSettings.clear();
}

// clear scaling
void PeakAnalysisPanel::clearScaling(){
	m_scaling.clear();
	for( unsigned int i=0; i < m_scalingSettings.size(); i++ ){
		delete m_scalingSettings[ i ];
	}
	m_scalingSettings.clear();
}

#ifdef using_multivariate_analysis   
// clear multivariate analysis   
void PeakAnalysisPanel::clearMultivariateAnalysis(){   
	m_multivariate_analysis.clear();   
	for( unsigned int i=0; i < m_multivariate_analysisSettings.size(); i++ ){   
		delete m_multivariate_analysisSettings[ i ];   
	}   
	m_multivariate_analysisSettings.clear();   
}   
#endif  

// Update standard samples on this panel
void PeakAnalysisPanel::updateStandardSamples(bool bReadFromINIfile)
{
}

// Refresh settings for matrix
void PeakAnalysisPanel::updateSettings()
{
	if( m_helpPanel != NULL ){
		m_helpPanel->updateSettings();
	}
}

// Refresh panel contents for matrix
void PeakAnalysisPanel::RefreshPanelContentsForMatrix()
{
	updateStandardSamples();

	updateSettings();
}

// on toggle an item in the Target Samples checklistbox
void PeakAnalysisPanel::onToggleTargetSample( wxCommandEvent& evt )
{
	SetEdited();
}

// on a setting changing in the settings panel
void PeakAnalysisPanel::onSettingChanged(kome::window::SettingsPanelEvent& evt)
{
	SetEdited();
}


// -------------------------------------------------------------------------------------
// class PeakAnalysisHelpPanel 
// -------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( PeakAnalysisHelpPanel, kome::window::ParametersAndHelpPanel )	
END_EVENT_TABLE()

// constructor
PeakAnalysisHelpPanel::PeakAnalysisHelpPanel( wxWindow* parent, const char* section )
	: kome::window::ParametersAndHelpPanel( parent ), m_params( NULL, false ){
	
	m_section = section;
	m_scaling = "";
	m_analysis = "";
	#ifdef using_multivariate_analysis  
		m_multivariate_analysis = "";
	#endif
	m_book0 = NULL;
	m_book1 = NULL;
	#ifdef using_multivariate_analysis  
		m_book2 = NULL;
	#endif

	// create controls
	createControls();
}

// destructor
PeakAnalysisHelpPanel::~PeakAnalysisHelpPanel(){
}

// create main sizer
wxSizer* PeakAnalysisHelpPanel::createMainSizer(){
	
	m_book0 = new kome::window::RadioBoxBook( this );
	m_book1 = new kome::window::RadioBoxBook( this );
	
	#ifdef using_multivariate_analysis
		m_book2 = new kome::window::RadioBoxBook( this ); // ラジオボックスで表示・非表示するブックを生成  
	#endif 
	
	wxBoxSizer* pVerticalSizer = new wxBoxSizer( wxVERTICAL );  // ブックを垂直に追加していく 
		
	pVerticalSizer->Add( new wxStaticText( this, wxID_ANY, wxT("Set parameters for peak scaling and analysis.")) );

	// page
	kome::plugin::SettingsPage* page0 = new kome::plugin::SettingsPage( m_params );  // パラメータ選択ボタン(page)を生成 
	page0->setName( "peak analysis" );

	// peak scaling
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_LIST );

	kome::plugin::SettingsValue* subVal = m_params.addValue();         // リスト下部の「追加アイコン」をクリックするとひらく子ダイアログ(上のパラメータ群) 
	subVal->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );   // 子ダイアログに、プラグイン列挙型を配置 
	subVal->setEnumKey( SCALING_PLUGIN_TYPE );                         // 列挙するプラグインは、SCALING_PLUGIN_TYPEプラグインタイプのプラグイン 
	subVal->setName( SCALING_PARAM_NAME );                             // 列挙するプラグインは、SCALING_PLUGIN_TYPEプラグインタイプのプラグイン 
	subVal->setRequired( true );
	val->addChildValue( subVal );                                      // 子ダイアログを、親ダイアログ上に配置 
	val->setRequired( false );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( SCALING_PARAM_NAME );
	param->setValue( val );
	param->setSection( m_section.c_str() );
	param->setKey( PM_SCALING_PARAM_KEY );
	
	kome::plugin::SettingsForm* form = page0->addForm();               // pageにフォームを追加する準備 
	form->setParam( param );
	form->setTitle( "Peak &Scaling" );
	form->setDesc( "Set peak scaling methods for adjusting peak values." );
	
	kome::window::SettingsPanel* panel0 = new kome::window::SettingsPanel( m_book0, page0, false, &m_settings );
	m_book0->AddPage( panel0, wxT("Executing peak scaling") );
	
	kome::plugin::SettingsPage* page1 = new kome::plugin::SettingsPage( m_params );
	// panel 
	wxPanel* panel1 = new wxPanel( m_book0 );
	m_book0->AddPage( panel1, wxT( "Not Executing peak scaling" ) );

	pVerticalSizer->Add( m_book0, 1, wxALL | wxGROW, BORDER_SIZE );
	
	// peak search
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_LIST );
	
	subVal = m_params.addValue();
	subVal->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	subVal->setEnumKey( ANALYSIS_PLUGIN_TYPE );
	subVal->setName( ANALYSIS_PARAM_NAME );
	subVal->setRequired( true );
	val->addChildValue( subVal );
	val->setRequired( false );

	param = m_params.addParam();
	param->setName( ANALYSIS_PARAM_NAME );
	param->setValue( val );
	param->setSection( m_section.c_str() );
	param->setKey( PM_ANALYSIS_PARAM_KEY );
	
	form = page1->addForm();
	form->setParam( param );
	form->setTitle( "Peak &Analysis" );
	form->setDesc( "Set peak analysis methods." );

	#ifdef using_multivariate_analysis   
		kome::plugin::SettingsPage* page2 = new kome::plugin::SettingsPage( m_params );  
	#endif 

	// panel
	kome::window::SettingsPanel* panel2 = new kome::window::SettingsPanel( m_book1, page1, false, &m_settings );
	m_book1->AddPage( panel2, wxT("Executing peak analysis") );
	
	// panel 
	wxPanel* panel3 = new wxPanel( m_book1 );
	m_book1->AddPage( panel3, wxT( "Not Executing peak analysis" ) );

	pVerticalSizer->Add( m_book1, 1, wxALL | wxGROW, BORDER_SIZE );

	#ifdef using_multivariate_analysis   
		val = m_params.addValue();  
		val->setType( kome::plugin::SettingsValue::TYPE_LIST );  
  
		subVal = m_params.addValue();  
		subVal->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );  
		subVal->setEnumKey(MULTIVARIATE_ANALYSIS_PLUGIN_TYPE);
		subVal->setName( MULTIVARIATE_ANALYSIS_PARAM_NAME );  
		subVal->setRequired( true );  
		val->addChildValue( subVal );  
		val->setRequired( false );  
  
		param = m_params.addParam();  
		param->setName( MULTIVARIATE_ANALYSIS_PARAM_NAME );  
		param->setValue( val );  
		param->setSection( m_section.c_str() );  
		param->setKey( PM_MULTIVARIATE_ANALYSIS_PARAM_KEY );  
  
		form = page2->addForm();  
		form->setParam( param );  
		form->setTitle( "Multivaliate &Analysis" );  
		form->setDesc( "Set multivaliate analysis methods." );  
  
		// panel  
		kome::window::SettingsPanel* panel4 = new kome::window::SettingsPanel( m_book2, page2, false, &m_settings );  
		m_book2->AddPage( panel4, wxT("Executing multivariate analysis") ); // リストなどがあるパネルを追加  
  
		// panel  
		wxPanel* panel5 = new wxPanel( m_book2 );  
		m_book2->AddPage( panel5, wxT( "Not Executing multivariate analysis" ) ); // 空のパネルを追加  
  
		 pVerticalSizer->Add( m_book2, 1, wxALL | wxGROW, BORDER_SIZE );  
	#endif 

	return pVerticalSizer;
}

// transer data from window
bool PeakAnalysisHelpPanel::TransferDataFromWindow(){
	// default
	if( !kome::window::ParametersAndHelpPanel::TransferDataFromWindow() ) {
		return false;
	}
	
	if( m_book0->GetSelection() == 0 ){
		m_scaling = m_settings.getParameterValue( SCALING_PARAM_NAME );
	}

	if( m_book1->GetSelection() == 0 ){
		m_analysis = m_settings.getParameterValue( ANALYSIS_PARAM_NAME );
	}
	
	#ifdef using_multivariate_analysis   
		if( m_book2->GetSelection() == 0 ){  
		   m_multivariate_analysis = m_settings.getParameterValue( MULTIVARIATE_ANALYSIS_PARAM_NAME ); // 各ラジオボックスブックに登録されている全手法全設定を、string型で得る。  
	}  
	#endif

	return true;
}

void PeakAnalysisHelpPanel::updateSettings(){
	// First of all, update the section for all parameters.
	for (unsigned int i = 0; i < m_params.getNumberOfParams(); i++)
	{
		kome::plugin::SettingsParam* param = m_params.getParam(i);
		param->setSection( m_section.c_str() );
	}

	// Refresh the parameters for all pages - this is necessary so that
	// the values are updated in the UI when a different matrix is selected.
	for (unsigned int j = 0; j < m_params.getNumberOfPages(); j++)
	{
		kome::plugin::SettingsPage* page = m_params.getPage(j);
		page->setParameters(m_settings);
	}
	// Load values from the INI file
	// Update the settings in the UI
	TransferDataToWindow();
}

const char* PeakAnalysisHelpPanel::getScalingParam(){
	
	return m_scaling.c_str();
}

const char* PeakAnalysisHelpPanel::getAnalysisParam(){
	
	return m_analysis.c_str();
}

#ifdef using_multivariate_analysis  
const char* PeakAnalysisHelpPanel::getMultivariateAnalysisParam(){   
  
return m_multivariate_analysis.c_str();   
}   
#endif
