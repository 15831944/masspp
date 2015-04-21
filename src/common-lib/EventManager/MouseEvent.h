/**
 * @file MouseEvent.h
 * @brief interfaces of MouseEvent class
 *
 * @author S.Tanaka
 * @date 2007.01.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_EVT_MOUSE_EVENT_H__
#define __KOME_EVT_MOUSE_EVENT_H__


namespace kome {
	namespace evt {

		/**
		 * @class MouseEvent
		 * @brief mouse event information
		 */
		class EVENT_MANAGER_CLASS MouseEvent {
		public:
			/**
			 * @fn MouseEvent()
			 * @brief constructor
			 */
			MouseEvent();

			/**
			 * @fn ~MouseEvent()
			 * @brief destructor
			 */
			~MouseEvent();

		protected:
			/** x coordinate */
			int m_x;

			/** y coordinate */
			int m_y;

			/** l button flag */
			bool m_lbutton;
			/** m button flag */
			bool m_mbutton;
			/** r button flag */
			bool m_rbutton;

			/** shift key flag */
			bool m_shift;
			/** ctrl key flag */
			bool m_ctrl;
			/** alt key flag */
			bool m_alt;

		public:
			/**
			 * @fn void setPosition( const int px, const int py )
			 * @brief sets cursor position
			 * @param[in] px x coordinate of cursor position
			 * @param[in] py y coordinate of cursor position
			 */
			void setPosition( const int x, const int y );

			/**
			 * @fn void x( const int px )
			 * @brief sets x coordinate of cursor position
			 * @param[in] px x coordinate of cursor position
			 */
			void x( const int px );

			/**
			 * @fn int x()
			 * @brief gets x coordinate of cursor position
			 * @return x coordinate of cursor position
			 */
			int x();

			/**
			 * @fn void y( const int py )
			 * @brief sets y coordinate of cursor position
			 * @param[in] py y coordinate of cursor position
			 */
			void y( const int py );

			/**
			 * @fn int y()
			 * @brief gets y coordinate of cursor position
			 * @return y coordinate of cursor position
			 */
			int y();

			/**
			 * @fn void lbutton( const bool lbtn )
			 * @brief sets l L button flag
			 * @param[in] lbtn L button flag
			 */
			void lbutton( const bool lbtn );

			/**
			 * @fn bool lbutton()
			 * @brief gets L button flag
			 * @return L button flag
			 */
			bool lbutton();

			/**
			 * @fn void mbutton( const bool mbtn )
			 * @brief sets M button flag
			 * @param[in] mbtn M button flag
			 */
			void mbutton( const bool mbtn );

			/**
			 * @fn bool mbutton()
			 * @brief gets M button flag
			 * @return M button flag
			 */
			bool mbutton();

			/**
			 * @fn void rbutton( const bool rbutton )
			 * @brief sets R button flag
			 * @param[in] rbutton R button flag
			 */			 
			void rbutton( const bool rbutton );

			/**
			 * @fn bool rbutton()
			 * @brief gets R button flag
			 * @return R button flag
			 */
			bool rbutton();

			/**
			 * @fn void shift( const bool s )
			 * @brief sets shift flag
			 * @param[in] s shift flag
			 */
			void shift( const bool s );

			/**
			 * @fn bool shift()
			 * @brief gets shift flag
			 * @return shift flag
			 */
			bool shift();

			/**
			 * @fn void ctrl( const bool c )
			 * @brief sets ctrl flag
			 * @param[in] c ctrl flag
			 */
			void ctrl( const bool c );

			/**
			 * @fn bool ctrl()
			 * @brief gets ctrl flag
			 * @return ctrl flag
			 */
			bool ctrl();

			/**
			 * @fn void alt( const bool a )
			 * @brief sets alt flag
			 * @param[in] a alt flag
			 */
			void alt( const bool a );

			/**
			 * @fn bool alt()
			 * @brief gets alt flag
			 * @return alt flag
			 */
			bool alt();
		};
	}
}

#endif		// __KOME_EVT_MOUSE_EVENT_H__
