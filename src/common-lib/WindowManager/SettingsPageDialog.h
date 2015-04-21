/**
 * @file SettingsPageDialog.h
 * @brief interfaces of SettingsPageDialog class
 *
 * @author S.Tanaka
 * @date 2007.08.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_SETTINGS_PAGE_DIALOG_H__
#define __KOME_WINDOW_SETTINGS_PAGE_DIALOG_H__


#include "StandardDialog.h"


namespace kome {
	namespace window {


		/**
		 * @fn bool openSettingsPageDialog(
		 * @brief opens settings page dialog
					 kome::plugin::SettingsPage* page,
					 kome::objects::SettingParameterValues* settings,
					 const bool saveVal,
					 const char* prefix,
					 const bool dfltBtnFlg,
					 const char* commitBtnName
				 )
		 * @brief opens settings page dialog
		 * @param[in] page settings page information
		 * @param[in] settings the object to store settings values
		 * @param[in] saveVal save value flag
		 * @param[in] prefix parameter key prefix
		 * @param[in] dfltBtnFlg If true, the "Default" button appears.
		 * @param[in] commitBtnName alternative name for the "OK" commit button
		 * @return If true, OK button is clicked.
		 */
		bool openSettingsPageDialog(
			kome::plugin::SettingsPage* page,
			kome::objects::SettingParameterValues* settings,
			const bool saveVal,
			const char* prefix,
			const bool dfltBtnFlg,
			const char* commitBtnName
		);

		class SettingsHelpPanel;

		/**
		 * @class SettingsPageDialog
		 * @brief standard dialog
		 */
		class WINDOW_MANAGER_CLASS SettingsPageDialog : public StandardDialog {
		public:
			/**
			 * @fn SettingsPageDialog(
						wxWindow* parent,
						kome::plugin::SettingsPage* page,
						kome::objects::SettingParameterValues* settings = NULL,
						const bool saveVal = true,
						const char* prefix = NULL,
						const bool dfltBtnFlg = true,
						const char* commitBtnName = NULL
				   )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] page settings page information
			 * @param[in] settings the object to store settings values
			 * @param[in] saveVal save value flag
			 * @param[in] prefix parameter key prefix
			 * @param[in] dfltBtnFlg If true, the "Default" button appears.
			 * @param[in] commitBtnName alternative name for the "OK" commit button
			 */
			SettingsPageDialog(
				wxWindow* parent,
				kome::plugin::SettingsPage* page,
				kome::objects::SettingParameterValues* settings = NULL,
				const bool saveVal = true,
				const char* prefix = NULL,
				const bool dfltBtnFlg = true,
				const char* commitBtnName = NULL
			);

			/**
			 * @fn virtual ~SettingsPageDialog()
			 * @brief destructor
			 */
			virtual ~SettingsPageDialog();

		protected:
			/** settings page */
			kome::plugin::SettingsPage* m_page;

			/** settings values */
			kome::objects::SettingParameterValues* m_settings;

			/** save value flag */
			const bool m_saveVal;

			/** prefix */
			std::string m_prefix;

			/** [DEFAULT] button flag */
			bool m_dfltBtnFlg;

			/** edited */
			bool m_edited;

			/** panel */
			SettingsHelpPanel* m_panel;

		public:
			/**
			 * @fn void setEdited( const bool edited )
			 * @brief sets edited flag
			 * @param[in] edited edited flag state
			 */
			void setEdited( const bool edited );

			/**
			 * @fn bool isEdited()
			 * @brief gets edited flag
			 * @return edited flag state
			 */
			bool isEdited();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

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


#endif		// __KOME_WINDOW_SETTINGS_PAGE_DIALOG_H__
