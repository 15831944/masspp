/**
 * @file PluginSelectionPanel.cpp
 * @brief implements of PluginSelectionPanel class
 *
 * @author S.Tanaka
 * @date 2011.05.18
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "PluginSelectionPanel.h"

#include "WindowTool.h"
#include "HelpGetterPanel.h"
#include "SettingsPanel.h"
#include "SettingsPageDialog.h"

#include <wx/choicebk.h>



using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP               3
#define BORDER_SIZE            3

#define ID_SELECTOR           98
#define ID_PARAMS_BUTTON      99



BEGIN_EVENT_TABLE( PluginSelectionPanel, HelpGetterPanel )
    EVT_BUTTON( ID_PARAMS_BUTTON, PluginSelectionPanel::onParams )
	EVT_COMBOBOX( ID_SELECTOR, PluginSelectionPanel::onCombo )
END_EVENT_TABLE()


// constructor
PluginSelectionPanel::PluginSelectionPanel(
		wxWindow* parent,
		const int id,
		kome::plugin::SettingsForm* form,
		kome::objects::SettingParameterValues* settings,
		const bool titleFlg,
		PanelStyle style,
        const bool saveVal ) : HelpGetterPanel( parent, id ) {
	// initialize
	m_form = form;
	m_newSettings = false;
	m_settings = settings;
	m_newSettings = ( m_settings == NULL );
	if( m_newSettings ) {
		m_settings = new kome::objects::SettingParameterValues();
	}

	m_style = style;
	m_titleFlg = titleFlg;
	m_button = NULL;
	m_saveVal = saveVal;

	// control
	createControls();
}

// destructor
PluginSelectionPanel::~PluginSelectionPanel() {
	if( m_newSettings ) {
		delete m_settings;
		m_settings = NULL;
	}
}

// get selected index
int PluginSelectionPanel::getSelectedIndex() {
	// return value
	int idx = -1;

	// window
	wxWindow* wnd = FindWindow( ID_SELECTOR );
	if( wnd == NULL ) {
		return idx;
	}

	// get index
	if( m_style == TYPE_CHOICE_BOOK ) {
		wxChoicebook* book = (wxChoicebook*)wnd;
		idx = book->GetSelection();
	}
	else {
		wxComboBox* combo = (wxComboBox*)wnd;
		idx = combo->GetSelection();
	}

	return idx;
}

// process event
void PluginSelectionPanel::processEvent() {
	// parent
	SettingsPanel* panel = dynamic_cast< SettingsPanel* >( GetParent() );

	// event
	if( panel != NULL ) {
		panel->processEvent( m_form );
	}
}

// enumeration
void PluginSelectionPanel::updateEnum( const char* val ) {
	// selection
	int sel = getSelectedIndex();
	kome::plugin::PluginFunctionItem* selected = NULL;
	if( sel >= 0 ) {
		selected = m_items[ sel ];
	}

	// create combo
	getItems( val );
	createCombo();

	// select
	setItem( selected == NULL ? "" : selected->getShortName() );
}

// create main sizer
wxSizer* PluginSelectionPanel::createMainSizer() {
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// create sizer
	if( value->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {
		// get items
		getItems();

		if( m_style == TYPE_CHOICE_BOOK ) {
			createChoicebookSizer( sizer );
		}
		else {
			createComboSizer( sizer );
		}

		// enable
		if( m_items.size() == 0 ) {
			FindWindow( ID_SELECTOR )->Enable( false );
		}
	}

	return sizer;
}

// get item
kome::plugin::PluginFunctionItem* PluginSelectionPanel::getItem() {
	// index
	int idx = getSelectedIndex();

	// item
	return ( idx < 0 ? NULL : m_items[ idx ] );
}

// set item
void PluginSelectionPanel::setItem( const char* val ) {
	// string
	std::string v = NVL( val, "" );

	// index
	int idx = -1;
	for( unsigned int i = 0; i < m_items.size() && idx < 0; i++ ) {
		kome::plugin::PluginFunctionItem* item = m_items[ i ];
		if( item != NULL ) {
			if( v.compare( item->getShortName() ) == 0 ) {
				idx = (int)i;
			}
		}
	}
	if( idx < 0 ) {
		idx = 0;
	}

	// set selection
	wxWindow* wnd = FindWindow( ID_SELECTOR );

	if( m_style == TYPE_CHOICE_BOOK ) {
		wxChoicebook* book = (wxChoicebook*)wnd;
		book->SetSelection( idx );
	}
	else {
		wxComboBox* combo = (wxComboBox*)wnd;
		combo->SetSelection( idx );
	}
}

// get items
void PluginSelectionPanel::getItems( const char* val ) {
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );

	// clear
	m_items.clear();

	// none
	if( !value->isRequired() ) {
		m_items.push_back( NULL );
	}

	// items
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	std::vector< kome::plugin::SettingsValue::EnumItem > enumeration;
	value->getEnumeration( enumeration );

	for( unsigned int i = 0; i < enumeration.size(); i++ ) {
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( enumeration[ i ].section.c_str(), enumeration[ i ].value.c_str() );
		std::string section = item->getCall()->getType();
		if( item != NULL && ( val == NULL || section.compare( val ) == 0 ) ) {
			m_items.push_back( item );

			if( m_settings != NULL ) {
				kome::objects::SettingParameterValues* settings
					= m_settings->getSubParameterValues( param->getName(), item->getShortName() );

				kome::plugin::SettingsPage* page = item->getSettingsPage();
				if( page != NULL ) {
					page->setParameters( *settings, param->getPrefix() );
				}
			}
		}		
	}
}

// set value
void PluginSelectionPanel::setValue( const char* val ) {
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	if( value == NULL ) {
		return;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// item value & sub parameters
	std::string v = kome::plugin::SettingsValue::removeOuterBracket( val );
	if( v.empty() ) {
		return;
	}

	std::string itemName = v;
	std::string subParams;
	std::string::size_type pos = v.find( "[" );
	if( pos != v.npos ) {
		itemName = v.substr( 0, pos );
		subParams = v.substr( pos );
	}

	itemName = kome::plugin::SettingsValue::convertToDisplayingString( itemName.c_str() );
	
	// function type
	std::vector< std::string > funcTypes;
	kome::plugin::SettingsValue::separateListValue( value->getEnumKey(), funcTypes );

	if( value->getEnumType() != kome::plugin::SettingsValue::ENUM_FUNCTION || funcTypes.empty() ) {
		// @date 2012.08.23 <Add> M.Izumi ->
		// Spec No.86544 Mass++ does not remember the last peak detector used - it always resets to the default option
		// set item
		setItem( itemName.c_str() );
		
		// Set the state of the "Parameters" button.
		SetParametersButtonEnabledState(m_button);
		// @date 2012.08.23 <Add> M.Izumi <-
		return;
	}

	m_settings->setValue( param->getName(), itemName.c_str() );
	if( !subParams.empty() ) {
		kome::plugin::PluginFunctionItem* item = NULL;
		for( unsigned int i = 0; i < funcTypes.size() && item == NULL; i++ ) {
			item = plgMgr.getFunctionItem( funcTypes[ i ].c_str(), itemName.c_str() );
		}

		if( item != NULL ) {
			kome::objects::SettingParameterValues* subSettings = m_settings->getSubParameterValues( param->getName(), itemName.c_str() );
			kome::plugin::SettingsPage* page = item->getSettingsPage();
			if( page != NULL ) {
				page->setParameterString( *subSettings, subParams.c_str() );

				// set item
				setItem( itemName.c_str() );

				// set value
				if( m_style == TYPE_CHOICE_BOOK ) {
					wxWindow* wnd = FindWindow( ID_SELECTOR );
					wxChoicebook* book = (wxChoicebook*)wnd;

					kome::window::SettingsPanel* panel = dynamic_cast< kome::window::SettingsPanel* >( book->GetCurrentPage() );
					if( panel != NULL ) {
						panel->TransferDataToWindow();
					}
				}
			}
		}
	}
}

// set default parameters
void PluginSelectionPanel::setDefaultParameters() {
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	if( value == NULL ) {
		return;
	}

	// set values
	for( unsigned int i = 0; i < m_items.size(); i++ ) {
		kome::plugin::PluginFunctionItem* item = m_items[ i ];
		kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );
		if( page != NULL && m_settings != NULL ) {
			kome::objects::SettingParameterValues* settings = m_settings->getSubParameterValues( param->getName(), item->getShortName() );
			page->setParameters( *settings );
		}
	}

	// panels
	for( unsigned int i = 0; i < m_panels.size(); i++ ) {
		m_panels[ i ]->TransferDataToWindow();
	}

	SetParametersButtonEnabledState(m_button);
}

// create combo
void PluginSelectionPanel::createCombo() {
	if( m_style == TYPE_BUTTON ) {	// combobox
		wxComboBox* combo = dynamic_cast< wxComboBox* >( FindWindow( ID_SELECTOR ) );
		if( combo == NULL ) {
			return;
		}

		// clear
		combo->Clear();

		// add
		for( unsigned int i = 0; i < m_items.size(); i++ ) {
			kome::plugin::PluginFunctionItem* item = m_items[ i ];
			if( item == NULL ) {
				combo->Append( wxT( "(None)" ) );
			}
			else {
				combo->Append( wxT( item->getLongName() ) );
			}
		}
	}
}

// create combo sizer
void PluginSelectionPanel::createComboSizer( wxSizer* sizer ) {
	// check the parameter
	if( sizer == NULL ) {
		return;
	}

	// inner sizer
	wxSizer* innerSizer = new wxBoxSizer( wxHORIZONTAL );

	// title
	if( m_titleFlg ) {
		innerSizer->Add(
			new wxStaticText( this, wxID_ANY, wxT( m_form->getTitle() ) ),
			0,
			wxLEFT | wxALIGN_CENTER_VERTICAL,
			BORDER_SIZE
		);
	}

	// combo
	wxString* strs = NULL;
	if( m_items.size() > 0 ) {
		strs = new wxString[ m_items.size() ];
	}
	for( unsigned int i = 0; i < m_items.size(); i++ ) {
		kome::plugin::PluginFunctionItem* item = m_items[ i ];
		strs[ i ] = ( item == NULL ? "(None)" : item->getLongName() );
	}

	wxComboBox* combo = new wxComboBox(
		this,
		ID_SELECTOR,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		m_items.size(),
		strs,
		wxCB_READONLY
	);
	if( m_items.size() > 0 ) {
		combo->SetSelection( 0 );
	}

	innerSizer->Add( combo, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	if( strs != NULL ) {
		delete[] strs;
	}	

	// parameters button
	m_button = new wxButton( this, ID_PARAMS_BUTTON, wxT( "Parameters..." ) );
	innerSizer->Add( m_button, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, BORDER_SIZE );
	SetParametersButtonEnabledState(m_button);

	// sizer
	sizer->Add( innerSizer, 1, wxALL | wxGROW );
}

// create choicebook sizer
void PluginSelectionPanel::createChoicebookSizer( wxSizer* sizer ) {
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );

	// choice book
	wxChoicebook* book = new wxChoicebook( this, ID_SELECTOR );
	if( m_titleFlg ) {
		book->SetLabel( wxT( m_form->getTitle() ) );
	}
	sizer->Add( book, 1, wxALL | wxGROW, BORDER_SIZE );

	// add pages
	for( unsigned int i = 0; i < m_items.size(); i++ ) {
		kome::plugin::PluginFunctionItem* item = m_items[ i ];
		kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );

		if( page == NULL ) {
			HelpGetterPanel* panel = new HelpGetterPanel( book, wxID_ANY );
			book->AddPage( panel, wxT( item->getLongName() ), ( i == 0 ) );
		}
		else {
			kome::objects::SettingParameterValues* settings = NULL;
			if( m_settings != NULL ) {
				settings = m_settings->getSubParameterValues( param->getName(), item->getShortName() );
			}

			SettingsPanel* panel = new SettingsPanel( book, page, false, settings, true );
			book->AddPage( panel, wxT( item->getLongName() ), ( i == 0 ) );
			m_panels.push_back( panel );
		}
	}
}

// Validate
bool PluginSelectionPanel::Validate() {
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );

	// selected index
	int idx = getSelectedIndex();

	// check sub parameters
	bool ret = true;

	if( m_style == TYPE_CHOICE_BOOK ) {
		// choice book
		wxChoicebook* book = (wxChoicebook*)FindWindow( ID_SELECTOR );

		// set to default
		for( unsigned int i = 0; i < m_items.size(); i++ ) {
			if( i == idx ) {
				ret = book->GetPage( i )->Validate();
			}
			else {
				SettingsPanel* panel = dynamic_cast< SettingsPanel* >( book->GetPage( i ) );
				if( panel != NULL ) {
					panel->setDefaultValues();
				}
			}
		}
	}

	return ret;
}

// get help
std::string PluginSelectionPanel::getDescription( wxWindow* wnd ) {
	// description
	std::string ret = ( m_form == NULL ? "" : m_form->getDesc() );

	for( unsigned int i = 0; i < m_items.size(); i++ ) {
		if( m_items[ i ] != NULL ) {
			ret.append( "\n\n" );
			ret.append( FMT( "[%s]\n", m_items[ i ]->getLongName() ) );
			ret.append( m_items[ i ]->getDescription() );
		}
	}

	return ret;
}

// get image file path
std::string PluginSelectionPanel::getImageFilePath( wxWindow* wnd ) {
	std::string ret;
	return ret;
}

// data -> window
bool PluginSelectionPanel::TransferDataToWindow() {
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );

	// default
	if( !HelpGetterPanel::TransferDataToWindow() ) {
		return false;
	}

	// selected plug-in
	std::string val;
	if( m_settings != NULL && param != NULL ) {
		const char* v = m_settings->getParameterValue( param->getName() );
		val = NVL( v, "" );
	}

	// set item
	setItem( val.c_str() );

	// Set the state of the "Parameters" button.
	SetParametersButtonEnabledState(m_button);

	return true;
}

// window -> data
bool PluginSelectionPanel::TransferDataFromWindow() {
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );

	// selected index
	int idx = getSelectedIndex();

	// set value
	kome::plugin::PluginFunctionItem* item = ( idx < 0 ? NULL : m_items[ idx ] );
	if( m_settings != NULL ) {
		m_settings->setValue( param->getName(), ( item == NULL ? "none" : item->getShortName() ) );
	}

	// sub parameters
	bool ret = true;

	if( m_style == TYPE_CHOICE_BOOK  && idx >= 0 ) {
		wxChoicebook* book = (wxChoicebook*)FindWindow( ID_SELECTOR );
		ret = book->GetPage( idx )->TransferDataFromWindow();
	}

	return ret;
}

// on parameters button
void PluginSelectionPanel::onParams( wxCommandEvent& evt ) {
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );

	// get selection
	int idx = getSelectedIndex();
	if( idx < 0 ) {
		return;
	}

	// item
	kome::plugin::PluginFunctionItem* item = NULL;
	if( idx >= 0 ) {
		item = m_items[ idx ];
	}

	// page
	kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );
	if( page == NULL ) {
		return;
	}

	// prefix
	std::string prefix = param->getPrefix();

	// settings
	kome::objects::SettingParameterValues* settings = NULL;
	if( m_settings != NULL ) {
		settings = m_settings->getSubParameterValues( param->getName(), item->getShortName() );
	}

	// dialog
	SettingsPageDialog dlg( this, page, settings, m_saveVal, prefix.c_str() );
	if( dlg.ShowModal() == wxID_OK ) {
		if( dlg.isEdited() ) {
			processEvent();
		}
	}
}

// on combo
void PluginSelectionPanel::onCombo( wxCommandEvent& evt ) {
	// check the member
	if( m_style == TYPE_CHOICE_BOOK ) {
		return;
	}

	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );

	// button
	wxButton* button = (wxButton*)FindWindow( ID_PARAMS_BUTTON );
	if( button == NULL ) {
		return;
	}

	SetParametersButtonEnabledState(button);

	// process evet
	processEvent();
}

// sets the Parameters... button enabled state
void PluginSelectionPanel::SetParametersButtonEnabledState(wxButton* button)
{
	// check the parameter
	if( button == NULL ) {
		return;
	}

	// get selection
	int idx = getSelectedIndex();
	if( idx < 0 ) {
		return;
	}

	// item
	kome::plugin::PluginFunctionItem* item = NULL;
	if( idx >= 0 ) {
		item = m_items[ idx ];
	}

	// page
	kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );
/*
	// @date 2013.11.11 <Add> Ozaki >>>>>>
 	if  ( (kome::plugin::SettingsPage *)NULL != page && (kome::plugin::PluginFunctionItem *)NULL != item )
 	{
 	 	std::string	strCheck = tolowercase( item->getLongName( ) );
  
 	 	if  ( 0 == strCheck.compare( "mspeaks" ) )
 	 	{
 	 		page = (kome::plugin::SettingsPage *)NULL;
 	 	}
 	}
 	// @date 2013.11.11 <Add> Ozaki <<<<<<
*/		
	// set parameters button status
	if (button != NULL)
	{
		button->Enable( page != NULL );
	}
}
