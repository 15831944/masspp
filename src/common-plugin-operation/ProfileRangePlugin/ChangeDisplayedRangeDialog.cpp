/**
 * @file ChangeDisplayedRangeDialog.cpp
 * @brief implements of ChangeDisplayedRangeDialog class
 * @author T.Okuno
 * @date 2012.02.28
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ChangeDisplayedRangeDialog.h"
#include "ProfileRangeManager.h"

#include <wx/artprov.h>


using namespace kome::operation::sel_range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::operation::range;


#define BORDER_SIZE				 3

#define SPECTRUM_RANGE_ACTION_STATUS_NAME	"SPECTRUM_RANGE_ACTION"
#define CHROM_RANGE_ACTION_STATUS_NAME		"CHROMATOGRAM_RANGE_ACTION"
#define DLG_TITLE							"Change Displayed Range"
#define RANGE_PARAM_NAME					"range"


//--------------------------------------------------------------------------------------------
// Change Displayed Range dialog
//--------------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( ChangeDisplayedRangeDialog, kome::window::ParametersAndHelpDialog )
	EVT_BUTTON( wxID_OK, ChangeDisplayedRangeDialog::onOkButton )
END_EVENT_TABLE()

// constructor
ChangeDisplayedRangeDialog::ChangeDisplayedRangeDialog( wxWindow* parent, kome::objects::Spectrum* spec )
		: kome::window::ParametersAndHelpDialog( parent, "" ), m_params( NULL, true ) {
	m_spec     = NULL;
	m_chrom    = NULL;
	m_canvas   = NULL;
	m_startPos = -1;
	m_endPos   = -1;
	m_flg      = false;

	m_nDialogType = ProfileRangeManager::DIALOG_MZ;
  
	// initialization
	setWindowSettings();
}
ChangeDisplayedRangeDialog::ChangeDisplayedRangeDialog( wxWindow* parent, kome::objects::Chromatogram* chrom )
		: kome::window::ParametersAndHelpDialog( parent, "" ), m_params( NULL, true ) {
	m_spec     = NULL;
	m_chrom    = NULL;
	m_startPos = -1;
	m_endPos   = -1;
	m_flg      = false;
	m_nDialogType = ProfileRangeManager::DIALOG_RT;
  
	// initialization
	setWindowSettings();
}

// destructor
ChangeDisplayedRangeDialog::~ChangeDisplayedRangeDialog(){
}

// create main sizer
wxSizer* ChangeDisplayedRangeDialog::createMainSizer(){
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	if( ((m_spec == NULL) || (m_spec->getGroup() == NULL)) &&
		((m_chrom == NULL) || (m_chrom->getGroup() == NULL)) ){
		wxBoxSizer* innerSizer = new wxBoxSizer( wxHORIZONTAL );
		wxStaticBitmap* bitmap = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_WARNING ) );
		innerSizer->AddSpacer( 10 );
		innerSizer->Add( bitmap, wxALL | wxGROW, BORDER_SIZE );
		innerSizer->AddSpacer( 10 );
		
		if( m_nDialogType == ProfileRangeManager::DIALOG_MZ ){
			innerSizer->Add( new wxStaticText( this, wxID_ANY, wxT( "This operation is unavailable from spectra in this window." ) ) );
		}
		else{
			innerSizer->Add( new wxStaticText( this, wxID_ANY, wxT( "This operation is unavailable from chromatograms in this window." ) ) );
		}
		innerSizer->AddSpacer( 10 );

		sizer->AddSpacer( 10 );
		sizer->Add( innerSizer, wxALL | wxGROW, 20 );
		sizer->AddSpacer( 10 );

		return sizer;
	}
			
	kome::plugin::SettingsValue* val;	
	kome::plugin::SettingsParam* param;
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	kome::plugin::SettingsForm* form;
	
	// set dialog title 
	SetTitle( DLG_TITLE );

	// m/z
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE );
	val->getNumberRestriction().setInclusiveMinValue( 0.0 );
	val->setRequired( true );
	
	param = m_params.addParam();
	param->setName( RANGE_PARAM_NAME );
	param->setValue( val );
	form = page->addForm();
	form->setParam( param );
	form->setTitle( (m_nDialogType == ProfileRangeManager::DIALOG_MZ ? "m/z" : "RT") );
	form->setDesc( "Set the displayed range." ); // @date 2014.01.16 <Add> M.Izumi
		
	// panel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, false );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );
	
	return sizer;
}

// create button sizer
wxSizer* ChangeDisplayedRangeDialog::createButtonSizer() {
	int flg = wxOK | wxCANCEL;
	if( ((m_spec == NULL) || (m_spec->getGroup() == NULL)) &&
		((m_chrom == NULL) || (m_chrom->getGroup() == NULL)) ){
		flg = wxOK;
	}

	return wxDialog::CreateButtonSizer( flg );
}

// get start position
double ChangeDisplayedRangeDialog::getStartPos(){
	return m_startPos;
}

// set start position
void ChangeDisplayedRangeDialog::setStartPos( double sPos ){
	m_startPos = sPos;
}

// get end position
double ChangeDisplayedRangeDialog::getEndPos(){
	return m_endPos;
}

// set end position
void ChangeDisplayedRangeDialog::setEndPos( double ePos ){
	m_endPos = ePos;
}

// set Spectrum
void ChangeDisplayedRangeDialog::setSpec( kome::objects::Spectrum* spec ){
	m_spec = spec;
}

// set Chromatogram
void ChangeDisplayedRangeDialog::setChrom( kome::objects::Chromatogram* chrom ){
	m_chrom = chrom;
}

// set canvas
void ChangeDisplayedRangeDialog::setCanvas( wxWindow* canvas )
{
	m_canvas = canvas;
}

// transfer data from window
bool ChangeDisplayedRangeDialog::TransferDataFromWindow() {
	// initialize
	m_flg = false;

	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// member check
	if( ((m_spec == NULL) || (m_spec->getGroup() == NULL)) &&
		((m_chrom == NULL) || (m_chrom->getGroup() == NULL)) ){
		return true;
	}

	// range
	const char* range = m_settings.getParameterValue( RANGE_PARAM_NAME );
	if( range == NULL ) {

		return false;
	}

	std::vector< std::string > vals;
	stringtoken( range, RANGE_VALUES_DELIMITER, vals );
	if( vals.size() == 0 ) {
		return false;
	}

	double sPos;
	if( isdouble( vals[ 0 ].c_str(), &sPos ) ) {
		m_startPos = sPos;
	}
	else {
		return false;
	}

	double ePos;
	if( vals.size() == 1 ) {
		m_endPos = sPos;
	}
	else if( isdouble( vals[ 1 ].c_str(), &ePos ) ) {
		m_endPos = ePos;
	}
	else {
		return false;
	}
		
	// @date 2012.04.19 <Add> M.Izumi ->
	// SPEC No.85739 指定された範囲でスペクトルが表示される
	double min = 0.0;  
	double max = 0.0;  
	kome::core::XYData* xydata;
	if( m_spec != NULL ){		
		min = m_spec->getMinX();
		max = m_spec->getMaxX();
	}

	if( m_chrom != NULL ){
		xydata = m_chrom->getXYData();
		if( xydata != NULL ){
			min = xydata->getMinX();
			max = xydata->getMaxX();
		}
	}
	if( m_startPos < min || m_startPos > max ){
		m_startPos = min;
	}

	if( m_endPos < min || m_endPos > max ){
		m_endPos = max;
	}
	//<-

	// default
	m_flg = true;
	return m_flg;
}

// transfer data to window
bool ChangeDisplayedRangeDialog::TransferDataToWindow() {
	// check the member
	if( ((m_spec == NULL) || (m_spec->getGroup() == NULL)) &&
		((m_chrom == NULL) || (m_chrom->getGroup() == NULL)) ){
		return true;
	}

	// set the range
	std::string range = FMT( "%.4f%s%.4f", m_startPos, RANGE_VALUES_DELIMITER, m_endPos );
	m_settings.setValue( RANGE_PARAM_NAME, range.c_str() );

	// default
	return kome::window::StandardDialog::TransferDataToWindow();
}

// change position and style.
void ChangeDisplayedRangeDialog::setWindowSettings(){
	POINT pos;
	GetCursorPos(&pos);
	
	SetPosition( wxPoint( pos.x , pos.y) );
	SetWindowStyle( wxSYSTEM_MENU | wxCAPTION );
}

// OK
void ChangeDisplayedRangeDialog::onOkButton( wxCommandEvent& evt )
{
	// default
	kome::window::StandardDialog::onOkButton( evt );

	// refresh
	kome::window::WindowTool::refresh();
}
