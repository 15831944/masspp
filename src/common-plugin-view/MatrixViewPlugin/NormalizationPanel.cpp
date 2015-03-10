/**
 * @file NormalizationPanel.cpp
 * @brief interfaces of NormalizationPanel class
 *
 * @author M.Izumi
 * @date 2012.10.19
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "CreatePeakMatrixDialog.h"
#include "NormalizationPanel.h"
#include "PeakMatrixManager.h"
#include "MatrixViewManager.h"

#include "matrix_flow03.xpm"

#include <sstream>

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP						3
#define BORDER_SIZE						3
#define ID_CHKBX_THRESHOLD				2500
#define ID_CHKBX_RT_RANGE				2600
#define ID_CHKBX_MZ_RANGE				2700

#define NORMALIZ_METHOD					"Normalization Method"
#define NORMALIZ_PEAK_DETECT			"normaliz_peak_detect"
#define NORMALIZ_FILTER					"normalization_filter"

#define SET_VAL_MZ						"mz"
#define SET_VAL_MZTOL					"mz_tolerance"
#define SET_VAL_RT						"rt"
#define SET_VAL_RTTOL					"rt_tolerance"
#define SET_VAL_TYPE					"type"
#define SET_VAL_RTRANGE					"rt_range"
#define SET_VAL_MZRANGE					"mz_range"
#define SET_VAL_TOP						"top"

#define VAL_THRESHOLD					"THRESHOLDRange"
#define	VAL_RT							"RTRange"
#define VAL_MZ							"MZRange"

#define FILTER_SECTION					"Normaliz Filter"


BEGIN_EVENT_TABLE( NormalizationPanel, kome::window::PeakMatrixSettingsPageBase )
	EVT_SIZE( NormalizationPanel::onSize )
	EVT_KOME_SETTINGS_PANEL(wxID_ANY, NormalizationPanel::onSettingChanged)
END_EVENT_TABLE()

// constructor
NormalizationPanel::NormalizationPanel( wxWizard* pParent ) 
	: kome::window::PeakMatrixSettingsPageBase( pParent, "Normalization" ){

	// panel
	m_pSettingsPanel		  = NULL;
	m_methodPanel			  = NULL;
	m_helpPanel				  = NULL;
	
	m_book = NULL;

	m_pParentDlg = dynamic_cast<CreatePeakMatrixDialog*>( pParent );

	createControls();	
}

// destructor
NormalizationPanel::~NormalizationPanel(){
}

// create inner sizer
void NormalizationPanel::createInnerSizer(wxStaticBoxSizer* InnerSizer)
{
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableCol( 1 );

	// set bitmap
	wxBitmap addBmp( matrix_flow03_xpm );
	sizer->Add(
		new wxStaticBitmap( this, wxID_ANY, addBmp ),
	    0,
		wxALIGN_LEFT | wxALIGN_TOP,
		BORDER_SIZE
	);
		
	// book
	m_book = new kome::window::RadioBoxBook( this );
	// panel
	m_helpPanel = new NormalizHelpPanel( m_book, GetINISectionNameForMatrix() );

	m_book->AddPage( m_helpPanel, wxT("Executing normalization" ));

	// panel 2
	wxPanel* panel2 = new wxPanel( m_book );
	m_book->AddPage( panel2, wxT( "Not executing normalization" ) );
	
	sizer->Add( m_book, 1, wxALL | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	InnerSizer->Add( sizer, 1, wxALL | wxGROW, BORDER_SIZE );
}

// transfer data from window
bool NormalizationPanel::TransferDataFromWindow(){
	// default
	if( !PeakMatrixSettingsPageBase::TransferDataFromWindow() ) {
		return false;
	}
	if( m_helpPanel == NULL || m_book->GetSelection() == 0 ){
		
		std::string strMethod = m_helpPanel->getNormalizMethods();
		if( strMethod.compare("") != 0 ){
			std::string s = FMT( "- %s(%s)\n", m_sPanelName.c_str(), strMethod.c_str() );

			setSettingParam( s.c_str() );
		}
	}

	return true;
}

// size event
void NormalizationPanel::onSize( wxSizeEvent& evt ) {
	// default
	OnSize( evt );
}

// Validate panel settings 
bool NormalizationPanel::IsValid()
{
	return m_helpPanel->Validate();
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool NormalizationPanel::PreApplyValidationCheck(std::string& sError)
{
	bool bSuccess = true;

	std::ostringstream errorString;
		
	sError = errorString.str();

	return bSuccess;
}

// save panel settings
bool NormalizationPanel::SavePanelSettings(kome::core::Progress *pProgress)
{
	if( !TransferDataFromWindow() ) {
		return false;
	}

	if( m_helpPanel == NULL || m_book->GetSelection() != 0 ){
		return true;
	}

	pProgress->setStatus(GetPanelName().c_str());

	std::ostringstream infoString;

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = false;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();

	// selected project
	int project = GetActiveMatrix();

	// normalization peak detect
	std::string peakDetect = m_helpPanel->getNormalizPeakDetect();
	/*const char* detect;
	if( peakDetect.compare( "SPEC_PEAK_PICKING" ) == 0 ){
		detect = "peak_spec";
	}else if( peakDetect.compare( "2D_PEAK_PICKING" ) == 0 ){
		detect = "peak_2d";
	}*/

	kome::plugin::PluginFunctionItem* m_peakDetectFunc = plgMgr.getFunctionItem( NORMALIZ_PEAK_DETECT, peakDetect.c_str() );
	kome::plugin::SettingsPage* page = m_peakDetectFunc->getSettingsPage();
	kome::objects::SettingParameterValues* m_peakDetectSettings = m_helpPanel->getPeakDetectSettings();
	

	kome::objects::SettingParameterValues settings;
	if( page != NULL ){
		
		page->setParameters( settings );

		for( unsigned int i=0; i < m_peakDetectSettings->getNumberOfParameters(); i++ ){
			settings.setValue( m_peakDetectSettings->getParameterName(i), m_peakDetectSettings->getParameterValue(i) );
		}

		m_peakDetectSettings = &settings;
	}
	
	// normalization peak filtering
	m_helpPanel->setFilterSettings();
	
	// normalization method
	const char* normalization = m_helpPanel->getNormalizMethods();
	std::vector< std::string > normalizArry;
	kome::plugin::SettingsValue::separateListValue( normalization, normalizArry );

	// clear
	m_helpPanel->clearNormaliz();
	
	// clear Normalization
	identMgr.clearNormalization( project );

	if( normalizArry.empty() ){
		return true;
	}
	for( unsigned int i=0; i < normalizArry.size(); i++ ){
		std::string name = ( normalizArry[i].c_str() );
				
		int ifind = name.find_first_of("[");
		std::string strItem =  name.substr( 0, ifind );
		std::string strParam = name.substr( ifind );
	
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "normalization_method", strItem.c_str() );
		
		infoString << "Normalization: ";
		infoString << item->getLongName() << "\n";

		if( item != NULL ){
			kome::objects::SettingParameterValues settings;
			kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );
			if( page != NULL ){
				page->setParameterString( settings, strParam.c_str() );
			}

			m_helpPanel->addNormaliz( item );
			m_helpPanel->addNormalizHelp( settings );

			infoString << strItem.c_str() << "\n";

		}
	}
	
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

	// clear peaks
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, project );
	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		identMgr.deleteMatrixPeak( peaks[ i ] );
	}
		
	// search
	if( m_helpPanel->getNumberOfNormaliz() > 0 && m_helpPanel->getNumberOfNormalizSettings() > 0 ){
	
		// ‚±‚±‚Å³‹K‰»ˆ— ’Ç‰Á
		bSuccess = mgr.normalizations(
			m_peakDetectFunc,
			m_peakDetectSettings,
			m_helpPanel->getFilterThreshold(), 
			&m_helpPanel->getFilterThresholdSettings(),
			m_helpPanel->getFilterRt(),
			&m_helpPanel->getFilterRtSettings(),
			m_helpPanel->getFilterMz(),
			&m_helpPanel->getFilterMzSettings(),
			&( m_helpPanel->getNormalizFunc()[0] ),
			&( m_helpPanel->getNormalizSettings()[0] ),
			samples,
			m_helpPanel->getNumberOfNormaliz(), 
			m_stdId,
			pProgress
		);
	}

	if( !pProgress->isStopped() && bSuccess ){
		pProgress->fill();
		MatrixViewManager& viewMgr = MatrixViewManager::getInstance();
		viewMgr.updateMatrixList();
	}

	if (!bSuccess && !pProgress->isStopped())
	{
		kome::window::WindowTool::showError( "An error occurred while filling the matrix." );
	}

	return (bSuccess) && !pProgress->isStopped();
}

