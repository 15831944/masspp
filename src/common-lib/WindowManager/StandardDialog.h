/**
 * @file StandardDialog.h
 * @brief interfaces of StandardDialog class
 *
 * @author S.Tanaka
 * @date 2007.08.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_STANDARD_DIALOG_H__
#define __KOME_WINDOW_STANDARD_DIALOG_H__


#include <wx/wx.h>


namespace kome {
	namespace window {

		/**
		 * @class StandardDialog
		 * @brief standard dialog
		 */
		class WINDOW_MANAGER_CLASS StandardDialog : public wxDialog {
		public:
			/**
			 * @fn StandardDialog(
						wxWindow* parent,
						const char* title,
						const char* commitBtnName = NULL,
						const int px = -1,
						const int py = -1,
						const int width = -1,
						const int height = -1
					)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 * @param[in] commitBtnName alternative name for the "OK" commit button
			 * @param[in] px the x coordinate of the dialog position
			 * @param[in] py the y coordinate of the dialog position
			 * @param[in] width dialog width
			 * @param[in] height dialog height
			 */
			StandardDialog(
				wxWindow* parent,
				const char* title,
				const char* commitBtnName = NULL,
				const int px = -1,
				const int py = -1,
				const int width = -1,
				const int height = -1
			);

			/**
			 * @fn virtual ~StandardDialog()
			 * @brief destructor
			 */
			virtual ~StandardDialog();

		public:
			/**
			 * @fn void closeChildDialogs()
			 * @brief closes all child dialogs
			 */
			void closeChildDialogs();

		protected:
			/** OK commit button name */
			std::string m_commitBtnName;

		protected:
			/**
			 * @fn virtual void createControls()
			 * @brief creates controls and sizers
			 */
			virtual void createControls();

			/**
			 * @fn virtual wxSizer* createTopSizer()
			 * @brief creates top sizer (main sizer & button sizer)
			 * @return top sizer
			 */
			virtual wxSizer* createTopSizer();

			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:
			/**
			 * @fn virtual bool onOK()
			 * @brief This method is called when the "OK" button is clicked and values in the window is valid.
			 * @return If true, this window is closed.
			 */
			virtual bool onOK();

			/**
			 * @fn virtual bool onCancel()
			 * @brief This method is called when the "Cancel" button is clicked.
			 * @brief If true, this window is closed.
			 */
			virtual bool onCancel();

		protected:
			/**
			 * @fn virtual void onInitDialog( wxInitDialogEvent& evt )
			 * @brief This method is called on init dialog event
			 * @param[in] evt event information
			 */
			virtual void onInitDialog( wxInitDialogEvent& evt );

			/**
			 * @fn virtual void onOkButton( wxCommandEvent& evt )
			 * @brief This method is called when the "OK" button is clicked.
			 * @param[in] evt event information
			 */
			virtual void onOkButton( wxCommandEvent& evt );

			/**
			 * @fn virtual void onCancelButton( wxCommandEvent& evt )
			 * @brief This method is called when the "Cancel" button is clicked.
			 */
			virtual void onCancelButton( wxCommandEvent& evt );

		protected:
			/**
			 * @fn static void closeChildDialogs( wxWindow* parent )
			 * @brief closes child dialogs
			 * @param[in] parent parent window
			 */
			static void closeChildDialogs( wxWindow* parent );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_WINDOW_STANDARD_DIALOG_H__
