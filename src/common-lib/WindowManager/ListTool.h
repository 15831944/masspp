/**
 * @file ListTool.h
 * @brief interfaces of ListTool class
 *
 * @author S.Tanaka
 * @date 2006.09.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_LIST_TOOL_H__
#define __KOME_WINDOW_LIST_TOOL_H__


class wxListCtrl;


namespace kome {
	namespace window {

		/**
		 * @class ListTool
		 * @brief This class has some functions for list
		 */
		class WINDOW_MANAGER_CLASS ListTool {
		public:
			/**
			 * @fn static void getSelectedItems(
					wxListCtrl* listCtrl,
					std::vector< int >& items
				)
			 * @brief gets selected items
			 * @param[in] listCtrl list control
			 * @param[out] items the array to store selected item number
			 */
			static void getSelectedItems(
				wxListCtrl* listCtrl,
				std::vector< int >& items
			);

			/**
			 * @fn static void selectAll( wxListCtrl* listCtrl )
			 * @brief selects all items
			 * @param[in] listCtrl list control
			 */
			static void selectAll( wxListCtrl* listCtrl );

			/**
			 * @fn static void unselectAll( wxListCtrl* listCtrl )
			 * @brief unselects all items
			 * @param[in] listCtrl list control
			 */
			static void unselectAll( wxListCtrl* listCtrl );

			/**
			 * @fn static bool isSelected( wxListCtrl* listCtrl, int item )
			 * @brief judges whether the specified list item is selected
			 * @param[in] listCtrl list control
			 * @param[in] item item
			 */
			static bool isSelected( wxListCtrl* listCtrl, int item );

			/**
			 * @fn static void selectItem( wxListCtrl* listCtrl, int item )
			 * @brief selects specified list item
			 * @param[in] listCtrl list control
			 * @param[in] item item
			 */
			static void selectItem( wxListCtrl* listCtrl, int item );

			/**
			 * @fn static void unselectItem( wxListCtrl* listCtrl, int item )
			 * @brief unselects specified list item
			 * @param[in] listCtrl list control
			 * @param[in] item item
			 */
			static void unselectItem( wxListCtrl* listCtrl, int item );
		};
	}
}


#endif		// __KOME_WINDOW_LIST_TOOL_H__