// Update standard samples on this panel
void NormalizationPanel::updateStandardSamples(bool bReadFromINIfile){
}

// 
void NormalizationPanel::updateSettings(){
	
	m_helpPanel->updateSettings();

	// Load values from the INI file
	// Update the settings in the UI
	TransferDataToWindow();
}

// on a setting changing in the settings panel
void NormalizationPanel::onSettingChanged(kome::window::SettingsPanelEvent& evt)
{
	SetEdited();
}

// Refresh panel contents for matrix
void NormalizationPanel::RefreshPanelContentsForMatrix()
{
	updateStandardSamples();

	updateSettings();
}

// get nmber of normaliz method settings
bool NormalizationPanel::IsSettings(){
	
	if( !TransferDataFromWindow() ) {
		return false;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		
	// normalization method
	const char* normalization = m_helpPanel->getNormalizMethods();
	std::vector< std::string > normalizArry;
	kome::plugin::SettingsValue::separateListValue( normalization, normalizArry );
	
	if( !normalizArry.empty() ){
		SetEdited();

		return true;
	}
		
	return false;
}

// -------------------------------------------------------------------------------------
// class NormalizHelpPanel
// -------------------------------------------------------------------------------------

BEGIN_EVENT_TABLE( NormalizHelpPanel, kome::window::ParametersAndHelpPanel )	
END_EVENT_TABLE()

// constructor
NormalizHelpPanel::NormalizHelpPanel( wxWindow* parent, const char* section )
	: kome::window::ParametersAndHelpPanel( parent ), m_params( NULL, false ){
	
	m_section = section;

	// create controls
	createControls();
}

// destructor
NormalizHelpPanel::~NormalizHelpPanel(){
}

wxSizer* NormalizHelpPanel::createMainSizer(){
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );

	wxBoxSizer* textSizer = new wxBoxSizer( wxVERTICAL );
	textSizer->Add( new wxStaticText( this, wxID_ANY, wxT("Set the normalization filter and method. Normalization Filter is optional.")) );
	sizer->Add( textSizer, 1, wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );

	// Peak Detection
	sizer->Add(
		createPeakDetectSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
		);
	// Normalization Filter Group
	sizer->Add(
		createFilterSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
		);

	// Normalization Method List
	sizer->Add(
		createListEditPanelSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
		);

	return sizer;
}

