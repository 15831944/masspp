/**
 * @file  Timer.h
 * @brief interfaces of Timer class
 *
 * @author S.Tanaka
 * @date 2008.05.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_TIMER_H__
#define __KOME_CORE_TIMER_H__


#include <ctime>


namespace kome {
	namespace core {

		/**
		 * @class Timer
		 * @brief timer class
		 */
		class CORE_TOOLKITS_CLASS Timer {
		public:
			/**
			 * @fn Timer()
			 * @brief constructor
			 */
			Timer();

			/**
			 * @fn virtual ~Timer()
			 * @brief destructor
			 */
			virtual ~Timer();		

		protected:
			/** start time */
			clock_t m_start;

			/** total time */
			double m_total;

			/** stopped */
			bool m_stopped;

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
			 * @fn double getTime()
			 * @brief gets time
			 * @return time
			 */
			double getTime();

			/**
			 * @fn double getTotalTime()
			 * @brief gets total time
			 * @return total time
			 */
			double getTotalTime();
		};
	}
}


#endif		// __KOME_CORE_TIMER_H__

