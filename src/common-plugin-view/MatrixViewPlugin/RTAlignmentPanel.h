/**
 * @file RTAlignmentPanel.h
 * @brief interfaces of RTAlignmentPanel class
 *
 * @author M.Izumi
 * @date 2013.01.24
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_RT_ARIGNMENT_PANEL_H__
#define __KOME_RT_ARIGNMENT_PANEL_H__

namespace kome {
	namespace matrix {
		namespace view {

		class CreatePeakMatrixDialog;
		class RTAlignmentHelpPanel;

		/** 
		 * @fn class RTAlignmentPanel
		 * @brief RT Alignment Panel class
		 */
		class RTAlignmentPanel : public kome::window::PeakMatrixSettingsPageBase {
		public:
			/**
			 * @fn RTAlignmentPanel( wxWizard* pParent )
			 * @brief constructor
			 * @param pParent
			 */
			RTAlignmentPanel( wxWizard* pParent );

			/**
			 * @fn virtual ~RTAlignmentPanel()
			 * @brief destructor
			 */
			virtual ~RTAlignmentPanel();

		protected:
			/** parent dialog */
			CreatePeakMatrixDialog* m_pParentDlg;
			/** help panel */
			RTAlignmentHelpPanel* m_helpPanel;
			/** radio box book */
			kome::window::RadioBoxBook* m_book;
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
			virtual void createInnerSizer(wxStaticBoxSizer* pInnerSizer);
			
			/**
			* @fn virtual bool TransferDataFromWindow()
			* @brief transfer window control data to value
			* @return If a transfer failed, this method returns false.
			*/
			virtual bool TransferDataFromWindow();
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
		
		private:
			DECLARE_EVENT_TABLE();

		};


		// class RT Alignment help panel

		class RTAlignmentHelpPanel : public kome::window::ParametersAndHelpPanel {
		public:
			/**
			 * @fn RTAlignmentHelpPanel( wxWindow* parent, const char* section );
			 * @brief constructor
			 * @param parent
			 * @param section
			 */
			RTAlignmentHelpPanel( wxWindow* parent, const char* section );
			
			/**
			 * @fn virtual ~RTAlignmentHelpPanel()
			 * @brief destructor
			 */
			virtual ~RTAlignmentHelpPanel();

		protected:
			std::string m_section;
			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** settings of scaling **/
			kome::plugin::PluginFunctionItem* m_scaling;
			/** scaling setting parameter */
			kome::objects::SettingParameterValues* m_scalingSettings;

			/** settings of alignment **/
			kome::plugin::PluginFunctionItem* m_alignment;
			/** alignment setting parameter */
			kome::objects::SettingParameterValues* m_alignmentSettings;

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

			/** スケーリング **/
			/**
			 * @fn kome::plugin::PluginFunctionItem* getScaling()
			 * @brief get scaling
			 * @return scaling
			 */
			kome::plugin::PluginFunctionItem* getScaling(){ return m_scaling; }
			/**
			 * @fn void setScaling( kome::plugin::PluginFunctionItem* item )
			 * @brief set scaling 
			 * @param item
			 */
			void setScaling( kome::plugin::PluginFunctionItem* item ){ m_scaling = item; }
			/**
			 * @fn kome::objects::SettingParameterValues* getScalingSettings()
			 * @brief get scaling settings param
			 * @return  scaling settings
			 */
			kome::objects::SettingParameterValues* getScalingSettings(){ return m_scalingSettings; }
			/**
			 * @fn void setScalingSettings
			 * @brief set scaling settings param
			 * @param settings
			 */
			void setScalingSettings( kome::objects::SettingParameterValues* settings ){ m_scalingSettings = settings; }

			/** アライメント **/
			/**
			 * @fn kome::plugin::PluginFunctionItem* getAlignment()
			 * @brief get alignment
			 * @return alignment
			 */
			kome::plugin::PluginFunctionItem* getAlignment(){ return m_alignment; }
			/**
			 * @fn void setAlignment( kome::plugin::PluginFunctionItem* item )
			 * @brief set alilgnment
			 * @param item 
			 */
			void setAlignment( kome::plugin::PluginFunctionItem* item ){ m_alignment = item; }
			/**
			 * @fn kome::objects::SettingParameterValues* getAlignmentSettings()
			 * @brief get alignment settings param
			 * @return alignment settings
			 */
			kome::objects::SettingParameterValues* getAlignmentSettings(){ return m_alignmentSettings; }
			/**
			 * @fn void setAligmentSettings( kome::objects::SettingParameterValues* settings )
			 * @brief set alignment settings
			 */
			void setAligmentSettings( kome::objects::SettingParameterValues* settings ){ m_alignmentSettings = settings; }
		private:
			DECLARE_EVENT_TABLE();
		
		public:
			/**
			 * @fn wxSizer* createCorrectionSizer()
			 * @brief creates correction sizer
			 * @return correction sizer
			 */
			wxSizer* createCorrectionSizerScaling();     //  @date 2013/5/28 <Add> FUJITA
		};
		
		}
	}
}



#endif // __KOME_RT_ARIGNMENT_PANEL_H__
