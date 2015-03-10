/**
 * @file PeakDifferenceSearchDialog.cpp
 * @brief implements of PeakDifferenceSearchDialog class
 *
 * @author S.Tanaka
 * @date 2008.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakDifferenceSearchDialog.h"        
#include "PeakSearchDialog.h"	// @date 2011/08/26 <Add> OKADA

using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE					3
#define GRID_GAP					3
#define NUMBER_EDIT_WIDTH			75

#define ID_MZ_DIFF_TEXT				81
#define ID_MZ_TOL_TEXT				82


BEGIN_EVENT_TABLE( PeakDifferenceSearchDialog, PeakSearchDialog )
END_EVENT_TABLE()


// constructor
PeakDifferenceSearchDialog::PeakDifferenceSearchDialog( wxWindow* parent )
: PeakSearchDialog( parent ) {
	// initialize
	m_mzDiffText = NULL;
	m_mzTolText = NULL;

	m_mzDiff = -1.0;
	m_mzTol = -1.0;
}

// destructor
PeakDifferenceSearchDialog::~PeakDifferenceSearchDialog() {
}

// get m/z difference
double PeakDifferenceSearchDialog::getMzDiff() {
	return m_mzDiff;
}

// get m/z tolerance
double PeakDifferenceSearchDialog::getMzTol() {
	return m_mzTol;
}

// create main sizer
wxSizer* PeakDifferenceSearchDialog::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 1 );
	sizer->AddGrowableCol( 0 );

	// range sizer
	wxSizer* rangeSizer = createRangeSizer();
	sizer->Add( rangeSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	// @date 2011/05/27 <Add> OKADA
	// m/z sizer
	wxSizer* mzSizer = createMzSizer();
	sizer->Add( mzSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

wxSizer* PeakDifferenceSearchDialog::createMzSizer() {	// @date 2011/08/25 <Add> OKADA
	// create spectrum sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 2, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 1 );
	sizer->AddGrowableCol( 0 );

	// create spectrum sizer
	wxStaticBoxSizer* spectrumSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( DIFF_BOX_NAME ) );

	// page information
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setCommon( false );
	page->setName( "filter_peaks" );
		
	kome::plugin::SettingsValue* dblVal = m_params.addValue();
	dblVal->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	dblVal->getNumberRestriction().setInclusiveMinValue( 0.0 );
	dblVal->setRequired( false );

	kome::plugin::SettingsParam* param = m_params.addParam();
	kome::plugin::SettingsForm* form = page->addForm();

	param->setName( DIFF_PARAM_NAME );
	param->setValue( dblVal );
	
	form->setTitle( DIFF_PARAM_NAME );
	form->setParam( param );
	form->setDesc( "Set the differences of peaks." );

	param = m_params.addParam();
	form = page->addForm();
	param->setName( TOL_PARAM_NAME );
	param->setValue( dblVal );
	
	form->setTitle( TOL_PARAM_NAME );
	form->setParam( param );
	form->setDesc( "Set the tolerance of differences." );

	// panel
	kome::window::SettingsPanel* specPanel = new kome::window::SettingsPanel(
		this,
		page,
		false,
		&m_settings,
		false
	);
	spectrumSizer->Add( specPanel, 1, wxALL | wxGROW, BORDER_SIZE );
	sizer->Add( spectrumSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// transfer data from window
bool PeakDifferenceSearchDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// set range
	const char* range;
	range = m_settings.getParameterValue( DIFF_PARAM_NAME );
	// @date 2011.12.01 <Mod> M.Izumi
	if( strcmp( range, "" ) == 0 ){
		m_mzDiff = -1.0;
	}else{
		m_mzDiff = atof( range );
	}

	range = m_settings.getParameterValue( TOL_PARAM_NAME );
	// @date 2011.12.01 <Mod> M.Izumi
	if( strcmp( range, "" ) == 0 ){
		m_mzTol = -1.0;
	}else{
		m_mzTol = atof( range );
	}

	// @date 2011/05/27 <ADD> OKADA ------->
	// check input
	if(( m_mzDiff < 0.0 )&&( m_mzTol < 0.0 )){
		// Diff.‚ÆTol.‚Ì—¼•û‚ª–¢“ü—Í‚Ìê‡
		return PeakSearchDialog::TransferDataFromWindow();
	}
	// @date 2011/05/27 <ADD> OKADA <-------

	// check value
	if(  0.0 < m_mzDiff) {	// @date 2011/05/27 <ADD> OKADA
		;	
	}else{					// @date 2011/05/27 <ADD> OKADA
		// illegal value
		kome::window::WindowTool::showInfo( "m/z Difference must be a number greater than 0.0" );
		return false;
	}
	if( m_mzTol < 0.0 ) {
		kome::window::WindowTool::showInfo( "m/z Tolerance must be a number greater than or equal to 0.0" );
		return false;
	}

	// default
	return PeakSearchDialog::TransferDataFromWindow();
}