// create peak filtering sizer
wxSizer* NormalizHelpPanel::createListEditPanelSizer(){
	// create sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setName( "normalizMethod" );
	page->setTitle( NORMALIZ_METHOD );
	
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_LIST );

	kome::plugin::SettingsValue* subVal = m_params.addValue();
	subVal->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	subVal->setEnumKey( "normalization_method" );	
	
	subVal->setName( "Method" );
	subVal->setRequired( true );
	
	val->addChildValue( subVal );
	val->setRequired( false );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( NORMALIZ_METHOD );
	param->setKey( "METHOD" );
	
	param->setValue( val );
	param->setSection( "Normalization" );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );	
	form->setDesc("Set normalization methods.");		
	// panel
	 kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
		
	sizer->Add( panel, 0, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// create filter sizer
wxSizer* NormalizHelpPanel::createFilterSizer(){
	
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// target list
	wxStaticBoxSizer* targetSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Peak Filter" ) );

	// Threshold
	targetSizer->Add( createPanelThreshold(), 0, wxALL | wxGROW, BORDER_SIZE );
	
	// RT Range
	targetSizer->Add( createPanelRtRange(), 0, wxALL | wxGROW, BORDER_SIZE );

	// m/z Range
	targetSizer->Add( createPanelMzRange(), 0, wxALL | wxGROW, BORDER_SIZE );

	sizer->Add( targetSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// create panel Threshold
kome::window::SettingsPanel* NormalizHelpPanel::createPanelThreshold(){
	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setName( "PageThreshold" );

	// Threshold
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->addEnumItem( "Absolute", "Absolute", "absolute" );
	val->addEnumItem( "Relative (%)", "Relative", "relative" );
	val->setRequired( true );
	val->setDefaultValue( "absolute" );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( "type" );
	param->setValue( val );
	param->setSection( "Normaliz Filter Threshold" );
	param->setKey( "type" );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Intensity Value Type" );
	form->setDesc( "This is a form to set the way of specifying intensity range.\n"
		"If \"Absolute\" is set, intensity range is specified by intensity value.\n "
		"If \"Reletive\" is set, intensity range is specified by percentage.\n"
		"(Maximum intensity value is 100%)." );

	val = m_params.addValue();
	val->setName( "threshold_range" );
	val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE );
	val->getNumberRestriction().setInclusiveMinValue( 0.0 );
	val->setRequired( false );
		
	param = m_params.addParam();
	param->setName( "threshold" );
	param->setValue( val );
	param->setSection( FILTER_SECTION );
	param->setKey( "THRESHOLD" );
	
	form = page->addForm();
	form->setParam( param );
	form->setTitle( "Intensity Range" );
	form->setDesc( "Intensity range of peak filter.\n"
		"If intensity range filter is not needed, set this form empty." );	

	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	
	return panel;
}

// create panel RT Range
kome::window::SettingsPanel* NormalizHelpPanel::createPanelRtRange(){
	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setName( "PageRtRange" );

	// RT Range
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setName( VAL_RT );
	val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE );
	val->getNumberRestriction().setInclusiveMinValue( 0.0 );
	val->setRequired( false );
	
	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( SET_VAL_RTRANGE );
	param->setValue( val );
	param->setSection( FILTER_SECTION );
	param->setKey( SET_VAL_RTRANGE );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "RT Range" );
	form->setDesc( "RT range of peak filter.\nIf RT range filter is not needed, set this form empty." );

	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	
	return panel;
}

