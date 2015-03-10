/**
 * @file SpectrumLabelingDialog.cpp
 * @brief implements of SpectrumLabelingDialog class
 *
 * @author S.Tanaka
 * @date 2006.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectrumLabelingDialog.h"            
#include "PeakPickingManager.h"


using namespace kome::labeling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE							3
#define GRID_GAP							3
#define ID_IMPORT_BUTTON					10
#define ID_EXPORT_BUTTON					20

#define SECTION								"Auto Peak Picking"
#define PEAK_ALGORITHM_KEY					"SPEC_PEAK_ALGORITHM"
#define CHARGE_FLAG_KEY						"CHARGE_FLAG"
#define CHARGE_DETECTOR_KEY					"CHARGE_DETECTOR"

#define SPECTRUM_PEAK_PICKING_FUNC_TYPE		"SPEC_PEAK_PICKING"
#define CHARGE_DETECT_FUNC_TYPE				"CHARGE_DETECT"

#define LABELING_NAME						"Peak Detector"
#define CHARGE_NAME							"Charge Detector"
#define DEFAULT_SPEC_PEAK_DETECT			"local"

#define PEAK_FILTERING						"Append Peak Filter"

#define ID_TEST_BUTTON						 1



BEGIN_EVENT_TABLE( SpectrumLabelingDialog, kome::window::ParametersAndHelpDialog )
	EVT_BUTTON( ID_TEST_BUTTON, SpectrumLabelingDialog::onTestButton )	
END_EVENT_TABLE()


// constructor
SpectrumLabelingDialog::SpectrumLabelingDialog(
		wxWindow* parent,
		kome::objects::Spectrum* spec
) : kome::window::ParametersAndHelpDialog(
		parent,
		"Detect Peaks",
		"Detect"
), m_parameters( NULL, false ) {
   // initialize
	m_target = PeakPickingManager::TARGET_UNKNOWN;
	m_peakPickingFunction = NULL;
	m_chargeDetectFunction = NULL;
	m_peakPickingSettings = NULL;
	m_chargeDetectSettings = NULL;

	m_spec = spec;

	m_simple = false;

	m_helpGetter = NULL;
}

SpectrumLabelingDialog::SpectrumLabelingDialog( wxWindow* parent )
		: kome::window::ParametersAndHelpDialog( parent, "Detect Peaks", "Detect" ),
		m_parameters( NULL, false ) {
   // initialize
	m_target = PeakPickingManager::TARGET_UNKNOWN;
	m_peakPickingFunction = NULL;
	m_chargeDetectFunction = NULL;
	m_peakPickingSettings = NULL;
	m_chargeDetectSettings = NULL;

	m_spec = NULL;

	m_simple = true;
	m_helpGetter = NULL;
}

// destructor
SpectrumLabelingDialog::~SpectrumLabelingDialog() {
}

// get peak picking target
PeakPickingManager::PeakPickingTarget SpectrumLabelingDialog::getTarget() {
	return m_target;
}

// get peak picking function
kome::plugin::PluginFunctionItem* SpectrumLabelingDialog::getPeakPickingFunction() {
	return m_peakPickingFunction;
}

// get charge detect function
kome::plugin::PluginFunctionItem* SpectrumLabelingDialog::getChargeDetectFunction() {
	return m_chargeDetectFunction;
}

// get peak picking settings
kome::objects::SettingParameterValues* SpectrumLabelingDialog::getPeakPickingSettings() {
	return m_peakPickingSettings;
}

// get charge detect settings
kome::objects::SettingParameterValues* SpectrumLabelingDialog::getChargeDetectSettings() {
	return m_chargeDetectSettings;
}

// get peak filtering param
const char* SpectrumLabelingDialog::getPeakFilterParam(){
	return m_peakFilter.c_str();
}

// create top sizer
wxSizer* SpectrumLabelingDialog::createMainSizer() {
	if( m_simple ) {
		wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
		sizer->Add( createPeakSizer(), 1, wxALL | wxGROW, BORDER_SIZE );
		sizer->Add( createChargeSizer(), 1, wxALL | wxGROW, BORDER_SIZE );
		return sizer;
	}

	// field sizer
	wxFlexGridSizer* fieldSizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	fieldSizer->AddGrowableCol( 0 );
	fieldSizer->AddGrowableCol( 1 );
	fieldSizer->AddGrowableRow( 0 );
	
	// help getter panel
	m_helpGetter = new SpectrumLabelHelpGetterPanel( this, m_spec );
	
	// create target box
	fieldSizer->Add(
		m_helpGetter,
		1,
		wxGROW | wxALL,
		BORDER_SIZE
	);

	// right sizer
	wxFlexGridSizer* rightSizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	rightSizer->AddGrowableCol( 0 );
	rightSizer->AddGrowableRow( 0 );
	rightSizer->AddGrowableRow( 1 );
	fieldSizer->Add( rightSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	// create peak sizer
	rightSizer->Add(
		createPeakSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
	);

	// create charge sizer
	rightSizer->Add(
		createChargeSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
	);
	
	// create Peak Filtering Sizer
	rightSizer->Add(
		createPeakFilteringSizer(),
		1,
		wxGROW | wxALL,
		BORDER_SIZE
	);
	return fieldSizer;
}

// create test sizer
wxSizer* SpectrumLabelingDialog::createButtonSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// test button
	wxButton* button = new wxButton(
		this,
		ID_TEST_BUTTON,
		wxT( "Apply" )
	);

	sizer->Add(
		button,
		0,
		wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT,
		BORDER_SIZE 
	);

	// default sizer
	sizer->Add(
		kome::window::StandardDialog::createButtonSizer(),
		1,
		wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxGROW,
		BORDER_SIZE
	);

	// cancel->close
	button = dynamic_cast< wxButton* >( FindWindow( wxID_CANCEL ) );
	if( button != NULL ) {
		button->SetLabel( wxT( "Close" ) );
	}

	return sizer;
}

// create peak sizer
wxSizer* SpectrumLabelingDialog::createPeakSizer() {
	// create sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// get peak pickig algorithms
	PeakPickingManager& mgr = PeakPickingManager::getInstance();

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );

	// peak detector
	kome::plugin::SettingsValue* value = m_parameters.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( SPECTRUM_PEAK_PICKING_FUNC_TYPE );
	value->setDefaultValue( DEFAULT_SPEC_PEAK_DETECT );//デフォルトセット @date 2012.04.17 <Add> M.Izumi
	value->setRequired( true );

	kome::plugin::SettingsParam* param = m_parameters.addParam();
	param->setName( LABELING_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( PEAK_ALGORITHM_KEY );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Peak Detector" );
	form->setDesc( "Select the peak detection method." ); // @date 2014.01.23 <Add> M.Izumi

	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	
	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// create peak sizer
wxSizer* SpectrumLabelingDialog::createChargeSizer() {
	// create sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// get charge detect algorithms
	PeakPickingManager& mgr = PeakPickingManager::getInstance();

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );

	// charge detector
	kome::plugin::SettingsValue* value = m_parameters.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setDefaultValue("defaultcharge");
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( CHARGE_DETECT_FUNC_TYPE );
	value->setRequired( true );

	kome::plugin::SettingsParam* param =  m_parameters.addParam();
	param->setName( CHARGE_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( CHARGE_DETECTOR_KEY );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Charge Detector" ); 
	form->setDesc("Select the charge calculation method.");

	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// create peak filtering sizer
wxSizer* SpectrumLabelingDialog::createPeakFilteringSizer(){
	// create sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );
	page->setName( "peak filtering" );
	page->setTitle( "Append Peak Filter" );

	// peak search
	kome::plugin::SettingsValue* val = m_parameters.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_LIST );

	kome::plugin::SettingsValue* subVal = m_parameters.addValue();
	subVal->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	subVal->setEnumKey( "spec_peak_filter" );
	subVal->setName( "Method" );
	subVal->setRequired( true );
	val->addChildValue( subVal );
	val->setRequired( false );

	kome::plugin::SettingsParam* param = m_parameters.addParam();
	param->setName( PEAK_FILTERING );
	param->setValue( val );
	param->setSection( "Peak_Filter" );
	param->setKey( "filter" );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Peak Filtering" );
	form->setDesc( "Set the peak filter." );
	
	form->setParam( param );
	// set menu Item @date 2012.12.06 <Add> M.Izumi
	form->setEditMenuItem( true );
	form->setEditMenuName( "Edit" );
	form->setAddMenuItem( true );
	form->setAddMenuName( "New" );
	form->setUpDownMenuItem( true );
	form->setSelectAllMenuItem( true );
	form->setDeleteMenuItem( true );
	form->setSaveAllMenuItem( true );
	form->setCopyMenuItem( false );
	form->setFilterMenuItem( false );

	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, true, &m_settings, true );
	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// get peak picking target
PeakPickingManager::PeakPickingTarget SpectrumLabelingDialog::getPeakPickingTarget() {
	// target
	PeakPickingManager::PeakPickingTarget targets[] = {
		PeakPickingManager::TARGET_CURRENT_SPECTRUM,
		PeakPickingManager::TARGET_MS_SPECTRA,
		PeakPickingManager::TARGET_MSMS_SPECTRA,
		PeakPickingManager::TARGET_FILTERED_SPECTRA,
		PeakPickingManager::TARGET_ALL_SPECTRA
	};
	int targetSize = (int)( sizeof( targets ) / sizeof( PeakPickingManager::PeakPickingTarget ) );

	int sel = 0;
	if( m_helpGetter == NULL ) {
		if( m_spec != NULL ) {
			sel = 1;
		}
	}
	else {
		sel = m_helpGetter->getTargetSel();
		if( m_helpGetter->getCount() < targetSize ) {
			sel++;
		}
		sel = CLAMP( sel, 0, targetSize - 1 );
	}
	
	return targets[ sel ];
}

// transfer data from window
bool SpectrumLabelingDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// target
	m_target = getPeakPickingTarget();

	// set functions
	const char* peakDetector = m_settings.getParameterValue( LABELING_NAME );
	m_peakPickingFunction = plgMgr.getFunctionItem( SPECTRUM_PEAK_PICKING_FUNC_TYPE, peakDetector );
	m_peakPickingSettings = m_settings.getSubParameterValues( LABELING_NAME, peakDetector );

	const char* chargeDetector = m_settings.getParameterValue( CHARGE_NAME );
	m_chargeDetectFunction = plgMgr.getFunctionItem( CHARGE_DETECT_FUNC_TYPE, chargeDetector );
	m_chargeDetectSettings = m_settings.getSubParameterValues( CHARGE_NAME, chargeDetector );

	// set peak filter
	const char* filter = m_settings.getParameterValue( PEAK_FILTERING );
	m_peakFilter = NVL( filter, "" );

	return true;
}

// on test button
void SpectrumLabelingDialog::onTestButton( wxCommandEvent& evt ) {
	// transfer
	if( !TransferDataFromWindow() ) {
		return;
	}

	// get active data map
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame == NULL ) {
		return;
	}

	kome::objects::MsDataVariant obj( frame->getActiveObject() );
	kome::objects::Spectrum* spec = obj.getSpectrum();
	if( spec == NULL ) {
		return;
	}

	// peak
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.createPeaks( spec );
	if( peaks == NULL ) {
		return;
	}

	// manager
	PeakPickingManager& mgr = PeakPickingManager::getInstance();

	// peak detection
	mgr.detectPeaks( spec, spec->getXYData(), m_peakPickingFunction, &m_settings, NULL, NULL );
	kome::plugin::PluginCallTool::filterSpecPeaks( &spec, &peaks, 1, m_peakFilter.c_str() );
	kome::window::WindowTool::refresh();
}

// -------------------------------------------------------------------------
// class spectrum label help getter panel
// -------------------------------------------------------------------------
BEGIN_EVENT_TABLE( SpectrumLabelHelpGetterPanel, kome::window::HelpGetterPanel )
END_EVENT_TABLE()

// constructor
SpectrumLabelHelpGetterPanel::SpectrumLabelHelpGetterPanel( wxWindow* parent, kome::objects::Spectrum* spec )
	: kome::window::HelpGetterPanel( parent ){

	m_targetBox = NULL;
	m_spec = spec;

	createControls();
}

// destructor
SpectrumLabelHelpGetterPanel::~SpectrumLabelHelpGetterPanel(){
}

// create main sizer
wxSizer* SpectrumLabelHelpGetterPanel::createMainSizer(){
	// create sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// create the array
	std::vector< wxString > items;

	if( m_spec != NULL ) {
		items.push_back( wxT( "Current Spectrum" ) );
	}
	items.push_back( wxT( "MS Spectra" ) );
	items.push_back( wxT( "MS/MS Spectra" ) );
	items.push_back( wxT( "Filtered Spectra" ) );
	items.push_back( wxT( "All Spectra" ) );

	// create radio box
	m_targetBox = new wxRadioBox(
		this,
		wxID_ANY,
		wxT( "Target" ),
		wxDefaultPosition,
		wxDefaultSize,
		items.size(),
		&( items[ 0 ] ),
		0,
		wxRA_SPECIFY_ROWS
	);
	m_targetBox->SetSelection( 0 );
	sizer->Add( m_targetBox, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// get description
std::string SpectrumLabelHelpGetterPanel::getDescription( wxWindow* wnd ){
	std::string s;

	if( wnd == m_targetBox ){
		s = "Select the target spectra to detect peaks.";
	}

	return s;
}
