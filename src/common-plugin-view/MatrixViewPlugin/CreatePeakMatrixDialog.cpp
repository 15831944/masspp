/**
 * @file CreatePeakMatrixDialog.cpp
 * @brief interfaces of CreatePeakMatrixDialog class
 *
 * @author M.Izumi
 * @date 2012.10.11
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#define BORDER_SIZE			3
#define GRID_GAP			3

#include "stdafx.h"
#include "CreatePeakMatrixDialog.h"
#include "EditionItemSelectPanel.h"
#include "PeakMatrixNamePanel.h"
#include "GroupSettingsPanel.h"
#include "NormalizationPanel.h"
#include "PeakAnalysisPanel.h"
#include "ConfirmationPanel.h"
#include "RTAlignmentPanel.h"
#include "MatrixViewManager.h"
#include "PeakDetectionPanel.h" 

#include <sstream>

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


BEGIN_EVENT_TABLE( CreatePeakMatrixDialog, kome::window::Wizard )
	EVT_WIZARD_CANCEL( wxID_ANY,  CreatePeakMatrixDialog::OnCancel )
	EVT_WIZARD_FINISHED( wxID_ANY, CreatePeakMatrixDialog::OnFinished )
END_EVENT_TABLE()

// constructor
CreatePeakMatrixDialog::CreatePeakMatrixDialog( wxWindow* pParent, bool useSizer, const bool bedit )
	: kome::window::Wizard(pParent, wxT("Create Peak Matrix"), wxNullBitmap, true ) {
	m_buseSizer = useSizer;
	m_wizardPage = NULL;
	
	m_panels.clear();
	m_nProject = 0;
	m_bEdit = bedit;
	m_indexSkip = -1;
	m_bSkip = false;

	createMainSizer();

	if( m_bEdit ){
		// selected project
		kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
		m_nProject = identMgr.getSelectedMatrix();

		SetActiveMatrixInAllPanels(m_nProject);
	}
}

// destructor
CreatePeakMatrixDialog::~CreatePeakMatrixDialog(){
}

// create top sizer
wxSizer* CreatePeakMatrixDialog::createMainSizer(){
	m_panels.push_back( new PeakMatrixNamePanel( this, m_bEdit ) );		
	m_panels.push_back( new GroupSettingsPanel( this  ) );
	m_panels.push_back( new NormalizationPanel( this ) );
	m_panels.push_back( new RTAlignmentPanel( this ) );
	m_panels.push_back( new PeakDetectionPanel( this ) );
	m_panels.push_back( new PeakAnalysisPanel( this ) );
	
	// Quantitation window call type
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	const unsigned int num = plgMgr.getNumberOfFunctionItems( "QUANTITATION_WINDOW" );
	for( unsigned int i=0; i < num; i++ ){
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "QUANTITATION_WINDOW", i );
		if( item != NULL ){
			kome::objects::Parameters params;
			kome::plugin::PluginCallTool::setWindow( params, this );
			kome::plugin::PluginCallTool::setString( params, "panelName", item->getLongName() );
			
			kome::window::PeakMatrixSettingsPageBase* p = (kome::window::PeakMatrixSettingsPageBase*)item->getCall()->invoke( &params ).prim.pt;
			if( p != NULL ){
				
				if( m_bEdit ){
					p->SetEdited( true );
				}
				m_panels.push_back( p );
			}
		}
	}

	// add pages
	if( m_bEdit ){
		std::vector<kome::window::PeakMatrixSettingsPageBase*>tmp;
		for( unsigned int i=0; i < m_panels.size(); i++ ){
			if( i==0 ){
				tmp.push_back( new EditionItemSelectPanel( this ) );
			}
			tmp.push_back( m_panels[i] );
		}
		m_panels.clear();
		m_panels = tmp;
	}

	m_panels.push_back( new ConfirmationPanel( this ) );
	
	// set panel
	for( unsigned int i=0; i < m_panels.size(); i++ ){
	
		if( i != 0 ){
			wxWizardPageSimple::Chain( m_panels[i-1], m_panels[i] );
			m_panels[i]->SetPrevPage( m_panels[i-1] );
		}else{
			m_panels[i]->SetPrevPage( NULL );
		}

		if( (i+1) < m_panels.size() ){
			m_panels[i]->SetNextPage( m_panels[i+1] );
		}
	}
	
	// set first page
	m_wizardPage =  m_panels[0];

	if( m_buseSizer ){
		GetPageAreaSizer()->Add( m_wizardPage ) ;
	}

	
	return GetPageAreaSizer();
}

// get first page
wxWizardPage* CreatePeakMatrixDialog::GetFirstPage(  ){
	return m_wizardPage;
}

// get project
int CreatePeakMatrixDialog::getProject() {
	return m_nProject;
}

// Saving process of edit mode
void CreatePeakMatrixDialog::SaveProcOfEditMode(){
	// EditionItemSelectPanelが入ってるためindex+1
	int index = m_index+1;
	int nApplyablePanelCount = 0;
		
	if( (index > -1) && m_panels[index]->IsValid() ){
		// Validation succeeded, so save this panel's settings.
		std::string sError;
		if (m_panels[index]->PreApplyValidationCheck(sError))
		{

			if (m_panels[index]->SavePanelSettings(m_pDlgProgress) && !m_pDlgProgress->isStopped() )
			{
				m_panels[index]->SetEdited(false);
			}
		}
		else
		{
			// Display any errors that occur.
			kome::window::WindowTool::showInfo(sError.c_str());
		}
	}

	if( !m_pDlgProgress->isStopped() ){
		m_pDlgProgress->fill();
	}
}


// on cancel
void CreatePeakMatrixDialog::OnCancel( wxWizardEvent& evt ){
}

// on finished
void CreatePeakMatrixDialog::OnFinished( wxWizardEvent& evt ){
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	MatrixViewManager& mgr = MatrixViewManager::getInstance();
	mgr.toggleDialog();

	// progress dialog
	kome::window::DialogProgress progressDlg( kome::window::WindowTool::getMainWindow(), "Applying Peak Matrix Settings" );
	m_pDlgProgress = &progressDlg;

	if( m_bEdit ){
		// Edit Mode
		// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Modify> Y.Takayama(SSD) ->
		//SaveProcOfEditMode();
		if( m_bSkip )
		{
			SaveMultiplePanelSettings( m_index + 1, m_indexSkip );
		} else {
			SaveMultiplePanelSettings( m_index + 1, m_panels.size() - 2 );
		}
		// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Modify> Y.Takayama(SSD) <-
	}else{
		if( m_bSkip )
		{
			SaveMultiplePanelSettings( 0, m_indexSkip );
		}else if (ValidateAllPanelSettings() && PreApplyValidateAllPanels())
		{	// Validation succeeded, so save the settings.
			SaveAllPanelSettings( *m_pDlgProgress );
		}
	}
	
	if( !m_pDlgProgress->isStopped() ){
		identMgr.commit();	 
		m_pDlgProgress->fill();
	}else{
		identMgr.rollback();
	}
	m_bSkip = false;
	
	mgr.updateMatrixList();
}

// validate all panel settings
bool CreatePeakMatrixDialog::ValidateAllPanelSettings()
{
	bool bAreAllPanelsValid = true;

	for( unsigned int i = 0; (i < m_panels.size()); i++ ) {
		if ( (m_panels[ i ]->IsEdited()) && (!m_panels[ i ]->IsValid()) )
		{
			bAreAllPanelsValid = false;
		}
	}

	return bAreAllPanelsValid;
}

// save all panel settings
bool CreatePeakMatrixDialog::SaveAllPanelSettings(kome::window::DialogProgress& progressDlg, bool bOnlyEditedPanels)
{
	bool bSuccess = true;

	// Find out how many panels will be applied by this action.
	int iFirstApplyablePanelIndex = -1;
	int nApplyablePanelCount = 0;
	for( unsigned int i = 0; (i < m_panels.size()) && (iFirstApplyablePanelIndex == -1); i++ )
	{
		if(  (m_panels[ i ]->IsEdited() || !bOnlyEditedPanels ))
		{
			iFirstApplyablePanelIndex = i;
		}
	}
	nApplyablePanelCount = m_panels.size() - iFirstApplyablePanelIndex;

	progressDlg.createSubProgresses(nApplyablePanelCount);
	int iSubProgressIndex = 0;

	m_bApplyAllProcessing = true;
	for( unsigned int i = iFirstApplyablePanelIndex; (i < m_panels.size()) && bSuccess; i++ )
	{
		kome::core::Progress *pSubProgress = progressDlg.getSubProgress(iSubProgressIndex);

		if (!bOnlyEditedPanels || m_panels[ i ]->IsEdited())
		{
			if (m_panels[ i ]->SavePanelSettings(pSubProgress))
			{
				m_panels[ i ]->SetEdited(false);
			}
			else
			{	// There was a failure.
				bSuccess = false;
			}
		}

		iSubProgressIndex++;
	}
	m_bApplyAllProcessing = false;

	return bSuccess;
}

// pre apply validate All panels
bool CreatePeakMatrixDialog::PreApplyValidateAllPanels()
{
	bool bSuccess = true;

	std::ostringstream errorString;

	// Check all panels. NOTE: Do not stop if 1 panel fails -
	// we must check all and collate any error messages.
	for( unsigned int i = 0; i < m_panels.size(); i++ )
	{
		if (m_panels[ i ] != NULL)
		{
			std::string sError;

			if (( m_panels[ i ]->IsEdited() ) && (!m_panels[ i ]->PreApplyValidationCheck(sError)))
			{	// There was a failure.

				if (!errorString.str().empty())
				{
					errorString << "\n\n";
				}

				errorString << m_panels[ i ]->GetPanelName().c_str() << " tab:\n" << sError;

				bSuccess = false;
			}
		}
	}

	if (!bSuccess)
	{
		// Display any errors that occur.
		kome::window::WindowTool::showInfo(errorString.str().c_str());
	}

	return bSuccess;
}

// save multiple panel settings (must be saved in order)
bool CreatePeakMatrixDialog::SaveMultiplePanelSettings(int iFirstPanelToSave, int iLastPanelToSave)
{
	bool bSuccess = true;
	
	// Find out how many panels will be applied by this action.
	int iFirstApplyablePanelIndex = -1;
	int nApplyablePanelCount = 0;
	for( unsigned int i = 0; (i < m_panels.size()) && (iFirstApplyablePanelIndex == -1); i++ )
	{
	
		if ( ((i >= static_cast<unsigned int>(iFirstPanelToSave)) && (i <= static_cast<unsigned int>(iLastPanelToSave))) &&
//			((m_panels[ i ]->IsEdited()) && m_panels[ i ]->IsValid()) )   //  実行してほしいパネルでも、無編集の場合処理が行われない　@date 2014.09.30 <Delete> Fujita
			( m_panels[ i ]->IsValid()) )
		{
			iFirstApplyablePanelIndex = i;
		}
	}
	nApplyablePanelCount = iLastPanelToSave - iFirstApplyablePanelIndex + 1;

	m_pDlgProgress->createSubProgresses(nApplyablePanelCount);
	int iSubProgressIndex = 0;

	for( int i = iFirstApplyablePanelIndex; (i <= iLastPanelToSave) && bSuccess; i++ )
	{
		kome::core::Progress *pSubProgress = m_pDlgProgress->getSubProgress(iSubProgressIndex);

		if (m_panels[ i ]->SavePanelSettings(pSubProgress))
		{
			m_panels[ i ]->SetEdited(false);
		}
		else
		{	// There was a failure.
			bSuccess = false;
		}

		iSubProgressIndex++;
	}

	return bSuccess;
}

// is any panel edited
bool CreatePeakMatrixDialog::IsAnyPanelEdited()
{
	bool bIsAnyPanelEdited = false;

	for( unsigned int i = 0; (i < m_panels.size()) && !bIsAnyPanelEdited; i++ ) {
		if (m_panels[ i ]->IsEdited())
		{
			bIsAnyPanelEdited = true;
		}
	}

	return bIsAnyPanelEdited;
}

// Set all panels to not edited
void CreatePeakMatrixDialog::SetAllPanelsToNotEdited()
{
	for( unsigned int i = 0; i < m_panels.size(); i++ )
	{
		m_panels[ i ]->SetEdited(false);
	}
}

// gets the number of samples that are currently registered on the Group Settings panel.
int CreatePeakMatrixDialog::GetTotalNumberOfSamples()
{
	int groupSetting = GROUP_SETTINGS_PANEL;
	if( m_bEdit ){
		groupSetting++;
	}
	return dynamic_cast<GroupSettingsPanel*>(m_panels[ groupSetting ])->GetTotalNumberOfSamples();
}

// Check whether a sample is selected as a standard sample on either the Get Peak Positions or Fill Matrix panel.
bool CreatePeakMatrixDialog::SampleSelectedAsStandardOnGetPPFillMorPeakAnalysisPanels(const char* sSample)
{
	bool bIsSelected = false;
	int groupSetting = GROUP_SETTINGS_PANEL;
	if( m_bEdit ){
		groupSetting++;
	}
	if( m_panels[ groupSetting ]->IsSelectedStandardSample(sSample) ){
		bIsSelected = true;
	}

	return bIsSelected;
}

// Update the Standard Samples presented on the Get Peak Positions and Fill Matrix panels.
void CreatePeakMatrixDialog::UpdateStandardSamplesOnGetPPFillMandPeakAnalysisPanels()
{
	int settingPanel = GROUP_SETTINGS_PANEL;
	if( m_bEdit ){
		settingPanel++;
	}
	m_panels[ settingPanel ]->updateStandardSamples( false );

}

// set active matrix in all panels
void CreatePeakMatrixDialog::SetActiveMatrixInAllPanels(int nProject)
{
	for( unsigned int i = 0; i < m_panels.size(); i++ ) {

		m_panels[ i ]->SetActiveMatrix(nProject);
		
		if( getbEdit() ){
			m_panels[ i ]->RefreshPanelContentsForMatrix();
		}
	}
}

// gets the list of samples that are currently registered on the Group Settings panel.
void CreatePeakMatrixDialog::GetMatrixSamplesFromGroupSettingsPanel(std::vector< std::string > *pSamples)
{
	int groupSettings = GROUP_SETTINGS_PANEL;
	if( m_bEdit ){
		groupSettings++;
	}
	dynamic_cast<GroupSettingsPanel*>(m_panels[ groupSettings ])->GetMatrixSamples(pSamples);
}

// get panel
kome::window::PeakMatrixSettingsPageBase* CreatePeakMatrixDialog::getPanel( int index ){
	return m_panels[ index ];
}

// get number of panels
unsigned int CreatePeakMatrixDialog::getNumberOfPanels(){
	return m_panels.size();
}

// transfer data from window
bool CreatePeakMatrixDialog::TransferDataFromWindow() {
	// default
	if( !wxWizard::TransferDataFromWindow() ) {
		return false;
	}
		
    return true;
}

// transfer data to window
bool CreatePeakMatrixDialog::TransferDataToWindow() {
	
	return true;
}

// on skip
void CreatePeakMatrixDialog::onSkip( wxWizardPage* currentPage ){
	int isize = m_panels.size()-1;
	wxWizardPageSimple::Chain( (wxWizardPageSimple*)currentPage, m_panels[isize] );

	for( unsigned int i=0; i < m_panels.size(); i++ ){
		if( currentPage == m_panels[i] ){
			m_indexSkip = i;
		}
	}
	
	m_bSkip = true;
}

// can skip
bool CreatePeakMatrixDialog::canSkip( wxWizardPage* currentPage ){
	// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Delete> Y.Takayama(SSD) ->
	//if( m_bEdit ){
	//	return false;
	//}
	// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Delete> Y.Takayama(SSD) <-
	
	kome::window::PeakMatrixSettingsPageBase* currentPanel = dynamic_cast<kome::window::PeakMatrixSettingsPageBase*>( currentPage );
	for( unsigned int i=0; i < m_panels.size(); i++ ){
		if( currentPanel == m_panels[i] ){
			// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Add> Y.Takayama(SSD) ->
			if( m_bEdit ){
				// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.18 <Modify> N.Tashiro(SSD) ->
				// アイテム選択画面ではSkipできない。
				//if( i == ( m_panels.size() - 1 )){
				if((i == (m_panels.size() - 1)) || (i == 0)){
					return false;
				} else {
					return true;
				}
			}
			// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Add> Y.Takayama(SSD) <-
			return m_panels[i]->IsSkipButton();
		}
	}

	return false;
}

// on next
void CreatePeakMatrixDialog::onNext( wxWizardPage* currentPage ){
	kome::window::PeakMatrixSettingsPageBase* currentPanel = dynamic_cast<kome::window::PeakMatrixSettingsPageBase*>( currentPage );
	
	if( currentPanel == NULL ){
		return;
	}
	int index = m_panels.size()-1;
	for( unsigned int i=0; i < m_panels.size(); i++ ){
		if( currentPanel == m_panels[i] ){
			if( m_bEdit ){
				if( currentPage != m_panels[0] ){
					// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Modify> Y.Takayama(SSD) ->
					// 最終パネルに移行→次パネルに移行
					//if( currentPage != m_panels[index] ){
					//	currentPanel->SetNext( m_panels[index] );
					//}
					currentPanel->SetNext( ( (i+1) < m_panels.size() ) ? m_panels[i+1] : NULL ); 
					// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Modify> Y.Takayama(SSD) <-
				}else{
					currentPanel->SetNext( m_panels[m_index+1] );
				}
			}else{
				currentPanel->SetNext( ( (i+1) < m_panels.size() ) ? m_panels[i+1] : NULL ); 
			}
			
			return;
		}
	}
}

// on prev 
void CreatePeakMatrixDialog::onPrev( wxWizardPage* currentPage ){
	kome::window::PeakMatrixSettingsPageBase* currentPanel = dynamic_cast<kome::window::PeakMatrixSettingsPageBase*>( currentPage );
	if( currentPanel == NULL ){
		return;
	}
	int index = m_panels.size()-1;
	// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Modify> Y.Takayama(SSD) ->
	// 基本は新規作成時と同じ、但し、m_index + 1までしか戻れず、現在がm_index+1なら0に戻るようにする		
#if 0	// Original Code
	for( unsigned int i=0; i < m_panels.size(); i++ ){
		if( m_bEdit ){
			if( currentPage == m_panels[index] ){
				currentPanel->SetPrev( m_panels[m_index+1] );
			}else{
				currentPanel->SetPrev( m_panels[0] );
			}
		}
		else
		{
			if( currentPanel == m_panels[i] ){
				if( i != 0 ){
					if( m_indexSkip != -1 ){
						currentPanel->SetPrev( m_panels[ m_indexSkip ] );
						m_indexSkip = -1;	// Spec No.92279 :ピークマトリックス作成wizardで前のステップに戻れない  @date 2013.06.21 <Add> M.Izumi
					}else{
						currentPanel->SetPrev( m_panels[i-1] );
					}
				}else{
					currentPanel->SetPrev( NULL );
				}
			}
		}
	}
#endif
	for( unsigned int i=0; i < m_panels.size(); i++ ){
		if( currentPanel == m_panels[i] ){
			if( m_bEdit ){
				if( currentPanel == m_panels[m_index+1] ){
					currentPanel->SetPrev( m_panels[0] );
					return;
				}
			}		
			
			if( i != 0 ){
				if( m_indexSkip != -1 ){
					currentPanel->SetPrev( m_panels[ m_indexSkip ] );
					m_indexSkip = -1;	// Spec No.92279 :ピークマトリックス作成wizardで前のステップに戻れない  @date 2013.06.21 <Add> M.Izumi
				}else{
					currentPanel->SetPrev( m_panels[i-1] );
				}
			}else{
				currentPanel->SetPrev( NULL );
			}
		}
	}
	// 仕様変更[編集時、対象以降の処理を全て行う] @date 2014.09.17 <Modify> Y.Takayama(SSD) <-
}

