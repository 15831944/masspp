/**
 * @file StandardPanel.h
 * @brief interfaces of StandardPanel class
 *
 * @author S.Tanaka
 * @date 2007.08.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_STANDARD_PANEL_H__
#define __KOME_WINDOW_STANDARD_PANEL_H__


#include <wx/wx.h>


namespace kome {
	namespace window {

		/**
		 * @class StandardPanel
		 * @brief standard panel
		 */
		class WINDOW_MANAGER_CLASS StandardPanel : public wxPanel {
		public:
			/**
			 * @fn StandardPanel( wxWindow* parent, const int id = -1 )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 */
			StandardPanel( wxWindow* parent, const int id = -1 );

			/**
			 * @fn virtual ~StandardPanel()
			 * @brief destructor
			 */
			virtual ~StandardPanel();

		protected:
			/**
			 * @fn virtual void createControls()
			 * @brief creates controls and sizers
			 */
			virtual void createControls();
			
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn virtual void onInitDialog( wxInitDialogEvent& evt )
			 * @brief This method is called on init panel event
			 * @param[in] evt event information
			 */
			virtual void onInitDialog( wxInitDialogEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_WINDOW_STANDARD_PANEL_H__
