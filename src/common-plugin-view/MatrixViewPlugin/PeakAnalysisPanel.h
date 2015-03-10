/**
 * @file PeakAnalysisPanel.h
 * @brief interfaces of PeakAnalysisPanel class
 *
 * @author S.Tanaka
 * @date 2012.03.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_MATRIX_PEAK_ANALYSIS_PANEL_H__
#define __KOME_MATRIX_PEAK_ANALYSIS_PANEL_H__


namespace kome {
	namespace matrix {
		namespace view {
			class PeakAnalysisHelpPanel;
			
			class CreatePeakMatrixDialog;

			/**
			 * @class PeakAnalysisPanel
			 * @brief fill matrix panel class
			 */
			class PeakAnalysisPanel : public kome::window::PeakMatrixSettingsPageBase
			{
				public:
					/**
					 * @fn PeakAnalysisPanel( wxWindow* parent, QuantitationDialog* pParentDlg )
					 * @brief constructor
					 * @param parent parent window
					 * @param pParentDlg parent dialog
					 */
					PeakAnalysisPanel( wxWizard* parent );

					/**
					 * @fn virtual ~PeakAnalysisPanel()
					 * @brief destructor
					 */
					virtual ~PeakAnalysisPanel(void);
				protected:
					/** peak analysis */
					std::vector< kome::plugin::PluginFunctionItem* > m_analysis;

					/** settings */
					std::vector< kome::objects::SettingParameterValues* > m_analysisSettings;
					
					/** peak scaling */
					std::vector< kome::plugin::PluginFunctionItem* > m_scaling;

					/** settings */
					std::vector< kome::objects::SettingParameterValues* > m_scalingSettings;

					/** multivariate analysis */   
					std::vector< kome::plugin::PluginFunctionItem* > m_multivariate_analysis;   
  
					/** settings */   
					std::vector< kome::objects::SettingParameterValues* > m_multivariate_analysisSettings;   
					
					/** selected groups */
					std::vector< int > m_samples;

					/** standard group ID */
					int m_stdId;
				
				private:
					/** help panel */
					PeakAnalysisHelpPanel* m_helpPanel;
					/** parent dialog */
					CreatePeakMatrixDialog* m_pParentDlg;
				public:
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
					 * @return If saving values failed, this method returns false.
					 */
					virtual bool SavePanelSettings(kome::core::Progress *pProgress);

					/**
					 * @fn virtual bool IsValid()
					 * @brief validate panel settings
					 * @return If validation failed, this method returns false.
					 */
					virtual bool IsValid();

                protected:
					/**
					 * @fn void clearAnalysis()
					 * @brief clears analysis
					 */
					void clearAnalysis();

					/**
					 * @fn void clearScaling()
					 * @brief clears scaling
					 */
					void clearScaling();

					/**  
					* @fn void clearMultivariateAnalysis() 
					* @brief clears Multivariate Analysis   
					*/   
					void clearMultivariateAnalysis(); 
  
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

					/**
					 * @fn void updateStandardSamples(bool bReadFromINIfile = true)
					 * @brief Update standard samples on this panel
					 * @param bReadFromINIfile whether to read from the INI file
					 */
					virtual void updateStandardSamples(bool bReadFromINIfile = true);

					/**
					 * @fn void updateSettings()
					 * @brief refresh settings for matrix
					 */
					void updateSettings();

					/**
					 * @fn void RefreshPanelContentsForMatrix()
					 * @brief refresh panel contents for matrix
					 */
					virtual void RefreshPanelContentsForMatrix();
										
				protected:
		
					/**
					 * @fn void onToggleTargetSample( wxListEvent& evt )
					 * @brief This method is called when an item in the Target Samples checklistbox is toggled.
					 * @param evt command event information
					 */
					void onToggleTargetSample( wxCommandEvent& evt );

					/**
					 * @fn void onSettingChanged(kome::window::SettingsPanelEvent& evt)
					 * @brief This method is called when an on a setting is changed in the settings panel.
					 * @param evt setting panel event information
					 */
					void onSettingChanged(kome::window::SettingsPanelEvent& evt);
			
				private:
					DECLARE_EVENT_TABLE();
			};

			/**
			 * @class PeakAnalysisHelpPanel
			 * @brief peak analysis help panel class
			 */
			class PeakAnalysisHelpPanel : public kome::window::ParametersAndHelpPanel{
			public:
				/**
				 * @fn PeakAnalysisPanel( wxWindow* parent, QuantitationDialog* pParentDlg )
				 * @brief constructor
				 * @param parent parent window
				 * @param pParentDlg parent dialog
				 */
				PeakAnalysisHelpPanel( wxWindow* parent, const char* section );

				/**
				 * @fn virtual ~PeakAnalysisHelpPanel()
				 * @brief destructor
				 */
				virtual ~PeakAnalysisHelpPanel();

			protected:
				/** parameters */
				kome::plugin::ParameterSettings m_params;

				/** settings */
				kome::objects::SettingParameterValues m_settings;

				/** section */
				std::string m_section;

				/** scaling */
				std::string m_scaling;

				/** analysis */
				std::string m_analysis;

				/** analysis */
				std::string m_multivariate_analysis;   // ラジオボックスブックで選択された多変量解析手法一覧が入る

			public:
				/** radio box book */
				kome::window::RadioBoxBook* m_book0;

				/** radio box book */
				kome::window::RadioBoxBook* m_book1;

				/** radio box book */
				kome::window::RadioBoxBook* m_book2;   // 多変量解析手法選択用のラジオボックスブック

			public:
				/**
				 * @fn void updateSettings()
				 * @brief update settings
				 */
				void updateSettings();

				/**
				 * @fn const char* getScalingParam()
				 * @brief get scaling param
				 */
				const char* getScalingParam();
				
				/**
				 * @fn const char* getAnalysisParam()
				 * @brief get analysis param
				 */
				const char* getAnalysisParam();

				/**   
			     * @fn const char* getMultivariateAnalysisParam()   
				 * @brief get multivariate analysis param   
				 */   
				const char* getMultivariateAnalysisParam(); 

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
			private:
					DECLARE_EVENT_TABLE();
			};


		}
	}
}

#endif		// __KOME_MATRIX_PEAK_ANALYSIS_PANEL_H__
