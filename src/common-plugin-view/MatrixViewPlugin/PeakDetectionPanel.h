/**
 * @file PeakDetectionPanel.h
 * @brief interfaces of PeakDetectionPanel class
 *
 * @author M.Izumi
 * @date 2013.12.10
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_VIEW_PEAKDETECTION_PANEL_H__
#define __KOME_VIEW_PEAKDETECTION_PANEL_H__

namespace kome {
	namespace matrix {
		namespace view {
			class CreatePeakMatrixDialog;
			class PeakDetectionHelpPanel;

			class PeakDetectionPanel : public kome::window::PeakMatrixSettingsPageBase  {

			public:
				/**
				 * @fn PeakDetectionPanel( wxWizard* pParent )
				 * @brief constructor
				 * @param pParent
				 */
				PeakDetectionPanel( wxWizard* pParent );
				/**
				 * @fn virtual ~PeakDetectionPanel()
				 * @brief destructor
				 */
				virtual ~PeakDetectionPanel();

			protected:
				/** parent dialog */
				CreatePeakMatrixDialog* m_pParentDlg;
				
				/** help panel */
				PeakDetectionHelpPanel* m_helpPanel;

				/** peak position function item */
				kome::plugin::PluginFunctionItem* m_peakPos;

				/** peak position settings */
				kome::objects::SettingParameterValues* m_peakPosSettings;

				/** peak search */
				kome::plugin::PluginFunctionItem* m_search;

				/** peak search settings */
				kome::objects::SettingParameterValues* m_searchSettings;
					

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
				virtual void createInnerSizer(wxStaticBoxSizer* pInnerSizer);
			
				/**
				* @fn virtual bool TransferDataFromWindow()
				* @brief transfer window control data to value
				* @return If a transfer failed, this method returns false.
				*/
				virtual bool TransferDataFromWindow();

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
				* @fn void onSettingChanged(kome::window::SettingsPanelEvent& evt)
				* @brief This method is called when an on a setting is changed in the settings panel.
				* @param evt setting panel event information
				*/
				void onSettingChanged(kome::window::SettingsPanelEvent& evt);
			
				/**
				 * @fn void updateSettings()
				 * @brief refresh settings for matrix
				 */
				void updateSettings();

			protected:
				/**
				 * @fn
				 * @brief
				 */
				bool peakValueCalc( int project, kome::core::Progress *pProgress, kome::ident::PeakPositions* peakPos, std::map< int, int >& peakIdMap );
			private:
				DECLARE_EVENT_TABLE();


			};

			// ------------------------------------
			// class Peak Detecion help panel
			// ------------------------------------
			class PeakDetectionHelpPanel : public kome::window::ParametersAndHelpPanel {
			public:
				PeakDetectionHelpPanel( wxWindow* parent, const char* section, bool bEdit );
				virtual ~PeakDetectionHelpPanel();

			protected:
				/** section */
				std::string m_section;
				
				/** edit flag */
				bool m_bEdit;
				
				/** parameters */
				kome::plugin::ParameterSettings m_params;

				/** settings */
				kome::objects::SettingParameterValues m_settings;
								
				/** peak position button */
				wxStaticBitmap* m_peakPosBmp;

				/** peak positon value button */
				wxStaticBitmap* m_peakPosValBmp;

				/** peak positon value button */
				wxStaticBitmap* m_peakPosValBmp2;
				
				/** peak postion panel */
				kome::window::SettingsPanel* m_posPanel;

				/** peak value panel */
				kome::window::SettingsPanel* m_valPanel;
			
				/** peak value status */
				bool m_bpeakVal;
			protected:
				/**
		 		 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();

				/**
				 * @fn virtual bool TransferDataFromWindow()
				 * @brief transfer window control data to value
				 * @return If a transfer failed, this method returns false.
				 */
				virtual bool TransferDataFromWindow();
			public:
				/**
				 * @fn void updateSettings()
				 * @brief update settings
				 */
				void updateSettings();

				/**
				 * @fn const char* getPeakPos()
				 * @brief get peak pos
				 * @return peak position name
				 */
				const char* getPeakPos();

				/**
				 * @fn kome::objects::SettingParameterValues* getPeakPosSettings( const char* paramName,  const char* peakPos )
				 * @brief get peak position settings
				 * @param paramName
				 * @param peakPos
				 * @return settingParameterValues object
				 */
				kome::objects::SettingParameterValues* getPeakPosSettings( const char* paramName, const char* peakPos );

				/**
				 * @fn bool getFlgParamName()
				 * @brief get flg param name
				 * @return parameter name flag
				 */
				bool getFlgParamName();
				
				/**
				 * @fn bool getPeakValParamName()
				 * @brief get flg param name
				 * @return parameter value name flag
				 */
				bool getPeakValParamName();

				/**
				 * @fn const char* getSearch()
				 * @brief get search
				 * @return search name
				 */
				const char* getSearch();
				
				/**
				 * @fn kome::objects::SettingParameterValues* getSearchSettings( const char* search )
				 * @brief get search settings
				 * @param search
				 * @return setting parameter values
				 */
				kome::objects::SettingParameterValues* getSearchSettings( const char* search );

				/**
				 * @fn double getTol( const char* paramName, double def )
				 * @brief get tol
				 * @param paramName
				 * @param def
				 * @return tol
				 */
				double getTol( const char* paramName, double def );

				/**
				 * @fn void getSettings()                      
				 * @brief get m_settings of Fill Matrix panel
				 * @return setting parameter values object
				 */
				kome::objects::SettingParameterValues getSettings();  // @date 2013/08/28 <Add> FUJITA

				/**
				 * @fn void setPeakValStatus()
				 * @brief set peak value status
				 */
				void setPeakValStatus();

				/**
				 * @fn bool getPeakValStatus()
				 * @brief get peak value status
				 * @return peak value status
				 */
				bool getPeakValStatus();
				
			private:
				DECLARE_EVENT_TABLE();

			};
		}
	}
}




#endif //  __KOME_VIEW_PEAKDETECTION_PANEL_H__
