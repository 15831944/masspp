/**
 * @file PluginFunctionItem.h
 * @brief interfaces of PluginFunctionItem class
 *
 * @author S.Tanaka
 * @date 2007.08.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_PLUGIN_FUNCTION_ITEM__
#define __KOME_PLUGIN_PLUGIN_FUNCTION_ITEM__


namespace kome {
	namespace plugin {

		class PluginCall;
		class SettingsPage;

		/**
		 * @class PluginFunctionItem
		 * @brief function item information class
		 */
		class PLUGIN_TOOLS_CLASS PluginFunctionItem {
		public:
			/**
			 * @fn PluginFunctionItem()
			 * @brief constructor
			 */
			PluginFunctionItem();

			/**
			 * @fn virtual ~PluginFunctionItem()
			 * @brief destructor
			 */
			virtual ~PluginFunctionItem();

		protected:
			/** detect function */
			PluginCall* m_call;

			/** long name */
			std::string m_longName;

			/** short name */
			std::string m_shortName;

			/** parameter settings page */
			SettingsPage* m_page;

			/** description */
			std::string m_desc;

		public:
			/**
			 * @fn void setCall( PluginCall* call )
			 * @brief sets detect function
			 * @param[in] call detect function object
			 */
			void setCall( PluginCall* call );

			/**
			 * @fn PluginCall* getCall()
			 * @brief get detect function object
			 * @return detect function object
			 */
			PluginCall* getCall();

			/**
			 * @fn void setLongName( const char* name )
			 * @brief sets long name
			 * @param[in] name long name
			 */
			void setLongName( const char* name );

			/**
			 * @fn const char* getLongName()
			 * @brief gets long name
			 * @return long name
			 */
			const char* getLongName();

			/**
			 * @fn void setShortName( const char* name )
			 * @brief sets short name
			 * @param[in] name short name
			 */
			void setShortName( const char* name );

			/**
			 * @fn const char* getShortName()
			 * @brief gets short name
			 * @return short name
			 */
			const char* getShortName();

			/**
			 * @fn void setSettingsPage( SettingsPage* page )
			 * @brief sets parameter settings page
			 * @param[in] page parameter settings page object
			 */
			void setSettingsPage( SettingsPage* page );

			/**
			 * @fn SettingsPage* getSettingsPage()
			 * @brief gets parameter settings page
			 * @return parameter settinigs page object
			 */
			SettingsPage* getSettingsPage();

			/**
			 * @fn void setDescription( const char* desc )
			 * @brief sets the description
			 * @param[in] desc description
			 */
			void setDescription( const char* desc );

			/**
			 * @fn const char* getDescription()
			 * @brief gets the description
			 * @return description
			 */
			const char* getDescription();

		public:
			/**
			 * @fn virtual void setInfo( PluginCall* call )
			 * @brief sets local members from function object
			 * @param[in] call detect function object
			 */
			virtual void setInfo( PluginCall* call );
		};
	}
}

#endif		// __KOME_PLUGIN_PLUGIN_FUNCTION_ITEM__
