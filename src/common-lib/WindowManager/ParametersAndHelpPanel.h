/**
 * @file ParametersAndHelpPanel.h
 * @brief interfaces of ParametersAndHelpPanel class
 *
 * @author S.Tanaka
 * @date 2007.08.15
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_PARAMETERS_AND_HELPPANEL_H__
#define __KOME_WINDOW_PARAMETERS_AND_HELPPANEL_H__


#include <wx/wx.h>
#include "StandardPanel.h"


namespace kome {
	namespace window {

		class BitmapCanvas;

		/**
		 * @class ParametersAndHelpPanel
		 * @brief standard panel
		 */
		class WINDOW_MANAGER_CLASS ParametersAndHelpPanel : public StandardPanel {
		public:
			/**
			 * @fn ParametersAndHelpPanel( wxWindow* parent, const int id = -1 )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 */
			ParametersAndHelpPanel( wxWindow* parent, const int id = -1 );

			/**
			 * @fn virtual ~ParametersAndHelpPanel()
			 * @brief destructor
			 */
			virtual ~ParametersAndHelpPanel();

		protected:
			/** text area */
			wxTextCtrl* m_text;

			/** image area */
			BitmapCanvas* m_bitmap;

		protected:
			/**
			 * @fn virtual void createControls()
			 * @brief creates controls and sizers
			 */
			virtual void createControls();

		protected:
			/**
			 * @fn wxSizer* createHelpSizer()
			 * @brief creates help sizer
			 * @return help sizer
			 */
			wxSizer* createHelpSizer();

		public:
			/**
			 * @fn void setDescription( const char* desc )
			 * @brief sets the description
			 * @param[in] desc description
			 */
			void setDescription( const char* desc );

			/**
			 * @fn void setImageFilePath( const char* path )
			 * @brief sets the image file path
			 * @param[in] path image file path
			 */
			void setImageFilePath( const char* path );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_WINDOW_PARAMETERS_AND_HELPPANEL_H__
