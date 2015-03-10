/**
 * @file MzRangeFilterDialog.cpp
 * @brief interfaces of MzRangeFilterDialog class
 *
 * @author M.Izumi
 * @date 2013.01.08
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "MzRangeFilterDialog.h"
#include "SpectraCheckList.h"

#define GRID_GAP					3
#define BORDER_SIZE				    3


using namespace kome::mzfilter;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


BEGIN_EVENT_TABLE( MzRangeFilterDialog, kome::window::ParametersAndHelpDialog )    
END_EVENT_TABLE()

// constructor
MzRangeFilterDialog::MzRangeFilterDialog( wxWindow* parent, kome::objects::Variant& activeObj )
	: kome::window::ParametersAndHelpDialog( parent, "m/z Range Filter" ), m_params( NULL, false ){
	m_sample = NULL;
	m_spec = NULL;
	m_strMzRange = "";
	m_list = NULL;
	
	// active object
	kome::objects::MsDataVariant obj( activeObj );
	m_spec = obj.getSpectrum();

}

// destructor
MzRangeFilterDialog::~MzRangeFilterDialog(){
}

// create main sizer
wxSizer* MzRangeFilterDialog::createMainSizer(){
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );
	
	kome::window::CheckGridListPanel* listpanel = new kome::window::CheckGridListPanel( this );
	listpanel->setDescription("Select target spectra.");

	m_list = new SpectraCheckList( listpanel, m_spec );
	
	m_list->create();
	// select active spectrum
	int isel = m_list->getSelectIndex();
	if( isel != -1 ){
		kome::window::ListTool::selectItem( m_list, isel );
		wxRect rect;
		if( m_list->GetSubItemRect( isel, 0, rect ) ) {
			m_list->ScrollList( 0, rect.y - rect.height );
		}
	}else{
		m_list->selectAll();
	}	

	sizer->Add( listpanel, 1, wxALL | wxGROW, BORDER_SIZE );

	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	kome::plugin::SettingsValue* val = m_params.addValue();

	val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE );
	val->getNumberRestriction().setInclusiveMinValue( 0.0 );
	val->setRequired( true );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setValue( val );
	param->setSection( "m/z Range Filter" );
	param->setName("mzRangefilter");
	param->setKey("MZ_RANGE_FILTER");

	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "m/z" );
	form->setDesc( "Set the m/z range." );

	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );
	return sizer;
}

// create button sizer
wxSizer* MzRangeFilterDialog::createButtonSizer(){
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// button sizer
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

	buttonSizer->Add( new wxButton( this, wxID_OK, wxT( "Filter" ) ) );
	buttonSizer->Add( new wxButton( this, wxID_CANCEL, wxT( "Close" ) ) );

	sizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );

	return sizer;
}

// get check spectra
void MzRangeFilterDialog::getCheckSpectra( std::vector< kome::objects::Spectrum* >& spectra ){
	int inum = m_list->getNumberOfData();
	for( int i=0; i < inum; i++ ){
		if( m_list->isSelected( i ) ){
			kome::objects::Spectrum* spec = m_list->getSelSpectrum( i );
			if( spec != NULL ){
				spectra.push_back( spec );
			}
		}
	}
}

// transfer data from window
bool MzRangeFilterDialog::TransferDataFromWindow() {
		
	// default
	if( !StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	m_strMzRange = m_settings.getParameterValue( "mzRangefilter" );
	
	// SPEC No.91670: m/z Range Filterダイアログで初期表示の全選択以外確認ダイアログが表示されて処理が出来ない
	// @date 2013.03.18 <Mod> M.Izumi
	std::vector< kome::objects::Spectrum* > tmpSpectra;
	getCheckSpectra( tmpSpectra );
		
	if( tmpSpectra.empty() ){		
		kome::window::WindowTool::showInfo( "No items are selected. Select at least one item." );
		return false;
	}

	return true;
}

// get m/z range
const char* MzRangeFilterDialog::getMzRange(){
	return m_strMzRange.c_str();
}
