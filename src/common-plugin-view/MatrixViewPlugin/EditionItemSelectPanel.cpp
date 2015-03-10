/**
 * @file EditionItemSelectPanel.cpp
 * @brief interfaces of EditionItemSelectPanel class
 *
 * @author M.Izumi
 * @date 2012.10.29
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "CreatePeakMatrixDialog.h"
#include "PeakMatrixManager.h"
#include "EditionItemSelectPanel.h"

#include <sstream>

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE						3

BEGIN_EVENT_TABLE( EditionItemSelectPanel, kome::window::PeakMatrixSettingsPageBase )
END_EVENT_TABLE()

// constructor
EditionItemSelectPanel::EditionItemSelectPanel( wxWizard* pParent )
	: kome::window::PeakMatrixSettingsPageBase( pParent, "Edition Item Selection", false ), m_params( NULL, false )
{
	m_editRadioBtn = NULL;
	m_pSettingsPanel = NULL;
	
	m_pParentDlg = dynamic_cast<CreatePeakMatrixDialog*>( pParent );

	createControls();
}

// destructor
EditionItemSelectPanel::~EditionItemSelectPanel(){
}

// create inner sizer
void EditionItemSelectPanel::createInnerSizer(wxStaticBoxSizer* InnerSizer){

	wxBoxSizer* textSizer = new wxBoxSizer( wxVERTICAL );
	textSizer->Add( new wxStaticText( this, wxID_ANY, wxT("Select the item to edit.")) );
	textSizer->AddSpacer( 50 );
	
	std::vector< wxString > items;
	int index = m_pParentDlg->getNumberOfPanels();
	for( int i=0; i < index; i++ ){
		kome::window::PeakMatrixSettingsPageBase* pane =  m_pParentDlg->getPanel( i );
		if( pane != NULL ){
			items.push_back( pane->GetPanelName().c_str() );
		}
	}
	
	m_editRadioBtn = new wxRadioBox( 
		this,
		wxID_ANY,
		wxT( "" ),
		wxDefaultPosition,
		wxDefaultSize,
		items.size(),
		&( items[ 0 ] ),
		0,
		wxRA_SPECIFY_ROWS 
	);

	textSizer->Add( m_editRadioBtn, 0, wxGROW | wxALL, BORDER_SIZE );

	InnerSizer->Add( textSizer, 0, wxALL | wxGROW, 20 );	
}

// Validate panel settings 
bool EditionItemSelectPanel::IsValid()
{
	return true;
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool EditionItemSelectPanel::PreApplyValidationCheck(std::string& sError)
{
	bool bSuccess = true;

	return bSuccess;
}

bool EditionItemSelectPanel::SavePanelSettings(kome::core::Progress *pProgress)
{
	if( !TransferDataFromWindow() ) {
		return false;
	}
	
	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = true;

	return (bSuccess) && !pProgress->isStopped();
}

// Update standard samples on this panel
void EditionItemSelectPanel::updateStandardSamples(bool bReadFromINIfile)
{	
}

// Refresh settings for matrix
void EditionItemSelectPanel::updateSettings()
{
	// First of all, update the section for all parameters.
	for (unsigned int i = 0; i < m_params.getNumberOfParams(); i++)
	{
		kome::plugin::SettingsParam* param = m_params.getParam(i);
		param->setSection(GetINISectionNameForMatrix());
	}

	// Refresh the parameters for all pages - this is necessary so that
	// the values are updated in the UI when a different matrix is selected.
	for (unsigned int j = 0; j < m_params.getNumberOfPages(); j++)
	{
		kome::plugin::SettingsPage* page = m_params.getPage(j);
		page->setParameters(m_settings);
	}

	// Load values from the INI file
	// Update the settings in the UI
	TransferDataToWindow();
}

// Refresh panel contents for matrix
void EditionItemSelectPanel::RefreshPanelContentsForMatrix()
{
	updateStandardSamples();

	updateSettings();
}

// transefer data from window
bool EditionItemSelectPanel::TransferDataFromWindow(){
	
	// default
	if( !PeakMatrixSettingsPageBase::TransferDataFromWindow() ) {
		return false;
	}

	if( m_pParentDlg != NULL ){
		m_pParentDlg->setSelBtn( m_editRadioBtn->GetSelection() );
	}
	return true;
}

