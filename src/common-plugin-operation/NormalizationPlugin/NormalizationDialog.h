/**
 * @file NormalizationDialog.h
 * @brief interfaces of NormalizationDialog class
 *
 * @author M.Izumi
 * @date 2012.07.02
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_NORMALIZATION_DIALOG_H__
#define __KOME_NORMALIZATION_DIALOG_H__

#include <vector>
#include <wx/wx.h>


namespace kome {
	namespace normalization {
		class NormalizHelpGetterPanel;

		/**
		 * @class NormalizationDialog
		 * @brief normalizationDialog subtract dialog
		 */
		class NormalizationDialog : public kome::window::ParametersAndHelpDialog {
			public:
			
				/**
				 * @fn NormalizationDialog( wxWindow* parent )
				 * @brief constructor
				 * @param parent parent window
				 */
				NormalizationDialog( wxWindow* parent );

				/**
				 * @fn virtual ~NormalizationDialog()
				 * @brief destructor
				 */
				virtual ~NormalizationDialog();

			protected:
		
				/** settings */
				kome::objects::SettingParameterValues m_settings;

				/** parameters */
				kome::plugin::ParameterSettings m_params;
				
				/** peak detect settings */
				kome::objects::SettingParameterValues* m_peakDetectSettings;
				
				/** method name */
				std::string m_method;

				/** peak detect name */
				std::string m_peakDetect;

				/** standard Group */
				kome::objects::DataGroupNode* m_stdGroup;

				/** treatment Group */
				kome::objects::DataGroupNode* m_trmtGroup;
				
				/** help getter panel */
				NormalizHelpGetterPanel* m_helpGetterPanel;

			public:
				/**
				 * @fn const char* getMethodName()
				 * @brief get the method name
				 * @return method name
				 */
				const char* getMethodName();

				/**
				 * @fn const char* getPeakDetect()
				 * @brief get the peak detect name
				 * @return peak detect name
				 */
				const char* getPeakDetect();

				/**
				 * @fn kome::objects::DataGroupNode* getStdGroup()
				 * @brief get the standarad group
				 * @return standard group
				 */
				kome::objects::DataGroupNode* getStdGroup();

				/**
				 * @fn kome::objects::DataGroupNode* getTrmtGroup()
				 * @brief get the treatment group
				 * @return treatment group
				 */
				kome::objects::DataGroupNode* getTrmtGroup();

				/**
				 * @fn kome::objects::SettingParameterValues* getPeakDetectSettings()
				 * @brief get the peak detect settings
				 * @return peak detect settings
				 */
				kome::objects::SettingParameterValues* getPeakDetectSettings();

		
			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer (override method)
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();

				/**
				 * @fn virtual wxSizer* createButtonSizer()
				 * @brief creates button sizer (override method)
				 * @return button sizer
				 */
				virtual wxSizer* createButtonSizer();

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
			protected:
				/**
				 * @fn void setSelectSamples()
				 * @brief set selected samples
				 */
				void setSelectSamples();

				/**
				 * @fn IsTreatmentChosen()
				 * @brief Is the treatment chosen
				 * @return It is true if chosen. 
				 */
				bool IsTreatmentChosen();

				/**
				 * @fn setFilterSettingParameterValues()
				 * @brief 
				 * @return 
				 */
				void setFilterSettingParameterValues();
				
				/**
				 * @fn setThresholdSetting()
				 * @brief 
				 * @return 
				 */
				void setThresholdSetting();
				
				/**
				 * @fn setRtRangeSetting()
				 * @brief 
				 * @return 
				 */
				void setRtRangeSetting();
				
				/**
				 * @fn setMzRangeSetting()
				 * @brief 
				 * @return 
				 */
				void setMzRangeSetting();


			protected:
				/**
				 * @fn virtual bool TransferDataFromWindow()
				 * @brief transfer window control data to value
				 * @return If a transfer failed, this method returns false.
				 */
				virtual bool TransferDataFromWindow();

			private:
				DECLARE_EVENT_TABLE();
		};

		// -----------------------------------------------
		// class Normalization Help Getter
		// -----------------------------------------------
		class NormalizHelpGetterPanel : public kome::window::HelpGetterPanel {
		public:
			/**
			 * @fn NormalizHelpGetterPanel( wxWindow* parent )
			 * @brief constructor
			 * @param parent
			 */
			NormalizHelpGetterPanel( wxWindow* parent );

			/**
			 * @fn virtual ~NormalizHelpGetterPanel()
			 * @brief destructor
			 */
			virtual ~NormalizHelpGetterPanel();

		protected:
			/** control sample combobox */
			wxComboBox* m_stdSmpCombo;

			/** treatment sample combobox */
			wxComboBox* m_trmtSmpCombo;
		protected:
			/**
			* @fn virtual wxSizer* createMainSizer()
			* @brief creates main sizer (override method)
			* @return main sizer
			*/
			virtual wxSizer* createMainSizer();
			
			/**
			 * @fn virtual std::string getDescription( wxWindow* wnd )
			 * @brief method to get the description from a prameter control
			 * @param[in] wnd parameter control
			 * @return description
			 */
			virtual std::string getDescription( wxWindow* wnd );
		protected:
			/**
			* @fn wxComboBox* createSampleCombo( const int id )
			* @brief creates sample combo box
			* @param id combo box ID
			* @return group combo box
			*/
			wxComboBox* createSampleCombo( const int id );

		public:
			/**
			 * @fn kome::objects::DataGroupNode* getSelGroup( const int id )
			 * @brief get select group
			 */
			kome::objects::DataGroupNode* getSelGroup( const int id );
			
		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	// __KOME_NORMALIZATION_DIALOG_H__
