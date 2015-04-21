/**
 * @file SettingsPageWrapper.h
 * @brief interfaces of SettingsPageWrapper class
 *
 * @author S.Tanaka
 * @date 2007.02.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SETTINGS_PAGE_WRAPPER_H__
#define __KOME_CLR_SETTINGS_PAGE_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class SettingsFormWrapper;		
		ref class ParameterSettingsWrapper;
		ref class PluginFunctionItemWrapper;

		/**
		 * @class SettingsPageWrapper
		 * @brief settings page wrapper class to use on CLR environment
		 */
		public ref class SettingsPageWrapper {
		public:
			/**
			 * @fn SettingsPageWrapper( kome::plugin::SettingsPage& page )
			 * @brief constructor
			 * @param[in] page settings page object to be wrapped
			 */
			SettingsPageWrapper( kome::plugin::SettingsPage& page );

			/**
			 * @fn SettingsPageWrapper( ParameterSettingsWrapper^ settings )
			 * @brief constructor
			 * @param[in] settings parameter settings object
			 */
			SettingsPageWrapper( ParameterSettingsWrapper^ settings );

			/**
			 * @fn virtual ~SettingsPageWrapper()
			 * @brief destructor
			 */
			virtual ~SettingsPageWrapper();

		protected:
			/** settings page object to be wrapped */
			kome::plugin::SettingsPage* m_page;

			/** delete flag */
			bool m_delFlg;

		public:
			/**
			 * @fn kome::plugin::SettingsPage& getSettingsPage()
			 * @brief gets settings page object
			 */
			kome::plugin::SettingsPage& getSettingsPage();

		public:
			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets page name
			 * @param[in] name page name
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets page name
			 * @return page name
			 */
			System::String^ getName();

			/**
			 * @fn void setTitle( System::String^ title )
			 * @brief sets page title
			 * @param[in] title page title
			 */
			void setTitle( System::String^ title );

			/**
			 * @fn System::String^ getTitle()
			 * @brief gets page title
			 * @return page title
			 */
			System::String^ getTitle();

			/**
			 * @fn void setOrder( int order )
			 * @brief sets order
			 * @param[in] order order
			 */
			void setOrder( int order );

			/**
			 * @fn int getOrder()
			 * @brief gets order
			 * @return order
			 */
			int getOrder();

			/**
			 * @fn void setCommon( bool common )
			 * @brief sets common page flag
			 * @param[in] common common page flag value
			 */
			void setCommon( bool common );

			/**
			 * @fn bool isCommon();
			 * @brief gets common page flag
			 * @return common page flag value
			 */
			bool isCommon();

			/**
			 * @fn SettingsFormWrapper^ addForm()
			 * @brief adds form
			 * @return the pointer to new form information object
			 */
			SettingsFormWrapper^ addForm();

			/**
			 * @fn unsigned int getNumberOfForms()
			 * @brief gets the number of forms
			 * @return the number of forms
			 */
			unsigned int getNumberOfForms();

			/**
			 * @fn SettingsFormWrapper^ getForm( unsigned int index )
			 * @brief gets the form
			 * @param[in] index form index
			 * @return form information object (If the index is illegal, this method returns NULL.)
			 */
			SettingsFormWrapper^ getForm( unsigned int index );

		public:
			/**
			 * @fn void setParameters(
					SettingParameterValuesWrapper^ settings,
					System::String^ prefix
				)
			 * @brief sets parameters
			 * @param[out] settings the object to store parameter values
			 * @param[in] prefix parameter key prefix
			 */
			void setParameters(
				SettingParameterValuesWrapper^ settings,
				System::String^ prefix
			);


			/**
			 * @fn void clearChecks()
			 * @brief clears check functions
			 */
			void clearChecks();
			
			/**
			 * @fn void addCheck( PluginFunctionItemWrapper^ fun )
			 * @brief adds check function
			 * @parma[in] fun check function
			 */
			void addCheck( PluginFunctionItemWrapper^ fun );

			/**
			 * @fn unsigned int getNumberOfChecks()
			 * @brief gets the number of check functions
			 * @return the numnber of check functions
			 */
			unsigned int getNumberOfChecks();

			/**
			 * @fn PluginFunctionItemWrapper^ getCheck( unsigned int idx )
			 * @brief gets the check function
			 * @param[in] idx check function index
			 * @return check function
			 */
			PluginFunctionItemWrapper^ getCheck( unsigned int idx );

        public:
			/**
			 * @fn System::String^ getParameterString( SettingParameterValuesWrapper^ settings )
			 * @brief gets parameter string
			 * @param[in] settings settings infromation
			 * @return parameter string
			 */
			System::String^ getParameterString( SettingParameterValuesWrapper^ settings );

			/**
			 * @fn void setParameterString( SettingParameterValuesWrapper^ settings, System::String^ str )
			 * @brief sets parameter string
			 * @param[out] settings settings object to store
			 * @param[in] str parameter string
			 */
			 void setParameterString( SettingParameterValuesWrapper^ settings, System::String^ str );
		};
	}
}

#endif		// __KOME_CLR_SETTINGS_PAGE_WRAPPER_H__
