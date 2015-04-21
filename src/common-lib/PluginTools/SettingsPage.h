/**
 * @file SettingsPage.h
 * @brief interfaces of SettingsPage class
 *
 * @author S.Tanaka
 * @date 2007.01.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_SETTINGS_PAGE_H__
#define __KOME_PLUGIN_SETTINGS_PAGE_H__


#include <string>
#include <vector>

/** Context menu */
enum PropertiesPageType{
	PROP_NONE,
	PROP_SPEC_VIEW,
	PROP_CHROM_VIEW,
	PROP_HEATMAP_VIEW,
	PROP_3D_VIEW,
};


namespace kome {
	namespace plugin {

		class ParameterSettings;
		class SettingsForm;
		class PluginFunctionItem;

		/**
		 * @class SettingsPage
		 * @brief setting parameters page information management class
		 */
		class PLUGIN_TOOLS_CLASS SettingsPage {
		public:
			/**
			 * @fn SettingsPage( ParameterSettings& settings )
			 * @brief constructor
			 * @param[in] settings information
			 */
			SettingsPage( ParameterSettings& settings );

			/**
			 * @fn virtual ~SettingsPage()
			 * @brief destructor
			 */
			virtual ~SettingsPage();

		protected:
			/** settings */
			ParameterSettings* m_settings;

			/** page name */
			std::string m_name;

			/** page title */
			std::string m_title;

			/** order */
			int m_order;

			/** common page flag */
			bool m_commonFlag;

			/** forms */
			std::vector< SettingsForm* > m_forms;

			/** check functions */
			std::vector< PluginFunctionItem* > m_checks;

		private:
			/** PropertiesPageType */
			PropertiesPageType	m_PropertiesPageType;

		public:
			/**
			 * @fn PropertiesPageType getPropertiesPageType( void )
			 * @brief get properties page type
			 */
			PropertiesPageType getPropertiesPageType( void );

			/** 
			 * @fn void setPropertiesPageType( PropertiesPageType type )
			 * @brief set properties page type
			 */
			void setPropertiesPageType( PropertiesPageType type );

		public:
			/**
			 * @fn ParameterSettings& getSettings()
			 * @brief gets parameter settings information
			 * @return settings information object
			 */
			ParameterSettings& getSettings();

			/**
			 * @fn void setName( const char* name )
			 * @brief sets page name
			 * @param[in] name page name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets page name
			 * @return page name
			 */
			const char* getName();

			/**
			 * @fn void setTitle( const char* title )
			 * @brief sets page title
			 * @param[in] title page title
			 */
			void setTitle( const char* title );

			/**
			 * @fn const char* getTitle()
			 * @brief gets page title
			 * @return page title
			 */
			const char* getTitle();

			/**
			 * @fn void setOrder( const int order )
			 * @brief sets order
			 * @param[in] order order
			 */
			void setOrder( const int order );
	
			/**
			 * @fn int getOrder()
			 * @brief gets order
			 * @return order
			 */
			int getOrder();

			/**
			 * @fn void setCommon( const bool common )
			 * @brief sets common page flag
			 * @param[in] common common page flag value
			 */
			void setCommon( const bool common );

			/**
			 * @fn bool isCommon()
			 * @brief gets common page flag
			 * @return common page flag value
			 */
			bool isCommon();

			/**
			 * @fn SettingsForm* addForm()
			 * @brief adds form
			 * @return the pointer to new form information object
			 */
			SettingsForm* addForm();

			/**
			 * @fn unsigned int getNumberOfForms()
			 * @brief gets the number of forms
			 * @return the number of forms
			 */
			unsigned int getNumberOfForms();

			/**
			 * @fn SettingsForm* getForm( const unsigned int index )
			 * @brief gets the form
			 * @param[in] index form index
			 * @return form information object (If the index is illegal, this method returns NULL.)
			 */
			SettingsForm* getForm( const unsigned int index );

			/** 
			 * @fn void setParameters(
					kome::objects::SettingParameterValues& settings,
					const char* prefix = NULL
				)
			 * @brief sets paramters
			 * @param[out] settings the object to store parameter values
			 * @param[in] prefix parameter key prefix
			 */
			void setParameters(
				kome::objects::SettingParameterValues& settings,
				const char* prefix = NULL
			);

			/**
			 * @fn void clearChecks()
			 * @brief clears check functions
			 */
			void clearChecks();
			
			/**
			 * @fn void addCheck( PluginFunctionItem* fun )
			 * @brief adds check function
			 * @param[in] fun check function
			 */
			void addCheck( PluginFunctionItem* fun );

			/**
			 * @fn unsigned int getNumberOfChecks()
			 * @brief gets the number of check functions
			 * @return the numnber of check functions
			 */
			unsigned int getNumberOfChecks();

			/**
			 * @fn PluginFunctionItem* getCheck( const unsigned int idx )
			 * @brief gets the check function
			 * @param[in] idx check function index
			 * @return check function
			 */
			PluginFunctionItem* getCheck( const unsigned int idx );

		public:
			/**
			 * @fn std::string getParameterString( kome::objects::SettingParameterValues& settings )
			 * @brief gets parameter string
			 * @param[in] settings settings infromation
			 * @return parameter string
			 */
			std::string getParameterString( kome::objects::SettingParameterValues& settings );

			/**
			 * @fn void setParameterString( kome::objects::SettingParameterValues& settings, const char* str )
			 * @brief sets parameter string
			 * @param[out] settings settings object to store
			 * @param[in] str parameter string
			 */
			 void setParameterString( kome::objects::SettingParameterValues& settings, const char* str );
		};
	}
}

#endif		// __KOME_PLUGIN_SETTINGS_PAGE_H__
