/**
 * @file OutputDialog.h
 * @brief interfaces of OutputDialog class
 *
 * @author M.Izumi
 * @date 2013.08.08
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_OUTPUT_DIALOG_H__
#define __KOME_OUTPUT_DIALOG_H__

#include <vector>
#include <wx/wx.h>

namespace kome { 
	namespace script { 

		class OutputDialog : public kome::window::StandardDialog {

		public:
			/**
			 * @fn OutputDialog( wxWindow* parent, stScriptInfo stInfo )
			 * @brief constructor
			 * @param parent parent window
			 */
			OutputDialog( wxWindow* parent, stScriptInfo stInfo );

			/**
			 * @fn virtual ~OutputDialog( )
			 * @brief destructor
			 */
			virtual ~ OutputDialog();


		protected:
			/** output */
			wxTextCtrl* m_outText;
					
			/** script info */
			stScriptInfo m_insInfo;

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:

			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value data to window controls
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();
		
		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif // __KOME_OUTPUT_DIALOG_H__
