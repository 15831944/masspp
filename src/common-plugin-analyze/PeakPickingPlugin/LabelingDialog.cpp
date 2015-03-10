/**
 * @file LabelingDialog.cpp
 * @brief implements of LabelingDialog class
 *
 * @author S.Tanaka
 * @date 2007.10.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LabelingDialog.h"      
#include "Peaks2dManager.h"


using namespace kome::labeling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE							3
#define GRID_GAP							3

#define LABELING_NAME						"Labeling"
#define CLUSTERING_NAME						"Clustering"
#define LABELING_FUNC_NAME					"2D_PEAK_PICKING"

#define SECTION								"2D Labeling"
#define LABELING_PARAM_NAME					"PEAK_DETECTOR"
#define DEFAULT_CLUSTERING					"ab3d"

#define ID_TEST_BUTTON						 1


BEGIN_EVENT_TABLE( LabelingDialog, kome::window::ParametersAndHelpDialog )
	EVT_BUTTON( ID_TEST_BUTTON, LabelingDialog::onTestButton )
	EVT_UPDATE_UI( ID_TEST_BUTTON, LabelingDialog::onUpdateTestButton )
END_EVENT_TABLE()


// constructor
LabelingDialog::LabelingDialog( wxWindow* parent ) 
		: kome::window::ParametersAndHelpDialog( parent, "Detect 2D Peaks", "Detect" ), m_parameters( NULL, false ) {
	m_peakDetector = NULL;
	m_peakSettings = NULL;
}

// destructor
LabelingDialog::~LabelingDialog() {
}

// get peak detector
kome::plugin::PluginFunctionItem* LabelingDialog::getPeakDetector() {
	return m_peakDetector;
}

// get peak settings
kome::objects::SettingParameterValues* LabelingDialog::getPeakSettings() {
	return m_peakSettings;
}

// create top sizer
wxSizer* LabelingDialog::createMainSizer() {
	// sizer
	wxGridSizer* sizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );

	// labeling
	kome::plugin::SettingsValue* value = m_parameters.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( LABELING_FUNC_NAME );
	value->setRequired( true );

	kome::plugin::SettingsParam* param = m_parameters.addParam();
	param->setName( LABELING_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( LABELING_PARAM_NAME );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Peak Detector" );
	form->setDesc( "Select the peak detection method." );
	
	// panel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	sizer->Add( panel, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// create test sizer
wxSizer* LabelingDialog::createButtonSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// test button
	wxButton* button = new wxButton(
		this,
		ID_TEST_BUTTON,
		wxT( "Test" )
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

	return sizer;
}

// transfer data from window
bool LabelingDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// peak detector
	const char* peakDetector = m_settings.getParameterValue( LABELING_NAME );
	m_peakDetector = plgMgr.getFunctionItem( LABELING_FUNC_NAME, peakDetector );
	m_peakSettings = m_settings.getSubParameterValues( LABELING_NAME, peakDetector );

	return true;
}

// on test button
void LabelingDialog::onTestButton( wxCommandEvent& evt ) {
	// transfer
	if( !TransferDataFromWindow() ) {
		return;
	}

	// get active data map
	kome::objects::MsDataVariant obj;
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		obj.setVariant( frame->getActiveObject() );
	}

	kome::objects::DataMapInfo* dataMap = obj.getDataMap();
	if( dataMap == NULL ) {
		return;
	}
	kome::objects::DataGroupNode* group = &( dataMap->getGroup() );

	// peak
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = pkMgr.createPeaks( group );
	if( peaks == NULL ) {
		return;
	}

	// progress
	kome::window::DialogProgress prgs( kome::window::WindowTool::getMainWindow(), "Detect 2D Peaks" );

	// peak detection
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	mgr.detectPeaks(
		*group,
		*group,
		*peaks,
		m_peakDetector,
		m_peakSettings,
		prgs,
		dataMap->getMinRt(),
		dataMap->getMaxRt(),
		dataMap->getMinMz(),
		dataMap->getMaxMz()
	);
}

// on update UI test button
void LabelingDialog::onUpdateTestButton( wxUpdateUIEvent& evt ) {
	// check enable
	bool enable = false;
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		kome::objects::MsDataVariant obj( frame->getActiveObject() );
		kome::objects::DataMapInfo* dataMap = obj.getDataMap();

		if( dataMap != NULL ) {
			enable = true;
		}
	}

	evt.Enable( enable );
}
