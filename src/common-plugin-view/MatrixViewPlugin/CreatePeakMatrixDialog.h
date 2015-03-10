/**
 * @file CreatePeakMatrixDialog.h
 * @brief interfaces of CreatePeakMatrixDialog class
 *
 * @author M.Izumi
 * @date 2012.10.11
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_CREATEPEAKMATRIX_DIALOG_H__
#define __KOME_CREATEPEAKMATRIX_DIALOG_H__

#include <wx/wx.h>
#include <wx/wizard.h> 


#define PEAK_MATRIX_NAME_PANEL		0
#define GROUP_SETTINGS_PANEL		1
#define PEAK_ANALYSIS_PANEL			6


namespace kome {
	namespace matrix {
		namespace view {
		
		/**
		 * @class CreatePeakMatrixDialog
		 * @brief create peak matrix dialog class
		 */
		class CreatePeakMatrixDialog : public kome::window::Wizard {
		public:
			/**
			 * @fn CreatePeakMatrixDialog( wxWindow* pParent, bool useSizer, const bool bedit = false )
			 * @brief constructor
			 * @param parent pParent window
			 * @param useSizer user sizer
			 * @param bedit true:edit mode
			 */
			CreatePeakMatrixDialog( wxWindow* pParent, bool useSizer, const bool bedit = false );

			/**
			 * @fn virtual ~CreatePeakMatrixDialog()
			 * @brief destructor
			 */
			virtual ~CreatePeakMatrixDialog();		
	
		protected:
			/** panels */
			std::vector< kome::window::PeakMatrixSettingsPageBase* > m_panels;
		private:
			/** first page */
			kome::window::PeakMatrixSettingsPageBase* m_wizardPage;
			
			/** use sizer */
			bool m_buseSizer;

			/** edit */
			bool m_bEdit;// Editƒpƒlƒ‹‚Ì—L–³

			bool m_bSkip;
			/** progress dialog */
			kome::window::DialogProgress* m_pDlgProgress;

			/** apply all processing */
			bool				m_bApplyAllProcessing;

			/** index skip */
			int m_indexSkip;

			/** project index */
			int m_nProject;
			/** index */
			int m_index;
		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer();
			 * @brief creates main sizer (override method)
			 * @return field sizer
			 */
			virtual wxSizer* createMainSizer();
		
			/**
			 * @fn void OnCancel( wxWizardEvent& evt )
			 * @brief This method is called when the "Cancel" button is clicked.
			 * @param[in] evt event information
			 */
			void OnCancel( wxWizardEvent& evt );

			/**
			 * @fn void OnFinished( wxWizardEvent& evt )
			 * @brief This method is called when the "Finish" button is clicked.
			 * @param[in] evt event information
			 */
			void OnFinished( wxWizardEvent& evt );
			
		public :
			/** 
			 * @fn wxWizardPage* GetFirstPage()
			 * @brief  get first peage
			 * @return wizard page
			 */
			wxWizardPage* GetFirstPage();
				
			/**
			 * @fn int getProject()
			 * @brief gets project
			 * @return project ID to be selected
			 */
			int getProject();

			/**
			 * @fn bool IsAnyPanelEdited()
			 * @brief Check edited state of all panels
			 * @return If any panel is edited, this method will return true, otherwise false.
			 */
			bool IsAnyPanelEdited();
					
			/**
			 * @fn bool ValidateAllPanelSettings()
			 * @brief This method checks settings on all Peak Matrix Settings panels.
			 * @return true / false
			 */
			bool ValidateAllPanelSettings();
	
			/**
			 * @fn bool SaveMultiplePanelSettings(int iFirstPanelToSave, int iLastPanelToSave)
			 * @brief save multiple panel settings (must be saved in order)
			 * @param iFirstPanelToSave ID of first panel to save
			 * @param iLastPanelToSave ID of last panel to save
			 */
			bool SaveMultiplePanelSettings(int iFirstPanelToSave, int iLastPanelToSave );

			/**
			 * @fn int GetTotalNumberOfSamples()
			 * @brief gets the number of samples that are currently registered on the Group Settings panel.
			 * @return the number of samples that are registered in the current matrix.
			 */
			int GetTotalNumberOfSamples();
		
			/**
			 * @fn bool SampleSelectedAsStandardOnGetPPFillMorPeakAnalysisPanels(const char* sSample)
			 * @brief check whether a sample is selected as a standard sample on either the Get Peak Positions or Fill Matrix panel.
			 * @param sSample sample filename
			 * @return If the sample is selected as a standard sample, return true.
			 */
			bool SampleSelectedAsStandardOnGetPPFillMorPeakAnalysisPanels(const char* sSample);

			/**
			 * @fn void UpdateStandardSamplesOnGetPPFillMandPeakAnalysisPanels()
			 * @brief update the Standard Samples presented on the Get Peak Positions and Fill Matrix panels.
			 */
			void UpdateStandardSamplesOnGetPPFillMandPeakAnalysisPanels();
			
			/**
			 * @fn void GetMatrixSamplesFromGroupSettingsPanel(std::vector< std::string > *pSamples)
			 * @brief gets the list of samples that are currently registered on the Group Settings panel.
			 * @param pSamples array of samples
			 */
			void GetMatrixSamplesFromGroupSettingsPanel(std::vector< std::string > *pSamples);

			/**
			 * @fn void SetActiveMatrixInAllPanels(int nProject)
			 * @brief sets the active matrix in all Peak Matrix Settings panels
			 * @param nProject matrix index
			 */
			void SetActiveMatrixInAllPanels(int nProject);

			/**
			 * @fn kome::window::DialogProgress* GetDialogProgressPtr()
			 * @brief Gets the pointer to the DialogProgress object.
			 * @return the pointer to the DialogProgress object.
			 */
			inline kome::window::DialogProgress* GetDialogProgressPtr() {return m_pDlgProgress;}

			/**
			 * @fn void SetDialogProgressPtr(kome::window::DialogProgress* pDlgProgress)
			 * @brief Sets the pointer to the DialogProgress object.
			 * @param pointer to the DialogProgress object
			 */
			inline void SetDialogProgressPtr(kome::window::DialogProgress* pDlgProgress) {m_pDlgProgress = pDlgProgress;}

			/**
			 * @fn bool ApplyAllProcessing()
			 * @brief Indicates whether or not Apply All processing is being executed.
			 * @return if true, Apply All processing is being executed.
			 */
			inline bool ApplyAllProcessing() {return m_bApplyAllProcessing;}

			/**
			 * @fn int getIndexSkip()
			 * @brief get index skip
			 * @return index skip
			 */
			int getIndexSkip(){ return m_indexSkip; }

			/**
			 * @fn void setIndexSkip( int index )
			 * @brief set index skip
			 * @param[in] index index skip
			 */
			void setIndexSkip( int index ){ m_indexSkip = index; }

			/**
			 * @fn bool getbEdit()
			 * @brief Get the state of the edit panel
			 * @return state edit panel
			 */
			bool getbEdit(){ return m_bEdit; }
			
			/**
			 * @fn void setbEdit( bool bEdit )
			 * @brief Set the state of the edit panel
			 * @param[in] bEdit  state edit panel
			 */
			void setbEdit( bool bEdit ){ m_bEdit = bEdit; }

			/**
			 * @fn wxWizardPageSimple* getPanel( int index )
			 * @brief Get the Page Panel
			 * @param[in] index panel index
			 * @return  panel
			 */
			kome::window::PeakMatrixSettingsPageBase* getPanel( int index );
			
			/**
			 * @fn unsigned int getNumberOfPanels()
			 * @brief get the number of panels
			 * @return panel size
			 */
			unsigned int getNumberOfPanels();

			/**
			 * @fn void setSelBtn( int index )
			 * @brief set the select button
			 * @param index
			 */
			void setSelBtn( int index ){ m_index = index; }

			/**
			 * @fn in tgetSelBtn()
			 * @brief get the select button
			 * @return index
			 */
			int getSelBtn(){ return m_index; }

		protected:
			/**
			 * @fn virtual bool SaveAllPanelSettings(bool bOnlyEditedPanels = false)
			 * @brief save settings on all panels
			 * @param bOnlyEditedPanels apply only those panels which are edited?
			 * @return If any panel failed to be saved, this method returns false.
			 */
			bool SaveAllPanelSettings(kome::window::DialogProgress& progressDlg, bool bOnlyEditedPanels = false);

			/**
			 * @fn bool PreApplyValidateAllPanels()
			 * @brief perform the pre-apply validation checks on all panels
			 * @return If any panel failed its pre-apply validation checks, this method returns false.
			 */
			bool PreApplyValidateAllPanels();

			/**
			 * @fn void SetAllPanelsToNotEdited()
			 * @brief Set edited state of all panels to false
			 */
			void SetAllPanelsToNotEdited();
	
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value data to window controls
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();

			/**
			 * @fn void SaveProcOfEditMode()
			 * @brief Saving process of edit mode
			 */
			void SaveProcOfEditMode();

		protected:
			/**
			 * @fn virtual void onSkip( wxWizardPage* currentPage )
			 * @brief This method is called when the Skip button is clicked.
			 * @param[in] currentPage current page
			 */
			virtual void onSkip( wxWizardPage* currentPage );

			/**
			 * @fn virtual bool canSkip( wxWizardPage* currentPage )
			 * @brief judges whether the Skip button is enabled or not
			 * @return If true, the Skip button is enabled.
			 */
			virtual bool canSkip( wxWizardPage* currentPage );

			/**
			 * @fn virtual void onNext( wxWizardPage* currentPage );
			 * @brief This method is called when the Next button is clicked.
			 * @param[in] currentPage current page
			 */
			virtual void onNext( wxWizardPage* currentPage );

			/**
			 * @fn virtual void onPrev( wxWizardPage* currentPage )
			 * @brief This method is called when the Back button is clicked.
			 * @param[in] currentPage current page
			 */
			virtual void onPrev( wxWizardPage* currentPage );


		private:
			DECLARE_EVENT_TABLE();

			};
		}
	}
}

#endif  // __KOME_CREATEPEAKMATRIX_DIALOG_H__
