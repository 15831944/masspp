/**
 * @file MagnifyDialog.cpp
 * @brief implements of MagnifyDialog class
 * @author T.Okuno
 * @date 2012.02.28
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MagnifyDialog.h"
#include "MagnifyingViewManager.h"
#include "MagnifyingViewOperation.h"

#include <wx/artprov.h>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE				 3

#define ID_CLEAR				 1

#define DEFAULT_SCALE			10

#define SPECTRUM_RANGE_ACTION_STATUS_NAME	"SPECTRUM_RANGE_ACTION"
#define DLG_TITLE							"Magnify"
#define RANGE_PARAM_NAME					"range"
#define SCALE_PARAM_NAME					"scale"


//--------------------------------------------------------------------------------------------
// Change Displayed Range dialog
//--------------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( MagnifyDialog, kome::window::ParametersAndHelpDialog )
	EVT_BUTTON( wxID_OK, MagnifyDialog::onOkButton )
    EVT_BUTTON( ID_CLEAR, MagnifyDialog::onClear )
END_EVENT_TABLE()

// constructor
MagnifyDialog::MagnifyDialog( wxWindow* parent, kome::objects::Spectrum* spec )
		: kome::window::ParametersAndHelpDialog( parent, "" ), m_params( NULL, true ) {
	m_id       = -1;
	m_startPos = -1;
	m_endPos   = -1;
	m_scale    = DEFAULT_SCALE;
	m_spec     = NULL;
	m_flg      = false;
	m_btnClear = NULL;
  
	// initialization
	setWindowSettings();
}

// destructor
MagnifyDialog::~MagnifyDialog(){
}

// create main sizer
wxSizer* MagnifyDialog::createMainSizer(){
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	if( (m_spec == NULL) || (m_spec->getGroup() == NULL) ){
		wxBoxSizer* innerSizer = new wxBoxSizer( wxHORIZONTAL );
		wxStaticBitmap* bitmap = new wxStaticBitmap( this, wxID_ANY, wxArtProvider::GetBitmap( wxART_WARNING ) );
		innerSizer->AddSpacer( 10 );
		innerSizer->Add( bitmap, wxALL | wxGROW, BORDER_SIZE );
		innerSizer->AddSpacer( 10 );
		innerSizer->Add( new wxStaticText( this, wxID_ANY, wxT( "This operation is unavailable from spectra in this window." ) ) );
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
	form->setTitle( "m/z" );
	form->setDesc( "Set the m/z range of the magnifying view." );

	// Scale
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_INT );
	val->getNumberRestriction().setInclusiveMinValue( 1.0 );
	val->getNumberRestriction().setInclusiveMaxValue( 100.0 );
	val->setRequired( true );
	
	param = m_params.addParam();
	param->setName( SCALE_PARAM_NAME );
	param->setValue( val );
	
	form = page->addForm();
	form->setParam( param );
	form->setTitle( "Scale" );
	form->setDesc( "Set the scale of the magnifying view." );	

	// panel
	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, false );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );
	
	return sizer;
}

// create button sizer
wxSizer* MagnifyDialog::createButtonSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// Clear button
	m_btnClear = new wxButton( this, ID_CLEAR, wxT( "Clear" ) );
	if( m_btnClear != NULL ){
		m_btnClear->Enable( (m_id >= 0 ? true : false) );
	}
	sizer->Add( m_btnClear, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT , BORDER_SIZE );

	// OK, Cancel
	wxSizer* buttonSizer = kome::window::StandardDialog::createButtonSizer();
	sizer->Add( buttonSizer, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );

	return sizer;
}

// get start position
double MagnifyDialog::getStartPos(){
	return m_startPos;
}

// set start position
void MagnifyDialog::setStartPos( double sPos ){
	m_startPos = sPos;
}

// get end position
double MagnifyDialog::getEndPos(){
	return m_endPos;
}

// set end position
void MagnifyDialog::setEndPos( double ePos ){
	m_endPos = ePos;
}

// get scale
int MagnifyDialog::getScale(){
	return m_scale;
}

// set scale
void MagnifyDialog::setScale( int scale ){
	m_scale = scale;
}

// get ID
int MagnifyDialog::getID(){
	return m_id;
}

// set ID
void MagnifyDialog::setID( int id ){
	m_id = id;
	if( m_btnClear != NULL ){
		m_btnClear->Enable( (m_id >= 0 ? true : false) );
	}
}

kome::objects::Spectrum* MagnifyDialog::getSpec(){
	return m_spec;
}

// set Spectrum
void MagnifyDialog::setSpec( kome::objects::Spectrum* spec ){
	m_spec = spec;
}

// transfer data from window
bool MagnifyDialog::TransferDataFromWindow() {
	// initialize
	m_flg = false;

	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// member check
	if( (m_spec == NULL) || (m_spec->getGroup() == NULL) ){
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

	// scale
	const char* szScale = m_settings.getParameterValue( SCALE_PARAM_NAME );
	if( szScale == NULL ) {
		return false;
	}

	int nScale;
	if( isint( szScale, 10, &nScale ) ) {
		m_scale = nScale;
	}
	else {
		return false;
	}
	
	// @date 2012.04.20 <Add> M.Izumi ->
	double min = 0.0;  
	double max = 0.0; 
	if( m_spec != NULL ){		
		min = m_spec->getMinX();
		max = m_spec->getMaxX();
	}
	// Spec No.85741 @date 2012.05.15 <Add> M.Izumi ->
	bool bStartFlg = false;
	if(  m_startPos < min || m_startPos > max ){
		bStartFlg = true;
	}
	bool bEndFlg = false;
	if( m_endPos < min ||  m_endPos > max ){
		bEndFlg = true;
	}

	if( bStartFlg && bEndFlg ){
		return false;
	}
	//<-
	if( m_startPos < min || m_startPos > max ){
		m_startPos = min;
	}

	if( m_startPos > m_endPos || m_endPos > max ){
		m_endPos = max;
	}
	// <-
	
	// default
	m_flg = true;
	return m_flg;
}

// transfer data to window
bool MagnifyDialog::TransferDataToWindow() {
	// check the member
	if( (m_spec == NULL) || (m_spec->getGroup() == NULL) ){
		return true;
	}

	// set the range
	std::string range = FMT( "%.4f%s%.4f", m_startPos, RANGE_VALUES_DELIMITER, m_endPos );
	m_settings.setValue( RANGE_PARAM_NAME, range.c_str() );

	// scale
	std::string scale = FMT( "%d", m_scale );
	m_settings.setValue( SCALE_PARAM_NAME, scale.c_str() );

	// default
	return kome::window::StandardDialog::TransferDataToWindow();
}

// change position and style.
void MagnifyDialog::setWindowSettings(){
	POINT pos;
	GetCursorPos(&pos);
	
	SetPosition( wxPoint( pos.x , pos.y) );
	SetWindowStyle( wxSYSTEM_MENU | wxCAPTION );
}

// OK button
void MagnifyDialog::onOkButton( wxCommandEvent& evt )
{
	// default
	kome::window::StandardDialog::onOkButton( evt );

	if( (m_spec == NULL) || (m_spec->getGroup() == NULL) || !m_flg ){
		// Spec No.85741 @date 2012.05.15 <Add> M.Izumi ->
		kome::window::WindowTool::showInfo( "The spectrum does not contain any data within the specified m/z range." );
		// <-
		return;
	}

	// refresh
	kome::window::WindowTool::refresh();
}

// Clear button
void MagnifyDialog::onClear( wxCommandEvent& evt )
{
	// check button state
	if( m_btnClear != NULL ){
		if( m_btnClear->IsEnabled() == false ){
			return;
		}
	}

	// operation 
	MagnifyingViewOperation* op = new MagnifyingViewOperation();
	op->setSpectrum( m_spec );
	op->setScaleIndex( m_id );
	op->setScale( -1.0 );
	op->setShortName("magnify_view");
	op->setOperationName("Magnifying View");
	op->setOperationType( kome::operation::Operation::TYPE_GUI );
	op->setTargetSample( m_spec->getSample() );

	if( op->execute() ) {
		kome::window::WindowTool::refresh();
	}
	else {
		delete op;
	}
		
	EndModal( wxCANCEL );
}
