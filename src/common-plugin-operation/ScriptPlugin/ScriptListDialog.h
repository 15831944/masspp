/**
 * @file ScriptListDialog.h
 * @brief interfaces of ScriptListDialog class
 *
 * @author A.Ozaki
 * @date 2013.08.05
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_SCRIPT_LIST_DIALOG_H__
#define __KOME_SCRIPT_LIST_DIALOG_H__


#include <vector>
#include <wx/wx.h>


namespace kome {
	namespace script {
		class   ScriptDialog;
		class	OutputDialog;
		class	ScriptListCtrl;

		/**
		 * @class ScriptListDialog
		 * @brief baseline subtract dialog
		 */
		class ScriptListDialog : public kome::window::StandardDialog {
		public:
			/**
			 * @fn ScriptListDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			ScriptListDialog( wxWindow* parent );

			/**
			 * @fn virtual ~ScriptListDialog( )
			 * @brief destructor
			 */
			virtual ~ScriptListDialog( );

		protected:
			/** list box */
			ScriptListCtrl*	m_pList;

			/** Run button */
			wxBitmapButton*	m_pBitmapButtonRun;

			/** Add button */
			wxBitmapButton*	m_pBitmapButtonAdd;

			/** Edit button */
			wxBitmapButton*	m_pBitmapButtonEdit;

			/** Delete button */
			wxBitmapButton*	m_pBitmapButtonDelete;

			/** save button */
			wxBitmapButton* m_pBitmapButtonSave;
						
		public:
			/** dialog */
			static ScriptListDialog	*m_pDlg;

		public:
			/**
			 * @fn ScriptListCtrl* getList()
			 * @brief get the list ctrl
			 * @return ScriptListCtrl
			 */
			ScriptListCtrl* getList(){ return m_pList; }

		public:
			/**
			 * @fn UpdateButtonControl
			 * @brief update button control
			 */
			void	UpdateButtonControls( );

		protected:
			/**
			 * @fn void onRun( void )
			 * @brief runs script
			 */
			void	onRun( void );

			/**
			 * @fn void onAdd( add )
			 * @brief adds script
			 */
			void	onAdd( void );

			/**
			 * @fn void onEdit( void )
			 * @brief edits script
			 */
			void	onEdit( void );

			/**
			 * @fn void onDelete( void )
			 * @brief deletes script
			 */
			void	onDelete( void );

			/**
			 * @fn void onSelect( void )
			 * @brief select script
			 */
			void onSelect( void );

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer( void )
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer( void );

			/**
			 * @fn virtual wxSizer* createButtonSizer( void )
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer( void );
						
		protected:
			/**
			 * @fn virtual bool onCancel( )
			 * @brief This method is called when the "Cancel" button is clicked.
			 * @brief If true, this window is closed.
			 */
			virtual bool onCancel( );

		protected:
			/**
			 * @fn void onRun( wxCommandEvent& evt )
			 * @brief This method is called when the Run button is clicked.
			 * @param[in] evt event information
			 */
			void	onRun( wxCommandEvent& evt );

			/**
			 * @fn void onAdd( wxCommandEvent& evt )
			 * @brief This method is called when the Add button is clicked.
			 * @param[in] evt event information
			 */
			void	onAdd( wxCommandEvent& evt );

			/**
			 * @fn void onEdit( wxCommandEvent& evt )
			 * @brief This method is called when the Edit button is clicked.
			 * @param[in] evt event information
			 */
			void	onEdit( wxCommandEvent& evt );

			/**
			 * @fn void onDelete( wxCommandEvent& evt )
			 * @brief This method is called when the Delete as button is clicked.
			 * @param[in] evt event information
			 */
			void	onDelete( wxCommandEvent& evt ); 

			/**
			 * @fn void onClose( wxCommandEvent& evt )
			 * @brief This method is called when the Close as button is clicked.
			 * @param[in] evt event information
			 */
			void	onClose( wxCommandEvent& evt ); 
		
			/**
			 * @fn onUpdateUI( wxUpdateUIEvent& evt )
			 * @brief updates button enable/disable
			 * @param[in] evt event information
			*/
			void	onUpdateUI( wxUpdateUIEvent& evt );

			/**
			 * @fn void	onSave( wxCommandEvent& evt )
			 * @brief This method is called when the Save as button is clicked.
			 * @param[in] evt event information 
			 */
			void	onSave( wxCommandEvent& evt );
			
		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value (override method)
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value data to window controls
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();

		private:
			DECLARE_EVENT_TABLE( );
		};
	}
}

#endif		// __KOME_SCRIPT_LIST_DIALOG_H__
