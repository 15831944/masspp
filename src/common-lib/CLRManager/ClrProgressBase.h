/**
 * @file ClrProgressBase.h
 * @brief interfaces of ClrProgressBase class
 *
 * @author S.Tanaka
 * @date 2006.11.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_PROGRESS_BASE_H__
#define __KOME_CLR_PROGRESS_BASE_H__

#include "ProgressWrapper.h"

namespace kome {
	namespace clr {
		/**
		 * @class ClrProgressBase
		 * @brief abstraction class of progress on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrProgressBase : public ProgressWrapper {
#else
		public ref class ClrProgressBase abstract : public ProgressWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrProgressBase()
			 * @brief constructor
			 */
			ClrProgressBase();

			/**
			 * @fn virtual ~ClrProgressBase()
			 * @brief destructor
			 */
			virtual ~ClrProgressBase();

		public:
			/**
			 * @fn virtual void onSetRange( int start, int end ) = 0
			 * @brief This method is called by setRange method. (abstract method)
			 * @param[in] start start position
			 * @param[in] end end position
			 */
			virtual void onSetRange( int start, int end ) = 0;

			/**
			 * @fn virtual void onSetPosition( int pos, int prevPos ) = 0
			 * @brief This method is called by setPosition method. (abstract method)
			 * @param[in] pos progress position
			 * @param[in] prevPos previous position
			 */
			virtual void onSetPosition( int pos, int prevPos ) = 0;

			/**
			 * @fn virtual void onSetStatus( System::String^ status, bool bForced ) = 0
			 * @brief This method is called by setStatus method. (abstract method)
			 * @param[in] status status
			 * @param[in] bForced flag for drawing forced
			 */
			virtual void onSetStatus( System::String^ status, bool bForced ) = 0;		// @Date:2013/11/25	<Add>	A.Ozaki

			/**
			 * @fn virtual bool onIsStopped() = 0
			 * @brief This method is called by isStopped method. (abstract method)
			 * @return If it has to finish, this method returns true
			 */
			virtual bool onIsStopped() = 0;

			// >>>>>>	@Date:2013/11/25	<Add>	A.Ozaki
			//
			/**
			 * @fn virtual void onFill( void )
			 * @brief This method is called by fill method. (abstract method)
			 */
			virtual void onFill( void ) = 0;
			//
			// <<<<<<	@Date:2013/11/25	<Add>	A.Ozaki
		};
	}
}

#endif	// __KOME_CLR_PROGRESS_BASE_H__
