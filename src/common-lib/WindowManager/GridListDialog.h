/**
 * @file GridListDialog.h
 * @brief interfaces of GridListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.09.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_GRID_LIST_DIALOG_H__
#define __KOME_WINDOW_GRID_LIST_DIALOG_H__


#include <wx/wx.h>

#include "StandardDialog.h"


namespace kome {
	namespace window {

		class GridListCtrl;

		/**
		 * @class GridListDialog
		 * @brief grid list dialog class
		 */
		class WINDOW_MANAGER_CLASS GridListDialog : public StandardDialog {
		public:
			/**
			 * @fn GridListDialog( wxWindow* parent, const char* title, const bool canDelete = false )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 * @param[in] canDelete delete button flag
			 */
			GridListDialog( wxWindow* parent, const char* title, const bool canDelete = false );

			/**
			 * @fn virtual ~GridListDialog()
			 * @brief destructor
			 */
			virtual ~GridListDialog();

		protected:
			/** grid list */
			GridListCtrl* m_list;

			/** delete flag */
			bool m_deleteFlg;

		public:
			/**
			 * @fn void setGridListCtrl( GridListCtrl* listCtrl )
			 * @brief sets grid list control
			 * @param[in] listCtrl grid list control
			 */
			void setGridListCtrl( GridListCtrl* listCtrl );

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

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif	// __KOME_WINDOW_GRID_LIST_DIALOG_H__
