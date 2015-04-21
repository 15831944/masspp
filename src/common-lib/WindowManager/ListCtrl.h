/**
 * @file ListCtrl.h
 * @brief interfaces of ListCtrl class
 *
 * @author S.Tanaka
 * @date 2011.05.18
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_LIST_CTRL_H__
#define __KOME_WINDOW_LIST_CTRL_H__


#include <wx/wx.h>
#include <wx/listctrl.h>


namespace kome {
	namespace window {

		/**
		 * @class ListCtrl
		 * @brief grid list control class
		 */
		class WINDOW_MANAGER_CLASS ListCtrl : public wxListCtrl {
		public:
			/**
			 * @fn ListCtrl(
						wxWindow* parent,
						const int id,
						const int width,
						const int height,
						const int style = wxLC_REPORT | wxLC_HRULES | wxLC_VRULES,
						const bool autoColumnWidth = true
					)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 * @param[in] width window width
			 * @param[in] height window height
			 * @param[in] style list style
			 * @param[in] autoColumnWidth auto column width flag
			 */
			ListCtrl(
				wxWindow* parent,
				const int id,
				const int width,
				const int height,
				const int style = wxLC_REPORT | wxLC_HRULES | wxLC_VRULES,
				const bool autoColumnWidth = true
			);

			/**
			 * @fn virtual ~ListCtrl()
			 * @brief destructor
			 */
			virtual ~ListCtrl();

		protected:
			/** auto column width flag */
			bool m_autoColumnWidth;

		public:
			/**
			 * @fn void setAutoColumnWidth( const bool autoColumnWidth )
			 * @brief sets auto column width flag
			 * @param[in] autoColumnWidth auto column width flag value
			 */
			void setAutoColumnWidth( const bool autoColumnWidth );

			/**
			 * @fn bool isAutoColumnWidth()
			 * @brief gets auto column width flag value
			 * @return auto column width flag value
			 */
			bool isAutoColumnWidth();

		public:
			/**
			 * @fn void setColumnWidths()
			 * @brief sets column widths automatically
			 */
			void setColumnWidths();

		protected:
			/**
			 * @fn void onSize( wxSizeEvent& evt )
			 * @brief This method is called when list size is changed
			 * @param[in] evt size event information
			 */
			void onSize( wxSizeEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_WINDOW_LIST_CTRL_H__
