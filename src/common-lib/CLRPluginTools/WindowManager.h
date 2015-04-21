/**
 * @file WindowManager.h
 * @brief interfaces of WindowManager class
 *
 * @author S.Tanaka
 * @date 2013.05.14
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_CLR_WINDOW_MANAGER_H__
#define __KOME_CLR_WINDOW_MANAGER_H__


namespace kome {
	namespace clr { 

		ref class SettingsPageWrapper;


		/**
		 * @class WindowManager
		 * @brief substance wrapper class to use on CLR environment
		 */
		public ref class WindowManager {
		protected:
			/**
			 * @fn WindowManager()
			 * @brief constructor
			 */
			WindowManager();

			/**
			 * @fn virtual ~WindowManager()
			 * @brief destructor
			 */
			virtual ~WindowManager();

		protected:
			/** window manager */
			static WindowManager^ m_mgr;

		public:
			/**
			 * @fn bool openSettingsPageDialog(
						SettingsPageWrapper^ page,
						SettingParameterValuesWrapper^ settings,
						bool saveVal,
						System::String^ prefix,
						bool dfltBtnFlg,
						System::String^ commitName
					)
			 * @brief opens settings page dialog
			 * @param[in] page settings page information
			 * @param[in] settings the object to store settings values
			 * @param[in] saveVal save value flag
			 * @param[in] prefix parameter key prefix
			 * @param[in] dfltBtnFlg If true, the "Default" button appears.
			 * @param[in] commitName alternative name for the "OK" commit button
			 * @return If true, OK button is clicked.
			 */
			bool openSettingsPageDialog(
				SettingsPageWrapper^ page,
				SettingParameterValuesWrapper^ settings,
				bool saveVal,
				System::String^ prefix,
				bool dfltBtnFlg,
				System::String^ commitName
			);

		public:
			/**
			 * @fn static WindowManager^ getInstance()
			 * @brief gets window manager object (This is the only object.)
			 * @return window manager object.
			 */
			static WindowManager^ getInstance();
		};
	}
}


#endif		// __KOME_CLR_WINDOW_MANAGER_H__

