/**
 * @file SettingsChapterWrapper.h
 * @brief interfaces of SettingsPageWrapper class
 *
 * @author S.Tanaka
 * @date 2007.05.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SETTINGS_CHAPTER_WRAPPER_H__
#define __KOME_CLR_SETTINGS_CHAPTER_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class SettingsPageWrapper;

		/**
		 * @class SettingsChapterWrapper
		 * @brief settings chapter wrapper class to use on CLR environment
		 */
		public ref class SettingsChapterWrapper {
		public:
			/**
			 * @fn SettingsChapterWrapper( kome::plugin::SettingsChapter& chapter )
			 * @brief constructor
			 * @param[in] chapter settings chapter object to be wrapped
			 */
			SettingsChapterWrapper( kome::plugin::SettingsChapter& chapter );

			/**
			 * @fn virtual ~SettingsChapterWrapper()
			 * @brief destructor
			 */
			virtual ~SettingsChapterWrapper();

		protected:
			/** settings chapter object to be wrapped */
			kome::plugin::SettingsChapter& m_chapter;

		public:
			/**
			 * @fn kome::plugin::SettingsChapter& getSettingsChapter()
			 * @brief gets settings chapter object
			 * @return settings chapter object
			 */
			kome::plugin::SettingsChapter& getSettingsChapter();

		public:
			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets chapter name
			 * @param[in] name chapter name
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets chapter name
			 * @return chapter name
			 */
			System::String^ getName();

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
			 * @fn SettingsChapterWrapper^ getParentChapter()
			 * @brief gets parent chapter
			 * @return parent chapter
			 */
			SettingsChapterWrapper^ getParentChapter();

			/**
			 * @fn SettingsChapterWrapper^ createChildChapter()
			 * @brief creates child chapter
			 * @return created child chapter
			 */
			SettingsChapterWrapper^ createChildChapter();

			/**
			 * @fn unsigned int getNumberOfChildren()
			 * @brief gets the number of child chapters
			 * @return the number of child chapters
			 */
			unsigned int getNumberOfChildren();

			/**
			 * @fn SettingsChapterWrapper^ getChildChapter( unsigned int index )
			 * @brief gets child chapter
			 * @param[in] index child index
			 * @return child chapter (If NULL, the index is illegal.)
			 */
			SettingsChapterWrapper^ getChildChapter( unsigned int index );

			/**
			 * @fn SettingsChapterWrapper^ getChildChapter( System::String^ name )
			 * @brief gets child chapter
			 * @param[in] name chapter name
			 * @return child chapter
			 */
			SettingsChapterWrapper^ getChildChapter( System::String^ name );

			/**
			 * @fn void addPage( SettingsPageWrapper^ page )
			 * @brief adds page
			 * @param[in] page page object to be added
			 */
			void addPage( SettingsPageWrapper^ page );

			/**
			 * @fn unsigned int getNumberOfPages()
			 * @brief gets the number of pages
			 * @return the number of pages
			 */
			unsigned int getNumberOfPages();

			/**
			 * @fn SettingsPageWrapper^ getPage( unsigned int index )
			 * @brief gets settings page
			 * @param[in] index settings page index
			 * @return settings page  (If NULL, the index is illegal.)
			 */
			SettingsPageWrapper^ getPage( unsigned int index );

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
		};
	}
}

#endif		// __KOME_CLR_SETTINGS_CHAPTER_WRAPPER_H__
