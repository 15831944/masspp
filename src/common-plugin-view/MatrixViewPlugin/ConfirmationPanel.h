/**
 * @file ConfirmationPanel.h
 * @brief interfaces of ConfirmationPanel class
 *
 * @author M.Izumi
 * @date 2012.10.22
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_CONFIRMATION_PANEL_H__
#define __KOME_CONFIRMATION_PANEL_H__

#include <wx/wx.h>
#include <wx/wizard.h>

namespace kome {
	namespace matrix {
		namespace view {

		
		/**
		 * @class ConfirmationPanel
		 * @brief confirmation panel class
		 */
		class ConfirmationPanel : public kome::window::PeakMatrixSettingsPageBase {
		public:
			/**
			 * @fn ConfirmationPanel( wxWindow* parent)
			 * @brief constructor
			 * @param parent parent window
			 */
			ConfirmationPanel( wxWizard* pParent );
			
			/**
			 * @fn virtual ~ConfirmationPanel()
			 * @brief destructor
			 */
			virtual ~ConfirmationPanel();
		private:
			/** finish text ctrl */
			wxTextCtrl* m_finishTxtCtrl;
			/** settings panel */
			kome::window::SettingsPanel* m_pSettingsPanel;	
			/** parent dialog */
			CreatePeakMatrixDialog* m_pParentDlg;
		public:

			/**
			* @fn virtual bool IsValid()
			* @brief validate panel settings
			* @return If validation failed, this method returns false.
			*/
			virtual bool IsValid();

			/**
			* @fn virtual bool PreApplyValidationCheck(std::string& sError)
			* @brief Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
			* @param sError error string from a single panel's pre-apply validation
			* @return If validation succeeded, return true, otherwise false.
			*/
			virtual bool PreApplyValidationCheck(std::string& sError);

			/**
			* @fn void SavePanelSettings(kome::core::Progress *pProgress)
			* @brief This method is called when saving values.
			* @param pProgress progress
			* @return If saSavePanelSettingsving values failed, this method returns false.
			*/
			virtual bool SavePanelSettings(kome::core::Progress *pProgress);
		
		protected:
			/**
			* @fn void updateStandardSamples(bool bReadFromINIfile = true)
			* @brief Update standard samples on this panel
			* @param bReadFromINIfile whether to read from the INI file
			*/
			virtual void updateStandardSamples(bool bReadFromINIfile = true);

			/**
			* @fn void RefreshPanelContentsForMatrix()
			* @brief refresh panel contents for matrix
			*/
			virtual void RefreshPanelContentsForMatrix();

			/**
			 * @fn virtual void createInnerSizer(wxStaticBoxSizer* pInnerSizer)
			 * @brief creates inner sizer
			 * @param pInnerSizer inner sizer
			 */
			virtual void createInnerSizer(wxStaticBoxSizer* InnerSizer);
			
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

	
		private:
			DECLARE_EVENT_TABLE();

			};

		}

	}
}

#endif // __KOME_CONFIRMATION_PANEL_H__
