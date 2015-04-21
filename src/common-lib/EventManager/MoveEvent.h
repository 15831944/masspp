/**
 * @file MoveEvent.h
 * @brief interfaces of MoveEvent class
 *
 * @author S.Tanaka
 * @date 2012.02.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_EVT_MOVE_EVENT_H__
#define __KOME_EVT_MOVE_EVENT_H__


namespace kome {
	namespace evt {

		/**
		 * @class MoveEvent
		 * @brief size event information
		 */
		class EVENT_MANAGER_CLASS MoveEvent {
		public:
			/**
			 * @fn MoveEvent()
			 * @brief constructor
			 */
			MoveEvent();

			/**
			 * @fn MoveEvent( const int px, const int py )
			 * @brief constructor
			 * @param[in] px the x-coordinate of the position
			 * @param[in] py the y-coordinate of the position
			 */
			MoveEvent( const int px, const int py );

			/**
			 * @fn ~MoveEvent()
			 * @brief destructor
			 */
			~MoveEvent();

		protected:
			/** width */
			int m_px;

			/** height */
			int m_py;

		public:
			/**
			 * @fn void x( const int px )
			 * @brief sets the x-coordinate
			 * @param[in] px x-coordinate
			 */
			void x( const int px );

			/**
			 * @fn int x()
			 * @brief gets x-coordinate
			 * @return x-coordinate
			 */
			int x();

			/**
			 * @fn void y( const int py )
			 * @brief sets the y-coordinate
			 * @param[in] py y-coordinate
			 */
			void y( const int py );

			/**
			 * @fn int y()
			 * @brief gets y-coordinate
			 * @return y-coordinate
			 */
			int y();
		};
	}
}


#endif		// __KOME_EVT_MOVE_EVENT_H__
