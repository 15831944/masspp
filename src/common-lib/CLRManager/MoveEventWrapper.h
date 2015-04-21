/**
 * @file MoveEventWrapper.h
 * @brief interfaces of MoveEventWrapper class
 *
 * @author S.Tanaka
 * @date 2012.02.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_MOVE_EVENT_WRAPPER__
#define __KOME_CLR_MOVE_EVENT_WRAPPER__


namespace kome {
	namespace clr {

		/**
		 * @class MoveEventWrapper
		 * @brief MoveEvent wrapper class to use on CLR environment
		 */
		public ref class MoveEventWrapper {
		public:
			/**
			 * @fn MoveEventWrapper( kome::evt::MoveEvent& evt )
			 * @brief constructor
			 * @param[in] evt mouse event object to be wrapped
			 */
			MoveEventWrapper( kome::evt::MoveEvent& evt );

			/**
			 * @fn virtual ~MoveEventWrapper()
			 * @brief destructor
			 */
			virtual ~MoveEventWrapper();

		protected:
			/** mouse event object */
			kome::evt::MoveEvent& m_evt;

		public:
			/**
			 * @fn kome::evt::MoveEvent& getMoveEvent()
			 * @brief gets mouse event object
			 * @return mouse event object
			 */
			kome::evt::MoveEvent& getMoveEvent();

		public:
			/**
			 * @fn void x( int px )
			 * @brief sets the x-coordinate
			 * @param[in] px x-coordinate
			 */
			void x( int px );

			/**
			 * @fn int x()
			 * @brief gets x-coordinate
			 * @return x-coordinate
			 */
			int x();

			/**
			 * @fn void y( int py )
			 * @brief sets the y-coordinate
			 * @param[in] py y-coordinate
			 */
			void y( int py );

			/**
			 * @fn int y()
			 * @brief gets y-coordinate
			 * @return y-coordinate
			 */
			int y();
		};
	}
}


#endif		// __KOME_CLR_MOVE_EVENT_WRAPPER__
