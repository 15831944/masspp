/**
 * @file EditSampleUserPropertiesDialog.cpp
 * @brief implementsation of EditSampleUserPropertiesDialog class
 *
 * @author H.Parry
 * @date 2011.04.04
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "EditSampleUserPropertiesDialog.h"
#include "SampleTreeManager.h"
#include "DetailsViewManager.h"
#include "SampleUserPropertiesOperation.h"
#include "DetailsListCtrl.h"
#include "SampleTreeCtrl.h"

using namespace kome::properties;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define ID_SELECT_BUTTON	2010
#define ID_LISTBOX			2011
#define ID_NEW_BUTTON		2012
#define ID_DELETE_BUTTON	2013
#define ID_KEY_EDIT			2014
#define ID_VALUE_EDIT		2015

#define ID_ADD_BUTTON                     66
#define ID_DEL_BUTTON                     68

BEGIN_EVENT_TABLE( EditSampleUserPropertiesDialog, kome::window::StandardDialog )
	EVT_MENU( ID_ADD_BUTTON, EditSampleUserPropertiesDialog::onButtonNew )
	EVT_MENU( ID_DEL_BUTTON, EditSampleUserPropertiesDialog::onButtonDelete )
	EVT_BUTTON( ID_NEW_BUTTON, EditSampleUserPropertiesDialog::onButtonNew )
	EVT_BUTTON( ID_DELETE_BUTTON, EditSampleUserPropertiesDialog::onButtonDelete )
	EVT_TEXT( ID_KEY_EDIT, EditSampleUserPropertiesDialog::onKeyEditChanged )
	EVT_TEXT( ID_VALUE_EDIT, EditSampleUserPropertiesDialog::onValueEditChanged )
END_EVENT_TABLE()

// constructor
EditSampleUserPropertiesDialog::EditSampleUserPropertiesDialog(
		wxWindow* parent
) : kome::window::StandardDialog(
		parent,
		"Sample User Properties")
{
   // initialize
	m_treeCtrl = NULL;
	m_list = NULL;
	m_iSelected = 0;

	m_pDetailsViewMgr = new DetailsViewManager;
	m_pActiveSpectrum = NULL;
	m_pActiveChromatogram = NULL;
	m_pActiveGroup = NULL;
	m_pNewButton = NULL;
	m_pDeleteButton = NULL;
	m_pKeyStaticText = NULL;
	m_pValueStaticText = NULL;
	m_pKeyTextCtrl = NULL;
	m_pValueTextCtrl = NULL;
	m_lSelectedItem = -1;
	m_bIsEdited = false;
}

// destructor
EditSampleUserPropertiesDialog::~EditSampleUserPropertiesDialog() {
	if (m_pDetailsViewMgr != NULL)
	{
		delete m_pDetailsViewMgr;
		m_pDetailsViewMgr = NULL;
	}
}

// empty sample
bool EditSampleUserPropertiesDialog::EmptySample(kome::objects::Sample* sample)
{
	bool bEmptySample = true;
	unsigned int nGroups = sample->getNumberOfGroups();
	kome::objects::DataGroupNode* group = NULL;

	for (unsigned int iGroupIndex = 0; (iGroupIndex < nGroups) && bEmptySample; iGroupIndex++)
	{
		group = sample->getGroup(iGroupIndex);

		if( group == NULL ) {
			LOG_ERROR( FMT( "A group is not active." ) );
		}

		if ( (group->getNumberOfSpectra() > 0) || (group->getNumberOfChromatograms() > 0) )
		{
			bEmptySample = false;
		}
	}

	return bEmptySample;
}

// create top sizer
wxSizer* EditSampleUserPropertiesDialog::createMainSizer() {
	// field sizer
	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	// Get managers
	SampleTreeManager treeMgr;

	// create tree view
	treeMgr.createTreeCtrl(this);
	m_treeCtrl = treeMgr.getTreeCtrl();
	
	mainSizer->Add(m_treeCtrl, 1, wxEXPAND);

	// add sample
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	// アクティブなサンプルのみ表示 @date 2011.09.16 <Mod> M.Izumi
	kome::objects::Sample* sample = aoMgr.getActiveSample();
	if( sample->isOpened() && !EmptySample(sample) ){
		treeMgr.addSample( sample );
	}
		
	wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

	// Get details view manager
	m_pDetailsViewMgr->createPropertiesView(this);
	m_list = m_pDetailsViewMgr->getListCtrl();
	rightSizer->Add(m_list, 1, wxEXPAND);

	wxBoxSizer* textBoxesSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* textSizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_pKeyStaticText = new wxStaticText(this, wxID_ANY, "&Key:");
	textSizer1->Add(m_pKeyStaticText, 0, wxRIGHT, 5);
	m_pKeyTextCtrl = new wxTextCtrl(this, ID_KEY_EDIT);
	textSizer1->Add(m_pKeyTextCtrl);
	textBoxesSizer->Add(textSizer1, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	wxBoxSizer* textSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_pValueStaticText = new wxStaticText(this, wxID_ANY, "&Value:");
	textSizer2->Add(m_pValueStaticText, 0, wxRIGHT, 5);
	m_pValueTextCtrl = new wxTextCtrl(this, ID_VALUE_EDIT);
	textSizer2->Add(m_pValueTextCtrl);
	textBoxesSizer->Add(textSizer2, 0, wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	rightSizer->Add(textBoxesSizer, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | (wxALL & ~wxBOTTOM), 5);

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	m_pNewButton = new wxButton(this, ID_NEW_BUTTON, "&New");
	buttonSizer->Add(m_pNewButton); 
	m_pNewButton->Enable(false);
	m_pDeleteButton = new wxButton(this, ID_DELETE_BUTTON, "&Delete");
	buttonSizer->Add(m_pDeleteButton); 
	rightSizer->Add(buttonSizer, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);

	mainSizer->Add(rightSizer, 1, wxEXPAND);

	NoListItemSelected();
		
	return mainSizer;
}

// set active spectrum
void EditSampleUserPropertiesDialog::SetActiveSpectrum(kome::objects::Spectrum* pActiveSpectrum)
{
	m_pActiveChromatogram = NULL;
	m_pActiveSpectrum = pActiveSpectrum;
	m_pActiveGroup = NULL;

	if (m_pDetailsViewMgr != NULL)
	{
		m_pDetailsViewMgr->selectSpec(m_pActiveSpectrum);

		if (m_pNewButton != NULL)
		{
			m_pNewButton->Enable();
		}

		NoListItemSelected();
		
		m_bIsEdited = false;
	}
}

// set active chromatogram
void EditSampleUserPropertiesDialog::SetActiveChromatogram(kome::objects::Chromatogram* pActiveChromatogram)
{
	m_pActiveChromatogram = pActiveChromatogram;
	m_pActiveSpectrum = NULL;
	m_pActiveGroup = NULL;

	if (m_pDetailsViewMgr != NULL)
	{
		m_pDetailsViewMgr->selectChrom(m_pActiveChromatogram);

		if (m_pNewButton != NULL)
		{
			m_pNewButton->Enable();
		}

		NoListItemSelected();
		
		m_bIsEdited = false;
	}
}

// set active group
void EditSampleUserPropertiesDialog::SetActiveGroup(kome::objects::DataGroupNode* pActiveGroup)
{
	m_pActiveChromatogram = NULL;
	m_pActiveSpectrum = NULL;
	m_pActiveGroup = pActiveGroup;

	if (m_pDetailsViewMgr != NULL)
	{
		m_pDetailsViewMgr->selectGroup(m_pActiveGroup);

		if (m_pNewButton != NULL)
		{
			m_pNewButton->Enable();
		}

		NoListItemSelected();
		
		m_bIsEdited = false;
	}
}

// item selected
void EditSampleUserPropertiesDialog::ItemSelected(const long lSelectedItem,
							const std::string& sKey, const std::string& sValue)
{
	m_lSelectedItem = lSelectedItem;
	m_pKeyStaticText->Enable();
	m_pValueStaticText->Enable();
	m_pKeyTextCtrl->ChangeValue(sKey);
	m_pValueTextCtrl->ChangeValue(sValue);
	m_pKeyTextCtrl->Enable();
	m_pValueTextCtrl->Enable();

	if (m_pDeleteButton != NULL)
	{
		m_pDeleteButton->Enable();
	}

	// Set the focus to the "Key" edit field
	m_pKeyTextCtrl->SetFocus();
	m_pKeyTextCtrl->SelectAll();
}

// no list item selected
void EditSampleUserPropertiesDialog::NoListItemSelected()
{
	m_pKeyStaticText->Enable(false);
	m_pValueStaticText->Enable(false);
	m_pKeyTextCtrl->Enable(false);
	m_pValueTextCtrl->Enable(false);
	m_pKeyTextCtrl->ChangeValue("");
	m_pValueTextCtrl->ChangeValue("");
	if (m_pDeleteButton != NULL)
	{
		m_pDeleteButton->Enable(false);
	}
	m_lSelectedItem = -1;
}

// create a new user property
void EditSampleUserPropertiesDialog::onButtonNew( wxCommandEvent& evt )
{
	if (m_pDetailsViewMgr != NULL)
	{
		m_pDetailsViewMgr->newItem();

		m_bIsEdited = true;
	}
}

// delete a user property
void EditSampleUserPropertiesDialog::onButtonDelete( wxCommandEvent& evt )
{
	if (m_pDetailsViewMgr != NULL)
	{
		NoListItemSelected();

		m_pDetailsViewMgr->deleteItems();

		m_bIsEdited = true;
	}
}

// key edit change
void EditSampleUserPropertiesDialog::onKeyEditChanged( wxCommandEvent& evt )
{
	if (m_pDetailsViewMgr != NULL)
	{
		m_pDetailsViewMgr->updateItem(m_lSelectedItem, 0, m_pKeyTextCtrl->GetValue());

		m_bIsEdited = true;
	}
}

// valued edit changed
void EditSampleUserPropertiesDialog::onValueEditChanged( wxCommandEvent& evt )
{
	if (m_pDetailsViewMgr != NULL)
	{
		m_pDetailsViewMgr->updateItem(m_lSelectedItem, 1, m_pValueTextCtrl->GetValue());

		m_bIsEdited = true;
	}
}

// check for unsaved changes and validate
bool EditSampleUserPropertiesDialog::CheckForUnsavedChangesAndValidate()
{
	bool bRet = true;

	if (m_bIsEdited)
	{
		int nYesNoCancel = kome::window::WindowTool::showYesNoCancel( "Do you want to save your changes?", "Save", "Don't save" );

		switch (nYesNoCancel)
		{
			case 1:	// Yes

				{
					// add Operation
					SampleUserPropertiesOperation* opt = new SampleUserPropertiesOperation();
					opt->setShortName( "user_prop" );
					opt->setOperationName( "Sample User Properties" );
					opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
					if( getActiveSpec() != NULL ){
						opt->setTargetSample( getActiveSpec()->getSample() );
					}
					if(  getActiveChrom() != NULL ){
						opt->setTargetSample(  getActiveChrom()->getSample() );
					}
					if( getActiveGroup() != NULL ){
						opt->setTargetSample( getActiveGroup()->getSample() );
					}
					opt->setSpec( getActiveSpec() );
					opt->setChrom( getActiveChrom() );
					opt->setGroup( getActiveGroup() );
					opt->setEdit( getEdit() );
					opt->setValidate( validateUserProperties() );
				
					kome::core::Properties prop;
					getProperties( prop );
					
					std::vector< std::pair< std::string, std::string > > properties;
					for( unsigned int i=0; i < prop.getNumberOfProperties(); i++ ){
						std::pair< std::string, std::string > tmpPair;

						tmpPair.first = prop.getKey( i );
						tmpPair.second = prop.getValue( i );

						properties.push_back( tmpPair );

					}
					opt->setProperties( properties );

					if( opt->execute() ){
						kome::window::WindowTool::refresh();
					}else{
						delete opt;
						bRet = false;
					}
				}
				
				break;

			case 0: // No
				break;

			default: // Cancel
				bRet = false;
				break;
		}
	}

	return bRet;
}

// transfer data from window
bool EditSampleUserPropertiesDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

    return true;
}

// transfer data to window
bool EditSampleUserPropertiesDialog::TransferDataToWindow() {

	return true;
}

// get properties
void EditSampleUserPropertiesDialog::getProperties( kome::core::Properties& props){
	m_list->getProperties( props );
}

// validate user properties
bool EditSampleUserPropertiesDialog::validateUserProperties(){

	return m_list->validateProperties();
}
