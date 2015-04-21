/**
 * @file ListEditPanel.h
 * @brief interfaces of ListEditPanel class
 *
 * @author S.Tanaka
 * @date 2009.09.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_LIST_EDIT_PANEL_H__
#define __KOME_WINDOW_LIST_EDIT_PANEL_H__


#include "HelpGetterPanel.h"
#include "GridListCtrl.h"
#include "CheckGridListCtrl.h"
#include "CheckGridListPanel.h"


namespace kome {
	namespace window {
		/**
		 * @class ListEditPanel
		 * @brief standard panel
		 */
		class WINDOW_MANAGER_CLASS ListEditPanel : public HelpGetterPanel {

		public:
			/**
			 * @fn ListEditPanel(
						wxWindow* parent,
						const int id,
						kome::plugin::SettingsForm* form,
						kome::objects::SettingParameterValues* settings,
						const bool titleFlg,
						const bool singleSel,
						const bool hideAllCheckbox
				    )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id control ID
			 * @param[in] form form information
			 * @param[out] settings the object to store values
			 * @param[in] titleFlg title flag
			 * @param[in] singleSel selectin flag
			 * @param[in] hideAllCheckbox "Hide All Checkbox" flag
			 */
			ListEditPanel(
				wxWindow* parent,
				const int id,
				kome::plugin::SettingsForm* form,
				kome::objects::SettingParameterValues* settings,
				const bool titleFlg,
				const bool singleSel = false,
				const bool hideAllCheckbox = false
			);

			/**
			 * @fn virtual ~ListEditPanel()
			 * @brief destructor
			 */
			virtual ~ListEditPanel();

		protected:
			/**
			 * @typedef enum ValueType
			 * @brief value type
			 */
			typedef enum {
				VALUES_NORMAL = 0,
				VALUES_WITH_COLOR = 1,
				VALUES_FILE = 2,
				VALUES_ENUM = 4
			} ValueType;

		protected:
			/** form */
			kome::plugin::SettingsForm* m_form;

			/** settings */
			kome::objects::SettingParameterValues* m_settings;

			/** title flg */
			bool m_titleFlg;

			/** single selection flag */
			bool m_singleSel;

			/** "Hide All Checkbox" flag */
			bool m_hideAllCheckbox;

			/** values */
			std::vector< std::string > m_values;

			/** value type */
			int m_valueType;

			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** page */
			kome::plugin::SettingsPage* m_page;

			/** color index */
			int m_colorIdx;

			/** column names */
			std::vector< std::pair< std::string, std::string > > m_colNames;

			/** list control */
			GridListCtrl* m_list;

			/** list control(EditGridListCtrl) */
			void* m_listEditGridListCtrl;	// @date 2012/11/28 <Add> OKADA

			/** color */
			COLOR m_color;

			/** enumeration */
			std::vector< kome::plugin::SettingsValue::EnumItem > m_enumeration;

			/** add button */
			wxBitmapButton* m_addButton;

			/** edit button */
			wxBitmapButton* m_editButton;

			/** delete button */
			wxBitmapButton* m_delButton;

			/** up button */
			wxBitmapButton* m_upButton;

			/** down button */
			wxBitmapButton* m_downButton;

			/** color button */
			wxBitmapButton* m_colorButton;

			/** import button */
			wxButton* m_importButton;

			/** export button */
			wxButton* m_exportButton;

			/** Selected Item */
			int m_iSelectedItem;

		public:
			/**
			 * @fn std::string getValue( const unsigned int listIdx, const unsigned int elmIdx )
			 * @brief gets the value
			 * @param[in] listIdx list index
			 * @param[in] elmIdx element index
			 * @return value
			 */
			std::string getValue( const unsigned int listIdx, const unsigned int elmIdx );

			/**
			 * @fn int getIntValue( const unsigned int listIdx, const unsigned int elmIdx )
			 * @brief gets the integer value
			 * @param[in] listIdx list index
			 * @param[in] elmIdx element index
			 * @return integer value
			 */
			int getIntValue( const unsigned int listIdx, const unsigned int elmIdx );

			/**
			 * @fn double getDoubleValue( const unsigned int listIdx, const unsigned int elmIdx )
			 * @brief gets the double value
			 * @param[in] listIdx list index
			 * @param[in] elmIdx element index
			 * @return double value
			 */
			double getDoubleValue( const unsigned int listIdx, const unsigned int elmIdx );

			/**
			 * @fn std::string getValue()
			 * @brief gets the value
			 * @return value
			 */
			std::string getValue();

			/**
			 * @fn void setValue( const char* value )
			 * @brief sets the value
			 * @param[in] value value
			 */
			void setValue( const char* value );

			/**
		 	 * @fn unsigned int getNumberOfValues()
			 * @brief gets the number of values
			 * @return the number of values
			 */
			unsigned int getNumberOfValues();

			/**
			 * @fn unsigned int getNumberOfColumns()
			 * @brief gets the number of columns
			 * @return the number of columns
			 */
			unsigned int getNumberOfColumns();

			/**
			 * @fn const char* getColumnName( const unsigned int idx )
			 * @brief gets the column name
			 * @param[in] idx column index
			 * @return column name
			 */
			const char* getColumnName( const unsigned int idx );

			/**
			 * @fn kome::plugin::SettingsValue::ValueType getColumnType( const unsigned int idx )
			 * @brief gets the column type
			 * @param[in] idx column index
			 * @return column type
			 */
			kome::plugin::SettingsValue::ValueType getColumnType( const unsigned int idx );

			/**
			 * @fn unsigned int getEnumSize()
			 * @brief gets the enumeration size
			 * @return enumeration size
			 */
			unsigned int getEnumSize();

			/**
			 * @fn const char* getEnumName( const unsigned int idx )
			 * @brief gets the enumeration name
			 * @param[in] idx enumeration index
			 * @return enumeration name
			 */
			const char* getEnumName( const unsigned int idx );

			/**
			 * @fn const char* getEnumValue( const unsigned int idx )
			 * @brief gets the enumeration value
			 * @param[in] idx enumeration index
			 * @return enumeration name
			 */
			const char* getEnumValue( const unsigned int idx );

			/**
			 * @fn int getColorIndex()
			 * @brief gets the color index
			 * @return color index
			 */
			int getColorIndex();

			/**
			 * @fn COLOR getColor( const unsigned int idx )
			 * @brief gets the color
			 * @param idx list index
			 * @return color
			 */
			COLOR getColor( const unsigned int idx );

			/** 
			 * @fn std::string getEnumPluginName( const unsigned int listIdx, const unsigned int elmIdx )
			 * @brief gets the enumeration plug-in name
			 * @param[in] listIdx list index
			 * @param[in] elmIdx element index
			 * @return enumeration plug-in name
			 */
			std::string getEnumPluginName( const unsigned int listIdx, const unsigned int elmIdx );

			/**
			 * @fn void onKeyUp( wxKeyEvent& evt )
			 * @brief This method is called by key up event.
			 * @param[in] evt key event information
			 */
			void onKeyUp( wxKeyEvent& evt );

			/**
			 * @fn void removeValue( const long long data )
			 * @brief remove value
			 * @param[in] data data
			 */
			void removeValue( const long long data );

			/**
			 * @fn void updateList()
			 * @brief updates list
			 */
			void updateList();

		protected:
			/**
			 * @fn void createPage()
			 * @brief creates input page
			 */
			void createPage();

        public:
			/**
			 * @fn void updateEnumeration( const char* section )
			 * @brief updates enumeration
			 * @param[in] section section
			 */
			void updateEnumeration( const char* section );

        protected:
			/**
			 * @fn void processEvent()
			 * @brief processes event
			 */
			void processEvent();

		protected:
			/**
			* @fn void setValues( const bool update )
			* @brief sets values
			* @param[in] update update flag
			*/
			void setValues( const bool update );

			/**
			 * @fn void addFiles()
			 * @brief add files
			 */
			void addFiles();

		public:
			/**
			 * @fn void getSelectedValues( std::vector< std::string >& values )
			 * @brief gets the selected items
			 * @param[out] values the array to store selected item values
			 */
			void getSelectedValues( std::vector< std::string >& values );

		protected:
			/**
			 * @fn virtual void onSelectItem()
			 * @brief This method is called by onListItemSelected method
			 */
			virtual void onSelectItem();

			/**
			 * @fn virtual void onDoubleClickItem()
			 * @brief This method is called by onListItemDoubleClicked method
			 */
			virtual void onDoubleClickItem();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:
			/**
			 * @fn virtual std::string getDescription( wxWindow* wnd )
			 * @brief method to get the description from a prameter control
			 * @param[in] wnd parameter control
			 * @return description
			 */
			virtual std::string getDescription( wxWindow* wnd );

			/**
			 * @fn virtual std::string getImageFilePath( wxWindow* wnd )
			 * @brief method to get image file path from a parameter control
			 * @param[in] wnd parameter control
			 * @return image file path
			 */
			virtual std::string getImageFilePath( wxWindow* wnd );

		protected:
			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value to window control data
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();
		
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		protected:
			/**
			 * @fn void onAdd( wxCommandEvent& evt )
			 * @brief This method is called when the add button is clicked.
			 * @param[in] evt button event
			 */
			void onAdd( wxCommandEvent& evt );

			/**
			 * @fn void onEditDoubleClick( wxListEvent& evt )
			 * @brief This method is called when the list is double clicked.
			 *@ param[in] evt button event
			 */
			void onEditDoubleClick( wxListEvent& evt );

			/**
			 * @fn void onEdit( wxCommandEvent& evt )
			 * @brief This method is called when the edit button is clicked.
			 *@ param[in] evt button event
			 */
			void onEdit( wxCommandEvent& evt );

			/**
			 * @fn void onDelete( wxCommandEvent& evt )
			 * @brief This method is called when the delete button is clicked.
			 * @param[in] evt button event
			 */
			void onDelete( wxCommandEvent& evt );

		protected:
			/**
			 * @fn void onUp( wxCommandEvent& evt )
			 * @brief This method is called when the up button is clicked.
			 * @param[in] evt button event
			 */
			void onUp( wxCommandEvent& evt );

			/**
			 * @fn void onDown( wxCommandEvent& evt )
			 * @brief This method is called when the down button is clicked.
			 * @param[in] evt button event
			 */
			void onDown( wxCommandEvent& evt );

			/**
			 * @fn void onColor( wxCommandEvent& evt )
			 * @brief This method is called when the color button is clicked.
			 * @param[in] evt button event
			 */
			void onColor( wxCommandEvent& evt );
			
			/**
			 * @fn void onImport( wxCommandEvent& evt )
			 * @brief This method is called when the import button is clicked.
			 * @param[in] evt button event
			 */
			void onImport( wxCommandEvent& evt );

			/**
			 * @fn void onExport( wxCommandEvent& evt )
			 * @brief This method is called when the export button is clicked.
			 * @param[in] evt button event
			 */
			void onExport( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateSelection( wxUpdateUIEvent& evt )
			 * @brief updates user intefaces when selecting item
			 * @param[in] evt update event
			 */
			void onUpdateSelection( wxUpdateUIEvent& evt );

		protected:
			/**
			 * @fn void onListItemsSelected( wxListEvent& evt )
			 * @brief This method is called when list items are selected.
			 * @param[in] evt list event
			 */
			void onListItemsSelected( wxListEvent& evt );

			/**
			 * @fn void onListItemDoubleClicked( wxListEvent& evt )
			 * @brief This method is called when list item is double clicked.
			 * @param[in] evt mouse event
			 */
			void onListItemDoubleClicked( wxListEvent& evt );

		protected:
			/**
			 * @fn std::string removeBrackets( const char* s )
			 * @brief removes brackets
			 * @param[in] s character string
			 * @return character string whose barackets are removed
			 */
			static std::string removeBrackets( const char* s );

		private:
			DECLARE_EVENT_TABLE();
		};

		/**
		 * @class EditGridListCtrl
		 * @brief grid list control in a list edit panel
		 */
		class WINDOW_MANAGER_CLASS EditGridListCtrl : public GridListCtrl {
		public:
			/**
			 * @fn EditGridListCtrl( wxWindow* parent, const bool singleSel )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] singleSel single selection flag
			 */
			EditGridListCtrl( wxWindow* parent, const bool singleSel );

			/**
			 * @fn virtual ~EditGridListCtrl()
			 * @brief destructor
			 */
			virtual ~EditGridListCtrl();

		protected:
			/** list edit panel */
			ListEditPanel* m_panel;
		private:
			/** name of the Edit menu */
			std::string	m_strEditMenuName;

			/** name of the Add menu */
			std::string	m_strAddMenuName;

			/** up-down menu exists or not */
			bool m_flgUpDown;

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

	

		public:
			// Context Menu

			/**
			 * @fn void setEditMenuName( std::string strEditMenuName )
			 * @brief The name of the Edit menu is set. The Edit menu is not displayed when a null character is set. 
			 * @param[in] strEditMenuName name of the Edit menu
			 */
			void setEditMenuName( std::string strEditMenuName );

			/**
			 * @fn std::string getEditMenuName( void )
			 * @brief This function gets the name of the Edit menu. 
			 * @return std::string name of the Edit menu
			 */
			std::string getEditMenuName( void  );

			/**
			 * @fn void setAddMenuName( std::string strAddMenuName )
			 * @brief The name of the Add menu is set. The Add menu is not displayed when a null character is set. 
			 * @param[in] strAddMenuName name of the Add menu
			 */
			void setAddMenuName( std::string strAddMenuName );

			/**
			 * @fn std::string getAddMenuName( void )
			 * @brief This function gets the name of the Add menu. 
			 * @return std::string name of the Add menu
			 */
			std::string getAddMenuName( void );

			/**
			 * @fn void setUpDown( bool flgUpDown )
			 * @brief This function set whether a up-down menu exists or not. 
			 * @param[in] flgUpDown  true=exists, false=not exists
			 */
			void setUpDown( bool flgUpDown );

			/**
			 * @fn bool canUpDown( void )
			 * @brief This function returns whether a up-down menu exists or not. 
			 * @return bool true=exists, false=not exists
			 */
			bool canUpDown( void );

		private:
			/**
			 * @fn virtual wxMenu* createMenu()
			 * @brief creates menu
			 * @return created menu
			 */
			virtual wxMenu* createMenu();

		public:
			/**
			 * @fn void onContextMenu( wxContextMenuEvent& evt )
			 * @brief This method is called on requesting context menu
			 * @param[in] evt context menu event information
			 */
			void onContextMenu( wxContextMenuEvent& evt );

			/**
			 * @fn virtual std::string getPopupMenuName( PopupMenuFlg nPopupMenuFlag )
			 * @brief get Popup Menu Name
			 * @param[in] nPopupMenuFlag popoup-menu
			 * @return Popup Menu text
			 */
			virtual std::string getPopupMenuName( PopupMenuFlg nPopupMenuFlag );

			/**
			 * @fn virtual bool onDeleteData( const long long data )
			 * @brief This method is called when a item is deleted.
			 * @param[in] data deleted item data
			 * @return If true, succeeded to delete the item.
			 */
			virtual bool onDeleteData( const long long data );


			
		private:
			DECLARE_EVENT_TABLE();
		};	// EditGridListCtrl class

		/**
		 * @class EditCheckGridListCtrl
		 * @brief check grid list contro in a list edit panel
		 */
		class EditCheckGridListCtrl : public CheckGridListCtrl {
		public:
			/**
			 * @fn EditCheckGridListCtrl( wxWindow* parent )
			 * @brief constructor
			 */
			EditCheckGridListCtrl( wxWindow* parent );

			/**
			 * @fn virtual ~EditCheckGridListCtrl()
			 * @brief destructor
			 */
			virtual ~EditCheckGridListCtrl();

		protected:
			/** list edit panel */
			ListEditPanel* m_panel;

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method
			 */
			virtual void onCreate();

			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value
			 * @param[in] data data
			 * @param[in] column column number
			 */
			virtual std::string getString( const long long data, const int column ) const;

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_WINDOW_LIST_EDIT_PANEL_H__
