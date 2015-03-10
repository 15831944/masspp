/**
 * @file PeakMatrixNamePanel.h
 * @brief interfaces of PeakMatrixNamePanel class
 *
 * @author M.Izumi
 * @date 2012.10.11
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_PEAKMATRIXNAME_PANEL_H__
#define __KOME_PEAKMATRIXNAME_PANEL_H__

#include <wx/wx.h>
#include <wx/wizard.h> 

namespace kome {
	namespace matrix {
		namespace view {
		
		class CreatePeakMatrixDialog;

		/**
		 * @class PeakMatrixNamePanel
		 * @brief peak matrix name panel class
		 */
		class PeakMatrixNamePanel : public kome::window::PeakMatrixSettingsPageBase {
		public:
			/**
			 * @fn PeakMatrixNamePanel( wxWizard* parent , bool bEdit )
			 * @brief constructor
			 * @param parent window
			 * @param bedit bedit
			 */
			PeakMatrixNamePanel( wxWizard* parent , bool bEdit );

			/**
			 * @fn virtual ~PeakMatrixNamePanel()
			 * @brief destructor
			 */
			virtual ~PeakMatrixNamePanel();
		protected:
			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;
			/** settings panel */
			kome::window::SettingsPanel* m_pSettingsPanel;
			/** edit flag */
			bool m_bEdit;
			/** parent dialog */
			CreatePeakMatrixDialog* m_pParentDlg;
			/** select matrix name */
			std::string m_selMatrixName;		
		
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
			 * @fn MatrixNameExists( const char* name )
			 * @brief checks if a matrix with the specified name exists
			 * @param name the matrix name to check
			 * @return true if a matrix with this name exists, otherwise false
			 */
			bool MatrixNameExists( const char* name );

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

			/**
			* @fn void onSettingChanged(kome::window::SettingsPanelEvent& evt)
			* @brief This method is called when an on a setting is changed in the settings panel.
			* @param evt setting panel event information
			*/
			void onSettingChanged(kome::window::SettingsPanelEvent& evt);	
		
		private:
			DECLARE_EVENT_TABLE();

			};
		}
	}
}

#endif // __KOME_PEAKMATRIXNAME_PANEL_H__
