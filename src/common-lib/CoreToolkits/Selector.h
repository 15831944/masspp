/**
 * @file Selector.h
 * @brief interfaces of Selector class
 *
 * @author S.Tanaka
 * @date 2007.12.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_SELECTOR_H__
#define __KOME_CORE_SELECTOR_H__


#include <string>
#include <vector>
#include <utility>


namespace kome {
	namespace core {

		/**
		 * @class Selector
		 * @brief selector class
		 */
		class CORE_TOOLKITS_CLASS Selector {
		public:
			/**
			 * @fn Selector()
			 * @brief constructor
			 */
			Selector();

			/**
			 * @fn virtual ~Selector()
			 * @brief destructor
			 */
			virtual ~Selector();

		protected:
			/** item value */
			std::vector< std::pair< std::string, std::string > > m_items;

			/** selected flags */
			std::vector< bool > m_selected;

			/** selected item */
			int m_sel;

		public:
			/**
			 * @fn void clearItems()
			 * @brief clear items
			 */
			void clearItems();

			/**
			 * @fn void addItem( const char* name )
			 * @brief adds item
			 * @param[in] name item name
			 */
			void addItem( const char* name );

			/**
			 * @fn void addItem( const char* name, const char* value )
			 * @brief adds item
			 * @param[in] name item name
			 * @param[in] value item value
			 */
			void addItem( const char* name, const char* value );

			/**
			 * @fn unsigned int getNumberOfItems()
			 * @brief gets the number of items
			 * @return the number of items
			 */			  
			unsigned int getNumberOfItems();

			/**
			 * @fn const char* getItemName( const unsigned int index )
			 * @brief gets item name
			 * @param[in] index item index
			 * @return item name (If NULL, item index is illegal.)
			 */
			const char* getItemName( const unsigned int index );

			/**
			 * @fn const char* getItemValue( const unsigned int index )
			 * @brief gets item value
			 * @param[in] index item index
			 * @return item value (If NULL, item index is illegal.)
			 */
			const char* getItemValue( const unsigned int index );

			/**
			 * @fn bool isSelected( const unsigned int index )
			 * @brief gets selection flag value
			 * @param[in] index item index
			  *@return selectin flag value
			  */
			bool isSelected( const unsigned int index );

			/**
			 * @fn int getSelection()
			 * @brief[in] gets selected item
			 * @return selected item
			 */
			int getSelection();

			/**
			 * @fn bool select()
			 * @brief selects item
			 * @return If true, the selection was normally done.
			 */
			bool select();

		protected:
			/**
			 * @fn virtual bool onSelect(
					const unsigned int num,
					const char** items,
					const char** values,
					bool* selected
				) = 0
			 * @brief This method is called by select method. (abstract method)
			 * @param[in] num the number of items
			 * @param[in] items the array of item names
			 * @param[in] values the array of item values
			 * @param[out] selected the array to store selected flags
			 * @return If true, the selection was normally done.
			 */
			virtual bool onSelect(
				const unsigned int num,
				const char** items,
				const char** values,
				bool* selected
			) = 0;
		};
	}
}


#endif		// __KOME_CORE_SELECTOR_H__
