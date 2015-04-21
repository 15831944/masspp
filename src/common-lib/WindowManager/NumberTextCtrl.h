/**
 * @file NumberTextCtrl.h
 * @brief interfaces of NumberTextCtrl class
 *
 * @author S.Tanaka
 * @date 2011.08.01
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_NUMBER_TEXT_CTRL_H__
#define __KOME_WINDOW_NUMBER_TEXT_CTRL_H__


#include <wx/wx.h>
#include <wx/listctrl.h>


namespace kome {
	namespace window {

		/**
		 * @class NumberTextCtrl
		 * @brief grid list control class
		 */
		class WINDOW_MANAGER_CLASS NumberTextCtrl : public wxTextCtrl {
		public:
			/**
			 * @fn NumberTextCtrl(	wxWindow* parent, const int id, const bool integer, const bool neg )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 * @param[in] integer If true, the number should be integer value.
			 * @param[in] neg If true, the number can be negative value.
			 */
			NumberTextCtrl(	wxWindow* parent, const int id, const bool integer, const bool neg );

			/**
			 * @fn virtual ~NumberTextCtrl()
			 * @brief destructor
			 */
			virtual ~NumberTextCtrl();

		protected:
			/** integer flag */
			bool m_integer;

			/** negative flag */
			bool m_neg;

		public:
			/**
			 * @fn void onChar( wxKeyEvent& evt )
			 * @brief This method is called when the character is input.
			 * @param[in] evt keyboard event
			 */
			void onChar( wxKeyEvent& evt );

			/**
			 * @fn void onContextMenu( wxContextMenuEvent& evt )
			 * @brief This method is called on requesting context menu
			 * @param[in] evt context menu event information
			 */
			void onContextMenu( wxContextMenuEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_WINDOW_NUMBER_TEXT_CTRL_H__
