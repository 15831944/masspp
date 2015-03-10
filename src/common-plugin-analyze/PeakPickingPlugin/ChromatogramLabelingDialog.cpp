/**
 * @file ChromatogramLabelingDialog.cpp
 * @brief implements of ChromatogramLabelingDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ChromatogramLabelingDialog.h"            
#include "PeakPickingManager.h"


using namespace kome::labeling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE							3
#define GRID_GAP							3

#define SECTION								"Auto Labeling"
#define PEAK_ALGORITHM_KEY					"CHROM_PEAK_ALGORITHM"

#define CHROMATOGRAM_PEAK_PICKING_FUNC_TYPE	"CHROM_PEAK_PICKING"

#define LABELING_NAME						"Peak Detector"

#define ID_TEST_BUTTON						 1


BEGIN_EVENT_TABLE( ChromatogramLabelingDialog, kome::window::ParametersAndHelpDialog )
	EVT_BUTTON( ID_TEST_BUTTON, ChromatogramLabelingDialog::onTestButton )
END_EVENT_TABLE()


// constructor
ChromatogramLabelingDialog::ChromatogramLabelingDialog(	wxWindow* parent )
		: kome::window::ParametersAndHelpDialog( parent, "Detect Peaks", "Detect" ),	// @date 2011/06/15 <Mod> "Chromatogram Peak Detection" ->  "Peak Detection"
			m_parameters( NULL, false ) {
   // initialize
	m_peakPickingFunction = NULL;
	m_peakPickingSettings = NULL;
}

// destructor
ChromatogramLabelingDialog::~ChromatogramLabelingDialog() {
}

// get peak picking function
kome::plugin::PluginFunctionItem* ChromatogramLabelingDialog::getPeakPickingFunction() {
	return m_peakPickingFunction;
}

// get peak picking settings
kome::objects::SettingParameterValues* ChromatogramLabelingDialog::getPeakPickingSettings() {
	return m_peakPickingSettings;
}

// create top sizer
wxSizer* ChromatogramLabelingDialog::createMainSizer() {
	// field sizer
	wxGridSizer* sizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );

	// peak detector
	kome::plugin::SettingsValue* value = m_parameters.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( CHROMATOGRAM_PEAK_PICKING_FUNC_TYPE );
	value->setRequired( true );

	kome::plugin::SettingsParam* param = m_parameters.addParam();
	param->setName( LABELING_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( PEAK_ALGORITHM_KEY );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Peak Detector" );
	form->setDesc( "Select the peak detection method."); // @date 2014.01.15 <Add> M.Izumi

	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );

	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// create test sizer
wxSizer* ChromatogramLabelingDialog::createButtonSizer() {
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

// transfer data from window
bool ChromatogramLabelingDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// set functions
	const char* peakDetector = m_settings.getParameterValue( LABELING_NAME );
	m_peakPickingFunction = plgMgr.getFunctionItem( CHROMATOGRAM_PEAK_PICKING_FUNC_TYPE, peakDetector );
	m_peakPickingSettings = m_settings.getSubParameterValues( LABELING_NAME, peakDetector );

	return true;
}

// on test button
void ChromatogramLabelingDialog::onTestButton( wxCommandEvent& evt ) {
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
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	if( chrom == NULL ) {
		return;
	}

	// peak
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.createPeaks( chrom );
	if( peaks == NULL ) {
		return;
	}

	// manager
	PeakPickingManager& mgr = PeakPickingManager::getInstance();

	// peak detection
	mgr.detectPeaks( chrom, chrom->getXYData(), m_peakPickingFunction, &m_settings );
	kome::window::WindowTool::refresh();
}
