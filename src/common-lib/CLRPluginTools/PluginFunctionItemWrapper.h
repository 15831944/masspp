/**
 * @file PluginFunctionItemWrapper.h
 * @brief interfaces of PluginFunctionItemWrapper class
 *
 * @author S.Tanaka
 * @date 2007.08.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PLUGIN_FUNCTION_ITEM_WRAPPER_H__
#define __KOME_CLR_PLUGIN_FUNCTION_ITEM_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class PluginCallWrapper;
		ref class SettingsPageWrapper;


		/**
		 * @class PluginFunctionItemWrapper
		 * @brief plug-in function item wrapper class to use on CLR environment
		 */
		public ref class PluginFunctionItemWrapper {
		public:
			/**
			 * @fn PluginFunctionItemWrapper( kome::plugin::PluginFunctionItem& item )
			 * @brief constructor
			 * @param[in] item plug-in fucntion item object to be wrapped
			 */
			PluginFunctionItemWrapper( kome::plugin::PluginFunctionItem& item );

			/**
			 * @fn virtual ~PluginFunctionItemWrapper()
			 * @brief destructor
			 */
			virtual ~PluginFunctionItemWrapper();

		protected:
			/** plug-in function item object to be wrapped */
			kome::plugin::PluginFunctionItem& m_item;

		public:
			/**
			 * @fn kome::plugin::PluginFunctionItem& getPluginFunctionItem()
			 * @brief gets plug-in function item object to be wrapped
			 * @return plug-in function item object
			 */
			kome::plugin::PluginFunctionItem& getPluginFunctionItem();

		public:
			/**
			 * @fn void setCall( PluginCallWrapper^ call )
			 * @brief sets detect function
			 * @param[in] call detect function object
			 */
			void setCall( PluginCallWrapper^ call );

			/**
			 * @fn PluginCallWrapper^ getCall()
			 * @brief get detect function object
			 * @return detect function object
			 */
			PluginCallWrapper^ getCall();

			/**
			 * @fn void setLongName( System::String^ name )
			 * @brief sets long name
			 * @param[in] name long name
			 */
			void setLongName( System::String^ name );

			/**
			 * @fn System::String^ getLongName()
			 * @brief gets long name
			 * @return long name
			 */
			System::String^ getLongName();

			/**
			 * @fn void setShortName( System::String^ name )
			 * @brief sets short name
			 * @param[in] name short name
			 */
			void setShortName( System::String^ name );

			/**
			 * @fn System::String^ getShortName()
			 * @brief gets short name
			 * @return short name
			 */
			System::String^ getShortName();

			/**
			 * @fn void setSettingsPage( SettingsPageWrapper^ page )
			 * @brief sets parameter settings page
			 * @param[in] page parameter settings page object
			 */
			void setSettingsPage( SettingsPageWrapper^ page );

			/**
			 * @fn SettingsPageWrapper^ getSettingsPage()
			 * @brief gets parameter settings page
			 * @return parameter settinigs page object
			 */
			SettingsPageWrapper^ getSettingsPage();

			/**
			 * @fn void setDescription( System::String^ desc )
			 * @brief sets the description
			 * @param[in] desc description
			 */
			void setDescription( System::String^ desc );

			/**
			 * @fn System::String^ getDescription()
			 * @brief gets the description
			 * @return description
			 */
			System::String^ getDescription();

		public:
			/**
			 * @fn virtual void setInfo( PluginCallWrapper^ call )
			 * @brief sets local members from function object
			 * @param[in] call detect function object
			 */
			virtual void setInfo( PluginCallWrapper^ call );
		};
	}
}


#endif		// __KOME_CLR_PLUGIN_FUNCTION_ITEM_WRAPPER_H__
