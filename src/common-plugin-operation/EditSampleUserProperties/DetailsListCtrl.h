/**
 * @file DetailsListCtrl.h
 * @brief interfaces of DetailsListCtrl class
 *
 * @author H.Parry
 * @date 2011.04.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_VIEW_DETAILS_LIST_CTRL_H__
#define __KOME_VIEW_DETAILS_LIST_CTRL_H__


#include <vector>
#include <utility>


namespace kome {
	namespace properties {

		class EditSampleUserPropertiesDialog;

		/**
		 * @class DetailsListCtrl
		 * @brief details list control class
		 */
		class DetailsListCtrl : public kome::window::EditGridListCtrl {
		public:
			/**
			 * @fn DetailsListCtrl( wxWindow* parent )
			 * @brief constructor
			 */
			DetailsListCtrl( EditSampleUserPropertiesDialog* pParentDlg );

			/**
			 * @fn virtual ~DetailsListCtrl()
			 * @brief destructor
			 */
			virtual ~DetailsListCtrl();

		protected:
			/** properties */
			std::vector< std::pair< std::string, std::string > > m_properties;

		protected:
			/** 
			 * @fn void create()
			 * @brief creates list
			 */
			void create();

		public:
			/**
			 * @fn void clear()
			 * @brief clears properties
			 */
			void clear();

			/**
			 * @fn void setProperties( kome::core::Properties& props )
			 * @brief sets properties
			 * @param props properties object
			 */
			void setProperties( kome::core::Properties& props );

			/**
			 * @fn void getProperties( kome::core::Properties& props )
			 * @brief gets properties
			 * @param props properties object
			 */
			void getProperties( kome::core::Properties& props );

			/**
			 * @fn void validateProperties( )
			 * @brief validate properties
			 */
			bool validateProperties();

			/**
			 * @fn void deleteItems( )
			 * @brief delete items
			 */
			void deleteItems();

			/**
			 * @fn void newItem( )
			 * @brief append a new item
			 */
			void newItem();

			/**
			 * @fn void updateItem(const long lRow, const int iColumn, const char *s)
			 * @brief sets a cell's text in the details list
			 * @param lRow row number
			 * @param iColumn column number
			 * @param s text string
			 */
			void updateItem(const long lRow, const int iColumn, const char *s);

		protected:
			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value (override method)
			 * @param data data
			 * @param column column number
			 */
			virtual std::string getString( const long long data, const int column ) const;

			void setString( const long long data, const int column, const std::string s );

			/**
			 * @fn void onSelectItem( wxListEvent& evt )
			 * @brief called when an item is selected in the grid
			 * @param evt list event
			 */
			void onSelectItem( wxListEvent& evt );

			/**
			 * @fn void onClickCol( wxListEvent& evt )
			 * @brief called when a column header cell is clicked in the grid
			 * @param evt list event
			 */
			void onClickCol( wxListEvent& evt );

			/**
			 * @fn virtual void setItemValue( const long long data, const int column, const char* s )
			 * @brief sets item property value
			 * @param[in] data data
			 * @param[in] column column number
			 * @param[in] s data value
			 */
			virtual void setItemValue( const long long data, const int column, const char* s );

			
			/**
			 * @fn virtual void onActivateData( const long long data )
			 * @brief This method is called when the data is activated (abstract method)
			 * @param[in] data activated data
			 */
			virtual void onActivateData( const long long data );
			
			/**
			 * @fn virtual bool onDeleteData( const long long data )
			 * @brief This method is called when a item is deleted.
			 * @param[in] data deleted item data
			 * @return If true, succeeded to delete the item.
			 */
			virtual bool onDeleteData( const long long data );

			/**
			 * @fn virtual wxMenu* createMenu()
			 * @brief creates menu
			 * @return created menu
			 */
			virtual wxMenu* createMenu();
		private:
			DECLARE_EVENT_TABLE()

			EditSampleUserPropertiesDialog* m_pParentDlg;
		};
	}
}


#endif	// __KOME_VIEW_DETAILS_LIST_CTRL_H__
