/**
 * @file PeakDifferenceListCtrl.h
 * @brief interfaces of PeakDifferenceListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PEAK_DIFFERENCE_LIST_CTRL_H__
#define __KOME_PEAK_DIFFERENCE_LIST_CTRL_H__


#include <wx/wx.h>


namespace kome {
	namespace search {

		/**
		 * @class PeakSearch Dialog
		 * @brief peak difference search dialog
		 */
		class PeakDifferenceListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn PeakDifferenceListCtrl( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param parent parent window
			 * @param width window width
			 * @param height window height 
			 */
			PeakDifferenceListCtrl( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~PeakDifferenceListCtrl()
			 * @brief destructor
			 */
			virtual ~PeakDifferenceListCtrl();

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method (override method)
			 */
			virtual void onCreate();

			/**
			 * @fn virtual int getInt( const long long data, const int column ) const
			 * @brief gets int value from data (override method)
			 * @param data data
			 * @param column column number
			 */
			virtual int getInt( const long long data, const int column ) const;

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


#endif	// __KOME_PEAK_DIFFERENCE_LIST_CTRL_H__
