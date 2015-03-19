/**
 * @file SearchResultDialog.h
 * @brief interfaces of GridListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.09.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IDENT_SEARCH_RESULT_DIALOG_H__
#define __KOME_IDENT_SEARCH_RESULT_DIALOG_H__


#include <wx/wx.h>


namespace kome {
	namespace ident {

		class GridListCtrl;
		class SubstanceDialog;		// @date 2013/02/06 <Add> OKADA
		class SubstanceListCtrl;	// @date 2013/02/06 <Add> OKADA

		/**
		 * @class SearchResultDialog
		 * @brief grid list dialog class
		 */
		class SearchResultDialog : public kome::window::StandardDialog {
		public:
			/**
			 * @fn SearchResultDialog( wxWindow* parent, const char* title, const bool canDelete = false )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 * @param[in] canDelete delete button flag
			 */
			SearchResultDialog( wxWindow* parent, const char* title, const bool canDelete = false );

			/**
			 * @fn virtual ~SearchResultDialog()
			 * @brief destructor
			 */
			virtual ~SearchResultDialog();

		protected:
			/** grid list */
			kome::window::GridListCtrl* m_list;

			/** delete flag */
			bool m_deleteFlg;

		private:

			/**
			 * @struct ResultView
			 * @brief Result View
			 */
			struct ResultView {
				kome::plugin::PluginFunctionItem* fun;
				wxIcon icon0;
				wxIcon icon1;
				wxBitmapButton* button;
				int pos;
			};

			/** peak actions */
			std::vector< ResultView > m_resultViews;


		public:
			/**
			 * @fn void setGridListCtrl( GridListCtrl* listCtrl )
			 * @brief sets grid list control
			 * @param[in] listCtrl grid list control
			 */
			void setGridListCtrl( kome::window::GridListCtrl* listCtrl );

		public:
			/**
			 * @fn void updateDialog()
			 * @brief updates dialog
			 */
			void updateDialog();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:
			/**
			 * @fn void saveData( wxCommandEvent& evt )
			 * @brief saves data
			 * @param[in] evt event information
			 */
			void saveData( wxCommandEvent& evt );

			/**
			 * @fn void deleteData( wxCommandEvent& evt )
			 * @brief deletes data
			 * @param[in] evt event information
			 */
			void deleteData( wxCommandEvent& evt );

			/**
			 * @fn void updateUI( wxUpdateUIEvent& evt )
			 * @brief updates UI
			 * @param[in] evt evnet information
			 */
			void updateUI( wxUpdateUIEvent& evt );

		protected:
			/**
			 * @fn std::string getStatusName( const unsigned int idx )
			 * @brief gets the status name
			 * @param[in] idx peak action index
			 * @return status name
			 */
			std::string getStatusName( const unsigned int idx );	// @date 2013/01/25 <Add> OKADA

			/**
			 * @fn void onResultButton( wxCommandEvent& evt )
			 * @brief This method is called when a result button is clicked.
			 *@ param[in] evt event information
			 */
			void onResultButton( wxCommandEvent& evt );

			/**
			 * @fn void onResultUpdateUIButton( wxUpdateUIEvent& evt )
			 * @brief updates the UI of the result button
			 * @parma[in] evt event information
			 */
			void onResultUpdateUIButton( wxUpdateUIEvent& evt );
			
			/**
			 * @fn void onClose( wxCommandEvent& evt );
			 * @brief close dialog
			 */
			void onClose( wxCommandEvent& evt ); // Spec No.92224:ダイアログが複数表示される @date 2013.06.24 <Add> M.Izumi
		protected:
			/**
			 * @fn bool isVisibleDialog( const unsigned int idx )
			 * @brief judges whether the dilaog is visible or not
			 * @param[in] idx peak action index
			 * @return If true, the specified dialog is visible.
			 */
			bool isVisibleDialog( const unsigned int idx );

		public:
			/**
			 * @fn void clearStatus( const char szInitStr[] = "" )
			 * @brief clear status
			 * @param[in] szInitStr initial string
			 */
			void clearStatus( const char szInitStr[] = "" );

			/**
			 * @fn void getSearchID()
			 * @brief get Search ID
			 * @param[in] idx index
			 * @return Search ID. fail:-1
			 */
			int getSearchID();

			/**
			 * @fn kome::window::GridListCtrl* SearchResultDialog::getList( void )
			 * @brief get m_list
			 * @return kome::window::GridListCtrl* SearchResultDialog::getList( void )
			 */
			kome::window::GridListCtrl* getList( void );

			/**
			 * @fn void onDoubleClickAtList( void )
			 * @brief call DoubleClickAtList 
			 */
			void onDoubleClickAtList( void );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif	// __KOME_WINDOW_GRID_LIST_DIALOG_H__
