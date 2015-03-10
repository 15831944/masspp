/**
 * @file NormalizationDialog.cpp
 * @brief implements of NormalizationDialog class
 *
 * @author M.Izumi
 * @date 2012.07.02
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "NormalizationDialog.h"
#include "NormalizationManager.h"


using namespace kome::normalization;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP						3
#define BORDER_SIZE						3

#define ID_CONTROL_SAMPLE_COMBO			1000
#define ID_TREATMENT_SAMPLE_COMBO		2000
#define ID_TYPE_COMBO					3000

#define ID_CHKBX_THRESHOLD				2500
#define ID_CHKBX_RT_RANGE				2600
#define ID_CHKBX_MZ_RANGE				2700


#define NORMALIZ_METHOD					"Normalization Method"
#define NORMALIZ_PEAK_DETECT			"normaliz_peak_detect"
#define NORMALIZ_FILTER					"normalization_filter"


BEGIN_EVENT_TABLE( NormalizationDialog, kome::window::ParametersAndHelpDialog )
END_EVENT_TABLE()

// constructor
NormalizationDialog::NormalizationDialog( wxWindow* parent )
		: kome::window::ParametersAndHelpDialog( parent, "Normalization" ), m_params( NULL, false ) {
	// initialize
	//m_stdSmpCombo = NULL;
	//m_trmtSmpCombo = NULL;

	m_stdGroup = NULL;
	m_trmtGroup = NULL;
	
	m_method = "";
	m_peakDetect = "";

	m_helpGetterPanel = NULL;
}

// destructor
NormalizationDialog::~NormalizationDialog() {
}

wxSizer* NormalizationDialog::createMainSizer(){
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );

	//// group sizer
	//wxFlexGridSizer* groupSizer = new wxFlexGridSizer( 2, 2, GRID_GAP, GRID_GAP );
	//groupSizer->AddGrowableCol( 1 );
	//
	//groupSizer->Add(
	//	new wxStaticText( this, wxID_ANY, wxT( "Standard Sample" ) ),
	//	0,
	//	wxLEFT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
	//	BORDER_SIZE
	//);

	//m_stdSmpCombo = createSampleCombo( ID_CONTROL_SAMPLE_COMBO );
	//wxSizer* comboSizer = new wxBoxSizer( wxHORIZONTAL );
	//comboSizer->Add( m_stdSmpCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	//groupSizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	//groupSizer->Add(
	//	new wxStaticText( this, wxID_ANY, wxT( "Treatment" ) ),
	//	0,
	//	wxLEFT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
	//	BORDER_SIZE
	//);

	//m_trmtSmpCombo = createSampleCombo( ID_TREATMENT_SAMPLE_COMBO );
	//comboSizer = new wxBoxSizer( wxHORIZONTAL );
	//comboSizer->Add( m_trmtSmpCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	//groupSizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	m_helpGetterPanel = new NormalizHelpGetterPanel( this );
	sizer->Add( m_helpGetterPanel, 0, wxALL | wxGROW, BORDER_SIZE );
		
	// Normalization Method List
	sizer->Add(
		createListEditPanelSizer(),
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

	// Peak Detection
	sizer->Add(
		createPeakDetectSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
		);

	wxButton* button = new wxButton( this, wxID_OK, "Normalize" );
	sizer->Add( button, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
	
	return sizer;
}

// create button sizer
wxSizer* NormalizationDialog::createButtonSizer() {
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// button sizer
	wxSizer* buttonSizer = new wxBoxSizer( wxVERTICAL );

	wxButton* pCloseButton = new wxButton(this, wxID_CANCEL, "Close");
	buttonSizer->Add(pCloseButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, 5); 

	sizer->Add( buttonSizer, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL , 5 );

	return sizer;
}

// create peak filtering sizer
wxSizer* NormalizationDialog::createListEditPanelSizer(){
	// create sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

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
	form->setTitle( NORMALIZ_METHOD );
	form->setDesc( "Normalization method list. The normalization methods are performed in order of the list.\nIf the same normalization methods are listed, normalization is performed using the index closest to the m/z direction. "
"For example, when a sample contains 3 internal standards and the 3 internal standards are set to [Internal Standard (Multi Sample)] method, normalizatoins are performed in the following procedure:"
"(1) In [Treatmet] samle, m/z directions are separated 3 areas.\n"
"The region closest to the red-square internal standard  is A.\n"
"The region closest to the blue-square internal standard  is B.\n"
"The region closest to the green-square internal standard  is C.\n"
"(2) In the region A, peak intensity of red-square in [Treatment] is normalized to equal peak intensity of red-square in [Standard Smple]. Normalizations are performed in the region B and C in the same way." ); // @date 2014.01.15 <Add> M.Izumi	

	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );
		
	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );
	
	return sizer;
}

// create filter sizer
wxSizer* NormalizationDialog::createFilterSizer(){
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	
	// target list
	wxStaticBoxSizer* targetSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Normalization Filter" ) );
	
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
kome::window::SettingsPanel* NormalizationDialog::createPanelThreshold(){
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
	form->setTitle( "Type" );
	form->setDesc("Specify type of threshold intensity value from following:\n・Absolute\n・Relative" );
	
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
	form->setTitle( "Threshold Value" );
	form->setDesc("Peaks which have the peak intensity within the specified threshold value are target to normalization.|n"
"If [Type] is set to [Absolute] and [Threshold Value] is set to [100] - [1000],  only the peaks which have [100] - [1000] peak intensity are normalized. ");
	
	// panel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	
	return panel;
}

// create panel RT Range
kome::window::SettingsPanel* NormalizationDialog::createPanelRtRange(){
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
	param->setKey( "RT_RANGE" );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "RT" );
	form->setDesc( "Peaks within the specified threshold RT value are target to normalization.\n"
"If [Threshold Value] is set to [20] - [40],  only the peaks within RT = [20] - [40] are normalized. " );

	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	
	return panel;
}

// create panel m/z Range
kome::window::SettingsPanel* NormalizationDialog::createPanelMzRange(){
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
	form->setTitle( "m/z" );
	form->setDesc( "Peaks within the specified threshold RT value are target to normalization.\n"
"If [Threshold Value] is set to [20] - [40],  only the peaks within RT = [20] - [40] are normalized. " );
	
	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	
	return panel;
}

// create peak detect sizer
wxSizer* NormalizationDialog::createPeakDetectSizer(){
	// create sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

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
	form->setTitle( "Peak Type" );
	form->setDesc( "Specify the peak list type to be used for  normalization from following:\n・Spectrum\n・2D Peak" );
	
	// panel
	kome::window::SettingsPanel* panel 
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );

	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );
	return sizer;
}

// set selected samples
void NormalizationDialog::setSelectSamples(){	
	// get select standard sample
	m_stdGroup = m_helpGetterPanel->getSelGroup( ID_CONTROL_SAMPLE_COMBO );

	// get select treatment sample
	m_trmtGroup = m_helpGetterPanel->getSelGroup( ID_TREATMENT_SAMPLE_COMBO );
}

// transfer data from window
bool NormalizationDialog::TransferDataFromWindow() {
		
	// default
	if( !StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// 選択中のサンプルをセット( standard / treatement )
	setSelectSamples();
		
	const char* valMethod = m_settings.getParameterValue( NORMALIZ_METHOD );
	m_method = NVL( valMethod, "" );

	// Spec No.87218 Normalization - should check that at least one Normalization Method is specified before executing.
	// @date 2012.09.03 <Add> M.Izumi
	if( m_method.empty() ){
		kome::window::WindowTool::showInfo("At least one Normalization Method must be specified.");
		return false;
	}

	const char* valPeakDetect = m_settings.getParameterValue( NORMALIZ_PEAK_DETECT );
	m_peakDetect = NVL( valPeakDetect, "" );
	
	m_peakDetectSettings = m_settings.getSubParameterValues( NORMALIZ_PEAK_DETECT, valPeakDetect );
	
	if( m_stdGroup == m_trmtGroup ){
		kome::window::WindowTool::showInfo( "Standard Sample and Treatment Sample must be different. Change one of the selections." );
		return false;
	}

	if( !IsTreatmentChosen() ){
		return false;
	}
		
	// 各フィルタのSettingParameterValuesをセット
	setFilterSettingParameterValues();

	return true;
}

// Is the treatment chosen
bool NormalizationDialog::IsTreatmentChosen(){
	// サンプル間の正規表現時にTreatmentが選択されいていない場合はエラーメッセージを表示する。
	std::vector< std::string >tokens;
	kome::plugin::SettingsValue::separateListValue( m_method.c_str(), tokens );
	for( unsigned int i=0; i < tokens.size(); i++ ){
		std::string s = tokens[i];

		int ifind = s.find_first_of("[");
		std::string strItem =  s.substr( 0, ifind );
		
		if( strItem.compare( INT_STD_SAMPLE ) == 0 || strItem.compare( INTEGRAT_SAMPLE ) == 0 ||
			strItem.compare( MAX_INTENT_SAMPLE ) == 0 || strItem.compare( MEDIAN_SAMPLE ) == 0 ||
			strItem.compare( PERCENT_SAMPLE ) == 0 
		){
			
			if( m_trmtGroup == NULL ){
				kome::window::WindowTool::showInfo("No Treatment Sample has been selected, so normalization cannot be performed. Select a Treatment Sample.");
				return false;
			}
		}else{
			if( m_peakDetect.compare( "peak_spec" ) != 0 ){
				kome::window::WindowTool::showInfo("2D Peak detection cannot be used because a normalization method involving fractions has been specified. Select Spectrum peak detection.");
				return false;
			}
		}
	}

	return true;
}

// 各フィルタのSettingParameterValuesをセット
void NormalizationDialog::setFilterSettingParameterValues(){
	kome::objects::SettingParameterValues settings;
	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	mgr.setThresholdSetting( settings );
	mgr.setRtRangeSetting( settings );
	mgr.setMzRangeSetting( settings );
	
	setThresholdSetting();
	setRtRangeSetting();
	setMzRangeSetting();
}


void NormalizationDialog::setThresholdSetting(){
	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	
	kome::plugin::SettingsPage* page = m_params.getPage( "PageThreshold" );
	
	if( page != NULL ){
		kome::objects::SettingParameterValues settings;
		page->setParameters( settings );
		
		const char* strVal = m_settings.getParameterValue( "threshold" );
		std::vector< std::string >tokens;
		stringtoken( strVal, ":", tokens );
		if( !tokens.empty() ){
			
			mgr.setThresholdSetting( settings );
		}
	}
}

void NormalizationDialog::setRtRangeSetting(){
	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	
	kome::plugin::SettingsPage* page = m_params.getPage( "PageRtRange" );

	if( page != NULL ){
		kome::objects::SettingParameterValues settings;
		page->setParameters( settings );
		
		const char* strVal = m_settings.getParameterValue( SET_VAL_RTRANGE );

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
			mgr.setRtRangeSetting( settings );
		}
	}
}

void NormalizationDialog::setMzRangeSetting(){
	// manager
	NormalizationManager& mgr = NormalizationManager::getInstance();
	
	kome::plugin::SettingsPage* page = m_params.getPage( "PageMzRange" );

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
			mgr.setMzRangeSetting( settings );
		}
	}
}

// get method name
const char* NormalizationDialog::getMethodName(){
	return m_method.c_str();
}

// get peak detection
const char* NormalizationDialog::getPeakDetect(){
	return m_peakDetect.c_str();
}

// get Peak Detect Settings
kome::objects::SettingParameterValues* NormalizationDialog::getPeakDetectSettings(){
	return m_peakDetectSettings;
}

// get standard group
kome::objects::DataGroupNode* NormalizationDialog::getStdGroup(){
	return m_stdGroup;
}

// get tretment group
kome::objects::DataGroupNode* NormalizationDialog::getTrmtGroup(){
	return m_trmtGroup;
}

// -----------------------------------------------
// class Normalization Help Getter
// -----------------------------------------------
BEGIN_EVENT_TABLE( NormalizHelpGetterPanel, kome::window::HelpGetterPanel )
END_EVENT_TABLE()

// constructor
NormalizHelpGetterPanel::NormalizHelpGetterPanel( wxWindow* parent )
	: kome::window::HelpGetterPanel( parent ){
	m_stdSmpCombo = NULL;
	m_trmtSmpCombo = NULL;

	createControls();
}

// destructor
NormalizHelpGetterPanel::~NormalizHelpGetterPanel(){
}

// create main sizer
wxSizer* NormalizHelpGetterPanel::createMainSizer(){
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );

	// group sizer
	wxFlexGridSizer* groupSizer = new wxFlexGridSizer( 2, 2, GRID_GAP, GRID_GAP );
	groupSizer->AddGrowableCol( 1 );
	
	groupSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "Standard Sample" ) ),
		0,
		wxLEFT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	m_stdSmpCombo = createSampleCombo( ID_CONTROL_SAMPLE_COMBO );
	wxSizer* comboSizer = new wxBoxSizer( wxHORIZONTAL );
	comboSizer->Add( m_stdSmpCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	groupSizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	groupSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "Treatment" ) ),
		0,
		wxLEFT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	m_trmtSmpCombo = createSampleCombo( ID_TREATMENT_SAMPLE_COMBO );
	comboSizer = new wxBoxSizer( wxHORIZONTAL );
	comboSizer->Add( m_trmtSmpCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	groupSizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );
		
	sizer->Add( groupSizer, 0, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// create group combo
wxComboBox* NormalizHelpGetterPanel::createSampleCombo( const int id ) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// groups
	std::vector< kome::objects::DataGroupNode* > groups;
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		for( unsigned int j = 0; j < sample->getNumberOfGroups(); j++ ) {
			kome::objects::DataGroupNode* group = sample->getGroup( j );
			if( group->getNumberOfChromatograms() > 0 ) {    // @2014/2/27  <Del> Y.Fujita   RT情報がないデータ(LCなしMALDI等)でも正規化の対象にする。
 				groups.push_back( group );
			}                                                // @2014/2/27  <Del> Y.Fujita
		}
	}

	// Spec No.86127 @date 2012.06.22 <Mod> M.Izumi
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
			
			groupNames[ i + 1 ] = wxT( name.c_str() );
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

// get select group
kome::objects::DataGroupNode* NormalizHelpGetterPanel::getSelGroup( const int id ){
	int isel = -1;
	if( id == ID_CONTROL_SAMPLE_COMBO ){
		isel = m_stdSmpCombo->GetSelection();
	
		return (kome::objects::DataGroupNode*)m_stdSmpCombo->GetClientData( isel );
	
	}else if( id == ID_TREATMENT_SAMPLE_COMBO ){
		isel = m_trmtSmpCombo->GetSelection();
		
		return (kome::objects::DataGroupNode*)m_trmtSmpCombo->GetClientData( isel );
	}
	
	return NULL;
}

// get description
std::string NormalizHelpGetterPanel::getDescription( wxWindow* wnd ){
	std::string s;

	if( wnd == m_stdSmpCombo ){
		s = "Select the standard sample for normalization from opened samples in [Sample Tree]. ";
	}else if( wnd = m_trmtSmpCombo ){
		s = "Select the target sample for normalization from opened samples in [Sample Tree]. ";
	}

	return s;
}
