/**
 * @file CompositionDialog.cpp
 * @brief implements of CompositionDialog class
 *
 * @author S.Tanaka
 * @date 2009.02.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CompositionDialog.h"
 

using namespace kome::composition;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP					  3
#define BORDER_SIZE					  3

#define ID_CONTROL_GROUP_COMBO		  1
#define ID_TREATMENT_GROUP_COMBO	  2


#define COMPOSITION_NAME			  "composition"
#define COMPOSITION_TYPE_NAME		  "COMPOSITION"
#define ALIGNMENT_NAME				  "alignment"
#define ALIGNMENT_TYPE_NAME		      "ALIGNMENT"

#define SECTION					      "Composition"
#define ALIGN_KEY_NAME				  "ALIGNMENT_ALGORITHM"
#define COMPOSITION_KEY_NAME		  "COMPOSITION_ALGORITHM"


BEGIN_EVENT_TABLE( CompositionDialog, kome::window::ParametersAndHelpDialog )
END_EVENT_TABLE()


// constructor
CompositionDialog::CompositionDialog( wxWindow* parent )
		: kome::window::ParametersAndHelpDialog( parent, "Sample Fusion", "Process" ), m_params( NULL, false ) {
	// initialize
	m_ctrlGrpCombo = NULL;
	m_trmtGrpCombo = NULL;

	m_ctrlGroup = NULL;
	m_trmtGroup = NULL;

	m_alignment = NULL;
	m_alignSettings = NULL;
	m_composition = NULL;
	m_compositionSettings = NULL;
}

// destructor
CompositionDialog::~CompositionDialog() {
}

// get parameters
kome::objects::SettingParameterValues& CompositionDialog::getParameters() {
	return m_settings;
}

// get control group
kome::objects::DataGroupNode* CompositionDialog::getCtrlGroup() {
	return m_ctrlGroup;
}

// get treatment group
kome::objects::DataGroupNode* CompositionDialog::getTrmtGroup() {
	return m_trmtGroup;
}

// get alignment
kome::plugin::PluginFunctionItem* CompositionDialog::getAlignment() {
	return m_alignment;
}

// get alignment settings
kome::objects::SettingParameterValues* CompositionDialog::getAlignmentSettings() {
	return m_alignSettings;
}

// get composition
kome::plugin::PluginFunctionItem* CompositionDialog::getComposition() {
	return m_composition;
}

// get composition settings
kome::objects::SettingParameterValues* CompositionDialog::getCompositionSettings() {
	return m_compositionSettings;
}

// create main sizer
wxSizer* CompositionDialog::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );

	// group sizer
	wxStaticBoxSizer* staticGroupSizer = new wxStaticBoxSizer(
		wxVERTICAL,
		this,
		wxT( "Group" )
	);
	sizer->Add( staticGroupSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	wxFlexGridSizer* groupSizer = new wxFlexGridSizer( 2, 2, GRID_GAP, GRID_GAP );
	groupSizer->AddGrowableCol( 1 );
	staticGroupSizer->Add( groupSizer, 1, wxGROW | wxALL, BORDER_SIZE );

	groupSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "Control" ) ),
		0,
		wxLEFT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	m_ctrlGrpCombo = createGroupCombo( ID_CONTROL_GROUP_COMBO );
	setDescription( m_ctrlGrpCombo, "Select the control sample." );
	wxSizer* comboSizer = new wxBoxSizer( wxHORIZONTAL );
	comboSizer->Add( m_ctrlGrpCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	groupSizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	groupSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "Treatment" ) ),
		0,
		wxLEFT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	m_trmtGrpCombo = createGroupCombo( ID_TREATMENT_GROUP_COMBO );
	setDescription( m_trmtGrpCombo, "Select the treatment sample." );
	comboSizer = new wxBoxSizer( wxHORIZONTAL );
	comboSizer->Add(m_trmtGrpCombo, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, BORDER_SIZE );
	groupSizer->Add( comboSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	// combosition panel
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );

	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	val->setEnumKey( ALIGNMENT_TYPE_NAME );
	val->setRequired( false );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setValue( val );
	param->setName( ALIGNMENT_NAME );
	param->setSection( SECTION );
	param->setKey( ALIGN_KEY_NAME );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "RT Alignment" );
	form->setDesc( "Select the RT alignment method." );

	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	val->setEnumKey( COMPOSITION_TYPE_NAME );
	val->setRequired( true );

	param = m_params.addParam();
	param->setValue( val );
	param->setName( COMPOSITION_NAME );
	param->setSection( SECTION );
	param->setKey( COMPOSITION_KEY_NAME );
	
	form = page->addForm();
	form->setParam( param );
	form->setTitle( "Composition" );
	form->setDesc( "Select the sample fusion method." );

	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel(
		this,
		page,		
		false,
		&m_settings,
		true
	);

	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// create group combo
wxComboBox* CompositionDialog::createGroupCombo( const int id ) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// groups
	std::vector< kome::objects::DataGroupNode* > groups;
	for( unsigned int i = 0; i < aoMgr.getNumberOfOpenedSamples(); i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		for( unsigned int j = 0; j < sample->getNumberOfGroups(); j++ ) {
			kome::objects::DataGroupNode* group = sample->getGroup( j );
			if( group->getNumberOfChromatograms() > 0 ) {
				groups.push_back( group );
			}
		}
	}

	// Spec No.86127 @date 2012.06.22 <Mod> M.Izumi
	wxString* groupNames = NULL;
	groupNames = new wxString[ groups.size() + 1 ];
	groupNames[0] = wxT("Select File");

	if( groups.size() > 0 ) {
		for( unsigned int i = 0; i < groups.size(); i++ ) {
			kome::objects::DataGroupNode* group = groups[ i ];
			std::string name;
			while( group != NULL ) {
				if( !name.empty() ) {
					name.append( " : " );
				}
				name.append( group->getName() );
				group = group->getParentGroup();
			}

			groupNames[ i + 1 ] = wxT(name.c_str());
		}
	}

	// combo box
	wxComboBox* combo = new wxComboBox(
		this,
		id,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		groups.size() + 1,
		groupNames,
		wxCB_SIMPLE | wxCB_READONLY
	);
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		combo->SetClientData( i + 1, groups[ i ] );
	}

	if( groups.size() == 0 ) {
		combo->Enable( false );
	}
	else {
		combo->SetSelection( 0 );
	}

	// delete array
	if( groupNames != NULL ) {
		delete[] groupNames;
	}

	return combo;
}

// data from window
bool CompositionDialog::TransferDataFromWindow() {
	// default
	if( !StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// group
	m_ctrlGroup = NULL;
	int sel = m_ctrlGrpCombo->GetSelection();
	if( sel >= 1 ) {
		m_ctrlGroup = (kome::objects::DataGroupNode*)m_ctrlGrpCombo->GetClientData( sel );
	}
	if( m_ctrlGroup == NULL ) {
		kome::window::WindowTool::showInfo( "Select a Control Group." );
		return false;
	}

	m_trmtGroup = NULL;
	sel = m_trmtGrpCombo->GetSelection();
	if( sel >= 1 ) {
		m_trmtGroup = (kome::objects::DataGroupNode*)m_trmtGrpCombo->GetClientData( sel );
	}
	if( m_trmtGroup == NULL ) {
		kome::window::WindowTool::showInfo( "Select a Treatment Group." );
		return false;
	}

	if( m_ctrlGroup == m_trmtGroup ) {
		kome::window::WindowTool::showInfo( "Control and Treatment groups must be different. Change one of the selected groups." );
		return false;
	}

	// alignment
	const char* alignment = m_settings.getParameterValue( ALIGNMENT_NAME );
	m_alignment = plgMgr.getFunctionItem( ALIGNMENT_TYPE_NAME, alignment );

	m_alignSettings = m_settings.getSubParameterValues( ALIGNMENT_NAME, alignment );

	// composition
	const char* composition = m_settings.getParameterValue( COMPOSITION_NAME );
	m_composition = plgMgr.getFunctionItem( COMPOSITION_TYPE_NAME, composition );

	m_compositionSettings = m_settings.getSubParameterValues( COMPOSITION_NAME, composition );

	return true;
}
