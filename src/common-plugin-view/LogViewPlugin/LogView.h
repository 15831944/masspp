/**
 * @file LogView.h
 * @brief interfaces of LogView class
 *
 * @author S.Tanaka
 * @date 2008.07.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_LOG_VIEW_H__
#define __KOME_VIEW_LOG_VIEW_H__


#include <string>
#include <list>
#include <wx/wx.h>


namespace kome {
	namespace view {

		/**
		 * @class LogView
		 * @brief log view text control class
		 */
		class LogView : public wxTextCtrl {
		public:
			/**
			 * @fn LogView( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			LogView( wxWindow* parent );

			/**
			 * @fn virtual ~LogView()
			 * @brief destructor
			 */
			virtual ~LogView();

		protected:
			/** message list */
			std::list< std::string > m_msgList;

			/** destructor flag */
			bool	m_bDestructed;			// @Date:2013/09/24	<Add>	A.Ozaki

		public:
			/**
			 * @fn void addLog( const char* msg )
			 * @brief adds log message
			 * @param msg log message
			 */
			void addLog( const char* msg );			

		};
	}
}


#endif		// __KOME_VIEW_LOG_VIEW_H__

