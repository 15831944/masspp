/**
 * @file MessageCheckDialog.h
 * @brief interfaces of MessageCheckDialog class
 *
 * @author S.Tanaka
 * @date 2013.02.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_WINDOW_MESSAGE_CHECK_DIALOG_H__
#define __KOME_WINDOW_MESSAGE_CHECK_DIALOG_H__


#include <wx/wx.h>


#include "MessageDialog.h"


namespace kome {
	namespace window {

		/**
		 * @class MessageCheckDialog
		 * @brief standard dialog
		 */
		class WINDOW_MANAGER_CLASS MessageCheckDialog : public MessageDialog {
		public:
			/**
			 * @fn MessageCheckDialog(
						wxWindow* parent,
						const char* title,
						const char* message,
						const char* key
						const IconType icon = ICON_INFO,
						int buttonStyle = BUTTON_OK
					)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 * @param[in] message message
			 * @param[in] key save key
			 * @param[in] icon icon type
			 * @param[in] buttonStyle button style
			 */
			MessageCheckDialog(
				wxWindow* parent,
				const char* title,
				const char* message,
				const char* key,
				const IconType icon = ICON_INFO,
				int buttonStyle = BUTTON_OK
			);

			/**
			 * @fn virtual ~MessageCheckDialog()
			 * @brief destructor
			 */
			virtual ~MessageCheckDialog();

		protected:
			/** key */
			std::string m_key;

		protected:
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
			 * @return If true, this window is closed. (override method)
			 */
			virtual bool onOK();


		public:
			/**
			 * @fn static void openDialogAfterChecking(
						wxWindow* parent,
						const char* title,
						const char* message,
						const char* key,
						const IconType icon = ICON_INFO,
						int buttonStyle = BUTTON_OK
					)
			 * @brief opens dialog
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 * @param[in] message message
			 * @param[in] key save key
			 * @param[in] icon icon type
			 * @param[in] buttonStyle button style
			 */
			static void openDialogAfterChecking(
				wxWindow* parent,
				const char* title,
				const char* message,
				const char* key,
				const IconType icon = ICON_INFO,
				int buttonStyle = BUTTON_OK
			);

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_WINDOW_MESSAGE_CHECK_DIALOG_H__
