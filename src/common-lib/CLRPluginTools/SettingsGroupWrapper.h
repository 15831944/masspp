/**
 * @file SettingsGroupWrapper.h
 * @brief interfaces of SettingsGroupWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SETTINGS_GROUP_WRAPPER_H__
#define __KOME_CLR_SETTINGS_GROUP_WRAPPER_H__


namespace kome { 
	namespace clr {

		/**
		 * @class SettingsGroupWrapper
		 * @brief settings group wrapper class to use on CLR environment
		 */
		public ref class SettingsGroupWrapper {
		public:
			/**
			 * @fn SettingsGroupWrapper( kome::plugin::SettingsGroup& group )
			 * @brief constructor
			 * @param[in] group settings group object to be wrapped
			 */
			SettingsGroupWrapper( kome::plugin::SettingsGroup& group );

			/**
			 * @fn virtual ~SettingsGroupWrapper()
			 * @brief destructor
			 */
			virtual ~SettingsGroupWrapper();

		protected:
			/** settings group object to be wrapped */
			kome::plugin::SettingsGroup& m_group;

		public:
			/**
			 * @fn kome::plugin::SettingsGroup& getSettingsGroup()
			 * @brief gets settings group object
			 * @return gets settings group object
			 */
			kome::plugin::SettingsGroup& getSettingsGroup();

		public:
			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets group name
			 * @param[in] name group name
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets group name
			 * @return group name
			 */
			System::String^ getName();
		};
	}
}

#endif		//  __KOME_CLR_SETTINGS_GROUP_WRAPPER_H__
