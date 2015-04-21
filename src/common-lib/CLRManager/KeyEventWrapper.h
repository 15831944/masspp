/**
 * @file KeyEventWrapper.h
 * @brief interfaces of KeyEventWrapper class
 *
 * @author S.Tanaka
 * @date 2007.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_KEY_EVENT_WRAPPER__
#define __KOME_CLR_KEY_EVENT_WRAPPER__


namespace kome {
	namespace clr {

		/**
		 * @class KeyEventWrapper
		 * @brief KeyEvent wrapper class to use on CLR environment
		 */
		public ref class KeyEventWrapper {
		public:
			/**
			 * @fn KeyEventWrapper( kome::evt::KeyEvent& evt )
			 * @brief constructor
			 * @param[in] evt key event object to be wrapped
			 */
			KeyEventWrapper( kome::evt::KeyEvent& evt );

			/**
			 * @fn virtual ~KeyEventWrapper()
			 * @brief destructor
			 */
			virtual ~KeyEventWrapper();

		public:
			/** key code */
			ENUM_CLASS KeyCode {
				BACKSPACE = 0x0100,
				TAB	   = 0x0101,
				RETURN	= 0x0102,
				SPACE	 = 0x0103,
				DELETE	= 0x0104,
				SHIFT	 = 0x0105,
				ALT	   = 0x0106,
				CONTROL   = 0x0107,
				PAUSE	 = 0x0108,
				END	   = 0x0109,
				HOME	  = 0x010A,
				LEFT	  = 0x010B,
				UP		= 0x010C,
				RIGHT	 = 0x010D,
				DOWN	  = 0x010E,
				INSERT	= 0x010F,
				F1		= 0x0110,
				F2		= 0x0111,
				F3		= 0x0112,
				F4		= 0x0113,
				F5		= 0x0114,
				F6		= 0x0115,
				F7		= 0x0116,
				F8		= 0x0117,
				F9		= 0x0118,
				F10	   = 0x0119,
				F11	   = 0x011A,
				F12	   = 0x011B,
				F13	   = 0x011C,
				F14	   = 0x011D,
				F15	   = 0x011E,
				F16	   = 0x011F,
				NUMLOCK   = 0x0120,
				SCROLL	= 0x0121,
				PAGEUP	= 0x0122,
				PAGEDOWN  = 0x0123
			};

		protected:
			/** key event object */
			kome::evt::KeyEvent& m_evt;

		public:
			/**
			 * @fn kome::evt::KeyEvent& getKeyEvent()
			 * @brief gets key event object
			 * @return key event object
			 */
			kome::evt::KeyEvent& getKeyEvent();

		public:
			/**
			 * @fn void setCode( int code )
			 * @brief sets key code
			 * @param[in] code key code
			 */
			void setCode( int code );

			/**
			 * @fn int getCode()
			 * @brief gets key code
			 * @return key code
			 */
			int getCode();

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

#endif		// __KOME_CLR_KEY_EVENT_WRAPPER__
