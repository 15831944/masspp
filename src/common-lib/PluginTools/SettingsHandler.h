/**
 * @file SettingsHandler.h
 * @brief interfaces of SettingsHandler class
 *
 * @author S.Tanaka
 * @date 2007.01.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_SETTINGS_HANDLER_H__
#define __KOME_PLUGIN_SETTINGS_HANDLER_H__

#include <map>


namespace kome {
	namespace plugin {

		class PluginInfo;
		class ParameterSettings;
		class SettingsValue;
		class SettingsGroup;
		class SettingsPage;
		class SettingsParam;
		class SettingsChapter;
		class SettingsForm;

		/**
		 * @class SettingsHandler
		 * @brief XML handler class to read settings file.
		 */
		class SettingsHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn SettingsHandler( ParameterSettings& settings )
			 * @brief constructor
			 * @param[in] settings parameter settings object
			 */
			SettingsHandler( ParameterSettings& settings );

			/**
			 * @fn virtual ~SettingsHandler()
			 * @brief destructor
			 */
			virtual ~SettingsHandler();

		protected:
			/** parameter settings object */
			ParameterSettings& m_settings;

		protected:
			/** current settings value */
			SettingsValue* m_currentValue;

			/** settings value map */
			std::map< int, SettingsValue* > m_valueMap;

			/** enum section */
			std::string m_enumSection;

			/** enum name */
			std::string m_enumName;

			/** enum value */
			std::string m_enumVal;

			/** enum description */
			std::string m_enumDesc;

			/** filter name */
			std::string m_filterName;

			/** filter ext */
			std::string m_filterExt;

			/** filter description */
			std::string m_filterDesc;

			/** current settings group */
			SettingsGroup* m_currentGroup;

			/** settings group map */
			std::map< int, SettingsGroup* > m_groupMap;

			/** current settings page */
			SettingsPage* m_currentPage;

			/** current settings parameter */
			SettingsParam* m_currentParam;

			/** settings parameter map */
			std::map< int, SettingsParam* > m_paramMap;

			/** chapter map */
			std::map< int, SettingsChapter* > m_chapterMap;

			/** current chapter */
			SettingsChapter* m_currentChapter;

			/** current form */
			SettingsForm* m_currentForm;

			/** chapter ID */
			int m_chapterId;

			/** chapter order */
			int m_chapterOrder;
		protected:
			/**
			 * @fn virtual void onStartDocument()
			 * @brief This method is called by startDocument method. (override method)
			 */
			virtual void onStartDocument();

			/**
			 * @fn virtual void onEndDocument()
			 * @brief This method is called by endDocument method. (override method)
			 */
			virtual void onEndDocument();

			/**
			 * @fn virtual void onStartElement( const char* name, kome::core::Properties& attrs )
			 * @brief This method is called by startElement method. (override method)
			 * @param[in] name tag name
			 * @param[in] attrs attributes
			 */
			virtual void onStartElement( const char* name, kome::core::Properties& attrs );

			/**
			 * @fn virtual void onEndElement( const char* name, const char* text )
			 * @brief This method is called by end element method. (override method)
			 * @param[in] name tag name
			 * @param[in] text body text
			 */
			virtual void onEndElement( const char* name, const char* text );

		
		};
	}
}

#endif		// __KOME_PLUGIN_SETTINGS_HANDLER_H__
