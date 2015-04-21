/**
 * @file ClientWindow.h
 * @brief interfaces of ClientWindow class
 *
 * @author S.Tanaka
 * @date 2008.12.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_CLIENT_WINDOW_H__
#define __KOME_WINDOW_CLIENT_WINDOW_H__


#include <wx/wx.h>


namespace kome {
	namespace window {

		class MainWindow;

		/**
		 * @class ClientWindow
		 * @brief main frame class
		 */
		class WINDOW_MANAGER_CLASS ClientWindow : public wxMDIClientWindow {
		public:
			/**
			 * @fn ClientWindow()
			 * @brief constructor
			 */
			ClientWindow();

			/**
			 * @fn ClientWindow( wxMDIParentFrame* parent )
			 * @brief constructor
			 * @param[in] parent parent frame
			 */
			ClientWindow( wxMDIParentFrame* parent );

			/** 
			 * @fn virtual ~ClientWindow()
			 * @brief destructor
			 */
			virtual ~ClientWindow();

		protected:
			/** previous size */
			wxSize m_prevSize;

		protected:
			/**
			 * @fn virtual void DoSetSize(
						int x,
						int y,
						int width,
						int height,
						int sizeFlags = wxSIZE_AUTO
					)
			 * @brief do set size
			 * @param[in] x the x coordinate of the position
			 * @param[in] y the y coordinate of the position
			 * @param[in] width width
			 * @param[in] height height
			 * @param[in] sizeFlags flags
			 */
			virtual void DoSetSize(
				int x,
				int y,
				int width,
				int height,
				int sizeFlags = wxSIZE_AUTO
			);

		protected:
			/**
			 * @fn void onSize( wxSizeEvent& evt )
			 * @brief on size event
			 * @param[in] evt event information
			 */
			void onSize( wxSizeEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif	// __KOME_WINDOW_CLIENT_WINDOW_H__
