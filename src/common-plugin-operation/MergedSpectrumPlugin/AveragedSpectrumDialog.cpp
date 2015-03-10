/**
 * @file AveragedSpectrumDialog.cpp
 * @brief implements of AveragedSpectrumDialog class
 * @author T.Okuno
 * @date 2012.02.28
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "AveragedSpectrumDialog.h"
#include "MergedSpectrumManager.h"

#include <wx/artprov.h>

using namespace kome::operation::sel_range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::operation::merged;

#define BORDER_SIZE							 3

#define CHROM_RANGE_ACTION_STATUS_NAME		"CHROMATOGRAM_RANGE_ACTION"
#define DLG_TITLE							"Average Spectrum"	// SPEC 85547 
#define RANGE_PARAM_NAME					"range"


//--------------------------------------------------------------------------------------------
// Averaged Spectrum dialog
//--------------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( AveragedSpectrumDialog, kome::window::StandardDialog )
END_EVENT_TABLE()

// constructor
AveragedSpectrumDialog::AveragedSpectrumDialog( wxWindow* parent, kome::objects::Chromatogram* chrom )
		: kome::window::StandardDialog( parent, "" ), m_params( NULL, true ) {
	m_chrom    = NULL;
	m_startPos = -1;
	m_endPos   = -1;
	m_flg      = false;
  
	// initialization
	setWindowSettings();
}

// destructor
AveragedSpectrumDialog::~AveragedSpectrumDialog(){
}

// create main sizer
wxSizer* AveragedSpectrumDialog::createMainSizer(){
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	if( (m_chrom == NULL) || (m_chrom->getGroup() == NULL) ){
		wxBoxSizer* innerSizer = new wxBoxSizer( wxHORIZONTAL );
		wxStaticBitmap* bitmap = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_WARNING ) );
		innerSizer->AddSpacer( 10 );
		innerSizer->Add( bitmap, wxALL | wxGROW, BORDER_SIZE );
		innerSizer->AddSpacer( 10 );
		innerSizer->Add( new wxStaticText( this, wxID_ANY, wxT( "This operation is unavailable from chromatograms in this window." ) ) );
		innerSizer->AddSpacer( 10 );

		sizer->AddSpacer( 10 );
		sizer->Add( innerSizer, wxALL | wxGROW, 20 );
		sizer->AddSpacer( 10 );

		return sizer;
	}
	
	// @date 2011.09.27 <Mod> M.Izumi
	wxArrayString oprations;
	oprations.clear();
	
	kome::plugin::SettingsValue* val;	
	kome::plugin::SettingsParam* param;
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	kome::plugin::SettingsForm* form;

	// get status value
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* tmp = statusMgr.getValue( CHROM_RANGE_ACTION_STATUS_NAME );
	std::string strStatus = NVL( tmp, "" );

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
	form->setTitle( "RT" );
	form->setDesc( "Set the RT range which contains spectra to be averaged." ); // @date 2014.01.16 <Add> M.Izumi

	// panel
	kome::window::SettingsHelpPanel* panel = new kome::window::SettingsHelpPanel( this, page, false, &m_settings, false );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );
	
	return sizer;
}

// create button sizer
wxSizer* AveragedSpectrumDialog::createButtonSizer() {
	int flg = wxOK | wxCANCEL;
	if( (m_chrom == NULL) || (m_chrom->getGroup() == NULL) ){
		flg = wxOK;
	}

	return wxDialog::CreateButtonSizer( flg );
}

// get start position
double AveragedSpectrumDialog::getStartPos(){
	return m_startPos;
}

// set start position
void AveragedSpectrumDialog::setStartPos( double sPos ){
	m_startPos = sPos;
}

// get end position
double AveragedSpectrumDialog::getEndPos(){
	return m_endPos;
}

// set end position
void AveragedSpectrumDialog::setEndPos( double ePos ){
	m_endPos = ePos;
}

// set Chromatogram
void AveragedSpectrumDialog::setChrom( kome::objects::Chromatogram* chrom ){
	m_chrom = chrom;
}

// transfer data from window
bool AveragedSpectrumDialog::TransferDataFromWindow() {
	// initialize
	m_flg = false;

	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// member check
	if( (m_chrom == NULL) || (m_chrom->getGroup() == NULL) ){
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

	// default
	m_flg = true;
	return m_flg;
}

// transfer data to window
bool AveragedSpectrumDialog::TransferDataToWindow() {
	// check the member
	if( (m_chrom == NULL) || (m_chrom->getGroup() == NULL) ){
		return true;
	}

	// set the range
	std::string range = FMT( "%.4f%s%.4f", m_startPos, RANGE_VALUES_DELIMITER, m_endPos );
	m_settings.setValue( RANGE_PARAM_NAME, range.c_str() );

	// default
	return kome::window::StandardDialog::TransferDataToWindow();
}

// change position and style.
void AveragedSpectrumDialog::setWindowSettings(){
	POINT pos;
	GetCursorPos(&pos);
	
	SetPosition( wxPoint( pos.x , pos.y) );
	SetWindowStyle( wxSYSTEM_MENU | wxCAPTION );
}

