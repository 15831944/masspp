/**
 * @file EMailTextCtrl.h
 * @brief interfaces of EmailTextCtrl class
 *
 * @author M.Izumi
 * @date 2011.09.20
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_EMAIL_TEXT_CTRL_H__
#define __KOME_WINDOW_EMAIL_TEXT_CTRL_H__


#include <wx/wx.h>
#include <wx/listctrl.h>


namespace kome {
	namespace window {

		/**
		 * @class EMailTextCtrl
		 * @brief grid list control class
		 */
	class WINDOW_MANAGER_CLASS EMailTextCtrl : public wxTextCtrl {
	  public:
			/**
			 * @fn EMailTextCtrl( wxWindow* parent, const int id )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 */
			EMailTextCtrl( wxWindow* parent, const int id );
			
			/**
			 * @fn virtual ~EMailTextCtrl()
			 * @brief destructor
			 */
			virtual ~EMailTextCtrl();
		
		public:
			/**
			 * @fn void onChar( wxKeyEvent& evt )
			 * @brief This method is called when the character is input.
			 * @param[in] evt keyboard event
			 */
			void onChar( wxKeyEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_WINDOW_EMAIL_TEXT_CTRL_H__
