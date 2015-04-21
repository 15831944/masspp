/**
 * @file SettingsGroup.h
 * @brief interfaces of SettingsGroup class
 *
 * @author S.Tanaka
 * @date 2007.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_PLUGIN_SETTINGS_GROUP_H__
#define __KOME_PLUGIN_SETTINGS_GROUP_H__

#include <string>

namespace kome {
	namespace plugin {

		/**
		 * @class SettingsGroup
		 * @brief group of settings parameter information class
		 */
		class PLUGIN_TOOLS_CLASS SettingsGroup {
		public:
			/**
			 * @fn SettingsGroup()
			 * @brief constructor
			 */
			SettingsGroup();

			/**
			 * @fn virtual ~SettingsGroup()
			 * @brief destructor
			 */
			virtual ~SettingsGroup();

		protected:
			/** group name */
			std::string m_name;

		public:
			/**
			 * @fn void setName( const char* name )
			 * @brief sets group name
			 * @param[in] name group name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets group name
			 * @return group name
			 */
			const char* getName();
		};
	}
}


#endif		//  __KOME_PLUGIN_SETTINGS_GROUP_H__
