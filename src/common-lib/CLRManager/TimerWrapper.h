/**
 * @file  TimerWrapper.h
 * @brief interfaces of TimerWrapper class
 *
 * @author S.Tanaka
 * @date 2008.05.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_TIMER_WRAPPER_H__
#define __KOME_CLR_TIMER_WRAPPER_H__


namespace kome {
	namespace clr {

		/**
		 * @class TimerWrapper
		 * @brief timer wrapperc lass to use on CLR environment
		 */
		public ref class TimerWrapper {
		public:
			/**
			 * @fn TimerWrapper( kome::core::Timer& timer )
			 * @brief constructor
			 * @param timer wrapped timer object
			 */
			TimerWrapper( kome::core::Timer& timer );

			/**
			 * @fn virtual ~TimerWrapper()
			 * @brief destructor
			 */
			virtual ~TimerWrapper();		

		protected:
			/** wrapped timer */
			kome::core::Timer& m_timer;

		public:
			/**  
			 * @fn kome::core::Timer& getTimer()
			 * @brief gets wrapped timer object
			 * @return wrapped timer object
			 */
			kome::core::Timer& getTimer();

		public:
			/**
			 * @fn void reset()
			 * @brief resets timer
			 */
			void reset();

			/**
			 * @fn void start()
			 * @brief starts timer
			 */
			void start();

			/**
			 * @fn double stop()
			 * @brief stops timer
			 * @return lap time
			 */
			double stop();

			/**
			 * @fn double getTotalTime()
			 * @brief gets total timer
			 * @return total time
			 */
			double getTotalTime();
		};
	}
}


#endif		// __KOME_CLR_TIMER_WRAPPER_H__

