/**
 * @file CheckGridListCtrl.h
 * @brief interfaces of CheckGridListCtrl class
 *
 * @author S.Tanaka
 * @date 2011.04.27
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_WINDOW_CHECK_GRID_LIST_CTRL_H__
#define __KOME_WINDOW_CHECK_GRID_LIST_CTRL_H__


#include "GridListCtrl.h"

#include <set>
#include <vector>


namespace kome {
	namespace window {

		/**
		 * @class CheckGridListCtrl
		 * @brief grid list control class
		 */
		class WINDOW_MANAGER_CLASS CheckGridListCtrl : public GridListCtrl {
		public:
			/**
			 * @fn CheckGridListCtrl(
						wxWindow* parent,
						const int id,
						const int width,
						const int height,
						const bool editable
					)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 * @param[in] width window width
			 * @param[in] height window height
			 * @param[in] editable editable flag
			 */
			CheckGridListCtrl(
				wxWindow* parent,
				const int id,
				const int width,
				const int height,
				const bool editable
			);

			/**
			 * @fn virtual ~CheckGridListCtrl()
			 * @brief destructor
			 */
			virtual ~CheckGridListCtrl();

		protected:
			/** selected data set */
			std::set< long long > m_selectedDataSet;

			/** checked box icon */
			wxIcon m_checkedIcon;

			/** unchecked box icon */
			wxIcon m_uncheckedIcon;

			/** default attribute */
			wxListItemAttr m_defaultAttr;

		public:
			/**
			 * @fn void setSelected( const long long data, const bool selected = true )
			 * @brief sets selected
			 * @param[in] data data
			 * @param[in] selected If true, the specified data is selected.
			 */
			void setSelected( const long long data, const bool selected = true );

			/**
			 * @fn bool isSelected( const long long data )
			 * @brief gets the specified data is selected or not
			 * @return If true, the specified data is selected.
			 */
			bool isSelected( const long long data );

			/**
			 * @fn void getSelectedData( std::vector< long long >& selectedData )
			 * @brief gets selected data
			 * @param[out] selectedData the array to store selected data
			 */
			void getSelectedData( std::vector< long long >& selectedData );

			/**
			 * @fn void selectAll()
			 * @brief selects all items
			 */
			void selectAll();

			/**
			 * @fn void deselectAll()
			 * @brief deselects all items
			 */
			void deselectAll();

			/**
			 * @fn virtual void onSelectAll()
			 * @brief This method is called when a all item is selected.
			 */
			virtual void onSelectAll();

			/**
			 * @fn virtual void onDeselectAll()
			 * @brief This method is called when a all item is deselected.
			 */
			virtual void onDeselectAll();


		public:
			/**
			 * @fn virtual wxListItemAttr* OnGetItemAttr(long item) const
			 * @brief gets list item attribute (Overridde method)
			 * @param[in] item item number
			 * @return list item attribute
			 */
			virtual wxListItemAttr* OnGetItemAttr( long item ) const;

			/**
			 * @fn virtual int OnGetItemImage( long item ) const
			 * @brief gets the image number of item
			 * @param item item number
			 * @return image number
			 */
			virtual int OnGetItemImage( long item ) const;

		protected:
			/**
			 * @fn void onSelectItem( wxListEvent& evt )
			 * @brief This method is called when a item is selected.
			 * @param evt list event
			 */
			void onSelectItem( wxListEvent& evt );

			/**
			 * @fn virtual void onSelectItem( const int item )
			 * @brief This method is called when a item is selected.
			 * @param item index
			 */
			virtual void onSelectItem( const int item );

			/**
			 * @fn void onDeselectItem( wxListEvent& evt )
			 * @brief This method is called when a item is deselected.
			 * @param evt list event
			 */
			void onDeselectItem( wxListEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}



#endif    // __KOME_WINDOW_CHECK_GRID_LIST_CTRL_H__
