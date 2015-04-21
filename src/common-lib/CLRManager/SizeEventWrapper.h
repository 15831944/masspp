/**
 * @file SizeEventWrapper.h
 * @brief interfaces of SizeEventWrapper class
 *
 * @author S.Tanaka
 * @date 2007.09.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SIZE_EVENT_WRAPPER__
#define __KOME_CLR_SIZE_EVENT_WRAPPER__


namespace kome {
	namespace clr {

		/**
		 * @class SizeEventWrapper
		 * @brief SizeEvent wrapper class to use on CLR environment
		 */
		public ref class SizeEventWrapper {
		public:
			/**
			 * @fn SizeEventWrapper( kome::evt::SizeEvent& evt )
			 * @brief constructor
			 * @param[in] evt mouse event object to be wrapped
			 */
			SizeEventWrapper( kome::evt::SizeEvent& evt );

			/**
			 * @fn virtual ~SizeEventWrapper()
			 * @brief destructor
			 */
			virtual ~SizeEventWrapper();

		protected:
			/** mouse event object */
			kome::evt::SizeEvent& m_evt;

		public:
			/**
			 * @fn kome::evt::SizeEvent& getSizeEvent()
			 * @brief gets mouse event object
			 * @return mouse event object
			 */
			kome::evt::SizeEvent& getSizeEvent();

		public:
			/**
			 * @fn void width( unsigned int w )
			 * @brief sets width
			 * @param[in] w width
			 */
			void width( unsigned int w );

			/**
			 * @fn unsigned int width()
			 * @brief gets width
			 * @return width
			 */
			unsigned int width();

			/**
			 * @fn void height( unsigned int h )
			 * @brief sets height
			 * @param[in] h height
			 */
			void height( unsigned int h );

			/**
			 * @fn unsigned int height()
			 * @brief gets height
			 * @return height
			 */
			unsigned int height();
		};
	}
}


#endif		// __KOME_CLR_SIZE_EVENT_WRAPPER__
