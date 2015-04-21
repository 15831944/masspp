/**
 * @file GridListCtrl.h
 * @brief interfaces of GridListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.11.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_GRID_LIST_CTRL_H__
#define __KOME_WINDOW_GRID_LIST_CTRL_H__


#include "GridSearchDialog.h"
#include "ListCtrl.h"

#include <vector>
#include <set>
#include <wx/wx.h>
#include <wx/listctrl.h>

enum PopupMenuFlg{
	POPUP_DELETE	= 0x01,	// 1
	POPUP_COPY		= 0x02,	// 2
	POPUP_SAVE		= 0x04,	// 4	// Save All
	POPUP_SELECT	= 0x08,	// 8	// Select All
	POPUP_DESELECT	= 0x10,	// 16	// Deselect All
	POPUP_FILTER	= 0x20,	// 32

	POPUP_ADD		= 0x40,	// 64
	POPUP_UP		= 0x80,	// 128
	POPUP_DOWN		= 0x100,// 256
	POPUP_EDIT		= 0x200	// 512
};


// ï\é¶ÇÃèáî‘
/*
Edit 
New/Add 
Up
Down 
Select All
Deselect All 
Copy 
Delete 
Filter... 
Save All...
*/


namespace kome {
	namespace window {

		/**
		 * @class GridListCtrl
		 * @brief grid list control class
		 */
		class WINDOW_MANAGER_CLASS GridListCtrl : public ListCtrl {
		public:
			/**
			 * @fn GridListCtrl(
						wxWindow* parent,
						const int id,
						const int width,
						const int height,
						const bool editable = false,
						const bool autoColumnWidth = true,
						const bool singleSelect = false
					)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 * @param[in] width window width
			 * @param[in] height window height
			 * @param[in] editable editable flag
			 * @param[in] autoColumnWidth auto column width flag
			 * @param[in] singleSelect single select flag
			 */
			GridListCtrl(
				wxWindow* parent,
				const int id,
				const int width,
				const int height,
				const bool editable = false,
				const bool autoColumnWidth = true,
				const bool singleSelect = false
			);

			/**
			 * @fn virtual ~GridListCtrl()
			 * @brief destructor
			 */
			virtual ~GridListCtrl();

		public:
			/**
			 * @typedef ColumnDataType
			 * @brief column data type
			 */
			typedef enum {
				TYPE_INT,
				TYPE_UINT,
				TYPE_DOUBLE,
				TYPE_UDOUBLE,
				TYPE_STRING,
				TYPE_DATE_TIME_STRING,		// dd mmm yyyy hh:mm:ss ÇÃå`éÆ
			} ColumnDataType;

		protected:
			/** column data types array */
			std::vector< ColumnDataType > m_columnTypes;

			/** headers */
			std::vector< std::string > m_titles;

			/** all data */
			std::vector< long long > m_allData;

			/** data array */
			std::vector< long long > m_data;

			/** sort flag */
			int m_sort;

			/** color map */
			std::map< long long, wxListItemAttr > m_colorMap;

			/** title */
			std::string m_title;
		protected:
			/** filter search type */
			GridSearchDialog::SearchType m_filterSearchType;

			/** filter search column */
			int m_filterCol;

			/** filter keyword */
			std::string m_filterKeyword;

			/** value type */
			GridSearchDialog::ValueType m_filterValueType;

			/** filter min value */
			std::string m_filterMinVal;

			/** filter max value */
			std::string m_filterMaxVal;

		public:
			/**
			 * @fn void setHeaderTitle( const int column, const char* title )
			 * @brief sets header title
			 * @param[in] column column number
			 * @param[in] title header title
			 */
			void setHeaderTitle( const int column, const char* title );
			
			/**
			 * @fn const char* getHeaderTitle( const int column )
			 * @brief gets header title
			 * @param[in] column column number
			 * @return header title
			 */
			const char* getHeaderTitle( const int column );

			/**
			 * @fn void setDataType( const int column, const ColumnDataType type )
			 * @brief sets column data type
			 * @param[in] column column number
			 * @param[in] type column data type
			 */
			void setDataType( const int column, const ColumnDataType type );

			/**
			 * @fn ColumnDataType getDataType( const int column )
			 * @brief gets column data type
			 * @return column data type
			 */
			ColumnDataType getDataType( const int column );

			/**
			 * @fn void clearData( const bool update )
			 * @brief clears list data
			 * @param[in] update update flag
			 */
			void clearData( const bool update );

			/**
			 * @fn void addData( long long data, const bool update )
			 * @brief adds list data
			 * @param[in] data list data
			 * @param[in] update update flag
			 */
			void addData( long long data, const bool update );

			/**
			 * @fn void deleteData( long long data, const bool update )
			 * @brief deletes list data
			 * @param[in] data list data to be deleted
			 * @param[in] update update flag
			 */
			void deleteData( long long data, const bool update );

			/**
			 * @fn void swapData( const int idx0, const int idx1, const bool update )
			 * @brief swaps data
			 * @param[in] idx0 list data index to be swapped
			 * @param[in] idx1 list data index to swap
			 * @param[in] update update flag
			 */
			void swapData( const int idx0, const int idx1, const bool update );