// create panel m/z Range
kome::window::SettingsPanel* NormalizHelpPanel::createPanelMzRange(){
	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setName( "PageMzRange" );

	// m/z Range
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setName( VAL_MZ );
	val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE );
	
	val->getNumberRestriction().setInclusiveMinValue( 0.0 );
	val->setRequired( false );
	
	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( SET_VAL_MZRANGE );
	param->setValue( val );
	param->setSection( FILTER_SECTION );
	param->setKey( SET_VAL_MZRANGE );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "m/z Range" );
	form->setDesc( "m/z range of peak filter.\nIf m/z range filter is not needed, set this form empty." );
	
	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	
	return panel;
}

// create peak detect sizer
wxSizer* NormalizHelpPanel::createPeakDetectSizer(){
	// create sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setName("peakDetect");
	
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	val->setEnumKey( NORMALIZ_PEAK_DETECT );
	val->setRequired( true );

	// param
	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( NORMALIZ_PEAK_DETECT );
	param->setKey( "PEAK_DETECTOR" );
	param->setValue( val );
	param->setSection( "Normalization" );

	// form
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Using" );
	form->setDesc( "\"Normalization can use spectrum peaks or 2D peaks. Select one of them.\"" );
	
	//// third enumeration
	//kome::plugin::SettingsValue* val2 = m_params.addValue();
	//val2->setType( kome::plugin::SettingsValue::TYPE_STRING );
	//val2->addEnumItem( "Spectrum Peak Detection", "SPEC_PEAK_PICKING", "Using Spectrum Peak Detection" );
	//val2->addEnumItem( "2D Peak Detection", "2D_PEAK_PICKING", "Using 2D Peak Detection" );
	//val2->setRequired( true );

	//// forth enumeration
	//kome::plugin::SettingsValue* val3 = m_params.addValue();
	//val3->setType( kome::plugin::SettingsValue::TYPE_STRING );
	//val3->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	//val3->setEnumKey( "SPEC_PEAK_PICKING,2D_PEAK_PICKING" );    // comma separated values
	//val3->setRequired( true );

	//val2->addChildValue( val3 );

	//// param
	//kome::plugin::SettingsParam* param2 = m_params.addParam();
	//param2->setValue( val2 );
	//param2->setName( "peaks" );
	//
	//kome::plugin::SettingsParam* param3 = m_params.addParam();
	//param3->setValue( val3 );
	//param3->setName( "algorithm" );
	//
	//// form
	//kome::plugin::SettingsForm* form = page->addForm();
	//
	//form = page->addForm();
	//form->setParam( param2 );
	//form->setTitle( "Using" );
	//form->setDesc("\"Normalization can use spectrum peaks or 2D peaks. Select one of them.\""  );

	//form = page->addForm();
	//form->setParam( param3 );
	//form->setTitle( "Peak Detector" );
	//form->setDesc( "Select a peak detector." );
		
	//// panel
	//kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	// panel
	kome::window::PluginSelectionPanel* panel = new kome::window::PluginSelectionPanel(
		this,
		wxID_ANY,
		form,
		&m_settings,
		true,
		kome::window::PluginSelectionPanel::TYPE_CHOICE_BOOK
	);
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );
	return sizer;
}

