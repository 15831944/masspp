/**
 * @file PeakMatrixNamePanel.cpp
 * @brief interfaces of PeakMatrixNamePanel class
 *
 * @author M.Izumi
 * @date 2012.10.11
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "CreatePeakMatrixDialog.h"
#include "PeakMatrixNamePanel.h"
#include "PeakMatrixManager.h"
#include "MatrixViewManager.h"

#include "matrix_flow01.xpm"

#include <sstream>

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define BORDER_SIZE						3
#define  NAME_PARAM						"MATRIX_NAME"

BEGIN_EVENT_TABLE( PeakMatrixNamePanel, kome::window::PeakMatrixSettingsPageBase )
	EVT_KOME_SETTINGS_PANEL(wxID_ANY, PeakMatrixNamePanel::onSettingChanged)
END_EVENT_TABLE()

// constructor
PeakMatrixNamePanel::PeakMatrixNamePanel( wxWizard* pParent, bool bEdit )
	: kome::window::PeakMatrixSettingsPageBase( pParent, "Create Peak Matrix", false ),
	m_params( NULL, false ), 
	m_bEdit ( bEdit )
{
	m_pSettingsPanel = NULL;
	m_pParentDlg = dynamic_cast<CreatePeakMatrixDialog*>( pParent );
			
	m_selMatrixName="";

	createControls();
}

// destructor
PeakMatrixNamePanel::~PeakMatrixNamePanel(){
}

// create Inner Sizer
void PeakMatrixNamePanel::createInnerSizer(wxStaticBoxSizer* InnerSizer){
	// set bitmap
	wxBitmap addBmp( matrix_flow01_xpm );
	
	wxBoxSizer* pCategoriesSizer = new wxBoxSizer( wxVERTICAL );
	pCategoriesSizer->Add(  new wxStaticBitmap( this, wxID_ANY, addBmp ), 1, (wxALL & ~wxRIGHT) | wxGROW, BORDER_SIZE );

	wxSizer* gHoriznSizer = new wxBoxSizer( wxHORIZONTAL);
	gHoriznSizer->Add( pCategoriesSizer, 1, (wxALL & ~wxRIGHT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	gHoriznSizer->AddSpacer( 10 );
	InnerSizer->Add( gHoriznSizer );
	// field sizer	
	wxBoxSizer* textSizer = new wxBoxSizer( wxVERTICAL );
	textSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT("This window helps you to create peak matrix for quantitation.")),
		0,
		wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT,
		BORDER_SIZE
	);
	textSizer->AddSpacer( 50 );
	textSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT("Input the peak matrix name.")),
		0,
		wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT,
		BORDER_SIZE
	);
	textSizer->AddSpacer( 50 );
	
	// @date 2013.03.13 <Mod> M.Izumi ->
	if( m_bEdit ) {
		// manager
		kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

		int isel = identMgr.getSelectedMatrix();
		if( isel < 0 ) {
			kome::window::WindowTool::showInfo( "There is no active matrix. Select a matrix." );
		}

		// name
		m_selMatrixName = identMgr.getMatrixName( isel );
	}
	// @date 2013.03.13 <Mod> M.Izumi <-

	// parameters
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setName( "matrixName" );
	// name
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setName( "name" );
	val->setRequired( true );
	val->setDefaultValue( m_selMatrixName.c_str() );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( NAME_PARAM );
	param->setValue( val );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Name" );

	if( m_bEdit ){
		page->setTitle("Edit Matrix Name");
	}else{
		page->setTitle("New Matrix");
	}
	
	m_pSettingsPanel = new kome::window::SettingsPanel( this, page, false, &m_settings );
		
	textSizer->Add(
		m_pSettingsPanel,
		0,
		wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT | wxRIGHT | wxGROW,
		BORDER_SIZE
	);
			
	InnerSizer->Add( textSizer, 1, wxALL | wxGROW, 20 );	
	
}

// transfer data from window
bool PeakMatrixNamePanel::TransferDataFromWindow(){
	// default
	if( !PeakMatrixSettingsPageBase::TransferDataFromWindow() ) {
		return false;
	}

	const char* strName = m_settings.getParameterValue( NAME_PARAM );
	
	std::string s = FMT( "- %s(Name = %s)\n", m_sPanelName.c_str(), strName  );
	setSettingParam( s.c_str() );

	return true;
}

// transfer data to window
bool PeakMatrixNamePanel::TransferDataToWindow(){
	const char* strName = m_settings.getParameterValue( NAME_PARAM );
	// Spec No.92278 : ピークマトリックスの名称が消える @date 2013.06.20 <Add> M.Izumi
	if( !m_bEdit ){
		m_settings.setValue( NAME_PARAM, strName );
	}else{
		m_pSettingsPanel->setDefaultValues( true, true );
	}
	
	return true; 
}

// Validate panel settings 
bool PeakMatrixNamePanel::IsValid()
{
	return m_pSettingsPanel->Validate();
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool PeakMatrixNamePanel::PreApplyValidationCheck(std::string& sError)
{
	bool bSuccess = true;

	std::ostringstream errorString;

	sError = errorString.str();

	return bSuccess;
}

// save panel settings
bool PeakMatrixNamePanel::SavePanelSettings(kome::core::Progress *pProgress)
{
	if( !TransferDataFromWindow() ) {
		return false;
	}

	pProgress->setStatus(GetPanelName().c_str());

	std::ostringstream infoString;

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected project
	int isel = identMgr.getSelectedMatrix();
	if( m_bEdit && isel < 0 ) {
		kome::window::WindowTool::showInfo( "There is no active matrix. Select a matrix." );
		return bSuccess;
	}

	// name
	std::string name;
	if( m_bEdit ) {
		name = identMgr.getMatrixName( isel );
	}

	const char* n = m_settings.getParameterValue( NAME_PARAM );
	if( MatrixNameExists(n) ){
		kome::window::WindowTool::showInfo( "A matrix with that name already exists." );
	}else{
		// CheckForUnsavedChangesAndValidate is only called if editMode is FALSE,
		// i.e. if a new matrix is added. Not called if editMode is TRUE, i.e. if
		// a matrix name is edited.
		
		// update
		if( m_bEdit ){
			
			identMgr.updateMatrix( isel, n, "" );
			
		}else{
			isel = identMgr.addMatrix( n, "", true, true, false );
			SetActiveMatrix( isel  );
		}

		// Commit the changes
		int project = m_pParentDlg->getProject();
		if( project >= 0 ){
			identMgr.selectMatrix( isel );
		}

		MatrixViewManager& viewMgr = MatrixViewManager::getInstance();
		viewMgr.setProject();
		viewMgr.updateMatrixCombo();

		if (!m_bEdit){ // Added a new matrix
			// Refresh the matrix list
			viewMgr.updateMatrixList();
			m_pParentDlg->SetActiveMatrixInAllPanels( isel );
		}
	
		
	}
	bSuccess = true;
	return (bSuccess) && !pProgress->isStopped();
}

// Update standard samples on this panel
void PeakMatrixNamePanel::updateStandardSamples(bool bReadFromINIfile)
{
}

void PeakMatrixNamePanel::updateSettings(){
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
void PeakMatrixNamePanel::RefreshPanelContentsForMatrix()
{
	updateSettings();
}

// on a setting changing in the settings panel
void PeakMatrixNamePanel::onSettingChanged(kome::window::SettingsPanelEvent& evt)
{
	SetEdited();
}

// checks if a matrix with the specified name exists
bool PeakMatrixNamePanel::MatrixNameExists( const char* name )
{
	bool bMatrixNameExists = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// get projects
	std::vector< int > projects;
	identMgr.getMatrices( projects );

	for( unsigned int i = 0; (i < projects.size()) && !bMatrixNameExists; i++ ) {

		int id = projects[ i ];
		if (!strcmp(name, identMgr.getMatrixName( id ).c_str()))
		{
			bMatrixNameExists = true;
		}
	}

	return bMatrixNameExists;
}
