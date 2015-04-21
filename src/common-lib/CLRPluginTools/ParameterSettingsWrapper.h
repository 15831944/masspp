/**
 * @file ParameterSettingsWrapper.h
 * @brief interfaces of ParameterSettingsWrapper class
 *
 * @author S.Tanaka
 * @date 2007.01.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PARAMETER_SETTINGS_WRAPPER_H__
#define __KOME_CLR_PARAMETER_SETTINGS_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class PluginInfoWrapper;
		ref class SettingsValueWrapper;
		ref class SettingsParamWrapper;
		ref class SettingsGroupWrapper;
		ref class SettingsPageWrapper;

		/**
		 * @class ParameterSettingsWrapper
		 * @brief parameter settings wrapper class to use on CLR environment
		 */
		public ref class ParameterSettingsWrapper {
		public:
			/**
			 * @fn ParameterSettingsWrapper( kome::plugin::ParameterSettings& settings )
			 * @brief constructor
			 * @param[in] settings parameter settings object to be wrapped
			 */
			ParameterSettingsWrapper( kome::plugin::ParameterSettings& settings );

			/**
			 * @fn ParameterSettingsWrapper( bool isIni )
			 * @brief constructor
			 * @param[in] isIni inifile flag
			 */
			ParameterSettingsWrapper( bool isIni );

			/**
			 * @fn virtual ~ParameterSettingsWrapper()
			 * @brief destructor
			 */
			virtual ~ParameterSettingsWrapper();

		protected:
			/** parameter settings object to be wrapped */
			kome::plugin::ParameterSettings* m_settings;

			/** delete flag */
			bool m_delFlg;

		public:
			/**
			 * @fn kome::plugin::ParameterSettings& getParameterSettings()
			 * @brief gets parameter settings object
			 * @return parameter settings object
			 */
			kome::plugin::ParameterSettings& getParameterSettings();

		public:
			/**
			 * @fn PluginInfoWrapper^ getPlugin()
			 * @brief gets plug-in information object
			 */
			PluginInfoWrapper^ getPlugin();

			/**
			 * @fn bool isIni()
			 * @brief ini file flag
			 * @return ini file flag value
			 */
			bool isIni();

			/**
			 * @fn bool loadSettingsFile( System::String^ path )
			 * @brief sets settings file path
			 * @param[in] path settings file path
			 */
			bool loadSettingsFile( System::String^ path );

			/**
			 * @fn System::String^ getSettingsFilePath()
			 * @brief gets settings file path
			 * @return settings file path
			 */
			System::String^ getSettingsFilePath();

			/**
			 * @fn SettingsValueWrapper^ addValue()
			 * @brief adds stttings value
			 * @return the pointer to new settings value object
			 */
			SettingsValueWrapper^ addValue();

			/**
			 * @fn unsigned int getNumberOfValues()
			 * @brief gets the number of settings values
			 * @return the number of settings values
			 */
			unsigned int getNumberOfValues();

			/**
			 * @fn SettingsValueWrapper^ getValue( unsigned int index )
			 * @brief gets settings value
			 * @param[in] index settings value index
			 * @return settings value information (If the index is illegal, this method returns NULL.)
			 */
			SettingsValueWrapper^ getValue( unsigned int index );

			/**
			 * @fn SettingsParamWrapper^ addParam()
			 * @brief adds parameter
			 * @return the pointer to new parameter information object
			 */
			SettingsParamWrapper^ addParam();

			/**
			 * @fn unsigned int getNumberOfParams()
			 * @brief gets the number of parameters
			 * @return the number of parameters
			 */
			unsigned int getNumberOfParams();

			/**
			 * @fn SettingsParamWrapper^ getParam( unsigned int index )
			 * @brief gets the parameter
			 * @param[in] index parameter index
			 * @return parameter information object (If the index is illegal, this method returns NULL.)
			 */
			SettingsParamWrapper^ getParam( unsigned int index );

			/**
			 * @fn SettingsGroupWrapper^ addGroup()
			 * @brief adds settings group
			 * @return the pointer to new settings group object
			 */			
			SettingsGroupWrapper^ addGroup();

			/**
			 * @fn unsigned int getNumberOfGroups()
			 * @brief gets the number of settings groups
			 * @return the number of settings groups
			 */
			unsigned int getNumberOfGroups();

			/**
			 * @fn SettingsGroupWrapper^ getGroup( unsigned int index )
			 * @brief gets settings group
			 * @param[in] index settings group index
			 * @return settings group information (If the index is illegal, this method returns NULL.)
			 */
			SettingsGroupWrapper^ getGroup( unsigned int index );

			/**
			 * @fn void addPage( SettingsPageWrapper^ page )
			 * @brief adds settings page
			 * @param[in] page settings page to be added
			 */
			void addPage( SettingsPageWrapper^ page );

			/**
			 * @fn unsigned int getNumberOfPages()
			 * @brief gets the number of setting pages
			 * @return the number of setting pages
			 */
			unsigned int getNumberOfPages();			

			/**
			 * @fn SettingsPageWrapper^ getPage( unsigned int index )
			 * @brief gets settings page
			 * @param[in] index page index
			 * @return settings page (If NULL, the index is illegal.)
			 */
			SettingsPageWrapper^ getPage( unsigned int index );

			/**
			 * @fn SettingsPageWrapper^ getPage( System::String^ name )
			 * @brief gets settings page
			 * @param[in] name page name
			 * @return settings page (If NULL, the name is illegal.)
			 */
			SettingsPageWrapper^ getPage( System::String^ name );
		};
	}
}

#endif		// __KOME_CLR_PARAMETER_SETTINGS_WRAPPER_H__
