/**
 * @file GenelateChromatogramDialog.cpp
 * @brief implements of GenelateChromatogramDialog class
 * @author T.Okuno
 * @date 2012.02.24
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "GenelateChromatogramDialog.h"
#include "ChromatogramOperationManager.h"

#include <wx/artprov.h>
#include <wx/utils.h>

using namespace kome::operation::sel_range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE				 3
#define ID_OPERAT_RADIO_BOX		 1

#define DLG_TITLE				"Generate Chromatogram"
#define RANGE_PARAM_NAME		"range"
#define GCD_TYPE_MC				"MC"
#define GCD_TYPE_BPC			"BPC"
#define GCD_TYPE_PROCES			"Processed MC"
#define MZ						"m/z"
#define MZTOL					"Tolerance"

//--------------------------------------------------------------------------------------------
// Genelate Chromatogram dialog
//--------------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( GenelateChromatogramDialog, kome::window::ParametersAndHelpDialog )
END_EVENT_TABLE()

// constructor
GenelateChromatogramDialog::GenelateChromatogramDialog( wxWindow* parent, kome::objects::DataGroupNode* group, bool menuFlg )
		: kome::window::ParametersAndHelpDialog( parent, "" ), m_params( NULL, true ), m_menuFlg( menuFlg ) {
	
	m_group = group;
	m_startPos = -1.0;
	m_endPos   = -1.0;
	m_flg      = false;
  
	m_helpPanel = NULL;
	if( m_menuFlg ){
		// initialization
		setWindowSettings();
	}
}

// destructor
GenelateChromatogramDialog::~GenelateChromatogramDialog(){
}

// create main sizer
wxSizer* GenelateChromatogramDialog::createMainSizer(){
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	if( m_group == NULL ) {
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
			
	kome::plugin::SettingsValue* val;	
	kome::plugin::SettingsParam* param;
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	kome::plugin::SettingsForm* form;

	// set dialog title 
	SetTitle( DLG_TITLE );

	m_helpPanel = new GenelateChromHelpGetterDlg( this );
	sizer->Add( m_helpPanel, 0, wxGROW | wxALL, 0 );
	
	if( m_menuFlg ){
		// m/z
		val = m_params.addValue();
		val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
		val->getNumberRestriction().setInclusiveMinValue( 0.0 );
		val->setRequired( true );
				
		param = m_params.addParam();
		param->setName( MZ );
		param->setValue( val );
	
		form = page->addForm();
		form->setParam( param );
		form->setTitle( "m/z" );
		form->setDesc( "Set the center of m/z range." ); // @date 2014.01.16 <Add> M.Izumi

		// Tolerance
		param = m_params.addParam();
		param->setName( MZTOL );
		param->setValue( val );
	
		form = page->addForm();
		form->setParam( param );
		form->setTitle( "Tolerance" );
		form->setDesc( "Set the half-width of m/z range." ); // @date 2014.01.16 <Add> M.Izumi

		// panel
		kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, false );
		sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );
		
	}else{
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
		form->setDesc( "set the m/z range." ); // @date 2014.01.16 <Add> M.Izumi
		
		// panel
		kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, false );
		sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );
	}
	return sizer;
}

// create button sizer
wxSizer* GenelateChromatogramDialog::createButtonSizer() {
	int flg = wxOK | wxCANCEL;
	if( m_group == NULL ) {
		flg = wxOK;
	}

	return wxDialog::CreateButtonSizer( flg );
}

// get start position
double GenelateChromatogramDialog::getStartPos(){
	return m_startPos;
}

// set start position
void GenelateChromatogramDialog::setStartPos( double sPos ){
	m_startPos = sPos;
}

// get end position
double GenelateChromatogramDialog::getEndPos(){
	return m_endPos;
}

// set end position
void GenelateChromatogramDialog::setEndPos( double ePos ){
	m_endPos = ePos;
}

// set Spectrum
void GenelateChromatogramDialog::setSpec( kome::objects::Spectrum* spec ){
	m_group = spec->getGroup();
}

// get the title
const char* GenelateChromatogramDialog::getTitle() {
	return m_title.c_str();
}

// transfer data from window
bool GenelateChromatogramDialog::TransferDataFromWindow() {
	
	// initialize
	m_flg = false;

	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// member check
	if( m_group == NULL ) {
		return true;
	}

	// title
	int idx = m_helpPanel->m_operatRadioBox->GetSelection();
	m_title = m_helpPanel->m_operatRadioBox->GetString( idx ).c_str();

	// @date 213.07.11 <Mod> M.Izumi
	if( !m_menuFlg ){
		
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
			m_title.append( FMT( " [%s", vals[ 0 ].c_str() ) );
		}
		else {
			return false;
		}

		double ePos;
		if( vals.size() == 1 ) {
			m_endPos = sPos;
			m_title.append( "]" );
		}
		else if( isdouble( vals[ 1 ].c_str(), &ePos ) ) {
			m_endPos = ePos;
			m_title.append( FMT( "-%s]", vals[ 1 ].c_str() ) );
		}
		else {
			return false;
		}

		// @date 2012.04.19 <Add> M.Izumi ->
		// SPEC No.84552 Spectrum -> MC/BPC: out of range values are used, but not shown in the generated chro window title
		double min = 0.0;  
		double max = 0.0; 
		if( m_group != NULL ){
			for( unsigned int i = 0; i < m_group->getNumberOfSpectra(); i++ ) {
				kome::objects::Spectrum* spec = m_group->getSpectrum( i );
				const double tmpMin = spec->getMinX();
				const double tmpMax = spec->getMaxX();
				if( i == 0 || tmpMin < min ) {
					min = tmpMin;
				}
				if( i == 0 || tmpMax > max ) {
					max = tmpMax;
				}
			}
		}

		if( m_startPos < min || m_startPos > max ){
			m_startPos = min;
		}

		if( m_endPos < min || m_endPos > max ){
			m_endPos = max;
		}
	// @date 2013.09.12 <Add> M.Izumi ->
	}else{ 
		const char* strmz = m_settings.getParameterValue( "m/z" );
		const char* strtol = m_settings.getParameterValue( "Tolerance" );

		m_title.append( FMT( " (mz=%s, tol=%s)", strmz, strtol ) );

		double mz = double();
		isdouble( strmz, &mz );

		double tol = double();
		isdouble( strtol, &tol );

		m_startPos = mz-tol;
		m_endPos = mz+tol;
	}
	// @date 2013.09.12 <Add> M.Izumi <-

		
	// get param name 
	m_strParamName = m_helpPanel->getParamName();
	// default
	m_flg = true;
	return m_flg;
}

// transfer data to window
bool GenelateChromatogramDialog::TransferDataToWindow() {
	// check the member
	if( m_group == NULL ) {
		return true;
	}

	//  @date 2013.07.11 <Mod> M.Izumi 
	if( !m_menuFlg ){
		// set the range
		std::string range = FMT( "%.4f%s%.4f", m_startPos, RANGE_VALUES_DELIMITER, m_endPos );
		m_settings.setValue( RANGE_PARAM_NAME, range.c_str() );
	}
	// default
	return kome::window::StandardDialog::TransferDataToWindow();
}

// change position and style.
void GenelateChromatogramDialog::setWindowSettings(){
	POINT pos;
	GetCursorPos(&pos);
	
	SetPosition( wxPoint( pos.x , pos.y) );
	SetWindowStyle( wxSYSTEM_MENU | wxCAPTION );
}

// -----------------------------------------------
// -----------------------------------------------
BEGIN_EVENT_TABLE( GenelateChromHelpGetterDlg, kome::window::HelpGetterPanel )
END_EVENT_TABLE()


GenelateChromHelpGetterDlg::GenelateChromHelpGetterDlg( wxWindow* parent )
	: kome::window::HelpGetterPanel( parent ){
	m_operatRadioBox = NULL;

	createControls();
}

GenelateChromHelpGetterDlg::~GenelateChromHelpGetterDlg(){
}

wxSizer* GenelateChromHelpGetterDlg::createMainSizer(){
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	// @date 2011.09.27 <Mod> M.Izumi
	wxArrayString oprations;
	oprations.clear();
		
	oprations.Insert( GCD_TYPE_MC, 0 );	
	oprations.Insert( GCD_TYPE_BPC, 1 );
	oprations.Insert( GCD_TYPE_PROCES, 2 );	// @date 2012.10.03 <Add> M.Izumi
	//¦@Processed BPC‚ª•K—v‚Èê‡‚Í‚±‚±‚É’Ç‰Á

	m_operatRadioBox = new wxRadioBox(
		this,
		ID_OPERAT_RADIO_BOX,
		wxT(""),
		wxDefaultPosition,
		wxDefaultSize,
		oprations,
		0,
		wxRA_SPECIFY_COLS
	);
	
	sizer->Add( m_operatRadioBox, 0, wxGROW | wxALL, 0 );
	return sizer;
}

std::string GenelateChromHelpGetterDlg::getDescription( wxWindow* wnd ){

	// return value
	std::string s;

	// description
	if( wnd == m_operatRadioBox ) {
		s = "Set the chromatogram type.\nMC: Mass Chromatogram.\nBPC: Base Peak Chromatogram.";
	}
	
	return s;
}

std::string GenelateChromHelpGetterDlg::getParamName(){
	std::string s;
	// default
	if( !kome::window::HelpGetterPanel::TransferDataFromWindow() ) {
		return false;
	}

	int	index = m_operatRadioBox->GetSelection();
	
	s = m_operatRadioBox->GetString( index );
	return s;
}
