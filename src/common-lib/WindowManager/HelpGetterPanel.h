/**
 * @file HelpGetterPanel.h
 * @brief interfaces of HelpGetterPanel class
 *
 * @author S.Tanaka
 * @date 2012.12.18
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_HELP_GETTERPANEL_H__
#define __KOME_WINDOW_HELP_GETTERPANEL_H__


#include <wx/wx.h>
#include "StandardPanel.h"


namespace kome {
	namespace window {

		/**
		 * @class HelpGetterPanel
		 * @brief standard panel
		 */
		class WINDOW_MANAGER_CLASS HelpGetterPanel : public StandardPanel {
		public:
			/**
			 * @fn HelpGetterPanel( wxWindow* parent, const int id = -1 )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 */
			HelpGetterPanel( wxWindow* parent, const int id = -1 );

			/**
			 * @fn virtual ~HelpGetterPanel()
			 * @brief destructor
			 */
			virtual ~HelpGetterPanel();

		protected:
			/**
			 * @fn void onFocus( wxChildFocusEvent& evt )
			 * @brief This method is called when a child window is focused
			 * @param[in] evt child focus event
			 */
			void onFocus( wxChildFocusEvent& evt );

		public:
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

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_WINDOW_HELP_GETTERPANEL_H__
