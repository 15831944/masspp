/**
 * @file ParameterTitlePanel.h
 * @brief interfaces of ParameterTitlePanel class
 *
 * @author S.Tanaka
 * @date 2013.02.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_PARAMETER_TITLE_PANEL_H__
#define __KOME_WINDOW_PARAMETER_TITLE_PANEL_H__


#include <wx/wx.h>
#include "StandardPanel.h"


namespace kome {
	namespace window {

		/**
		 * @class ParameterTitlePanel
		 * @brief standard panel
		 */
		class WINDOW_MANAGER_CLASS ParameterTitlePanel : public StandardPanel {
		public:
			/**
			 * @fn ParameterTitlePanel( wxWindow* parent, const char* title, const bool required = false )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title parameter title
			 * @param[in] required If true, the parameter is required.
			 */
			ParameterTitlePanel( wxWindow* parent, const char* title, const bool required = false );

			/**
			 * @fn virtual ~ParameterTitlePanel()
			 * @brief destructor
			 */
			virtual ~ParameterTitlePanel();

		protected:
			/** title */
			std::string m_title;

			/** required */
			bool m_required;

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


#endif		// __KOME_WINDOW_PARAMETER_TITLE_PANEL_H__
