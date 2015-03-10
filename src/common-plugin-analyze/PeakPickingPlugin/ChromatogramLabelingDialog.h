/**
 * @file ChromatogramLabelingDialog.h
 * @brief interfaces of ChromatogramLabelingDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_LABELING_CHROMATOGRAM_LABELING_DIALOG_H__
#define __KOME_LABELING_CHROMATOGRAM_LABELING_DIALOG_H__


#include "PeakPickingManager.h"

#include <wx/wx.h>


namespace kome {
	namespace labeling {

		/**
		 * @class ChromatogramLabelingDialog
		 * @brief auto labeling dialog class
		 */
		class ChromatogramLabelingDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn ChromatogramLabelingDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			ChromatogramLabelingDialog( wxWindow* parent );

			/**
			 * @fn virtual ~ChromatogramLabelingDialog()
			 * @brief destructor
			 */
			virtual ~ChromatogramLabelingDialog();

		protected:
			/** peak picking function item */
			kome::plugin::PluginFunctionItem* m_peakPickingFunction;

			/** peak picking settings */
			kome::objects::SettingParameterValues* m_peakPickingSettings;

			/** parameters */
			kome::plugin::ParameterSettings m_parameters;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

		public:
			/**
			 * @fn kome::plugin::PluginFunctionItem* getPeakPickingFunction()
			 * @brief gets peak picking function
			 * @return peak picking function
			 */
			kome::plugin::PluginFunctionItem* getPeakPickingFunction();

			/**
			 * @fn kome::objects::SettingParameterValues* getPeakPickingSettings()
			 * @brief gets peak picking settings
			 * @return peak picking settings
			 */
			kome::objects::SettingParameterValues* getPeakPickingSettings();

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

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	//  __KOME_LABELING_CHROMATOGRAM_LABELING_DIALOG_H__
