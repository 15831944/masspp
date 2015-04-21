/**
 * @file HelpPanel.h
 * @brief interfaces of HelpPanel class
 *
 * @author S.Tanaka
 * @date 2014.01.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_HELP_PANEL_H__
#define __KOME_WINDOW_HELP_PANEL_H__


#include <wx/wx.h>
#include "ParametersAndHelpPanel.h"


namespace kome {
	namespace window {

		class BitmapCanvas;

		/**
		 * @class HelpPanel
		 * @brief standard panel
		 */
		class WINDOW_MANAGER_CLASS HelpPanel : public ParametersAndHelpPanel {
		public:
			/**
			 * @fn HelpPanel( wxWindow* parent, const int id = -1 )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 */
			HelpPanel( wxWindow* parent, const int id = -1 );

			/**
			 * @fn virtual ~HelpPanel()
			 * @brief destructor
			 */
			virtual ~HelpPanel();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_WINDOW_HELP_PANEL_H__
