/**
 * @file BaselineSubtractDialog.cpp
 * @brief implements of BaselineSubtractDialog class
 *
 * @author S.Tanaka
 * @date 2007.02.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BaselineSubtractDialog.h"

#include "BaselineSubtractManager.h"


using namespace kome::baseline;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP				 3
#define BORDER_SIZE				 3

#define ID_DRAW_BUTTON			 1
#define ID_ERASE_BUTTON			 2
#define ID_APPLY_BUTTON			 3
#define ID_DEFAULT_BUTTON		 4

#define BASELINE_NAME			 "baseline"
#define BASELINE_TYPE_NAME		 "XYDATA_BASELINE"

#define SECTION					 "Baseline Subtraction"
#define BASELINE_KEY			 "BASELINE_ALGORITHM"

bool bflg = false;				// 2010.10.01 Add M.Izumi
bool bstatflg = false;			// 2010.10.01 Add M.Izumi
bool g_flgValueOK = true;		// @date 2013/03/18 <Add> OKADA	// SPEC 91677

BEGIN_EVENT_TABLE( BaselineSubtractDialog, kome::window::ParametersAndHelpDialog )
    EVT_KOME_SETTINGS_PANEL( wxID_ANY, BaselineSubtractDialog::onSettingsPanel )
	EVT_BUTTON( ID_DEFAULT_BUTTON, BaselineSubtractDialog::onDefault )
	EVT_BUTTON( ID_DRAW_BUTTON, BaselineSubtractDialog::onDraw )	// [Apply] button
	EVT_BUTTON( ID_ERASE_BUTTON, BaselineSubtractDialog::onErase )
END_EVENT_TABLE()


// constructor
BaselineSubtractDialog::BaselineSubtractDialog(	wxWindow* parent ) 
		: kome::window::ParametersAndHelpDialog( parent, "Baseline Subtract", "Subtract" ), m_parameters( NULL, false ) {
	m_baselineDetector = NULL;
	m_baselineSettings = NULL;

	m_panel = NULL;	// @date 2011/07/27 <Add> OKADA
	m_flgReDraw = 0;// @date 2011/07/27 <Add> OKADA
}

// destructor
BaselineSubtractDialog::~BaselineSubtractDialog() {
	if( m_panel != NULL ){
		delete m_panel;
		m_panel = NULL;
	}
}

// get baseline detector
kome::plugin::PluginFunctionItem* BaselineSubtractDialog::getBaselineDetector() {
	return m_baselineDetector;
}

// get baseline settings
kome::objects::SettingParameterValues* BaselineSubtractDialog::getBaselineSettings() {
	return m_baselineSettings;
}

// create main sizer
wxSizer* BaselineSubtractDialog::createMainSizer() {
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );

	kome::plugin::SettingsValue* value = m_parameters.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( BASELINE_TYPE_NAME );
	value->setRequired( true );

	kome::plugin::SettingsParam* param = m_parameters.addParam();
	param->setName( BASELINE_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( BASELINE_KEY );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Baseline" );
	form->setDetail( true );	// @date 2011/07/25 <Add> OKADA

	// panel
	if( m_panel != NULL ){
		delete m_panel;
	}
	m_panel = new kome::window::PluginSelectionPanel(
		this,
		wxID_ANY,
		form,
		&m_settings,
		true,
		kome::window::PluginSelectionPanel::TYPE_CHOICE_BOOK
	);
	sizer->Add( m_panel, 1, wxGROW | wxALL, BORDER_SIZE );

	TransferDataToWindow();	// @date 2011/09/26 <Add> OKADA

	drawBaseline();	// @date 2011/07/22 <Add> OKADA

	return sizer;
}

// transfer data from window
bool BaselineSubtractDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// baseline detector
	const char* baseline = m_settings.getParameterValue( BASELINE_NAME );
	m_baselineDetector = plgMgr.getFunctionItem( BASELINE_TYPE_NAME , baseline );
	m_baselineSettings = m_settings.getSubParameterValues( BASELINE_NAME, baseline );

	// SPEC 91677: 空欄のままOK出来ないようにする
	unsigned int uNum = m_baselineSettings->getNumberOfParameters();
	g_flgValueOK = true;
	for( unsigned int i=0; i<uNum; i++ ){	// @date 2013/06/10 <Mod> OKADA
		const char * pszVal = m_baselineSettings->getParameterValue( i );
		if( strlen( pszVal ) <= 0 ){
			g_flgValueOK = false;
			break;
		}
	}

	return true;
}

// TransferDataToWindow
bool BaselineSubtractDialog::TransferDataToWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataToWindow() ) {
		return false;
	}

	return true;
}


// onDefault
void BaselineSubtractDialog::onDefault( wxCommandEvent& evt ) {
	// @date 2014.03.03 <Add> M.Izumi ->
	if( m_panel != NULL ){
		m_panel->setDefaultParameters();
	}
	// @date 2014.03.03 <Add> M.Izumi <-
	return;
}

// on draw
void BaselineSubtractDialog::onDraw( wxCommandEvent& evt ) {
	if( m_flgReDraw != 0 ){
		// パラメータ変更された
		ProcDraw();		// 2010.10.01 Add M.Izumi
		bflg = true;
	}

	m_flgReDraw = 0;

}

// on erase
void BaselineSubtractDialog::onErase( wxCommandEvent& evt ) {	
	ProcErase();	// 2010.10.01 Add M.Izumi
	bflg = false;
}

bool BaselineSubtractDialog::onOK() {

	// SPEC 91677: 空欄のままOK出来ないようにする
	if( g_flgValueOK ){
		;	// OK
	}else{
		return false;	// NG
	}

	if( bflg ){			// @date 2011/07/27 <Add> OKADA
		ProcErase();	// @date 2011/07/27 <Add> OKADA
		bflg = false;	// @date 2011/07/27 <Add> OKADA
	}					// @date 2011/07/27 <Add> OKADA

	return kome::window::StandardDialog::onOK();
}

bool BaselineSubtractDialog::onCancel(){

	if( bflg ){			// @date 2011/07/27 <Add> OKADA
		ProcErase();	// @date 2011/07/27 <Add> OKADA
		bflg = false;	// @date 2011/07/27 <Add> OKADA
	}					// @date 2011/07/27 <Add> OKADA

	return kome::window::StandardDialog::onCancel();
}

// proc draw
void BaselineSubtractDialog::ProcDraw(){
	// transfer data from window
	if( !TransferDataFromWindow() ) {
		return;
	}

	// active object
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();

	kome::objects::MsDataVariant obj;
	if( frame != NULL ) {
		obj.setVariant( frame->getActiveObject() );
	}

	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	// check
	if( spec == NULL && chrom == NULL ) {
		LOG_ERROR( FMT("There is no active 2D data.") );
		return;
	}

	// manager
	BaselineSubtractManager& mgr = BaselineSubtractManager::getInstance();

	// set draw
	if( spec != NULL ) {
		mgr.setDrawBaseline( spec, m_baselineDetector, m_baselineSettings );
	}
	if( chrom != NULL ) {
		mgr.setDrawBaseline( chrom, m_baselineDetector, m_baselineSettings );
	}

	// refresh
	kome::window::WindowTool::refresh();
}

// proc Erase
void  BaselineSubtractDialog::ProcErase( ) {
	// active object
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();

	kome::objects::MsDataVariant obj;
	if( frame != NULL ) {
		obj.setVariant( frame->getActiveObject() );
	}

	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	// check
	if( spec == NULL && chrom == NULL ) {
		LOG_ERROR( FMT("There is no active 2D data.") );
		return;
	}

	// manager
	BaselineSubtractManager& mgr = BaselineSubtractManager::getInstance();
	if( spec != NULL ) {
		mgr.eraseBaseline( spec );
	}
	if( chrom != NULL ) {
		mgr.eraseBaseline( chrom );
	}

	// refresh
	kome::window::WindowTool::refresh();
}

// draw baseline
void BaselineSubtractDialog::drawBaseline() {
	ProcDraw();
	bflg = true;
}

// create button sizer
wxSizer* BaselineSubtractDialog::createButtonSizer() {
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 4, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );

	// button
	wxFlexGridSizer* drawSizer = new wxFlexGridSizer( 1, 4, GRID_GAP, GRID_GAP );

	// default button
	wxButton* button = new wxButton( this, ID_DEFAULT_BUTTON, wxT( "Default" ) );
	sizer->Add( button, 0, wxALIGN_BOTTOM | wxALIGN_RIGHT | wxRIGHT , BORDER_SIZE );

	// [Apply] button
	drawSizer->Add(
		new wxButton( this, ID_DRAW_BUTTON, wxT( "Apply" ) ),
		1,
		wxALIGN_BOTTOM | wxALIGN_RIGHT | wxRIGHT,
		BORDER_SIZE
	);
	sizer->Add( drawSizer, 1, wxALIGN_BOTTOM | wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );
	
	// OK, Cancel
	wxSizer* buttonSizer = kome::window::StandardDialog::createButtonSizer();
	sizer->Add( buttonSizer, 2, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );

	return sizer;
}

// on settings panel
void BaselineSubtractDialog::onSettingsPanel( kome::window::SettingsPanelEvent& evt ) {
	// form
	kome::plugin::SettingsForm* form = evt.getForm();
	if( form == NULL ) {
		return;
	}

	m_flgReDraw = 1;

}
