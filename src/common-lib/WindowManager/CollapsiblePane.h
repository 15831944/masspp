/**
 * @file CollapsiblePane.h
 * @brief interfaces of CollapsiblePane class
 *
 * @author S.Tanaka
 * @date 2011.04.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_COLLAPSIBLE_PANE_H__
#define __KOME_WINDOW_COLLAPSIBLE_PANE_H__



#include <wx/wx.h>
#include <wx/collpane.h>
#include <wx/generic/collpaneg.h>

#include <string>


namespace kome {
	namespace window {

		/**
		 * @class CollapsiblePane
		 * @brief standard panel
		 */
		class WINDOW_MANAGER_CLASS CollapsiblePane : public wxGenericCollapsiblePane {
		public:
			/**
			 * @fn CollapsiblePane( wxWindow* parent, const char* title, const int id = wxID_ANY )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title title
			 * @param[in] id window ID
			 */
			CollapsiblePane( wxWindow* parent, const char* title, const int id = wxID_ANY );

			/**
			 * @fn virtual ~CollapsiblePane()
			 * @brief destructor
			 */
			virtual ~CollapsiblePane();

		protected:
			/** text */
			wxStaticText* m_text;

		public:
			/**
			 * @fn virtual void Collapse( bool collapse = true )
			 * @brief collapses or expands the pane window (override method)
			 * @param collapse true: collapse, false: expand
			 */
			virtual void Collapse( bool collapse = true );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_WINDOW_COLLAPSIBLE_PANE_H__
