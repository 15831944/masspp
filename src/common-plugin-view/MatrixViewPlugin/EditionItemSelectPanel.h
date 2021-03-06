/**
 * @file EditionItemSelectPanel.h
 * @brief interfaces of EditionItemSelectPanel class
 *
 * @author M.Izumi
 * @date 2012.10.29
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_EDITION_ITEM_SELECT_PANEL_H__
#define __KOME_EDITION_ITEM_SELECT_PANEL_H__

#include <wx/wx.h>
#include <wx/wizard.h>

namespace kome {
	namespace matrix {
		namespace view {

		/**
		 * @class EditionItemSelectPanel
		 * @brief edition item select panel class
		 */
		class EditionItemSelectPanel : public kome::window::PeakMatrixSettingsPageBase {
		public:
			/**
			 * @fn EditionItemSelectPanel( wxWindow* parent)
			 * @brief constructor
			 * @param parent parent window
			 */
			EditionItemSelectPanel( wxWizard* parent );

			/**
			 * @fn virtual ~EditionItemSelectPanel()
			 * @brief destructor
			 */
			virtual ~EditionItemSelectPanel();
		protected:
			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;
			/** settings panel */
			kome::window::SettingsPanel* m_pSettingsPanel;
			/** edit radio button */
			wxRadioBox* m_editRadioBtn;
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
			 * @fn void updateSettings()
			 * @brief refresh settings for matrix
			 */
			void updateSettings();
			
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
		
		private:
			DECLARE_EVENT_TABLE();
			};
		}
	}
}


#endif // __KOME_EDITION_ITEM_SELECT_PANEL_H__