// set threshold settings
void NormalizHelpPanel::setThresholdSetting(){
		
	kome::plugin::SettingsPage* page = m_params.getPage( "PageThreshold" );
	m_filterThreshold->setSettingsPage( page );

	if( page != NULL ){
		kome::objects::SettingParameterValues settings;
		page->setParameters( settings );
		
		const char* strVal = m_settings.getParameterValue( "threshold" );
		std::vector< std::string >tokens;
		stringtoken( strVal, ":", tokens );
		if( !tokens.empty() ){
			m_filterThresholdSettings = settings;
		}
	}
}

// set rt range settings
void NormalizHelpPanel::setRtRangeSetting(){
	
	kome::plugin::SettingsPage* page = m_params.getPage( "PageRtRange" );
	m_filterRt->setSettingsPage( page );

	if( page != NULL ){
		kome::objects::SettingParameterValues settings;
		page->setParameters( settings );
		
		const char* strVal = m_settings.getParameterValue( SET_VAL_RTRANGE  );

		std::vector< std::string >tokens;
		stringtoken( strVal, ":", tokens );
		if( !tokens.empty() ){
			int inum = m_params.getNumberOfValues();
			for( int i=0; i < inum; i++ ){
				kome::plugin::SettingsValue* val = m_params.getValue(i);
				if( strcmp( val->getName(), VAL_RT ) == 0 ){
					val->setRequired( true );
				}
			}
			m_filterRtSettings =  settings;
		}
	}
}

// set m/z range settings
void NormalizHelpPanel::setMzRangeSetting(){
	
	kome::plugin::SettingsPage* page = m_params.getPage( "PageMzRange" );
	m_filterMz->setSettingsPage( page );

	if( page != NULL ){
		kome::objects::SettingParameterValues settings;
		page->setParameters( settings );
		
		const char* strVal = m_settings.getParameterValue( SET_VAL_MZRANGE );
		
		std::vector< std::string >tokens;
		stringtoken( strVal, ":", tokens );
		if( !tokens.empty() ){
			int inum = m_params.getNumberOfValues();
			for( int i=0; i < inum; i++ ){
				kome::plugin::SettingsValue* val = m_params.getValue(i);
				if( strcmp( val->getName(), VAL_MZ ) == 0 ){
					val->setRequired( true );
				}
			}
			m_filterMzSettings = settings;
		}
	}
}

