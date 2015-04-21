/**
 * @file SizeEvent.h
 * @brief interfaces of SizeEvent class
 *
 * @author S.Tanaka
 * @date 2007.09.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_EVT_SIZE_EVENT_H__
#define __KOME_EVT_SIZE_EVENT_H__


namespace kome {
	namespace evt {

		/**
		 * @class SizeEvent
		 * @brief size event information
		 */
		class EVENT_MANAGER_CLASS SizeEvent {
		public:
			/**
			 * @fn SizeEvent()
			 * @brief constructor
			 */
			SizeEvent();

			/**
			 * @fn SizeEvent( const unsigned int w, const unsigned int h )
			 * @brief constructor
			 * @param[in] w the width of the size
			 * @param[in] h the height of the size
			 */
			SizeEvent( const unsigned int w, const unsigned int h );

			/**
			 * @fn ~SizeEvent()
			 * @brief destructor
			 */
			~SizeEvent();

		protected:
			/** width */
			unsigned int m_width;

			/** height */
			unsigned int m_height;

		public:
			/**
			 * @fn void width( const unsigned int w )
			 * @brief sets width
			 * @param[in] w width
			 */
			void width( const unsigned int w );

			/**
			 * @fn unsigned int width()
			 * @brief gets width
			 * @return width
			 */
			unsigned int width();

			/**
			 * @fn void height( const unsigned int h )
			 * @brief sets height
			 * @param[in] h height
			 */
			void height( const unsigned int h );

			/**
			 * @fn unsigned int height()
			 * @brief gets height
			 * @return height
			 */
			unsigned int height();
		};
	}
}


#endif		// __KOME_EVT_SIZE_EVENT_H__
