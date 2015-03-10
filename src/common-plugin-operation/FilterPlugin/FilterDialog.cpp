/**
 * @file FilterDialog.cpp
 * @brief implements of FilterDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "FilterDialog.h"


using namespace kome::filter;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP					3
#define BORDER_SIZE				    3

#define ID_DEFAULT                 99

#define FILTER_TYPE_NAME		  "XYDATA_FILTER"
#define FILTER_NAME				  "filter"

#define SECTION					  "XYData Filter"
#define FILTER_KEY				  "FILTER"



BEGIN_EVENT_TABLE( FilterDialog, kome::window::ParametersAndHelpDialog )
    EVT_BUTTON( ID_DEFAULT, FilterDialog::onDefault )
END_EVENT_TABLE()


// constructor
FilterDialog::FilterDialog(	wxWindow* parent ) 
		: kome::window::ParametersAndHelpDialog( parent, "Filter Waveform", "Filter" ), m_parameters( NULL, false ) {
	m_filter = NULL;
	m_filterSettings = NULL;
	m_panel = NULL;
}

// destructor
FilterDialog::~FilterDialog() {
}

// get filter
kome::plugin::PluginFunctionItem* FilterDialog::getFilter() {
	return m_filter;
}

// get filter settings
kome::objects::SettingParameterValues* FilterDialog::getFilterSettings() {
	return m_filterSettings;
}

// create main sizer
wxSizer* FilterDialog::createMainSizer() {
	// create sizer
	wxGridSizer* sizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );

	kome::plugin::SettingsValue* value = m_parameters.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( FILTER_TYPE_NAME );
	value->setRequired( true );

	kome::plugin::SettingsParam* param = m_parameters.addParam();
	param->setName( FILTER_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( FILTER_KEY );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Filter" );
	form->setDetail( true );

	// panel
	m_panel = new kome::window::PluginSelectionPanel(
		this,
		wxID_ANY,
		form,
		&m_settings,
		true,
		kome::window::PluginSelectionPanel::TYPE_CHOICE_BOOK
	);
	sizer->Add( m_panel, 1, wxGROW | wxALL, BORDER_SIZE );

	return sizer;
}

// create button sizer
wxSizer* FilterDialog::createButtonSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// default button
	wxButton* button = new wxButton( this, ID_DEFAULT, wxT( "Default" ) );
	sizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT , BORDER_SIZE );

	// OK, Cancel
	wxSizer* buttonSizer = kome::window::StandardDialog::createButtonSizer();
	sizer->Add( buttonSizer, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );

	return sizer;
}

// transfer data from window
bool FilterDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::ParametersAndHelpDialog::TransferDataFromWindow() ) {
		return false;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// baseline detector
	const char* baseline = m_settings.getParameterValue( FILTER_NAME );
	m_filter = plgMgr.getFunctionItem( FILTER_TYPE_NAME , baseline );
	m_filterSettings = m_settings.getSubParameterValues( FILTER_NAME, baseline );
	
	// SPEC No.89229：Waveform filter 拡張で落ちる (NULLチェック追加)@date 2012.12.13 <Mod> M.Izumi
	kome::plugin::SettingsPage* page = m_filter->getSettingsPage();
	if( page != NULL ){
		// SPEC:84105 I can set it more than the upper limit of the set point.  @date 2012.01.23 <Add> M.Izumi
		int inum = page->getNumberOfForms();
		for( int i=0; i < inum; i++ ){
			kome::plugin::SettingsForm* form = page->getForm(i);
			if( form == NULL ){
				return false;
			}
			kome::plugin::SettingsParam* param = form->getParam();
			if( param == NULL ){
				return false;
			}
			kome::plugin::SettingsValue* val = param->getValue();
			if( val == NULL ){
				return false;
			}
			int paramNum = m_filterSettings->getNumberOfParameters();
			for( int j=0; j < paramNum; j++ ){
				std::string strName = m_filterSettings->getParameterName(j);
				if( strName.compare( param->getName() ) == 0 ){
					std::string strVal = m_filterSettings->getParameterValue( j );
					std::string  strMsg ="";
					if( strVal.empty() ){
						strMsg = val->checkValue( strVal.c_str(), param->getName() );
						kome::window::WindowTool::showInfo( strMsg.c_str() );
						return false;
					}
					double dval = atof( strVal.c_str() );
					double min = -1;
					double max = -1;
					bool bflg = false;		
								
					if( val->getType() == kome::plugin::SettingsValue::TYPE_INT ){
						min = val->getNumberRestriction().getInclusiveMinValue();
						max = val->getNumberRestriction().getInclusiveMaxValue(); 
										
						if( min > dval || dval > max ){
							strMsg = val->checkValue( strVal.c_str(), param->getName() );
							bflg = true;
						}

					}else if( val->getType() ==  kome::plugin::SettingsValue::TYPE_DOUBLE ){
						min = val->getNumberRestriction().getExclusiveMinValue( - FLT_MAX );
						if( dval <= min ){	// 0より大きい場合にOK（0はNG）	// SPEC 91676	// @date 2013/03/19 <Add> OKADA
							strMsg = val->checkValue( strVal.c_str(), param->getName() );
							bflg = true;
						}
					}

					if( bflg ){
						kome::window::WindowTool::showInfo( strMsg.c_str() );
						return false;
					}
				}
			}
		}
	}
	
	return true;
}

// on default
void FilterDialog::onDefault( wxCommandEvent& evt ) {
	// @date 2012.04.23 <Mod> M.Izumi
	// SPEC No.84106  A set point does not become the value of the default.
	if( m_panel != NULL ) {
		m_panel->setDefaultParameters(); // @date2014.03.03 <Mod> M.Izumi
	}
}
