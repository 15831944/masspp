/**
 * @file KeyEvent.h
 * @brief interfaces of KeyEvent class
 *
 * @author S.Tanaka
 * @date 2007.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_EVT_KEY_EVENT_H__
#define __KOME_EVT_KEY_EVENT_H__


namespace kome {
	namespace evt {

		/**
		 * @class KeyEvent
		 * @brief key event information
		 */
		class EVENT_MANAGER_CLASS KeyEvent {
		public:
			/**
			 * @fn KeyEvent()
			 * @brief constructor
			 */
			KeyEvent();

			/**
			 * @fn ~KeyEvent()
			 * @brief destructor
			 */
			~KeyEvent();

		protected:
			/** key code */
			int m_code;

			/** shift key flag */
			bool m_shift;
			/** ctrl key flag */
			bool m_ctrl;
			/** alt key flag */
			bool m_alt;

		public:
			/**
			 * @fn void setCode( const int code )
			 * @brief sets key code
			 * @param[in] code key code
			 */
			void setCode( const int code );

			/**
			 * @fn int getCode()
			 * @brief gets key code
			 * @return key code
			 */
			int getCode();

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

#endif		// __KOME_EVT_KEY_EVENT_H__
