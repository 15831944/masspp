/**
 * @file SettingsParam.h
 * @brief interfaces of SettingsParam class
 *
 * @author S.Tanaka
 * @date 2007.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_SETTINGS_PARAM_H__
#define __KOME_PLUGIN_SETTINGS_PARAM_H__


namespace kome {
	namespace plugin {

		class SettingsValue;

		/**
		 * @class SettingsParam
		 * @brief settings parameter information class
		 */
		class PLUGIN_TOOLS_CLASS SettingsParam {
		public:
			/**
			 * @fn SettingsParam()
			 * @brief constructor
			 */
			SettingsParam();

			/**
			 * @fn virtual ~SettingsParam()
			 * @brief destructor
			 */
			virtual ~SettingsParam();

		protected:
			/** parameter name */
			std::string m_name;

			/** prefix */
			std::string m_prefix;

			/** settings value */
			SettingsValue* m_value;

			/** parameter section */
			std::string m_section;

			/** parameter key */
			std::string m_key;

		public:
			/**
			 * @fn void setName( const char* name )
			 * @brief sets parameter name
			 * @param[in] name parameter name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets parameter name
			 * @return parameter name
			 */
			const char* getName();

			/** 
			 * @fn void setPrefix( const char* prefix )
			 * @brief sets prefix
			 * @param[in] prefix prefix
			 */
			void setPrefix( const char* prefix );

			/** 
			 * @fn const char* getPrefix()
			 * @brief gets prefix
			 * @return prefix
			 */
			const char* getPrefix();

			/**
			 * @fn void setValue( SettingsValue* value )
			 * @brief sets settings value
			 * @param[in] value settings value
			 */
			void setValue( SettingsValue* value );

			/**
			 * @fn SettingsValue* getValue()
			 * @brief gets settings value
			 * @return settings value
			 */
			SettingsValue* getValue();

			/**
			 * @fn void setSection( const char* section )
			 * @brief gets parameter section
			 * @param[in] section parameter section
			 */
			void setSection( const char* section );

			/**
			 * @fn const char* getSection()
			 * @brief gets parameter section
			 * @return parameter section
			 */
			const char* getSection();

			/**
			 * @fn void setKey( const char* key )
			 * @brief sets parameter key
			 * @param[in] key parameter key
			 */
			void setKey( const char* key );

			/**
			 * @fn const char* getKey()
			 * @brief gets parameter key
			 * @return parameter key
			 */
			const char* getKey();

		};
	}
}

#endif		// __KOME_PLUGIN_SETTINGS_PARAM_H__