			/**
			 * @fn unsigned int getRowSize()
			 * @brief gets row size
			 * @return row size
			 */
			unsigned int getRowSize();

			/**
			 * @fn long long getListData( const int row )
			 * @brief gets list data
			 * @param[in] row row number
			 * @return list data
			 */
			long long getListData( const int row );

			/**
			 * @fn void updateList()
			 * @brief updates list
			 */
			void updateList();

			/**
			 * @fn void setTitle( const char* title )
			 * @brief sets title
			 * @param[in] title title name
			 */
			void setTitle( const char* title );
		public:
			/** 
			 * @fn void create()
			 * @brief create list
			 */
			void create();

		public:
			/**
			 * @fn unsigned int getNumberOfData()
			 * @brief gets the number of data
			 * @return the number of data
			 */
			unsigned int getNumberOfData();

			/**
			 * @fn long long getData( const unsigned int item )
			 * @brief get data
			 * @param[in] item item index
			 * @return data value
			 */
			long long getData( const unsigned int item );

		public:
			/**
			 * @fn bool exportData()
			 * @brief exports data
			 */
			bool exportData();

			/**
			 * @fn void executeFilter()
			 * @brief executes filter
			 */
			void executeFilter();

		public:
			/**
			 * @fn void addColumn( const char* title, const ColumnDataType type, const int width = 80 )
			 * @brief adds column
			 * @param[in] title column title
			 * @param[in] type column data type
			 * @param[in] width column width
			 */
			void addColumn( const char* title, const ColumnDataType type, const int width = 80 );

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

		public:
			/**
			 * @fn void setDataColor( const long long data, const COLOR color )
			 * @brief sets data color
			 * @param[in] data data
			 * @param[in] color data color
			 */
			void setDataColor( const long long data, const COLOR color );
			
			/**
			 * @fn void setBackgroundColor( const long long data, const COLOR color )
			 * @brief sets background color
			 * @param[in] data data
			 * @param[in] color data color
			 */
			void setBackgroundColor( const long long data, const COLOR color );

			/**
			 * @fn void clearDataColor()
			 * @brief clears data color
			 */
			void clearDataColor();

		protected:
			/**
			 * @fn bool lessData( long long data0, long long data1, const int column, const bool desc )
			 * @brief compare data to sort
			 * @param[in] data0 list data to be compared
			 * @param[in] data1 list data to compare
			 * @param[in] column column number
			 * @param[in] desc descending sort flag
			 * @return If true, data0 sorts before data1.
			 */
			bool lessData( long long data0, long long data1, const int column, const bool desc );

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method
			 */
			virtual void onCreate();

			/**
			 * @fn virtual int getInt( const long long data, const int column ) const
			 * @brief gets integer value from data
			 * @param[in] data data
			 * @param[in] column column number
			 * @return integer value
			 */
			virtual int getInt( const long long data, const int column ) const;

			/**
			 * @fn virtual double getDouble( const long long data, const int column ) const
			 * @brief gets double value from data
			 * @param[in] data data
			 * @param[in] column column number
			 */
			virtual double getDouble( const long long data, const int column ) const;

			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value
			 * @param[in] data data
			 * @param[in] column column number
			 */
			virtual std::string getString( const long long data, const int column ) const;

			/**
			 * @fn virtual void setItemValue( const long long data, const int column, const char* s )
			 * @brief sets item property value
			 * @param[in] data data
			 * @param[in] column column number
			 * @param[in] s data value
			 */
			virtual void setItemValue( const long long data, const int column, const char* s );

			/**
			 * @fn virtual wxMenu* createMenu()
			 * @brief creates menu
			 * @return created menu
			 */
			virtual wxMenu* createMenu();

			/**
			 * @fn virtual void onActivateData( const long long data )
			 * @brief This method is called when the data is activated
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
		
		protected:
			/**
			 * @fn virtual std::string getItemText( const long long data, const int column ) const
			 * @brief gets item text
			 * @param[in] data data
			 * @param[in] column column number
			 * @return item text
			 */
			virtual std::string getItemText( const long long data, const int column ) const;
			
		public:
			/**
			 * @fn virtual wxString OnGetItemText( long item, long column ) const
			 * @brief gets item text (Override method)
			 * @param[in] item item number
			 * @param[in] column column number
			 * @return item text
			 */
			virtual wxString OnGetItemText( long item, long column ) const;

			/**
			 * @fn virtual wxListItemAttr* OnGetItemAttr(long item) const
			 * @brief gets list item attribute (Overridde method)
			 * @param[in] item item number
			 * @return list item attribute
			 */
			virtual wxListItemAttr* OnGetItemAttr( long item ) const;

		protected:
			/**
			 * @fn void onColClick( wxListEvent& evt )
			 * @brief This method is called when list column is clicked
			 * @param[in] evt list event information
			 */
			void onColClick( wxListEvent& evt );

