/**
 * @file DbDialog.h
 * @brief interfaces of DbDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_DB_DIALOG_H__
#define __KOME_IO_DB_DIALOG_H__


#include <wx/wx.h>


namespace kome {
	namespace io {
		namespace db {

			/**
			 * @class DbDialog
			 * @brief labeling dialog class
			 */
			class DbDialog : public kome::window::ParametersAndHelpDialog {
			public:
				/**
				 * @fn DbDialog( wxWindow* parent )
				 * @brief constructor
				 * @param parent parent window
				 */
				DbDialog( wxWindow* parent );

				/**
				 * @fn virtual ~DbDialog()
				 * @brief destructor
				 */
				virtual ~DbDialog();

			protected:
				/** parameters */
				kome::plugin::ParameterSettings m_parameters;

				/** settings */
				kome::objects::SettingParameterValues m_settings;

				/** DB getter */
				kome::plugin::PluginFunctionItem* m_dbGetter;

				/** DB settings */
				kome::objects::SettingParameterValues* m_dbSettings;

			public:
				/**
				 * @fn kome::plugin::PluginFunctionItem* getDbGetter()
				 * @brief gets DB getter function
				 * @return DB getter function
				 */
				kome::plugin::PluginFunctionItem* getDbGetter();

				/**
				 * @fn kome::objects::SettingParameterValues* getDbSettings()
				 * @brief gets DB settings
				 * @return DB settings
				 */
				kome::objects::SettingParameterValues* getDbSettings();

			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer();
				 * @brief creates main sizer (override method)
				 * @return field sizer
				 */
				virtual wxSizer* createMainSizer();

                /**
                 * @fn virtual wxSizer* createButtonSizer()
                 * @brief creates button sizer(override method)
				 * @return button sizer
                 */
				virtual wxSizer* createButtonSizer();
			protected:
				/**
				 * @fn void onVacuumButton( wxCommandEvent& evt )
				 * @brief This method is called when the vacuum button is clicked.
				 * @param evt command event
				 */
				void onVacuumButton( wxCommandEvent& evt );

				/**
				 * @fn void onInitButton( wxCommandEvent& evt )
				 * @brief This method is called when the initialize button is clicked.
				 * @param evt command event
				 */
				void onInitButton( wxCommandEvent& evt );

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
		}
	}
}

#endif	//  __KOME_IO_DB_DIALOG_H__
