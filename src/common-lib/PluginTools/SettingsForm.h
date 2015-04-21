/**
 * @file SettingsForm.h
 * @brief interfaces of SettingsParam class
 *
 * @author S.Tanaka
 * @date 2007.08.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_SETTINGS_FORM_H__
#define __KOME_PLUGIN_SETTINGS_FORM_H__


#include <string>


namespace kome {
	namespace plugin {

		class SettingsGroup;
		class SettingsParam;

		/**
		 * @class SettingsForm
		 * @brief settings form information class
		 */
		class PLUGIN_TOOLS_CLASS SettingsForm {
		public:
			/**
			 * @fn SettingsForm()
			 * @brief constructor
			 */
			SettingsForm();

			/**
			 * @fn ~SettingsForm()
			 * @brief destructor
			 */
			~SettingsForm();

		protected:
			/** title */
			std::string m_title;

			/** description */
			std::string m_desc;

			/** group */
			SettingsGroup* m_group;

			/** parameter */
			SettingsParam* m_param;

			/** details view */
			bool m_detail;

			/** image path */
			std::string m_img;

		public:
			/**
			 * @fn void setTitle( const char* title )
			 * @brief sets form title
			 * @param[in] title form title
			 */
			void setTitle( const char* title );

			/**
			 * @fn const char* getTitle()
			 * @brief gets form title
			 * @return form title
			 */
			const char* getTitle();

			/**
			 * @fn void setDesc( const char* desc )
			 * @brief sets form description
			 * @param[in] desc form description
			 */
			void setDesc( const char* desc );

			/**
			 * @fn const char* getDesc()
			 * @brief gets form description
			 * @return form description
			 */
			const char* getDesc();

			/**
			 * @fn void setImageFilePath( const char* path )
			 * @brief sets the image file path
			 * @param[in] path image file path
			 */
			void setImageFilePath( const char* path );

			/**
			 * @fn const char* getImageFilePath()
			 * @brief gets the image file path
			 * @return image file path
			 */
			const char* getImageFilePath();

			/**
			 * @fn void setGroup( SettingsGroup* group )
			 * @brief sets settings group
			 * @param[in] group settings group
			 */
			void setGroup( SettingsGroup* group );

			/**
			 * @fn SettingsGroup* getGroup()
			 * @brief gets settings group
			 * @return settings group
			 */
			SettingsGroup* getGroup();

			/**
			 * @fn void setParam( SettingsParam* param )
			 * @brief sets settings parameter
			 * @param[in] param settings parameter
			 */
			void setParam( SettingsParam* param );

			/**
			 * @fn SettingsParam* getParam()
			 * @brief gets settings parameter
			 * @return settings parameter
			 */
			SettingsParam* getParam();

			/**
			 * @fn void setDetail( const bool detail )
			 * @brief sets detail flag
			 * @param detail detail flag value
			 */
			void setDetail( const bool detail );

			/**
			 * @fn bool isDetail()
			 * @brief gets detail flag value
			 * @return detail flag value
			 */
			bool isDetail();

			/** Context Menu */
		private:
			/** Edit Menu Item */
			bool m_bEditMenuItem;

			/** Add Menu Item */
			bool m_bAddMenuItem;

			/** Up Down Menu Item */
			bool m_bUpDownMenuItem;

			/** Delete Menu Item */
			bool m_bDeleteMenuItem;

			/** Copy Menu Item */
			bool m_bCopyMenuItem;

			/** Save All Menu Item */
			bool m_bSaveAllMenuItem;

			/** Select All Menu Item */
			bool m_bSelectAllMenuItem;

			/** Filter Menu Item */
			bool m_bFilterMenuItem;

			/** Edit Menu Name */
			std::string m_strEditMenuName;

			/** Add Menu Name */
			std::string m_strAddMenuName;

		public:
			/**
			 * @fn bool hasEditMenuItem( void )
			 * @brief has Edit Menu Item
			 * @return The flag of whether this menu is displayed 
			 */
			bool hasEditMenuItem( void );

			/**
			 * @fn bool hasAddMenuItem( void )
			 * @brief has Add Menu Item
			 * @return The flag of whether this menu is displayed 
			 */
			bool hasAddMenuItem( void );

			/**
			 * @fn bool hasUpDownMenuItem( void )
			 * @brief has Up Down Menu Item
			 * @return The flag of whether this menu is displayed 
			 */
			bool hasUpDownMenuItem( void );

			/**
			 * @fn bool hasDeleteMenuItem( void )
			 * @brief has Copy Menu Item
			 * @return The flag of whether this menu is displayed 
			 */
			bool hasDeleteMenuItem( void );

			/**
			 * @fn bool hasCopyMenuItem( void )
			 * @brief has Copy Menu Item
			 * @return The flag of whether this menu is displayed 
			 */
			bool hasCopyMenuItem( void );

			/**
			 * @fn bool hasSaveAllMenuItem( void )
			 * @brief has Save All Menu Item
			 * @return The flag of whether this menu is displayed 
			 */
			bool hasSaveAllMenuItem( void );

			/**
			 * @fn bool hasSelectAllMenuItem( void )
			 * @brief has Select All Menu Item
			 * @return The flag of whether this menu is displayed 
			 */
			bool hasSelectAllMenuItem( void );

			/**
			 * @fn bool hasFilterMenuItem( void )
			 * @brief has Filter Menu Item
			 * @return The flag of whether this menu is displayed 
			 */
			bool hasFilterMenuItem( void );

			/**
			 * @fn void setEditMenuItem( bool bVal )
			 * @brief set Edit Menu Item
			 * @param bVal The flag of whether this menu is displayed 
			 */
			void setEditMenuItem( bool bVal );

			/**
			 * @fn void setAddMenuItem( bool bVal )
			 * @brief set Up Down Menu Item
			 * @param bVal The flag of whether this menu is displayed 
			 */
			void setAddMenuItem( bool bVal );

			/**
			 * @fn void setUpDownMenuItem( bool bVal )
			 * @brief set Up Down Menu Item
			 * @param bVal The flag of whether this menu is displayed 
			 */
			void setUpDownMenuItem( bool bVal );

			/**
			 * @fn void setDeleteMenuItem( bool bVal )
			 * @brief set Delete Menu Item
			 * @param bVal The flag of whether this menu is displayed 
			 */
			void setDeleteMenuItem( bool bVal );

			/**
			 * @fn void setCopyMenuItem( bool bVal )
			 * @brief set Copy Menu Item
			 * @param bVal The flag of whether this menu is displayed 
			 */
			void setCopyMenuItem( bool bVal );

			/**
			 * @fn void setSaveAllMenuItem( bool bVal )
			 * @brief set Save All Menu Item
			 * @param bVal The flag of whether this menu is displayed 
			 */
			void setSaveAllMenuItem( bool bVal );

			/**
			 * @fn void setSelectAllMenuItem( bool bVal )
			 * @brief set Select All MenuItem
			 * @param bVal The flag of whether this menu is displayed 
			 */
			void setSelectAllMenuItem( bool bVal );

			/**
			 * @fn void setFilterMenuItem( bool bVal )
			 * @brief set Filter Menu Item
			 * @param bVal The flag of whether this menu is displayed 
			 */
			void setFilterMenuItem( bool bVal );

			/**
			 * @fn void setEditMenuName( std::string strName )
			 * @brief set Edit Menu Name
			 * @param strName Edit Menu Name
			 */
			void setEditMenuName( std::string strName );

			/**
			 * @fn void setAddMenuName( std::string strName )
			 * @brief set Add Menu Name
			 * @param strName Add Menu Name
			 */
			void setAddMenuName( std::string strName );

			/**
			 * @fn std::string getEditMenuName( void )
			 * @brief gets edit menu name
			 * @return edit menu name
			 */
			std::string getEditMenuName( void );

			/**
			 * @fn std::string getAddMenuName( void )
			 * @brief gets add menu name
			 * @return add menu name
			 */
			std::string getAddMenuName( void );

		};
	}
}


#endif		// __KOME_PLUGIN_SETTINGS_FORM_H__
