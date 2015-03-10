/**
 * @file ConfirmationPanel.cpp
 * @brief interfaces of ConfirmationPanel class
 *
 * @author M.Izumi
 * @date 2012.10.22
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "CreatePeakMatrixDialog.h"
#include "ConfirmationPanel.h"
#include "PeakMatrixManager.h"
#include "EditionItemSelectPanel.h"


#include <sstream>

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( ConfirmationPanel, kome::window::PeakMatrixSettingsPageBase )
END_EVENT_TABLE()

// constructor
ConfirmationPanel::ConfirmationPanel( wxWizard* pParent )
	: kome::window::PeakMatrixSettingsPageBase( pParent, "Confirmation", false )
{
	m_pSettingsPanel = NULL;
	m_finishTxtCtrl = NULL;
	
	m_pParentDlg = dynamic_cast<CreatePeakMatrixDialog*>( pParent );

	createControls();
}

// destructor
ConfirmationPanel::~ConfirmationPanel()
{
}

// create inner sizer
void ConfirmationPanel::createInnerSizer( wxStaticBoxSizer* InnerSizer )
{

	wxBoxSizer* textSizer = new wxBoxSizer( wxVERTICAL );
	textSizer->Add( new wxStaticText( this, wxID_ANY, 
		wxT("Input operations for peak matrix are below.\n"
		"If there is no problem, click the Finish button to make a peak matrix.\n"
		"Otherwise click the Prev button to change the settings.") ) );

	textSizer->AddSpacer( 10 );

	m_finishTxtCtrl =  new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( 400, 600 ),
		wxHSCROLL | wxTE_MULTILINE | wxTE_READONLY
	);
	textSizer->Add( m_finishTxtCtrl , 1, wxALL | wxGROW |  wxALIGN_CENTER_HORIZONTAL  ); 

	InnerSizer->Add( textSizer, 1, wxALL | wxGROW | wxALIGN_CENTER_HORIZONTAL  , 20 );
	
}

// transefer data from window
bool ConfirmationPanel::TransferDataFromWindow()
{
	// default
	if( !PeakMatrixSettingsPageBase::TransferDataFromWindow() ) {
		return false;
	}
	return true;
}

// transefor Data to window
bool ConfirmationPanel::TransferDataToWindow()
{
	int inum = m_pParentDlg->getNumberOfPanels();
	std::string str = "";
	
	if( m_pParentDlg->getbEdit() ){
		// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Modify> Y.Takayama(SSD) ->
		//PeakMatrixSettingsPageBase* panel = dynamic_cast<PeakMatrixSettingsPageBase*>( m_pParentDlg->getPanel(m_pParentDlg->getSelBtn()+1));
		//if( panel != NULL ){
		//	str.append( panel->getSettingParam().c_str() );
		//}
		if( m_pParentDlg->getIndexSkip() != -1 ){	//スキップあり
			inum = m_pParentDlg->getIndexSkip() + 1;
			// memo
			//	もし、新規作成時もスキップを考慮するなら、このifブロックはinum設定直後に移動
		}

		for( int i=m_pParentDlg->getSelBtn()+1; i < inum; i++ ){
			PeakMatrixSettingsPageBase* tmp = dynamic_cast<PeakMatrixSettingsPageBase*>(m_pParentDlg->getPanel(i));
			str.append( tmp->getSettingParam().c_str() );
		}
		// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Modify> Y.Takayama(SSD) <-
	}else{
		for( int i=0; i < inum; i++ ){
			PeakMatrixSettingsPageBase* tmp = dynamic_cast<PeakMatrixSettingsPageBase*>(m_pParentDlg->getPanel(i));
			str.append( tmp->getSettingParam().c_str() );
		}
	}
	// default
	if( m_finishTxtCtrl != NULL ){
		m_finishTxtCtrl->SetValue( str );
	}
	
	// default
	return true;
}

// Validate panel settings 
bool ConfirmationPanel::IsValid()
{
	return true;
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool ConfirmationPanel::PreApplyValidationCheck(std::string& sError)
{
	bool bSuccess = true;

	std::ostringstream errorString;


	sError = errorString.str();

	return bSuccess;
}

// save panel settings
bool ConfirmationPanel::SavePanelSettings(kome::core::Progress *pProgress)
{
	if( !TransferDataFromWindow() ) {
		return false;
	}

	pProgress->setStatus(GetPanelName().c_str());

	std::ostringstream infoString;

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = false;

	return (bSuccess) && !pProgress->isStopped();
}

// Update standard samples on this panel
void ConfirmationPanel::updateStandardSamples(bool bReadFromINIfile){
}

// Refresh panel contents for matrix
void ConfirmationPanel::RefreshPanelContentsForMatrix(){
}
