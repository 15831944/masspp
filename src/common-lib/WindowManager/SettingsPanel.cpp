/**
 * @file SettingsPanel.cpp
 * @brief implements of SettingsPanel class
 *
 * @author S.Tanaka
 * @date 2007.02.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsPanel.h"

#include "WindowTool.h"
#include "SettingsPageDialog.h"
#include "ListTool.h"
#include "PluginSelectionPanel.h"
#include "ListEditPanel.h"
#include "RawDataFileDialog.h"
#include "NumberTextCtrl.h"
#include "EMailTextCtrl.h"
#include "ParameterTitlePanel.h"
#include "FontCanvas.h"
#include "FontTool.h"

#include <math.h>
#include <float.h>

#include <wx/spinctrl.h>
#include <wx/colordlg.h>
#include <wx/listctrl.h>
#include <wx/fontdlg.h>
#include <wx/generic/fontdlgg.h>

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define CLUSTER_PARAM              12
#define LIST_PARAM                  4
#define GRID_GAP                    3
#define BORDER_SIZE                 3
#define MIN_COMBO_SIZE              2
#define INDENT_SIZE                10
#define INTEGER_EDIT_WIDTH         75
#define NUMBER_EDIT_WIDTH         100
#define STRING_EDIT_WIDTH         150
#define FILE_EDIT_WIDTH           350
#define COLOR_WIDTH                45

#define MIN_CONTROL_ID	         1001
#define MAX_CONTROL_ID           4000
#define MIN_PARAM_BUTTON_ID	     4001
#define MAX_PARAM_BUTTON_ID	     5000

#define DEFAULT_BUTTON_ID		  999


BEGIN_EVENT_TABLE( SettingsPanel, HelpGetterPanel )
	EVT_BUTTON( DEFAULT_BUTTON_ID, SettingsPanel::onDefaultButton )
	EVT_CUSTOM_RANGE( wxEVT_SCROLL_THUMBTRACK, MIN_CONTROL_ID, MAX_CONTROL_ID, SettingsPanel::onSpin )
	EVT_CUSTOM_RANGE( wxEVT_COMMAND_TEXT_UPDATED, MIN_CONTROL_ID, MAX_CONTROL_ID, SettingsPanel::onText )
	EVT_CUSTOM_RANGE( wxEVT_COMMAND_COMBOBOX_SELECTED, MIN_CONTROL_ID, MAX_CONTROL_ID, SettingsPanel::onCombo )
	EVT_CUSTOM_RANGE( wxEVT_COMMAND_CHECKBOX_CLICKED, MIN_CONTROL_ID, MAX_CONTROL_ID, SettingsPanel::onCombo )
	EVT_CUSTOM_RANGE( wxEVT_COMMAND_BUTTON_CLICKED, MIN_PARAM_BUTTON_ID, MAX_PARAM_BUTTON_ID, SettingsPanel::onParamButton )
END_EVENT_TABLE()


// constructor
SettingsPanel::SettingsPanel(
		wxWindow* parent,
		kome::plugin::SettingsPage* page,
		const bool dfltBtn,
		kome::objects::SettingParameterValues* settings,
		const bool saveVal
) : HelpGetterPanel( parent ), m_saveVal( saveVal ),
		m_page( page ), m_dfltBtn( dfltBtn ) {
	// initialize
	m_settings = settings;
	m_newSettings = ( m_settings == NULL );
	if( m_newSettings ) {
		m_settings = new kome::objects::SettingParameterValues();
	}

	if( m_settings->getNumberOfParameters() == 0 ) {
		page->setParameters( *m_settings );
	}

	m_edited = false;
	m_evtFlg = true;

	m_newId = 0;

	// create controls
	createControls();
}

// destructor
SettingsPanel::~SettingsPanel() {
	if( m_newSettings ) {
		m_newSettings = false;
		delete m_settings;
		m_settings = NULL;
	}
}

// set edited flag
void SettingsPanel::setEdited( const bool edited ) {
	m_edited = edited;
}

// get edited flag state
bool SettingsPanel::isEdited() {
	return m_edited;
}

// create groups
void SettingsPanel::createGroups( std::vector< GroupInfo >& groups ) {
	// check the member
	if( m_page == NULL ) {
		return;
	}

	// previous group
	kome::plugin::SettingsGroup* prevGroup = NULL;

	// get forms
	for( unsigned int i = 0; i < m_page->getNumberOfForms(); i++ ) {
		// parameter
		kome::plugin::SettingsForm* form = m_page->getForm( i );
		// Spec No.89503 @date 2012.12.18 <Mod> M.Izumi add null check
		if( form != NULL ){
			kome::plugin::SettingsParam* param = form->getParam();
			if( param != NULL ){
				// get group
				kome::plugin::SettingsGroup* group = form->getGroup();
				if( group != prevGroup || groups.size() == 0 || group == NULL ) {
					// add new group
					groups.resize( groups.size() + 1 );
					groups.back().name = std::string( group == NULL ? "" : group->getName() );
					groups.back().size = 0;

					prevGroup = group;
				}
				GroupInfo& g = groups.back();

				// add parameter to group
				g.params.push_back( form );

				unsigned int tmpSize = 1;
				kome::plugin::SettingsValue* val = param->getValue();
				if( val != NULL ){
					if( val->getType() == kome::plugin::SettingsValue::TYPE_LIST ) {
						tmpSize = LIST_PARAM;
					}
				}

				g.size += tmpSize;
			}
		}
	}
}

// search form
SettingsPanel::FormInfo* SettingsPanel::searchForm( const int id ) {
	FormInfo* fi = NULL;
	for( unsigned int i = 0; i < m_forms.size(); i++ ) {
		if( id >= m_forms[ i ].id ) {
			fi = &( m_forms[ i ] );
		}
	}

	return fi;
}

// search window
wxWindow* SettingsPanel::searchWindow( kome::plugin::SettingsForm* form ) {
	// ID
	int id = -1;
	for( unsigned int i = 0; i < m_forms.size() && id < 0; i++ ) {
		FormInfo* fi = &( m_forms[ i ] ); 
		if( fi->form == form ) {
			id = fi->id;
		}
	}

	// window
	wxWindow* wnd = FindWindow( id + MIN_CONTROL_ID );
	return wnd;
}

// create top sizer
wxSizer* SettingsPanel::createMainSizer() {
	// create groups
	std::vector< GroupInfo > groups;
	createGroups( groups );
	if( groups.size() == 0 ) {
		return NULL;
	}

	// create clusters
	std::vector< ClusterInfo > clusters;
	createClusters( clusters, groups );

	// get the number of cluster rows
	unsigned int rows = getNumberOfRows( clusters.size() );
	int startRow = 0;
	int gridRows = rows;
	if( m_dfltBtn ) {
		gridRows += 1;
	}

	// create sizer
	wxFlexGridSizer* topSizer = new wxFlexGridSizer( gridRows, 1, GRID_GAP, GRID_GAP );
	topSizer->AddGrowableCol( 0 );
	for( int i = startRow; i < gridRows; i++ ) {
		topSizer->AddGrowableRow( i );
	}

	// create cluster row sizer
	unsigned int idx = 0;
	unsigned int minCols = clusters.size() / rows;

	for( unsigned int i = 0; i < rows; i++ ) {
		// get the number of columns
		unsigned int cols = minCols;
		if( ( clusters.size() - idx ) % minCols > 0 ) {
			cols++;
		}

		// create raw sizer
		wxFlexGridSizer* rawSizer = new wxFlexGridSizer( 1, cols, GRID_GAP, GRID_GAP );

		rawSizer->AddGrowableRow( 0 );
		for( unsigned int i = 0; i < cols; i++ ) {
			rawSizer->AddGrowableCol( i );
		}

		// add cluster sizer
		for( unsigned i = 0; i < cols; i++ ) {
			ClusterInfo& cluster = clusters[ idx + i ];

			wxSizer* clusterSizer = createClusterSizer( cluster );

			rawSizer->Add( clusterSizer, 1, wxGROW | wxALL );
		}

		topSizer->Add( rawSizer, 1, wxGROW | wxALL, BORDER_SIZE );

		idx += cols;
	}

	// add default button
	if( m_dfltBtn ) {
		topSizer->Add( createDefaultButtonSizer(), 1, wxGROW | wxALL, BORDER_SIZE );
	}

	return topSizer;
}

// create cluster sizer
wxSizer* SettingsPanel::createClusterSizer( ClusterInfo& cluster ) {
	// create sizer
	wxFlexGridSizer* clusterSizer = new wxFlexGridSizer( cluster.groups.size(), 1, GRID_GAP, GRID_GAP );
	clusterSizer->AddGrowableCol( 0 );
	for( unsigned int i = 0; i < cluster.groups.size(); i++ ) {
		clusterSizer->AddGrowableRow( i );
	}

	// create group sizer
	for( unsigned int i = 0; i < cluster.groups.size(); i++ ) {
		wxSizer* groupSizer = createGroupSizer( *(cluster.groups[ i ]) );
		
		clusterSizer->Add( groupSizer, 1, wxGROW | wxALL );
	}

	return clusterSizer;
}

// create group sizer
wxSizer* SettingsPanel::createGroupSizer( GroupInfo& group ) {
	// group sizer
	wxSizer* sizer = NULL;
	if( group.name.empty() ) {
		sizer = new wxBoxSizer( wxVERTICAL );
	}
	else {
		sizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( group.name.c_str() ) );
	}

	// grid sizer
	wxFlexGridSizer* gridSizer = new wxFlexGridSizer( group.params.size(), 1, GRID_GAP, GRID_GAP );
	gridSizer->AddGrowableCol( 0 );
	for( unsigned int i = 0; i < group.params.size(); i++ ) {
		gridSizer->AddGrowableRow( i );
	}

	// add forms
	for( unsigned int i = 0; i < group.params.size(); i++ ) {
		// value
		kome::plugin::SettingsForm* form = group.params[ i ];
		kome::plugin::SettingsParam* param = ( form == NULL ? NULL : form->getParam() );
		kome::plugin::SettingsValue* val = ( param == NULL ? NULL : param->getValue() );

		// parameter sizer
		wxSizer* ctrlSizer = createParameterSizer( group.params[ i ] );

		if( val != NULL ) {
			if( form->isDetail() ) {
				gridSizer->Add( ctrlSizer, 1, wxALL | wxGROW, 0 );
			}
			else {
				if( val->getType() == kome::plugin::SettingsValue::TYPE_LIST ) {	// list parameter
					gridSizer->Add( ctrlSizer, 1, wxALL | wxGROW, 0 );
				}
				else {	// single parameter
					wxFlexGridSizer* paramSizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
					gridSizer->AddGrowableCol( 1 );
					gridSizer->AddGrowableRow( 0 );

					// title
					std::string title = FMT( "%s", group.params[ i ]->getTitle() );
					paramSizer->Add(
						new ParameterTitlePanel( this, title.c_str(), val->isRequired() ),
						0,
						wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
						BORDER_SIZE
					);
					paramSizer->Add( ctrlSizer, 1, wxALL | wxGROW, BORDER_SIZE );
					gridSizer->Add( paramSizer, 1, wxALL | wxGROW, BORDER_SIZE );
				}
			}
		}
	}

	sizer->Add( gridSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// create parameter sizer
wxSizer* SettingsPanel::createParameterSizer( kome::plugin::SettingsForm* form ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// add form information
	const int id = m_newId;
	m_newId++;

	m_forms.resize( m_forms.size() + 1 );
	m_forms.back().form = form;
	m_forms.back().id = id;

	// create form sizer
	wxSizer* sizer = NULL;

	if( value->getType() == kome::plugin::SettingsValue::TYPE_LIST ) {    // list
		sizer = createListSizer( form, id, value->getHideAllCheckbox() );
	}
	else if( value->isEnumValue() ) {	// combobox
		sizer = createComboBoxSizer( form, id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_BOOL ) {	// checkbox
		sizer = createCheckBoxSizer( form, id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_INT ) {	// integer
		sizer = createIntValueSizer( form, id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_DOUBLE ) {	// double
		sizer = createRealNumberValueSizer( form, id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_STRING ) {		// string
		sizer = createStringValueSizer( form, id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_PASSWD ) {		// password
		sizer = createPasswdValueSizer( form, id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_INPUT
			|| value->getType() == kome::plugin::SettingsValue::TYPE_OUTPUT
			|| value->getType() == kome::plugin::SettingsValue::TYPE_DIR
			|| value->getType() == kome::plugin::SettingsValue::TYPE_RAWDATA ) {    // file
		sizer = createFileSizer( form, id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_COLOR ) {		// color
		sizer = createColorSizer( form, id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_INT_RANGE ) {	// integer range
		sizer = createIntRangeSizer( form, id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE ) {	// double range
		sizer = createDoubleRangeSizer( form, id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_EMAIL ){			// e-mail
		sizer = creatEmailValueSizer(form, id);
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_FONT ) {    // font
		sizer = createFontSizer( form, id );
	}

	return sizer;
}

// create combo sizer
wxSizer* SettingsPanel::createComboBoxSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// create
	if( value->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {
		// style
		bool bookStyle = form->isDetail();

		sizer->Add(
			new PluginSelectionPanel(
				this, id + MIN_CONTROL_ID,
				form,
				m_settings,
				false,
				bookStyle ? PluginSelectionPanel::TYPE_CHOICE_BOOK : PluginSelectionPanel::TYPE_BUTTON,
				m_saveVal
			),
			1,
			wxALL | wxGROW
		);
	}
	else {
		// values
		std::vector< wxString > names;
		std::vector< int > indexes;
		bool editable = false;

		std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration = &( m_comboMap[ id ] );
		value->getEnumeration( *enumeration );

		if( !value->isRequired() ){
			kome::plugin::SettingsValue::EnumItem item;
			item.name = "(None)";
			item.value = MSPP_SETTINGS_NONE_VALUE;

			enumeration->insert( enumeration->begin(), item );
		}

		for( unsigned int i = 0; i < enumeration->size(); i++ ) {
			std::string name = ( *enumeration )[ i ].name;

			if( name.compare( "###" ) == 0 ) {
				editable = true;
			}
			else {
				names.push_back( wxT( name.c_str() ) );
				indexes.push_back( (int)i );
			}
		}

		// combobox
		wxComboBox* combo = new wxComboBox(
			this,
			id + MIN_CONTROL_ID,
			wxEmptyString,
			wxDefaultPosition,
			wxDefaultSize,
			names.size(),
			names.size() == 0 ? NULL : &( names[ 0 ] ),
			editable ? wxCB_DROPDOWN : wxCB_READONLY
		);

		for( unsigned int i = 0; i < indexes.size(); i++ ) {
			combo->SetClientData( i, &( ( *enumeration )[ i ] ) );
		}

		if( names.size() == 0 && !editable ) {
			combo->Enable( false );
		}
		
		sizer->Add( combo, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}

	return sizer;
}

// create check box sizer
wxSizer* SettingsPanel::createCheckBoxSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxSizer* sizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	// check box
	wxCheckBox* checkBox = new wxCheckBox( 
		this,
		MIN_CONTROL_ID + id,
		wxEmptyString
	);

	sizer->Add( checkBox, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );

	return sizer;
}

// create integer value size
wxSizer* SettingsPanel::createIntValueSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// range
	kome::core::NumberRestriction& restriction = value->getNumberRestriction();

	int minVal = (int)restriction.getLowerBound( (double)INT_MIN );
	if( restriction.hasExclusiveMinValue() ) {
		if( roundnum( restriction.getExclusiveMinValue() ) == minVal ) {
			minVal++;
		}
	}

	int maxVal = (int)restriction.getUpperBound( (double)INT_MAX );
	if( restriction.hasExclusiveMaxValue() ) {
		if( roundnum( restriction.getExclusiveMaxValue() ) == maxVal ) {
			maxVal--;
		}
	}

	// text control
	wxTextCtrl* text = new NumberTextCtrl(
		this,
		MIN_CONTROL_ID + id,
		true,
		minVal < 0
	);

	sizer->Add( text, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// spin
	m_newId++;
	wxSpinButton* spin = new wxSpinButton(
		this,
		MIN_CONTROL_ID + id + 1,
		wxDefaultPosition,
		wxSize( -1, text->GetBestSize().GetHeight() ),
		wxSP_VERTICAL | wxSP_ARROW_KEYS
	);
	
	maxVal = std::min( 999999999, maxVal );
	spin->SetRange( minVal, maxVal );

	sizer->Add( spin, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return sizer;
}

// create real number value sizer
wxSizer* SettingsPanel::createRealNumberValueSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// restriction
	double minVal = value->getNumberRestriction().getLowerBound( - FLT_MAX );

	// text control
	wxTextCtrl* text = new NumberTextCtrl(
		this,
		MIN_CONTROL_ID + id,
		false,
		minVal < 0.0
	);

	sizer->Add( text, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return sizer;
}

// create string value sizer
wxSizer* SettingsPanel::createStringValueSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxSizer* sizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	// text control
	wxTextCtrl* text = new wxTextCtrl(
		this,
		MIN_CONTROL_ID + id,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( STRING_EDIT_WIDTH, -1 )
	);

	wxSizer* textSizer = new wxBoxSizer( wxHORIZONTAL );
	textSizer->Add( text, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	sizer->Add( textSizer, 1, wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );

	return sizer;
}

// creates e-mail value sizer			// @date 2011.09.21 <Add> M.Izumi
wxSizer* SettingsPanel::creatEmailValueSizer( kome::plugin::SettingsForm* form, const int id ){
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxSizer* sizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	// text control
	wxTextCtrl* text = new EMailTextCtrl( this, MIN_CONTROL_ID + id	);

	wxSizer* textSizer = new wxBoxSizer( wxHORIZONTAL );
	textSizer->Add( text, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	sizer->Add( textSizer, 1, wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );

	return sizer;
}

// create password value sizer
wxSizer* SettingsPanel::createPasswdValueSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxSizer* sizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	// text control
	wxTextCtrl* text = new wxTextCtrl(
		this,
		MIN_CONTROL_ID + id,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( STRING_EDIT_WIDTH, -1 ),
		wxTE_PASSWORD 
	);

	wxSizer* textSizer = new wxBoxSizer( wxHORIZONTAL );
	textSizer->Add( text, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	sizer->Add( textSizer, 1, wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );

	return sizer;
}

// add file sizer
wxSizer* SettingsPanel::createFileSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );

	// text control
	wxTextCtrl* text = new wxTextCtrl(
		this,
		MIN_CONTROL_ID + id,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( STRING_EDIT_WIDTH * 2, -1 )
	);

	wxSizer* textSizer = new wxBoxSizer( wxHORIZONTAL );
	textSizer->Add( text, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	sizer->Add( textSizer, 1, wxLEFT | wxRIGHT | wxGROW, BORDER_SIZE );

	// button
	wxButton* button = new wxButton(
		this,
		MIN_PARAM_BUTTON_ID + id,
		wxT( "&Browse..." )
	);

	sizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );

	return sizer;
}

// add color sizer
wxSizer* SettingsPanel::createColorSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// button
	wxButton* button = new wxButton(
		this,
		MIN_PARAM_BUTTON_ID + id,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( COLOR_WIDTH, -1 )
	);

	sizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT, BORDER_SIZE );

	return sizer;
}

// create integer range sizer
wxSizer* SettingsPanel::createIntRangeSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// range
	kome::core::NumberRestriction& restriction = value->getNumberRestriction();

	int minVal = (int)restriction.getLowerBound( (double)INT_MIN );
	if( restriction.hasExclusiveMinValue() ) {
		if( roundnum( restriction.getExclusiveMinValue() ) == minVal ) {
			minVal++;
		}
	}

	int maxVal = (int)restriction.getUpperBound( (double)INT_MAX );
	if( restriction.hasExclusiveMaxValue() ) {
		if( roundnum( restriction.getExclusiveMaxValue() ) == maxVal ) {
			maxVal--;
		}
	}

	// text control (min)
	wxTextCtrl* minText = new NumberTextCtrl(
		this,
		MIN_CONTROL_ID + id,
		true,
		minVal < 0
	);

	sizer->Add( minText, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// spin (min)
	m_newId++;
	wxSpinButton* minSpin = new wxSpinButton(
		this,
		MIN_CONTROL_ID + id + 1,
		wxDefaultPosition,
		wxSize( -1, minText->GetBestSize().GetHeight() ),
		wxSP_VERTICAL | wxSP_ARROW_KEYS
	);
	minSpin->SetRange( minVal, maxVal );

	sizer->Add( minSpin, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// dash
	sizer->AddSpacer( 10 );
	sizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "-" ) ),
		0,
		wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER,
		BORDER_SIZE
	);
	sizer->AddSpacer( 10 );

	// text control (max)
	m_newId++;
	wxTextCtrl* maxText = new NumberTextCtrl(
		this,
		MIN_CONTROL_ID + id + 2,
		true,
		minVal < 0
	);

	sizer->Add( maxText, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// spin (max)
	m_newId++;
	wxSpinButton* maxSpin = new wxSpinButton(
		this,
		MIN_CONTROL_ID + id + 3,
		wxDefaultPosition,
		wxSize( -1, maxText->GetBestSize().GetHeight() ),
		wxSP_VERTICAL | wxSP_ARROW_KEYS
	);
	maxSpin->SetRange( minVal, maxVal );

	sizer->Add( maxSpin, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return sizer;
}

// create double range sizer
wxSizer* SettingsPanel::createDoubleRangeSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// min value
	double minVal = value->getNumberRestriction().getLowerBound( - FLT_MAX );

	// sizer
	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// text control (min)
	wxTextCtrl* minText = new NumberTextCtrl(
		this,
		MIN_CONTROL_ID + id,
		false,
		minVal < 0.0
	);

	sizer->Add( minText, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// dash
	sizer->AddSpacer( 10 );
	sizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "-" ) ),
		0,
		wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER,
		BORDER_SIZE
	);
	sizer->AddSpacer( 10 );

	// text control (max)
	m_newId++;
	wxTextCtrl* maxText = new NumberTextCtrl(
		this,
		MIN_CONTROL_ID + id + 1,
		false,
		minVal < 0.0
	);

	sizer->Add( maxText, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return sizer;
}

// create list sizer
wxSizer* SettingsPanel::createListSizer(
		kome::plugin::SettingsForm* form,
		const int id,
		bool hideAllCheckbox
) {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// title flg 初期表示時 trueに変更 @2012.06.08 <Mod> M.Izumi
	// panel
	ListEditPanel* panel = new ListEditPanel( this, MIN_CONTROL_ID + id, form, m_settings, true, false, hideAllCheckbox );
	sizer->Add( panel, 1, wxALL | wxGROW, 0 );

	return sizer;
}

// create default button sizer
wxSizer* SettingsPanel::createDefaultButtonSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// space
	sizer->AddSpacer( INDENT_SIZE );

	// button
	wxButton* button = new wxButton(
		this,
		DEFAULT_BUTTON_ID,
		wxT( "Default" )
	);

	sizer->Add( button , 0, wxALIGN_LEFT, BORDER_SIZE );

	return sizer;
}

// create font sizer
wxSizer* SettingsPanel::createFontSizer( kome::plugin::SettingsForm* form, const int id ) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );

	// canvas
	FontCanvas* canvas = new FontCanvas(
		this,
		MIN_CONTROL_ID + id
	);

	sizer->Add( canvas, 1, wxALL | wxGROW, BORDER_SIZE );

	// button
	wxButton* button = new wxButton(
		this,
		MIN_PARAM_BUTTON_ID + id,
		wxT( "&Font..." )
	);

	sizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxRIGHT, BORDER_SIZE );

	return sizer;
}

// process event
void SettingsPanel::processEvent( kome::plugin::SettingsForm* form ) {
	// check the event flag
	if( !m_evtFlg ) {
		return;
	}

	// flag
	m_edited = true;

	// event object
	SettingsPanelEvent evt( GetId(), this, form );

	// process event
	GetEventHandler()->ProcessEvent( evt );
}

// check value
bool SettingsPanel::checkValue(
	   kome::plugin::SettingsForm* form,
	   const char* val,
	   std::string& msg 
) {
	// parameter, value
	kome::plugin::SettingsParam* param = form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// value string
	std::string s = NVL( val, "" );
	if( s.compare( MSPP_SETTINGS_NONE_VALUE ) == 0 ) {
		s = "";
	}

	// Need to remove any "&" characters from the title. These are used to
	// specify a keyboard shortcut (underlined) character in the UI, but they
	// should not be included in any error messages.
	std::string title = form->getTitle();
	std::string::size_type iPos = 0;
	while (iPos != std::string::npos)
	{
		iPos = title.find('&');
		if (iPos != std::string::npos)
		{
			title.erase(iPos, 1);
		}
	}

	// check required
	if( s.empty() ) {
		if( value->isRequired() ) {
			msg = FMT( "\"%s\" cannot be empty.", title.c_str() );
			return false;
		}
		else {
			return true;
		}
	}

	// check value
	std::string tmpMsg = value->checkValue( s.c_str(), title.c_str(), !value->isRequired() );
	msg = tmpMsg;

	return ( tmpMsg.empty() );
}

// get string value
std::string SettingsPanel::getValueFromWindow( const int id ) {
	// string object
	std::string str;

	// form
	FormInfo* form = searchForm( id );
	if( form == NULL ) {
		return str;
	}

	// parameter, value
	kome::plugin::SettingsParam* param = form->form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();
	if( value == NULL ) {
		return str;
	}

	// get value
	if( value->getType() == kome::plugin::SettingsValue::TYPE_LIST ) {    // list
		ListEditPanel* panel = dynamic_cast< ListEditPanel* >( FindWindow( MIN_CONTROL_ID + id ) );
		if( panel != NULL ) {
			str = panel->getValue();
		}
	}
	else if( value->isEnumValue() ) {
		if( value->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {    // plug-in
			PluginSelectionPanel* panel = (PluginSelectionPanel*)FindWindow( MIN_CONTROL_ID + id );
			if( panel == NULL ) {
				return str;
			}

			kome::plugin::PluginFunctionItem* item = panel->getItem();
			str = ( item == NULL ? MSPP_SETTINGS_NONE_VALUE : item->getShortName() );

			kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );
			if( page != NULL ) {
				std::string paramVal
					= page->getParameterString( *( m_settings->getSubParameterValues( param->getName(), item->getShortName() ) ) );
				if( !paramVal.empty() ) {
					str.append( paramVal );
				}
			}
		}
		else {
			// combobox
			wxComboBox* combo = (wxComboBox*)FindWindow( MIN_CONTROL_ID + id );
			if( combo == NULL ) {
				return str;
			}
			
			// get value
			int sel = combo->GetSelection();
			if( sel >= 0 && sel < (int)combo->GetCount() ) {
				kome::plugin::SettingsValue::EnumItem* item = (kome::plugin::SettingsValue::EnumItem*)combo->GetClientData( sel );
				if( item != NULL ) {
					str = item->value;
				}
			}
			else {
				str = combo->GetValue().c_str();
			}
		}
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_BOOL ) {	// checkbox
		// checkbox
		wxCheckBox* checkbox = (wxCheckBox*)FindWindow( MIN_CONTROL_ID + id );
		if( checkbox == NULL ) {
			return str;
		}

		// get value
		str = ( checkbox->GetValue() ? "true" : "false" );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_COLOR ) {	// color
		// button
		wxButton* button = (wxButton*)FindWindow( MIN_PARAM_BUTTON_ID + id );
		if( button == NULL ) {
			return str;
		}

		// get color
		wxColour color = button->GetBackgroundColour();
		COLOR c = kome::img::ColorTool::getColor( color.Red(), color.Green(), color.Blue() );

		str = kome::img::ColorTool::getString( c );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_FONT ) {    // font
		// canvas
		FontCanvas* canvas = (FontCanvas*)FindWindow( MIN_CONTROL_ID + id );
		kome::img::Font& font = canvas->getFont();
		COLOR color = canvas->getColor();

		str = kome::img::ColorTool::getString( color );
		str.append( "," );
		str.append(
			FMT(
				"%d,%d,%d,%d,%s",
				font.getSize(),
				font.getStyle(),
				font.getWeight(),
				font.getFamily(),
				font.getFace()
			) 
		);
	}
	// >>>>>>	@Date:2014/02/07	<Add>	A.Ozaki
	//
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_PASSWD ) {    // Password
		wxTextCtrl* text = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id );
		if( text == NULL ) {
			return str;
		}

		// get value
		str = text->GetValue();
		str = trimstring( str.c_str() );
		if  ( -1 == str.find( "@@@@" ) )
		{
			// 暗号化されていない場合は暗号化します。
			// 暗号化はIniFileクラスのメソッドを使用します。
			//
			kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
			kome::core::IniFile* ini = ini = msppMgr.getIniFile();
			bool	bRet = false;

			if  ( (kome::core::IniFile *)NULL != ini )
			{
				char	cBuff[1024];
				bRet = ini->encryptPassword( str.c_str( ), cBuff, sizeof( cBuff ) );
				if  ( true == bRet )
				{
					str = cBuff;
				}
			}
		}
	}
	//
	// <<<<<<	@Date:2014/02/07	<Add>	A.Ozaki
	else {	// text
		// text
		wxTextCtrl* text = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id );
		if( text == NULL ) {
			return str;
		}

		// get value
		str = text->GetValue();
		str = trimstring( str.c_str() );

		// range
		if( value->getType() == kome::plugin::SettingsValue::TYPE_INT_RANGE
				|| value->getType() == kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE ) {
			if( str.find( RANGE_VALUES_DELIMITER ) != str.npos ) {
				str = FMT( "[%s]", str.c_str() );
			}
					// separator
			str.append( RANGE_VALUES_DELIMITER );

			// get max value
			if( value->getType() == kome::plugin::SettingsValue::TYPE_INT_RANGE ) {
				text = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id + 2 );
			}
			else {
				text = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id + 1 );
			}

			if( text == NULL ) {
				return str;
			}

			std::string tmp = text->GetValue();
			tmp = trimstring( tmp.c_str() );
			if( tmp.find( RANGE_VALUES_DELIMITER ) != tmp.npos ) {
				tmp = FMT( "[%s]", tmp.c_str() );
			}

			str.append( tmp );
		}
	}

	return str;
}

// set value from string
bool SettingsPanel::setValueFromString( const int id, const char* str ) {
	// search form
	FormInfo* form = searchForm( id );
	if( form == NULL ) {
		return false;
	}

	// parameter, value
	kome::plugin::SettingsParam* param = form->form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// check the value
	std::string msg = value->checkValue( str, param->getName(), !value->isRequired() );
	if( !msg.empty() ) {
		LOG_WARN( FMT( "The value \"%s\" is not set in %s. [%s]", str, param->getName(), msg.c_str() ) );
		return false;
	}

	// string
	std::string s = NVL( str, "" );

	if( !value->isRequired() ) {
		if( s.compare( MSPP_SETTINGS_NONE_VALUE ) == 0 ) {
			s = "";
		}
	}

	// set value
	if( value->getType() == kome::plugin::SettingsValue::TYPE_LIST ) {    // list value
		ListEditPanel* panel = (ListEditPanel*)FindWindow( MIN_CONTROL_ID + id );
		if( panel == NULL ) {
			return false;
		}
		panel->setValue( s.c_str() );
			
	}
	else if( value->isEnumValue() ) {
		if( value->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {    // plug-in
			PluginSelectionPanel* panel = (PluginSelectionPanel*)FindWindow( MIN_CONTROL_ID + id );
			if( panel == NULL ) {
				return false;
			}

			panel->setValue( s.c_str() );
		}
		else {  // combo
			// get combo
			wxComboBox* combo = (wxComboBox*)FindWindow( MIN_CONTROL_ID + id );
			if( combo == NULL ) {
				return false;
			}

			if( value->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {
				std::string::size_type pos = s.find( "[" );
				if( pos != s.npos ) {
					s = s.substr( 0, pos );
				}
			}

			// select combo
			int sel = -1;
			for( int i = 0; i < (int)combo->GetCount() && sel < 0; i++ ) {
				kome::plugin::SettingsValue::EnumItem* item = (kome::plugin::SettingsValue::EnumItem*)combo->GetClientData( i );
				if( item != NULL && item->value.compare( s ) == 0 ) {
					sel = i;
				}
			}
		
			if( sel >= 0 && sel < (int)combo->GetCount() ) {
				combo->Select( sel );
			}
			else if( s.empty() && combo->GetColumns() >= 1 ) {
				combo->Select( 0 );
			}
			else {
				combo->SetValue( wxT( s.c_str() ) );
			}

			onSection( id );
		}
	} 
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_BOOL ) {	// check box
		// get checkboxset
		wxCheckBox* check = (wxCheckBox*)FindWindow( MIN_CONTROL_ID + id );
		if( check == NULL ) {
			return false;
		}

		s = kome::plugin::SettingsValue::convertToDisplayingString( s.c_str() );
		
		// set check
		check->SetValue( tobool( s.c_str(), false ) );

		onSection( id );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_COLOR ) {	// color button
		// get button
		wxButton* button = (wxButton*)FindWindow( MIN_PARAM_BUTTON_ID + id );
		if( button == NULL ) {
			return false;
		}

		// set color
		COLOR c = kome::img::ColorTool::getColor( s.c_str() );
		wxColour color( 
			kome::img::ColorTool::getRed( c ),
			kome::img::ColorTool::getGreen( c ),
			kome::img::ColorTool::getBlue( c )
		);
		button->SetBackgroundColour( color );
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_INT_RANGE
			|| value->getType() == kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE ) {	// range
		// get text
		wxTextCtrl* text0 = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id );
		wxTextCtrl* text1 = NULL;
		if( value->getType() == kome::plugin::SettingsValue::TYPE_INT_RANGE ) {
			text1 = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id + 2 );
		}
		else {
			text1 = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id + 1 );
		}

		// separate
		std::vector< std::string > toks;
		// --> <Mod> 20140717 fujita
		unsigned int cnt = stringseparate( s.c_str(), "\\:", toks);  // データが[100\:1000]のように区切られている場合      
		if (cnt == 1) {    // 上記の条件で分割できなかった場合
			toks.clear();
			stringseparate( s.c_str(), ":", toks);     
		}
		// --< <Mod> 
		
		for( unsigned int i = 0; i < toks.size(); i++ ) {
			toks[ i ] = trimstring( toks[ i ].c_str() );
		}

		// set values
		if( text0 != NULL && toks.size() >= 1 ) {
			text0->SetValue( wxT( toks[ 0 ].c_str() ) );
		}
		if( text1 != NULL ) {
			if( toks.size() >= 2 ) {
				text1->SetValue( wxT( toks[ 1 ].c_str() ) );
			}
			else if( toks.size() == 1 ) {
				text1->SetValue( wxT( toks[ 0 ].c_str() ) );
			}
		}
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_FONT ) {    // font
		// canvas
		FontCanvas* canvas = (FontCanvas*)FindWindow( MIN_CONTROL_ID + id );

		// get information
		std::vector< std::string > elms;
		stringtoken( s.c_str(), ",", elms );

		if( elms.size() >= 6 ) {
			COLOR color = kome::img::ColorTool::getColor( elms[ 0 ].c_str() );
			kome::img::Font font(
				toint( elms[ 1 ].c_str() ),
				(kome::img::Font::Family)toint( elms[ 4 ].c_str() ),
				(kome::img::Font::Style)toint( elms[ 2 ].c_str() ),
				(kome::img::Font::Weight)toint( elms[ 3 ].c_str() ),
				elms[ 5 ].c_str()
			);

			// set font
			canvas->setColor( color );
			canvas->setFont( font );
			canvas->Refresh();
		}
	}
	// >>>>>>	@Date:2014/02/07	<Add>	A.Ozaki
	//
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_PASSWD ) {    // Password
		wxTextCtrl* text = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id );
		if( text == NULL ) {
			return false;
		}
		
		s = kome::plugin::SettingsValue::convertToDisplayingString( s.c_str() );

		if  ( -1 != s.find( "@@@" ) )
		{
			//
			// 暗号化されている場合は復号化します。
			// 復号化はIniFileクラスのメソッドを使用します。
			//
			kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
			kome::core::IniFile* ini = ini = msppMgr.getIniFile();
			bool	bRet = false;

			if  ( (kome::core::IniFile *)NULL != ini )
			{
				char	cBuff[256];
				bRet = ini->decryptPassword( s.c_str( ), cBuff, sizeof( cBuff ) );
				if  ( true == bRet )
				{
					s = cBuff;
				}
			}
		}
		// set text
		text->SetValue( wxT( s.c_str() ) );
	}
	//
	// <<<<<<	@Date:2014/02/07	<Add>	A.Ozaki
	else {	// text
		// get text
		wxTextCtrl* text = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id );
		if( text == NULL ) {
			return false;
		}
		
		s = kome::plugin::SettingsValue::convertToDisplayingString( s.c_str() );

		// set text
		text->SetValue( wxT( s.c_str() ) );
	}

	return true;
}

// set default value
void SettingsPanel::setDefaultValues( const bool thisWindow, const bool subWindow ) {
	// set to default values
	if( thisWindow ) {
		for( unsigned int i = 0; i < m_forms.size(); i++ ) {
			// form
			const int id = m_forms[ i ].id;
			kome::plugin::SettingsForm* form = m_forms[ i ].form;
			kome::plugin::SettingsParam* param = form->getParam();
			kome::plugin::SettingsValue* value = param->getValue();
			
			// set default value
			setValueFromString( id, value->getDefaultValue() );
		}
	}

	// sub window
	if( subWindow ) {
		wxWindowList& children = GetChildren();

		for( wxWindowList::iterator it = children.begin(); it != children.end(); it++ ) {
			wxWindow* child = *it;
			SettingsPanel* panel = dynamic_cast< SettingsPanel* >( child );

			if( panel != NULL ) {
				panel->setDefaultValues( true, true );
			}
			// @date 2012.04.23 <Add> M.Izumi
			// SPEC No.84106  A set point does not become the value of the default.
			PluginSelectionPanel* pluginSelPanel = dynamic_cast< PluginSelectionPanel* >( child );
			if( pluginSelPanel != NULL ){
				pluginSelPanel->setDefaultParameters();
			}
		}
	}
	
}

// on combo
void SettingsPanel::onSection( const unsigned int id ) {
	// form
	FormInfo* f = searchForm( id );
	if( f == NULL ) {
		return;
	}

	// value
	kome::plugin::SettingsForm* form = f->form;
	kome::plugin::SettingsParam* param = ( form == NULL ? NULL : form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	if( value == NULL ) {
		return;
	}

	std::string val = getValueFromWindow( id );

	// children
	std::set< kome::plugin::SettingsValue* > childSet;
	for( unsigned int i = 0; i < value->getNumberOfChildValues(); i++ ) {
		childSet.insert( value->getChildValue( i ) );
	}

	// update
	for( unsigned int i = 0; i < m_forms.size(); i++ ) {
		kome::plugin::SettingsForm* cForm = m_forms[ i ].form;
		kome::plugin::SettingsParam* cParam = ( cForm == NULL ? NULL : cForm->getParam() );
		kome::plugin::SettingsValue* cValue = ( cParam == NULL ? NULL : cParam->getValue() );

		if( cValue != NULL && childSet.find( cValue ) != childSet.end() ) {
			updateEnumWindow( m_forms[ i ].id, val.c_str() );
		}
	}
}

// update enumeration window
void SettingsPanel::updateEnumWindow( const unsigned int id, const char* val ) {
	// value
	FormInfo* f = searchForm( id );
	if( f == NULL ) {
		return;
	}

	kome::plugin::SettingsForm* form = f->form;
	kome::plugin::SettingsParam* param = ( form == NULL ? NULL : form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	if( value == NULL ) {
		return;
	}

	// window
	wxWindow* wnd = FindWindow( id + MIN_CONTROL_ID );

	if( value->getType() == kome::plugin::SettingsValue::TYPE_LIST ) {    // list
		ListEditPanel* panel = dynamic_cast< ListEditPanel* >( wnd );
		if( panel != NULL ) {
			panel->updateEnumeration( val );
		}
	}
	else if( value->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {    // plugin selection
		PluginSelectionPanel* panel = dynamic_cast< PluginSelectionPanel* >( wnd );
		if( panel != NULL ) {
			panel->updateEnum( val );
		}
	}
	else {    // combobox
		wxComboBox* combo = dynamic_cast< wxComboBox* >( wnd );
		if( combo != NULL ) {
			combo->Clear();

			std::string v = NVL( val, "" );
			std::string oldVal;
			bool editable = false;
			int sel = combo->GetSelection();
			if( sel >= 0 && sel < (int)combo->GetCount() ) {
				kome::plugin::SettingsValue::EnumItem* item = (kome::plugin::SettingsValue::EnumItem*)combo->GetClientData( sel );
				if( item != NULL ) {
					oldVal = item->value;
				}
			}
			sel = -1;

			std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration = &( m_comboMap[ id ] );
			for( unsigned int i = 0; i < enumeration->size(); i++ ) {
				kome::plugin::SettingsValue::EnumItem* item = &( ( *enumeration )[ i ] );
				if( item->name.compare( "###" ) == 0 ) {
					editable = true;
				}
				else if( v.empty() || item->section.compare( v ) == 0 ) {
					if( sel < 0 && item->value.compare( oldVal ) == 0 ) {
						sel = combo->GetCount();
					}
					combo->Append( wxT( item->name.c_str() ), item );
				}
			}

			bool enable = ( editable || combo->GetCount() > 0 );
			combo->Enable( enable );
			if( enable ) {
				if( sel < 0 ) {
					if( editable ) {
						combo->SetValue( oldVal.c_str() );
					}
					else {
						sel = 0;
					}
				}

				if( sel >= 0 && sel < (int)combo->GetCount() ) {
					combo->SetSelection( sel );
				}
			}
		}
	}
}

// on spin
void SettingsPanel::onSpin( wxEvent& evt ) {
	// form information
	int id = evt.GetId() - MIN_CONTROL_ID - 1;

	FormInfo* form = searchForm( id );
	if( form == NULL ) {
		return;
	}

	// get spin
	wxSpinButton* spin = (wxSpinButton*)FindWindow( MIN_CONTROL_ID + id + 1 );
	if( spin == NULL ) {
		return;
	}

	// get text
	wxTextCtrl* text = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id );
	if( text == NULL ) {
		return;
	}

	// set value
	int val = spin->GetValue();
	if( val < spin->GetMin() ) {
		val = spin->GetMin();
	}
	if( val > spin->GetMax() ) {
		val = spin->GetMax();
	}
	text->SetValue( wxT( FMT( "%d", val ).c_str() ) );

	// event
	processEvent( form->form );
}

// on text
void SettingsPanel::onText( wxEvent& evt ) {
	// form information
	int id = evt.GetId() - MIN_CONTROL_ID;

	FormInfo* form = searchForm( id );
	if( form == NULL ) {
		return;
	}

	// text
	wxTextCtrl* text = dynamic_cast< wxTextCtrl* >( FindWindow( MIN_CONTROL_ID + id ) );
	if( text == NULL ) {
		return;
	}

	// parameter, value
	kome::plugin::SettingsParam* param = form->form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	if( !value->isEnumValue()
			&& ( value->getType() == kome::plugin::SettingsValue::TYPE_INT
			|| value->getType() == kome::plugin::SettingsValue::TYPE_INT_RANGE ) ) {	// int
		int v = int();
		if( isint( text->GetValue().c_str(), 10, &v ) ) {	// valid
			wxSpinButton* spin = (wxSpinButton*)FindWindow( MIN_CONTROL_ID + id + 1 );
			if( spin != NULL ) {
				spin->SetValue( v );
			}
		}
	}

	// event
	processEvent( form->form );
}

// on combo
void SettingsPanel::onCombo( wxEvent& evt ) {
	// form
	int id = evt.GetId() - MIN_CONTROL_ID;
	FormInfo* form = searchForm( id );
	if( form == NULL ) {
		return;
	}

	// update
	onSection( id );

	// event
	processEvent( form->form );
}

// on parameters button
void SettingsPanel::onParamButton( wxEvent& evt ) {
	// form
	int id = evt.GetId() - MIN_PARAM_BUTTON_ID;
	FormInfo* form = searchForm( id );
	if( form == NULL ) {
		return;
	}

	// parameter, value
	kome::plugin::SettingsParam* param = form->form->getParam();
	kome::plugin::SettingsValue* value = param->getValue();

	// button event
	if( value->getType() == kome::plugin::SettingsValue::TYPE_COLOR ) {	// color
		// button
		wxButton* button = (wxButton*)FindWindow( MIN_PARAM_BUTTON_ID + id );
		if( button == NULL ) {
			return;
		}

		// select color
		wxColourDialog dlg( this );
		if( dlg.ShowModal() == wxID_OK ) {
			wxColour& color = dlg.GetColourData().GetColour();
			button->SetBackgroundColour( color );
			button->Refresh();

			// event
			processEvent( form->form );
		}
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_INPUT 
			|| value->getType() == kome::plugin::SettingsValue::TYPE_OUTPUT ) {	// select file
		// text
		wxTextCtrl* text = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id );
		if( text == NULL ) {
			return;
		}

		// directoy
		std::string dir;
		std::string file = text->GetValue().c_str();
		if( !file.empty() ) {
			kome::core::MsppManager& mgr = kome::core::MsppManager::getInstance();
			std::string path = getpath( gethomedir().c_str(), file.c_str() );

			dir = getdir( path.c_str() );
			file = getfilename( path.c_str() );
		}

		// write flag
		const bool writeFlg = ( value->getType() == kome::plugin::SettingsValue::TYPE_OUTPUT );

		// get filters
		std::string filters;
		for (unsigned int i = 0; i < value->getFilterSize(); i++)
		{
			if (i > 0)
			{
				filters += "|";
			}

			filters += FMT( "%s (%s)|%s", value->getFilterItemName(i), value->getFilterItemExtension(i), value->getFilterItemExtension(i) );
		}

		// dialog
		wxFileDialog dlg(
			this,
			"Select File",
			wxT( dir.c_str() ),
			wxT( file.c_str() ),
			wxT( filters.c_str() ),
			writeFlg ? wxFD_SAVE : ( wxFD_OPEN | wxFD_FILE_MUST_EXIST )
		);

		if( dlg.ShowModal() == wxID_OK ) {
			wxString path = dlg.GetPath();
			text->SetValue( path );

			// event
			processEvent( form->form );
		}
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_RAWDATA ) {   // rawdata
		// text
		wxTextCtrl* text = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id );
		if( text == NULL ) {
			return;
		}

		// dialog
		RawDataFileDialog dlg( this, true );
		if( dlg.ShowModal() == wxID_OK ) {
			text->SetValue( wxT( dlg.getPath() ) );
			processEvent( form->form );
		}
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_DIR ) {	// directory
		// text
		wxTextCtrl* text = (wxTextCtrl*)FindWindow( MIN_CONTROL_ID + id );
		if( text == NULL ) {
			return;
		}

		// dialog
		wxDirDialog dlg(
			this,
			"Choose a directory",
			text->GetValue()
		);

		if( dlg.ShowModal() == wxID_OK ) {
			text->SetValue( dlg.GetPath() );
			processEvent( form->form );
		}
	}
	else if( value->getType() == kome::plugin::SettingsValue::TYPE_FONT ) {    // font
		// canvas
		FontCanvas* canvas = (FontCanvas*)FindWindow( MIN_CONTROL_ID + id );
		if( canvas == NULL ) {
			return;
		}

		// font
		kome::img::Font& font = canvas->getFont();
		COLOR color = canvas->getColor();

		wxFont f = FontTool::convertFont( font );
		wxColour c(
			kome::img::ColorTool::getRed( color ),
			kome::img::ColorTool::getGreen( color ),
			kome::img::ColorTool::getBlue( color )
		);

		// dialog
		wxFontData data;
		data.SetInitialFont( f );
		data.SetColour( c );
		data.EnableEffects( false );

		// set
		wxFontDialog dlg( this, &data );

		if( dlg.ShowModal() == wxID_OK ) {
			data = dlg.GetFontData();
			f = data.GetChosenFont();
			c = data.GetColour();

			font = FontTool::convertFont( f );
			color = kome::img::ColorTool::getColor( c.Red(), c.Green(), c.Blue() );

			canvas->setFont( font );
			canvas->setColor( color );
			canvas->Refresh();
		}
	}
}

// on default button
void SettingsPanel::onDefaultButton( wxCommandEvent& evt ) {
	setDefaultValues();
}

// validate
bool SettingsPanel::Validate() {
	// check form values
	for( unsigned int i = 0; i < m_forms.size(); i++ ) {
		// get value
		std::string val = getValueFromWindow( m_forms[ i ].id );

		// check value
		std::string msg;
		if( !checkValue( m_forms[ i ].form, val.c_str(), msg ) ) {
			WindowTool::showInfo( msg.c_str() );
			return false;		
		}
	}

	// check parameters
	kome::objects::SettingParameterValues settings;
	for( unsigned int i = 0; i < m_forms.size(); i++ ) {
		// form
		const int id = m_forms[ i ].id;
		kome::plugin::SettingsForm* form = m_forms[ i ].form;
		kome::plugin::SettingsParam* param = form->getParam();

		// get value
		std::string v = getValueFromWindow( id );
		if( v.compare( MSPP_SETTINGS_NONE_VALUE ) == 0 ) {
			v = "";
		}

		settings.setValue( param->getName(), v.c_str() );
	}

	if( m_page != NULL ) {
		for( unsigned int i = 0; i < m_page->getNumberOfChecks(); i++ ) {
			kome::plugin::PluginFunctionItem* fun = m_page->getCheck( i );
			if( fun != NULL ) {
				kome::objects::Parameters params;
				kome::plugin::PluginCallTool::setSettingValues( params, settings );

				kome::objects::Variant ret = fun->getCall()->invoke( &params );
				std::string msg = NVL( ret.getString(), "" );

				if( !msg.empty() ) {
					WindowTool::showInfo( msg.c_str() );
					return false;
				}
			}
		}
	}

	return true;
}

// transfer data to window
bool SettingsPanel::TransferDataToWindow() {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// flag
	m_evtFlg = false;

	// default
	const bool ret = HelpGetterPanel::TransferDataToWindow();

	// ini file
	kome::core::IniFile* ini = NULL;
	if( m_page != NULL ) {
		if( m_page->getSettings().isIni() ) {
			ini = msppMgr.getIniFile();
		}
		else {
			ini = msppMgr.getParameters();
		}
	}

	// set values
	bool flg = true;
	for( unsigned int i = 0; i < m_forms.size() && flg; i++ ) {
		// form
		const int id = m_forms[ i ].id;
		kome::plugin::SettingsForm* form = m_forms[ i ].form;
		kome::plugin::SettingsParam* param = form->getParam();
		kome::plugin::SettingsValue* value = param->getValue();

		// key
		std::string key = param->getKey();
		
		// get value
		std::string v;
		if( ini != NULL && m_saveVal && !key.empty() ) {
			v = ini->getString( param->getSection(), key.c_str(), value->getDefaultValue() );
			if( value->isRequired()
					&& ( v.empty() || v.compare( MSPP_SETTINGS_NONE_VALUE ) == 0 ) ) {
				v = value->getDefaultValue();
			}
			if( v.compare( MSPP_SETTINGS_NONE_VALUE ) == 0 ) {
				v = "";
			}
		}else {			
			v = value->getDefaultValue();			
		}

		if( m_settings != NULL ) {
			const char* tmp = m_settings->getParameterValue( param->getName() );
			if( tmp != NULL ) {
				v = tmp;
			}
		}

// >>>>>>	@Date:2014/02/07	<Modity>	A.Ozaki
		// type password
		if  ( kome::plugin::SettingsValue::TYPE_PASSWD == value->getType( ) && !v.empty( ) )
		{
			//
			// 暗号化されている場合は復号化します。
			// 復号化はIniFileクラスのメソッドを使用します。
			//
			if  ( -1 != v.find( "@@@" ) )
			{
				bool	bRet = false;

				char	cBuff[256];
				bRet = ini->decryptPassword( v.c_str( ), cBuff, sizeof( cBuff ) );
				if  ( true == bRet )
				{
					v = cBuff;
				}
			}
		}
// >>>>>>	@Date:2014/02/07	<Modity>	A.Ozaki

		// set value
		flg = setValueFromString( id, v.c_str( ) );
	}

	// default
	if( !flg ) {
		setDefaultValues();
	}

	// flag
	m_evtFlg = true;

	return ret;
}

// transfer data from window
bool SettingsPanel::TransferDataFromWindow() {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// ini file
	kome::core::IniFile* ini = NULL;
	if( m_page != NULL ) {
		if( m_page->getSettings().isIni() ) {
			ini = msppMgr.getIniFile();
		}
		else {
			ini = msppMgr.getParameters();
		}
	}

	// get values
	for( unsigned int i = 0; i < m_forms.size(); i++ ) {
		// form
		const int id = m_forms[ i ].id;
		kome::plugin::SettingsForm* form = m_forms[ i ].form;
		kome::plugin::SettingsParam* param = form->getParam();
		kome::plugin::SettingsValue* value = param->getValue();

		// key
		std::string key = param->getKey();

		// get value
		std::string v = getValueFromWindow( id );
		if( v.compare( MSPP_SETTINGS_NONE_VALUE ) == 0 ) {
			v = "";
		}
		
		// set value
		if( m_settings != NULL ) {
			m_settings->setValue( param->getName(), v.c_str() );
		}

// >>>>>>	@Date:2014/02/07	<Modity>	A.Ozaki
		// type password
		if  ( kome::plugin::SettingsValue::TYPE_PASSWD == value->getType( ) && !v.empty( ) )
		{
			//
			// 暗号化されている場合は復号化します。
			// 復号化はIniFileクラスのメソッドを使用します。
			//
			if  ( -1 != v.find( "@@@" ) )
			{
				bool	bRet = false;

				char	cBuff[256];
				bRet = ini->decryptPassword( v.c_str( ), cBuff, sizeof( cBuff ) );
				if  ( true == bRet )
				{
					v = cBuff;
				}
			}
		}
// <<<<<<	@Date:2014/02/07	<Modity>	A.Ozaki

		if  ( NULL != ini && m_saveVal && !key.empty( ) )
		{
			if  ( v.empty( ) )
			{
				if  ( !value->isRequired( ) )
				{
					ini->setString( param->getSection( ), key.c_str( ), MSPP_SETTINGS_NONE_VALUE );
				}
				else
				{
					ini->deleteValue( param->getSection( ), key.c_str( ) );
				}
			}
			else
			{
				ini->setString( param->getSection( ), key.c_str( ), v.c_str( ) );
			}
		}
	}

	// default
	const bool ret = HelpGetterPanel::TransferDataFromWindow();

	return ret;
}

// get description
std::string SettingsPanel::getDescription( wxWindow* wnd ) {
	// return value
	std::string s;

	// get form ID
	if( wnd == NULL ) {
		return s;
	}
	int id = wnd->GetId() - MIN_CONTROL_ID;

	// form
	FormInfo* form = searchForm( id );

	// get description
	if( form == NULL ) {
		return s;
	}
	s = form->form->getDesc();

	wxComboBox* combo = dynamic_cast< wxComboBox* >( wnd );
	int cnt = 0;
	if( combo != NULL ) {
		for( int i = 0; i < (int)combo->GetCount(); i++ ) {
			kome::plugin::SettingsValue::EnumItem* item
				= (kome::plugin::SettingsValue::EnumItem*)( combo->GetClientData( i ) );

			if( item != NULL ) {
				s.append( "\n\n" );
				s.append( FMT( "[%s]\n", item->name.c_str() ) );
				s.append( item->desc );
			}
		}
	}

	return s;
}

// get image file path
std::string SettingsPanel::getImageFilePath( wxWindow* wnd ) {
	// return value
	std::string s;

	// get form ID
	if( wnd == NULL ) {
		return s;
	}
	int id = wnd->GetId() - MIN_CONTROL_ID;

	// form
	FormInfo* form = searchForm( id );

	// get image file path
	if( form == NULL ) {
		return s;
	}
	s = form->form->getImageFilePath();

	return s;
}

// create clusters
void SettingsPanel::createClusters(
		std::vector< ClusterInfo >& clusters,
		std::vector< GroupInfo >& groups
) {
	// max size
	unsigned int maxSize = CLUSTER_PARAM;
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		maxSize = std::max( maxSize, groups[ i ].size );
	}

	// number of clusters
	int num = 1;
	unsigned int currentSize = 0;
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		unsigned int size = groups[ i ].size;
		currentSize += size;
		if( currentSize > maxSize ) {
			num++;
			currentSize = size;
		}
	}

	// create clusters
	std::list< int > indexList;
	indexList.push_back( 0 );
	double var = FLT_MAX;

	createClusters( clusters, groups, &var, indexList, maxSize, num - 1 );
}

// create clusters
void SettingsPanel::createClusters(
		std::vector< ClusterInfo >& clusters,
		std::vector< GroupInfo >& groups,
		double* var,
		std::list< int >& indexList,
		const unsigned int maxSize,
		int num
) {
	if( num == 0 ) {		// create cluster
		// calculate gap
		int index = (int)groups.size();
		std::vector< double > sizes;
		for( std::list< int >::reverse_iterator it = indexList.rbegin(); it != indexList.rend(); it++ ) {
			unsigned int size = 0;
			for( int i = *it; i < index; i++ ) {
				size += groups[ i ].size;
			}
			sizes.push_back( (double)size );

			index = *it;
		}

		double v = kome::numeric::Statistics::variance( &( sizes[ 0 ] ), sizes.size() );
		if( v < *var ) {	// set cluster
			index = (int)groups.size();
			int clusterIndex = (int)indexList.size();

			clusters.clear();			
			clusters.resize( clusterIndex );

			for( std::list< int >::reverse_iterator it = indexList.rbegin(); it != indexList.rend(); it++ ) {
				clusterIndex--;

				for( int i = *it; i < index; i++ ) {
					clusters[ clusterIndex ].groups.push_back( &( groups[ i ] ) );
				}

				index = *it;
			}

			*var = v;
		}		
	}
	else {
		int index = indexList.back();
		int size = 0;

		for( int i = index + 1; i < (int)groups.size() && size <= (int)maxSize; i++ ) {
			indexList.push_back( i );

			createClusters( clusters, groups, var, indexList, maxSize, num - 1 );

			indexList.pop_back();
			size += (int)groups[ i ].params.size();
		}
	}
}

// get the nubmer of cluster coloms
unsigned int SettingsPanel::getNumberOfRows( const unsigned int num ) {
	unsigned int row = ( num / 5 ) + 1;

	return row;
}



//
// SettingsPanelEvent
//

DEFINE_EVENT_TYPE( EVT_COMMAND_SETTINGS_PANEL )


// constructor
SettingsPanelEvent::SettingsPanelEvent( const int id, SettingsPanel* panel, kome::plugin::SettingsForm* form )
        : wxCommandEvent( EVT_COMMAND_SETTINGS_PANEL, id ) {
    // initialize
    m_panel = panel;
	m_form = form;
}

// destructor
SettingsPanelEvent::~SettingsPanelEvent() {
}

// get the panel
SettingsPanel* SettingsPanelEvent::getPanel() {
	return m_panel;
}

// get the form
kome::plugin::SettingsForm* SettingsPanelEvent::getForm() {
	return m_form;
}
