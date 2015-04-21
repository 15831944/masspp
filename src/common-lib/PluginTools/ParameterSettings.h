/**
 * @file ParameterSettings.h
 * @brief interfaces of ParameterSettings class
 *
 * @author S.Tanaka
 * @date 2007.01.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_PARAMETER_SETTINGS_H__
#define __KOME_PLUGIN_PARAMETER_SETTINGS_H__


#include <string>
#include <vector>


namespace kome {
	namespace plugin {

		class PluginInfo;
		class SettingsPage;
		class SettingsValue;
		class SettingsParam;
		class SettingsGroup;

		/**
		 * @class ParameterSettings
		 * @brief parameter settings management class
		 */
		class PLUGIN_TOOLS_CLASS ParameterSettings {
		public:
			/**
			 * @fn ParameterSettings( PluginInfo* plugin, const bool isIni )
			 * @brief constructor
			 * @param[in] plugin plug-in information object
			 * @param[in] isIni ini file flag
			 */
			ParameterSettings( PluginInfo* plugin, const bool isIni );

			/**
			 * @fn virtual ~ParameterSettings()
			 * @brief destructor
			 */
			virtual ~ParameterSettings();

		protected:
			/** plug-in information object */
			PluginInfo* m_plugin;

			/** settings file path */
			std::string m_settingsFilePath;

			/** settings values */
			std::vector< SettingsValue* > m_values;

			/** settings parameters */
			std::vector< SettingsParam* > m_params;

			/** settings groups */
			std::vector< SettingsGroup* > m_groups;

			/** settings page */
			std::vector< SettingsPage* > m_pages;

			/** ini file flag */
			bool m_isIni;

		public:
			/**
			 * @fn PluginInfo* getPlugin()
			 * @brief gets plug-in information object
			 */
			PluginInfo* getPlugin();

			/**
			 * @fn bool isIni()
			 * @brief ini file flag
			 * @return ini file flag value
			 */
			bool isIni();

			/**
			 * @fn bool loadSettingsFile( const char* path )
			 * @brief loads sets settings file
			 * @param[in] path settings file path
			 */
			bool loadSettingsFile( const char* path );

			/**
			 * @fn const char* getSettingsFilePath()
			 * @brief gets settings file path
			 * @return settings file path
			 */
			const char* getSettingsFilePath();

			/**
			 * @fn SettingsValue* addValue()
			 * @brief adds settings value
			 * @return the pointer to new settings value object
			 */
			SettingsValue* addValue();

			/**
			 * @fn unsigned int getNumberOfValues()
			 * @brief gets the number of settings values
			 * @return the number of settings values
			 */
			unsigned int getNumberOfValues();

			/**
			 * @fn SettingsValue* getValue( const unsigned int index )
			 * @brief gets settings value
			 * @param[in] index settings value index
			 * @return settings value information (If the index is illegal, this method returns NULL.)
			 */
			SettingsValue* getValue( const unsigned int index );

			/**
			 * @fn SettingsParam* addParam()
			 * @brief adds settings parameter
			 * @return the pointer to new settings parameter object
			 */
			SettingsParam* addParam();

			/**
			 * @fn unsigned int getNumberOfParams()
			 * @brief gets the number of settings parameters
			 * @return the number of settings parameters
			 */			
			unsigned int getNumberOfParams();

			/**
			 * @fn SettingsParam* getParam( const unsigned int index )
			 * @brief gets settings parameter
			 * @param[in] index settings parameter index
			 * @return settings parameter infromation (If the index is illegal, this method returns NULL.)
			 */
			SettingsParam* getParam( const unsigned int index );

			/**
			 * @fn SettingsGroup* addGroup()
			 * @brief adds settings group
			 * @return the pointer to new settings group object
			 */			
			SettingsGroup* addGroup();

			/**
			 * @fn unsigned int getNumberOfGroups()
			 * @brief gets the number of settings groups
			 * @return the number of settings groups
			 */
			unsigned int getNumberOfGroups();

			/**
			 * @fn SettingsGroup* getGroup( const unsigned int index )
			 * @brief gets settings group
			 * @param[in] index settings group index
			 * @return settings group information (If the index is illegal, this method returns NULL.)
			 */
			SettingsGroup* getGroup( const unsigned int index );

			/**
			 * @fn void addPage( SettingsPage* page )
			 * @brief adds settings page
			 * @param[in] page settings page to be added
			 */
			void addPage( SettingsPage* page );

			/**
			 * @fn unsigned int getNumberOfPages()
			 * @brief gets the number of setting pages
			 * @return the number of setting pages
			 */
			unsigned int getNumberOfPages();			

			/**
			 * @fn SettingsPage* getPage( const unsigned int index )
			 * @brief gets settings page
			 * @param[in] index page index
			 * @return settings page (If NULL, the index is illegal.)
			 */
			SettingsPage* getPage( const unsigned int index );

			/**
			 * @fn SettingsPage* getPage( const char* name )
			 * @brief gets settings page
			 * @param[in] name page name
			 * @return settings page (If NULL, the name is illegal.)
			 */
			SettingsPage* getPage( const char* name );
		};
	}
}

#endif // __KOME_PLUGIN_PARAMETER_SETTINGS_H__