			/**
			 * @fn void onLabelEdit( wxListEvent& evt )
			 * @brief This method is called when liste item is edited
			 * @param[in] evt list event information
			 */
			void onLabelEdit( wxListEvent& evt );

			/**
			 * @fn void onActivateItem( wxListEvent& evt )
			 * @brief This method is called when a list item is double-clicked or entered.
			 * @param[in] evt list vent
			 */
			void onActivateItem( wxListEvent& evt );

			/**
			 * @fn void onContextMenu( wxContextMenuEvent& evt )
			 * @brief This method is called on requesting context menu
			 * @param[in] evt context menu event information
			 */
			void onContextMenu( wxContextMenuEvent& evt );

			/**
			 * @fn void onUpdateUI( wxUpdateUIEvent& evt )
			 * @brief sets the status of menu
			 * @param[in] evt update UI event information
			 */
			void onUpdateUI( wxUpdateUIEvent& evt );

			/**
			 * @fn void onKeyUp( wxKeyEvent& evt )
			 * @brief This method is called by key up event.
			 * @param[in] evt key event information
			 */
			void onKeyUp( wxKeyEvent& evt );

		protected:

			/**
			 * @fn void onActive( wxCommandEvent& evt )
			 * @brief This method is called when "Select All" is selected.
			 * @param[in] evt event information
			 */
			void onActive( wxCommandEvent& evt );

			/**
			 * @fn void onSelectAll( wxCommandEvent& evt )
			 * @brief This method is called when "Select All" is selected.
			 * @param[in] evt event information
			 */
			void onSelectAll( wxCommandEvent& evt );

			/**
			 * @fn void onDeselectAll( wxCommandEvent& evt )
			 * @brief This method is called when "Deselect All" is selected.
			 * @param[in] evt event information
			 */
			void onDeselectAll( wxCommandEvent& evt );

			/**
			 * @fn void onCopy( wxCommandEvent& evt )
			 * @brief This method is called when "Copy" is selected.
			 * @param[in] evt event information
			 */
			void onCopy( wxCommandEvent& evt );

			/**
			 * @fn void onFilter( wxCommandEvent& evt )
			 * @brief This method is called when "Filter" is selected.
			 * @param[in] evt event information
			 */
			void onFilter( wxCommandEvent& evt );

			/**
			 * @fn void onSave( wxCommandEvent& evt )
			 * @brief This method is called when "Save" is selected.
			 * @param[in] evt event information
			 */
			void onSave( wxCommandEvent& evt );

			// Context Menu

		public:
			/**
			 * @fn void setPopupMenuFlag( int nPopupMenuFlags )
			 * @brief set Popup Menu Flags
			 * @param[in] nPopupMenuFlags Popup Menu Flags
			 */
			void setPopupMenuFlag( int nPopupMenuFlags );

			/**
			 * @fn int getPopupMenuFlag( void )
			 * @brief get Popup Menu Flag
			 * @return Popup Menu Flags
			 */
			int getPopupMenuFlag( void );

			/**
			 * @fn virtual std::string getPopupMenuName( PopupMenuFlg nPopupMenuFlag )
			 * @brief get Popup Menu Name
			 * @param[in] nPopupMenuFlag popoup-menu
			 * @return Popup Menu text
			 */
			virtual std::string getPopupMenuName( PopupMenuFlg nPopupMenuFlag );

			/**
			 * @fn void setActivateName( std::string strActivateName )
			 * @brief set default menu name
			 * @param[in] strActivateName setActiveName default menu name
			 */
			void setActivateName( std::string strActivateName );

			/**
			 * @fn std::string getActivateName( void )
			 * @brief set default menu name
			 * @return default menu name
			 */
			std::string getActivateName( void );

			/**
			 * @fn void onDelete( wxCommandEvent& evt )
			 * @brief This method is called when "Delete" is selected.
			 * @param[in] evt event information
			 */
			void onDelete( wxCommandEvent& evt );	// Context Menu

		public:
			/**
			 * @fn virtual void onActivateDataOther( const long long data )
			 * @brief This method is called when the data is activated by other method
			 * @param[in] data activated data
			 */
			virtual void onActivateDataOther( const long long data );	// @date 2013/01/28 <Add> OKADA

		private:
			/** popup menu */
			int m_flgPopupMenu;

			/** default menu name */
			std::string m_strActivateName;

		protected:
			/** parent dialog */
			void* m_dlgParent;	// @date 2012/12/13 <Add> OKADA

		protected:
			// >>>>>>	@Date:2013/12/10	<Add>	A.Ozaki
			//
			/**
			 * @fn std::string convertDateTimeString( const char *pcFormatedDate )
			 * @brief convert date/time string format
			 * @param[in] pcFormatedDate "dd mmm yyyy hh:mm:ss" formated
			 * @return "yyyy mm dd hh:mm:dd"
			 */
			std::string	convertDateTimeString( const char *pcFormatedDate );
			//
			// <<<<<<	@Date:2013/12/10	<Add>	A.Ozaki

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_WINDOW_GRID_LIST_CTRL_H__
