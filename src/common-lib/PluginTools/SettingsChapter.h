/**
 * @file SettingsChapter.h
 * @brief interfaces of SettingsChapter class
 *
 * @author S.Tanaka
 * @date 2007.05.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_SETTINGS_CHAPTER_H__
#define __KOME_PLUGIN_SETTINGS_CHAPTER_H__


#include <string>
#include <vector>


namespace kome {
	namespace plugin {

		class SettingsPage;

		/**
		 * @class SettingsChapter
		 * @brief settings chapter information management class
		 */
		class PLUGIN_TOOLS_CLASS SettingsChapter {
		public:
			/**
			 * @fn SettingsChapter()
			 * @brief constructor
			 */
			SettingsChapter();

			/**
			 * @fn virtual ~SettingsChapter()
			 * @brief destructor
			 */
			virtual ~SettingsChapter();

		protected:
			/** chapter name */
			std::string m_name;

			/** order */
			int m_order;

			/** parent chapter */
			SettingsChapter* m_parent;

			/** children */
			std::vector< SettingsChapter* > m_children;

			/** page */
			std::vector< SettingsPage* > m_pages;

			/** Context Menu */
			bool m_flgContextDisplay;	// @date 2012/11/22 <Add> Context Menu

		public:
			/**
			 * @fn void setName( const char* name )
			 * @brief sets chapter name
			 * @param[in] name chapter name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets chapter name
			 * @return chapter name
			 */
			const char* getName();

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
			 * @fn SettingsChapter* getParentChapter()
			 * @brief gets parent chapter
			 * @return parent chapter
			 */
			SettingsChapter* getParentChapter();

			/**
			 * @fn SettingsChapter* createChildChapter()
			 * @brief creates child chapter
			 * @return created child chapter
			 */
			SettingsChapter* createChildChapter();

			/**
			 * @fn unsigned int getNumberOfChildren()
			 * @brief gets the number of child chapters
			 * @return the number of child chapters
			 */
			unsigned int getNumberOfChildren();

			/**
			 * @fn SettingsChapter* getChildChapter( const unsigned int index )
			 * @brief gets child chapter
			 * @param[in] index child index
			 * @return child chapter (If NULL, the index is illegal.)
			 */
			SettingsChapter* getChildChapter( const unsigned int index );

			/**
			 * @fn SettingsChapter* getChildChapter( const char* name )
			 * @brief gets child chapter
			 * @param[in] name chapter name
			 * @return child chapter
			 */
			SettingsChapter* getChildChapter( const char* name );

			/**
			 * @fn void addPage( SettingsPage* page )
			 * @brief adds page
			 * @param[in] page page object to be added
			 */
			void addPage( SettingsPage* page );

			/**
			 * @fn unsigned int getNumberOfPages()
			 * @brief gets the number of pages
			 * @return the number of pages
			 */
			unsigned int getNumberOfPages();

			/**
			 * @fn SettingsPage* getPage( const unsigned int index )
			 * @brief gets settings page
			 * @param[in] index settings page index
			 * @return settings page  (If NULL, the index is illegal.)
			 */
			SettingsPage* getPage( const unsigned int index );

			/**
			 * @fn void sort()
			 * @brief sorts pages and child chapter
			 */
			void sort();

			/**
			 * @fn bool isCommon()
			 * @brief judges whether the chapter is common or not
			 * @return If true, this is common chapter
			 */
			bool isCommon();

		protected:
			/**
			 * @fn static bool lessChapter( SettingsChapter* c0, SettingsChapter* c1 )
			 * @brief compare to sort chapters
			 * @param[in] c0 chapter object to be compared
			 * @param[in] c1 chapter object to compare
			 * @return If c0 sorts before c1, this method returns true.
			 */
			static bool lessChapter( SettingsChapter* c0, SettingsChapter* c1 );

			/**
			 * @fn static bool lessPage( SettingsPage* p0, SettingsPage* p1 )
			 * @brief compare to sort pages
			 * @param[in] p0 page object to be compared
			 * @param[in] p1 page object to compare
			 * @return If p0 sorts before p1, this method returns true.
			 */
			static bool lessPage( SettingsPage* p0, SettingsPage* p1 );

		// Context Menu
		public:
			/**
			 * @fn void setFlgContextDisplay( bool flg )
			 * @brief set flag for Context Menu
			 * @param[in] flg this option display in context menu 
			 */
			void setFlgContextDisplay( bool flg );

			/**
			 * @fn bool getFlgContextDisplay( void )
			 * @brief set flag for Context Menu
			 * @return If true, this menu display in context menu.
			 */
			bool getFlgContextDisplay( void );

		};
	}
}


#endif		// __KOME_PLUGIN_SETTINGS_CHAPTER_H__
