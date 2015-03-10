/**
 * @file LabelingDialog.h
 * @brief interfaces of LabelingDialog class
 *
 * @author S.Tanaka
 * @date 2007.10.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_LABELING_DIALOG_H__
#define __KOME_LABELING_DIALOG_H__


#include <wx/wx.h>


namespace kome {
	namespace labeling {

		/**
		 * @class LabelingDialog
		 * @brief labeling dialog class
		 */
		class LabelingDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn LabelingDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			LabelingDialog( wxWindow* parent );

			/**
			 * @fn virtual ~LabelingDialog()
			 * @brief destructor
			 */
			virtual ~LabelingDialog();

		protected:
			/** parameters */
			kome::plugin::ParameterSettings m_parameters;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** peak detector */
			kome::plugin::PluginFunctionItem* m_peakDetector;

			/** peak settings */
			kome::objects::SettingParameterValues* m_peakSettings;

		public:
			/**
			 * @fn kome::plugin::PluginFunctionItem* getPeakDetector()
			 * @brief gets peak detector
			 * @return peak detector
			 */
			kome::plugin::PluginFunctionItem* getPeakDetector();

			/**
			 * @fn kome::objects::SettingParameterValues* getPeakSettings()
			 * @brief gets peak settings 
			 * @return peak settings
			 */
			kome::objects::SettingParameterValues* getPeakSettings();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer();
			 * @brief creates main sizer (override method)
			 * @return field sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		protected:
			/**
			 * @fn void onTestButton( wxCommandEvent& evt )
			 * @brief This button is called when the test button is clicked.
			 * @param evt event information
			 */
			void onTestButton( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateTestButton( wxUpdateUIEvent& evt )
			 * @brief updates test button user interface
			 * @param evt event information
			 */
			void onUpdateTestButton( wxUpdateUIEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	//  __KOME_LABELING_DIALOG_H__
