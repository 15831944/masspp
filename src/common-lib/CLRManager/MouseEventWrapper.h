/**
 * @file MouseEventWrapper.h
 * @brief interfaces of MouseEventWrapper class
 *
 * @author S.Tanaka
 * @date 2007.01.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_MOUSE_EVENT_WRAPPER__
#define __KOME_CLR_MOUSE_EVENT_WRAPPER__

namespace kome {
	namespace clr {
		/**
		 * @class MouseEventWrapper
		 * @brief MouseEvent wrapper class to use on CLR environment
		 */
		public ref class MouseEventWrapper {
		public:
			/**
			 * @fn MouseEventWrapper( kome::evt::MouseEvent& evt )
			 * @brief constructor
			 * @param[in] evt mouse event object to be wrapped
			 */
			MouseEventWrapper( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual ~MouseEventWrapper()
			 * @brief destructor
			 */
			virtual ~MouseEventWrapper();

		protected:
			/** mouse event object */
			kome::evt::MouseEvent& m_evt;

		public:
			/**
			 * @fn kome::evt::MouseEvent& getMouseEvent()
			 * @brief gets mouse event object
			 * @return mouse event object
			 */
			kome::evt::MouseEvent& getMouseEvent();

		public:
			/**
			 * @fn void setPosition( int px, int py )
			 * @brief sets cursor position
			 * @param[in] px x coordinate of cursor position
			 * @param[in] py y coordinate of cursor position
			 */
			void setPosition( int x, int y );

			/**
			 * @fn void x( int px )
			 * @brief sets x coordinate of cursor position
			 * @param[in] px x coordinate of cursor position
			 */
			void x( int px );

			/**
			 * @fn int x()
			 * @brief gets x coordinate of cursor position
			 * @return x coordinate of cursor position
			 */
			int x();

			/**
			 * @fn void y( int py )
			 * @brief sets y coordinate of cursor position
			 * @param[in] py y coordinate of cursor position
			 */
			void y( int py );

			/**
			 * @fn int y()
			 * @brief gets y coordinate of cursor position
			 * @return y coordinate of cursor position
			 */
			int y();

			/**
			 * @fn void lbutton( bool lbtn )
			 * @brief sets l L button flag
			 * @param[in] lbtn L button flag
			 */
			void lbutton( bool lbtn );

			/**
			 * @fn bool lbutton()
			 * @brief gets L button flag
			 * @return L button flag
			 */
			bool lbutton();

			/**
			 * @fn void mbutton( bool mbtn )
			 * @brief sets M button flag
			 * @param[in] mbtn M button flag
			 */
			void mbutton( bool mbtn );

			/**
			 * @fn bool mbutton()
			 * @brief gets M button flag
			 * @return M button flag
			 */
			bool mbutton();

			/**
			 * @fn void rbutton( bool rbtn )
			 * @brief sets R button flag
			 * @param[in] rbtn R button flag
			 */			 
			void rbutton( bool rbutton );

			/**
			 * @fn bool rbutton()
			 * @brief gets R button flag
			 * @return R button flag
			 */
			bool rbutton();

			/**
			 * @fn void shift( bool s )
			 * @brief sets shift flag
			 * @param[in] s shift flag
			 */
			void shift( bool s );

			/**
			 * @fn bool shift()
			 * @brief gets shift flag
			 * @return shift flag
			 */
			bool shift();

			/**
			 * @fn void ctrl( bool c )
			 * @brief sets ctrl flag
			 * @param[in] c ctrl flag
			 */
			void ctrl( bool c );

			/**
			 * @fn bool ctrl()
			 * @brief gets ctrl flag
			 * @return ctrl flag
			 */
			bool ctrl();

			/**
			 * @fn void alt( bool a )
			 * @brief sets alt flag
			 * @param[in] a alt flag
			 */
			void alt( bool a );

			/**
			 * @fn bool alt()
			 * @brief gets alt flag
			 * @return alt flag
			 */
			bool alt();
		};
	}
}

#endif		// __KOME_CLR_MOUSE_EVENT_WRAPPER__
