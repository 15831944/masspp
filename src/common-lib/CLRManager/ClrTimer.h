/**
 * @file  ClrTimer.h
 * @brief interfaces of ClrTimer class
 *
 * @author S.Tanaka
 * @date 2008.05.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_TIMER_H__
#define __KOME_CLR_TIMER_H__


#include "TimerWrapper.h"


namespace kome {
	namespace clr {

		/**
		 * @class ClrTimer
		 * @brief timer class
		 */
		public ref class ClrTimer : public TimerWrapper {
		public:
			/**
			 * @fn ClrTimer()
			 * @brief constructor
			 */
			ClrTimer();

			/**
			 * @fn virtual ~ClrTimer()
			 * @brief destructor
			 */
			virtual ~ClrTimer();		
		};
	}
}


#endif		// __KOME_CLR_TIMER_H__

