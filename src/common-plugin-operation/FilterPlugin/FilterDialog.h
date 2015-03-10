/**
 * @file FilterDialog.h
 * @brief interfaces of FilterDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_FILTER_DIALOG_H__
#define __KOME_FILTER_DIALOG_H__


#include <vector>
#include <wx/wx.h>


namespace kome {
	namespace filter {

		/**
		 * @class FilterDialog
		 * @brief baseline subtract dialog
		 */
		class FilterDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn FilterDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			FilterDialog( wxWindow* parent );

			/**
			 * @fn virtual ~FilterDialog()
			 * @brief destructor
			 */
			virtual ~FilterDialog();

		protected:
			/** parameters */
			kome::plugin::ParameterSettings m_parameters;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** filter */
			kome::plugin::PluginFunctionItem* m_filter;

			/** filter settings */
			kome::objects::SettingParameterValues* m_filterSettings;

			/** PluginSelectionPanel */
			kome::window::PluginSelectionPanel* m_panel;

		public:
			/**
			 * @fn kome::plugin::PluginFunctionItem* getFilter()
			 * @brief gets filter function
			 * @return filter function
			 */
			kome::plugin::PluginFunctionItem* getFilter();

			
			/**
			 * @fn kome::objects::SettingParameterValues* getFilterSettings()
			 * @brief gets filter settings
			 * @return filter settings
			 */
			kome::objects::SettingParameterValues* getFilterSettings();

		protected:
			/**
			 * @fn void setDefaultValue( wxWindow* parent )
			 * @brief sets default value
			 * @param[in] parent parent window
			 */
			void setDefaultValue( wxWindow* parent );

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

		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		protected:
			/**
			 * @fn void onDefault( wxCommandEvent& evt )
			 * @brief This method is called when the default button is clicked.
			 * @param[in] evt event information
			 */
			void onDefault( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_FILTER_DIALOG_H__
