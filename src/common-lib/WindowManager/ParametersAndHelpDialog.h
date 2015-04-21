/**
 * @file ParametersAndHelpDialog.h
 * @brief interfaces of ParametersAndHelpDialog class
 *
 * @author S.Tanaka
 * @date 2014.01.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_PARAMETERS_AND_HELP_DIALOG_H__
#define __KOME_WINDOW_PARAMETERS_AND_HELP_DIALOG_H__


#include "StandardDialog.h"
#include "HelpPanel.h"


namespace kome {
	namespace window {

		/**
		 * @class ParametersAndHelpDialog
		 * @brief standard dialog
		 */
		class WINDOW_MANAGER_CLASS ParametersAndHelpDialog : public StandardDialog {
		public:
			/**
			 * @fn ParametersAndHelpDialog(
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
			ParametersAndHelpDialog(
				wxWindow* parent,
				const char* title,
				const char* commitBtnName = NULL,
				const int px = -1,
				const int py = -1,
				const int width = -1,
				const int height = -1
			);

			/**
			 * @fn virtual ~ParametersAndHelpDialog()
			 * @brief destructor
			 */
			virtual ~ParametersAndHelpDialog();

		protected:
			/** help panel */
			HelpPanel* m_help;

			/** help map */
			std::map< wxWindow*, std::string > m_helpMap;

			/** image map */
			std::map< wxWindow*, std::string > m_imageMap;

		protected:
			/**
			 * @fn void setDescription( wxWindow* wnd, const char* help )
			 * @brief sets the description
			 * @param[in] wnd window
			 * @param[in] help help
			 */
			void setDescription( wxWindow* wnd, const char* help );

			/**
			 * @fn void setImageFilePath( wxWindow* wnd, const char* path )
			 * @brief sets the image file path
			 * @param[in] wnd window
			 * @param[in] path image file path
			 */
			void setImageFilePath( wxWindow* wnd, const char* path );

		protected:
			/**
			 * @fn virtual wxSizer* createTopSizer()
			 * @brief creates top sizer (main sizer & button sizer)
			 * @return top sizer
			 */
			virtual wxSizer* createTopSizer();

		protected:
			/**
			 * @fn void onFocus( wxChildFocusEvent& evt )
			 * @brief This method is called when a child window is focused
			 * @param[in] evt child focus event
			 */
			void onFocus( wxChildFocusEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_WINDOW_PARAMETERS_AND_HELP_DIALOG_H__