// set filter settings
void NormalizHelpPanel::setFilterSettings(){
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	m_filterThreshold = plgMgr.getFunctionItem( NORMALIZ_FILTER, "call_threshold" );
	m_filterRt = plgMgr.getFunctionItem( NORMALIZ_FILTER, "call_rt_range" );
	m_filterMz = plgMgr.getFunctionItem( NORMALIZ_FILTER, "call_mz_range" );

	setThresholdSetting();
	setRtRangeSetting();
	setMzRangeSetting();
}

// update settings
void NormalizHelpPanel::updateSettings(){
	// First of all, update the section for all parameters.
	for (unsigned int i = 0; i < m_params.getNumberOfParams(); i++)
	{
		kome::plugin::SettingsParam* param = m_params.getParam(i);
		param->setSection(m_section.c_str());
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

// clear analysis
void NormalizHelpPanel::clearNormaliz() {
	m_normaliz.clear();

	for( unsigned int i = 0; i < m_normalizSettings.size(); i++ ) {
		m_normalizSettings.erase( m_normalizSettings.begin() + i );
	}
	m_normalizSettings.clear();
}

// get normaliz methods
const char* NormalizHelpPanel::getNormalizMethods(){
	return m_settings.getParameterValue(NORMALIZ_METHOD);
}

// get normaliz peak detect
const char* NormalizHelpPanel::getNormalizPeakDetect(){
	return  m_settings.getParameterValue( NORMALIZ_PEAK_DETECT );
}

// get peak detect settings
kome::objects::SettingParameterValues* NormalizHelpPanel::getPeakDetectSettings(){
	std::string str = m_settings.getParameterValue( NORMALIZ_PEAK_DETECT );
	kome::objects::SettingParameterValues* sub = m_settings.getSubParameterValues( NORMALIZ_PEAK_DETECT, str.c_str() );
	
	return sub;
}

// add normaliz
void NormalizHelpPanel::addNormaliz( kome::plugin::PluginFunctionItem* item ){
	if( item != NULL ){
		m_normaliz.push_back( item );
	}
}

// get filter threshold
kome::plugin::PluginFunctionItem* NormalizHelpPanel::getFilterThreshold(){
	return m_filterThreshold;
}

// get filter threshold settings
kome::objects::SettingParameterValues NormalizHelpPanel::getFilterThresholdSettings(){
	return m_filterThresholdSettings;
}

// get filter rt
kome::plugin::PluginFunctionItem* NormalizHelpPanel::getFilterRt(){
	return m_filterRt;
}

// get filter rt settings
kome::objects::SettingParameterValues NormalizHelpPanel::getFilterRtSettings(){
	return m_filterRtSettings;
}

// get filter m/z
kome::plugin::PluginFunctionItem* NormalizHelpPanel::getFilterMz(){
	return m_filterMz;
}

// get filter m/z settings
kome::objects::SettingParameterValues NormalizHelpPanel::getFilterMzSettings(){
	return m_filterMzSettings;
}

// get normaliz function
std::vector<kome::plugin::PluginFunctionItem*> NormalizHelpPanel::getNormalizFunc(){
	return m_normaliz;
}

// get normaliz settings
std::vector< kome::objects::SettingParameterValues > NormalizHelpPanel::getNormalizSettings(){
	return m_normalizSettings;
}

// add normaliz help
void NormalizHelpPanel::addNormalizHelp( kome::objects::SettingParameterValues settings ){
	m_normalizSettings.push_back( settings );
}

// get number of normaliz
int NormalizHelpPanel::getNumberOfNormaliz(){
	return m_normaliz.size();
}

// get number of normaliz settings
int NormalizHelpPanel::getNumberOfNormalizSettings(){
	return m_normalizSettings.size();
}
