/**
 * @file PeakListCtrl.h
 * @brief interfaces of PeakListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.09.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PEAK_LIST_CTRL_H__
#define __KOME_PEAK_LIST_CTRL_H__


#include <wx/wx.h>


namespace kome {
	namespace search {

		/**
		 * @class PeakSearch Dialog
		 * @brief peaks search dialog
		 */
		class PeakListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn PeakListCtrl( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param parent parent window
			 * @param width window width
			 * @param height window height 
			 */
			PeakListCtrl( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~PeakListCtrl()
			 * @brief destructor
			 */
			virtual ~PeakListCtrl();

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method (override method)
			 */
			virtual void onCreate();

			/**
			 * @fn virtual double getDouble( const long long data, const int column ) const
			 * @brief gets double value from data (override method)
			 * @param data data
			 * @param column column number
			 */
			virtual double getDouble( const long long data, const int column ) const;

		protected:
			/**
			 * @fn void onDblClick( wxMouseEvent& evt )
			 * @brief This method is called when the list item is double clicked.
			 * @param evt mouse event information
			 */
			void onDblClick( wxMouseEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif	// __KOME_PEAK_LIST_CTRL_H__
