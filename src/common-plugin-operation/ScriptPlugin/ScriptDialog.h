/**
 * @file ScriptDialog.h
 * @brief interfaces of ScriptDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SCRIPT_DIALOG_H__
#define __KOME_SCRIPT_DIALOG_H__


#include <vector>
#include <wx/wx.h>


namespace kome {

	namespace script {

		/**
		 * @class ScriptDialog
		 * @brief baseline subtract dialog
		 */
		class ScriptDialog : public kome::window::ParametersAndHelpDialog {	// @Date:2014/01/24	<extend help>	A.Ozaki
		public:
			/**
			 * @fn ScriptDialog( wxWindow* parent, bool bEdit )
			 * @brief constructor
			 * @param parent parent window
			 */
			ScriptDialog( wxWindow* parent , bool bEdit );

			/**
			 * @fn virtual ~ScriptDialog()
			 * @brief destructor
			 */
			virtual ~ScriptDialog();

		protected:
			/** item */
			kome::plugin::PluginFunctionItem* m_item;

			/** combobox */
			wxComboBox* m_combo;

			/** input */
			wxTextCtrl* m_inText;

			/** output */
			wxTextCtrl* m_outText;

			/** save file path */
			std::string m_path;

			/** edited */
			bool m_edited;

			/** variables dialog */
			kome::window::GridListDialog* m_varDlg;

			/** name text */
			wxTextCtrl* m_nameText;

			/** event combobox */
			wxComboBox* m_evtCombo;
			
			/** edit flg */
			bool m_bEdit;
		public:
			/** dialog */
			static ScriptDialog* m_dlg;

		public:
			/**
			 * @fp void insertText( const char* s )
			 * @brief inserts text
			 * @param[in] s character string to be inserted
			 */
			void insertText( const char* s );

		protected:
			/**
			 * @fn void onCombo()
			 * @brief updates the selection of the combobox
			 */
			void onCombo();

			/**
			 * @fn void onLoad()
			 * @brief loads script
			 */
			void onLoad();

			/**
			 * @fn void onSave()
			 * @brief saves script
			 */
			void onSave();

			/**
			 * @fn void onSaveAs()
			 * @brief saves script in the specified file path
			 */
			void onSaveAs();

			/**
			 * @fn void onRun()
			 * @brief runs script
			 */
			void onRun();

		protected:
			/**
			 * @fn std::string getExt()
			 * @brief gets the extension of the selected script
			 * @return extension
			 */
			std::string getExt();

			/**
			 * @fn void saveScript( const char* path )
			 * @brief save the script
			 * @param[in] path file path
			 */
			void saveScript( const char* path );

			/**
			 * @fn void setInTextVal( const char* path )
			 * @brief sets the InTextCtrl Value
			 * @param path file path
			 */
			void setInTextVal( const char* path );

		protected:
			/**
			 * @fn void setDialogTitle()
			 * @brief sets the dialog title
			 */
			void setDialogTitle();

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
			 * @fn virtual bool onCancel()
			 * @brief This method is called when the "Cancel" button is clicked.
			 * @brief If true, this window is closed.
			 */
			virtual bool onCancel();

		protected:
			/**
			 * @fn void onCombo( wxCommandEvent& evt )
			 * @brief This method is called when a script is selected on the combobox.
			 * @param[in] evt event information
			 */
			void onCombo( wxCommandEvent& evt );

			/**
			 * @fn void onLoad( wxCommandEvent& evt )
			 * @brief This method is called when the Load button is clicked.
			 * @param[in] evt event information
			 */
			void onLoad( wxCommandEvent& evt );

			/**
			 * @fn void onSave( wxCommandEvent& evt )
			 * @brief This method is called when the Save button is clicked.
			 * @param[in] evt event information
			 */
			void onSave( wxCommandEvent& evt );

			/**
			 * @fn void onSaveAs( wxCommandEvent& evt )
			 * @brief This method is called when the Save as button is clicked.
			 * @param[in] evt event information
			 */
			void onSaveAs( wxCommandEvent& evt ); 
			
			/**
			 * @fn void onRun( wxCommandEvent& evt )
			 * @brief This method is called when the Run button is clicked.
			 * @param[in] evt event information
			 */
			void onRun( wxCommandEvent& evt );

			/**
			 * @fn void onText( wxCommandEvent& evt )
			 * @brief This method is called when text form is updated.
			 * @param[in] evt event information
			 */
			void onText( wxCommandEvent& evt );

			/**
			 * @fn void onVariable( wxCommandEvent& evt )
			 * @brief This method is called when the Variables button is clicked.
			 * @param[in] evt event information
			 */
			void onVariable( wxCommandEvent& evt );
			
			/**
			 * @fn void onSaveScriptList( wxCommandEvent& evt )
			 * @brief This method is called when the OK button is clicked.
			 * @param[in] evt evetn information
			 */
			void onSaveScriptList( wxCommandEvent& evt ) ;

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
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_SCRIPT_DIALOG_H__
