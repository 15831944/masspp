/**
 * @file NormalizationPanel.h
 * @brief interfaces of NormalizationPanel class
 *
 * @author M.Izumi
 * @date 2012.10.19
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_NORMALIZATION_PANEL_H__
#define __KOME_NORMALIZATION_PANEL_H__

namespace kome {
	namespace matrix {
		namespace view {
		
		class NormalizHelpPanel;
					
		/**
		 * @class NormalizationPanel class
		 * @brief normalization panel class
		 */
		class NormalizationPanel : public kome::window::PeakMatrixSettingsPageBase {

		public:
			/**
			 * @fn NormalizationPanel()
			 * @brief constructor
			 */
			NormalizationPanel( wxWizard* parent );

			/**
			 * @fn virtual ~NormalizationPanel()
			 * @brief destructor
			 */
			virtual ~NormalizationPanel();

		protected:
			
			/** selected groups */
			std::vector< int > m_samples;

			/** standard group ID */
			int m_stdId;

			/** method panel */
			kome::window::SettingsPanel* m_methodPanel;

			/** settings panel */
			kome::window::SettingsPanel*	m_pSettingsPanel;
			/** help panel */
			NormalizHelpPanel* m_helpPanel;
			/** radio box book */
			kome::window::RadioBoxBook* m_book;
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

			/**
			 * @fn IsSettings()
			 * @brief is settings
			 * @return If settings failed, this settings returns false.
			 */
			bool IsSettings();
		
		
		protected:
			/**
			* @fn void onSettingChanged(kome::window::SettingsPanelEvent& evt)
			* @brief This method is called when an on a setting is changed in the settings panel.
			* @param evt setting panel event information
			*/
			void onSettingChanged(kome::window::SettingsPanelEvent& evt);

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
			virtual void createInnerSizer(wxStaticBoxSizer* pInnerSizer);
			
			/**
			* @fn virtual bool TransferDataFromWindow()
			* @brief transfer window control data to value
			* @return If a transfer failed, this method returns false.
			*/
			virtual bool TransferDataFromWindow();

		   /* @fn void onSize( wxSizeEvent& evt )
			* @brief This method is called when the dialog size is changed
			* @param evt size event information
			*/
			void onSize( wxSizeEvent& evt );						
		
		private:
			DECLARE_EVENT_TABLE()

			};
		
					
		/**
		 * @class NormalizHelpPanel class
		 * @brief Normaliz Help panel class
		 */
		class NormalizHelpPanel : public kome::window::ParametersAndHelpPanel {

		public:
			/**
			 * @fn NormalizHelpPanel( wxWindow* parent, const char* section )
			 * @brief constructor
			 */
			NormalizHelpPanel( wxWindow* parent, const char* section  );

			/**
			 * @fn virtual ~NormalizHelpPanel()
			 * @brief destructor
			 */
			virtual ~NormalizHelpPanel();
		protected:
			std::string m_section;
			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** nomalization function item */
			std::vector< kome::plugin::PluginFunctionItem* > m_normaliz;
			
			/** nomalization settings */
			std::vector< kome::objects::SettingParameterValues > m_normalizSettings;

			/** filter threshold function item */
			kome::plugin::PluginFunctionItem* m_filterThreshold;

			/** filter threshold settings */
			kome::objects::SettingParameterValues m_filterThresholdSettings;

			/** filter rt range function item */
			kome::plugin::PluginFunctionItem* m_filterRt;

			/** filter rt range settings */
			kome::objects::SettingParameterValues m_filterRtSettings;

			/** filter m/z range function item */
			kome::plugin::PluginFunctionItem* m_filterMz;

			/** filter m/z range settings */
			kome::objects::SettingParameterValues m_filterMzSettings;

			kome::plugin::PluginFunctionItem* m_peakDetectItem;
			kome::objects::SettingParameterValues m_peakDetectSettings;
		protected:
			/**
			* @fn wxSizer* createListEditPanelSizer()
			* @brief create list edit panel sizer
			* @return List Edit Panel sizer
			*/
			wxSizer* createListEditPanelSizer();

			/**
			* @fn wxSizer* createFilterSizer()
			* @brief create filter sizer
			* @return filter sizer
			*/
			wxSizer* createFilterSizer();

			/**
			* @fn wxSizer* createPeakDetectSizer()
			* @brief create peak detect combo box 
			* @return peak detect combo box 
			*/
			wxSizer* createPeakDetectSizer();

			/**
			* @fn kome::window::SettingsPanel* createPanelThreshold()
			* @brief create panel threshold
			* @return settings panel [ Threshold ]
			*/
			kome::window::SettingsPanel* createPanelThreshold();

			/**
			* @fn kome::window::SettingsPanel* createPanelRtRange()
			* @brief create panel RT Range
			* @return settings panel [ RT Range ]
			*/
			kome::window::SettingsPanel* createPanelRtRange();

			/**
			* @fn kome::window::SettingsPanel* createPanelMzRange()
			* @brief create panel m/z Ragen
			* @return settings panel [ m/z Range ]
			*/
			kome::window::SettingsPanel* createPanelMzRange();
			
			/**
			 * @fn void setThresholdSetting()
			 * @brief set threshold settings
			 */
			void setThresholdSetting();

			/**
			 * @fn void setRtRangeSetting()
			 * @brief set rt range settings
			 */
			void setRtRangeSetting();

			/**
			 * @fn void setMzRangeSetting()
			 * @brief set m/z range settings
			 */
			void setMzRangeSetting();

			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		public:
			/**
			 * @fn void clearNormaliz()
			 * @brief clears normaliz
			 */
			void clearNormaliz();

			/**
			 * @fn void setFilterSettings()
			 * @brief set filter setting
			 */
			void setFilterSettings();

			void updateSettings();

			const char* getNormalizMethods();
			const char* getNormalizPeakDetect();

			kome::plugin::PluginFunctionItem* getNormalizPeakDetectItem( const char* detec );
			kome::objects::SettingParameterValues* getPeakDetectSettings();
			

			kome::plugin::PluginFunctionItem* getFilterThreshold();
			kome::objects::SettingParameterValues getFilterThresholdSettings();
			kome::plugin::PluginFunctionItem* getFilterRt();
			kome::objects::SettingParameterValues getFilterRtSettings();
			kome::plugin::PluginFunctionItem* getFilterMz();
			kome::objects::SettingParameterValues getFilterMzSettings();


			 std::vector<kome::plugin::PluginFunctionItem*> getNormalizFunc();
			 std::vector< kome::objects::SettingParameterValues > getNormalizSettings();

			void addNormaliz( kome::plugin::PluginFunctionItem* item );
			void addNormalizHelp( kome::objects::SettingParameterValues settings );

			int getNumberOfNormaliz();
			int getNumberOfNormalizSettings();
		private:
			DECLARE_EVENT_TABLE();
		};

		}
	}
}

#endif // __KOME_NORMALIZATION_PANEL_H__
