/**
 * @file MessageDialog.h
 * @brief interfaces of MessageDialog class
 *
 * @author S.Tanaka
 * @date 2011.05.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_WINDOW_MESSAGE_DIALOG_H__
#define __KOME_WINDOW_MESSAGE_DIALOG_H__


#include <wx/wx.h>


#include "StandardDialog.h"


namespace kome {
	namespace window {

		/**
		 * @class MessageDialog
		 * @brief standard dialog
		 */
		class WINDOW_MANAGER_CLASS MessageDialog : public StandardDialog {
		public:
			/**
			 * @typedef enum IconType
			 * @brief icon type
			 */
			typedef enum {
				ICON_NONE,
				ICON_INFO,
				ICON_ERROR,
				ICON_WARNING,
				ICON_QUESTION
			} IconType;

			/**
			 * @typedef enum ButtonType
			 * @brief button type
			 */
			typedef enum {
				BUTTON_OK     = 1,
				BUTTON_CANCEL = 2,
				BUTTON_YES    = 4,
				BUTTON_NO     = 8
			} ButtonType;

		public:
			/**
			 * @fn MessageDialog(
						wxWindow* parent,
						const char* title,
						const char* message,
						const IconType icon = ICON_INFO,
						int buttonStyle = BUTTON_OK,
						const char* infoTitle = "Information",
						const char* info = NULL,
						const char* yesBtnName = NULL,
						const char* noBtnName = NULL
					)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 * @param[in] message message
			 * @param[in] icon icon type
			 * @param[in] buttonStyle button style
			 * @param[in] infoTitle information title
			 * @param[in] info information
			 * @param[in] yesBtnName alternative name for the "Yes" button
			 * @param[in] noBtnName alternative name for the "No" button
			 */
			MessageDialog(
				wxWindow* parent,
				const char* title,
				const char* message,
				const IconType icon = ICON_INFO,
				int buttonStyle = BUTTON_OK,
				const char* infoTitle = "Information",
				const char* info = NULL,
				const char* yesBtnName = NULL,
				const char* noBtnName = NULL
			);

			/**
			 * @fn virtual ~MessageDialog()
			 * @brief destructor
			 */
			virtual ~MessageDialog();

		protected:
			/** icon type */
			IconType m_iconType;

			/** button style */
			int m_buttonStyle;

			/** message */
			std::string m_msg;

			/** information title */
			std::string m_infoTitle;

			/** information */
			std::string m_info;

			/** alternative name for the "Yes" button */
			std::string m_yesBtnName;

			/** alternative name for the "No" button */
			std::string m_noBtnName;

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
			 * @fn void onYes( wxCommandEvent& evt )
			 * @brief This method is called when the Yes button is clicked.
			 * @param[in] evt event information
			 */
			void onYes( wxCommandEvent& evt );

			/**
		 	* @fn void onNo( wxCommandEvent& evt )
			* @brief This method is called when the No button is clicked.
			* @param[in] evt event information
			*/
			void onNo( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_WINDOW_MESSAGE_DIALOG_H__
