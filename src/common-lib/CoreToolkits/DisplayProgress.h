/**
 * @file DisplayProgress.h
 * @brief interfaces of DisplayProgress class
 *
 * @author S.Tanaka
 * @date 2007.02.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_DISPLAY_PROGRESS_H__
#define __KOME_CORE_DISPLAY_PROGRESS_H__


#include "Progress.h"

#include <boost/progress.hpp>


namespace kome {
	namespace core {

		/**
		 * @class DisplayProgress
		 * @brief showing progress with display class
		 */
		class CORE_TOOLKITS_CLASS DisplayProgress : public Progress {
		public:
			/**
			 * @fn DisplayProgress( const char* title )
			 * @brief constructor
			 * @param[in] title progress title
			 */
			DisplayProgress( const char* title );

			/**
			 * @fn virtual ~DisplayProgress()
			 * @brief destructor
			 */
			virtual ~DisplayProgress();

		protected:
			/** progress */
			boost::progress_display* m_progress;

		protected:
			/**
			 * @fn virtual void onSetRange( const int start, const int end )
			 * @brief This method is called by setRange method. (override method)
			 * @param[in] start start position
			 * @param[in] end end position
			 */
			virtual void onSetRange( const int start, const int end );

			/**
			 * @fn virtual void onSetPosition( const int pos, const int prevPos )
			 * @brief This method is called by setPosition method. (override method)
			 * @param[in] pos progress position
			 * @param[in] prevPos previous position
			 */
			virtual void onSetPosition( const int pos, const int prevPos );

			/**
			 * @fn virtual void onSetStatus( const char* status, const bool bForced = false )
			 * @brief This method is called by setStatus method. (override method)
			 * @param[in] status status
			 * @param[in] bForced flag for drawing forced
			 */
			virtual void onSetStatus( const char* status, const bool bForced = false );		// @Date:2013/11/25	<Add>	A.Ozaki

			/**
			 * @fn virtual bool onIsStopped()
			 * @brief This method is called by isStopped method. (override method)
			 * @return If it has to finish, this method returns true
			 */
			virtual bool onIsStopped();

			// >>>>>>	@Date:2013/11/25	<Add>	A.Ozaki
			//
			/**
			 * @fn virtual void onFill( void )
			 * @brief This method is called by fill method. (override method)
			 */
			virtual void onFill( void );
			//
			// <<<<<<	@Date:2013/11/25	<Add>	A.Ozaki
		};
	}
}

#endif		// __KOME_CORE_DISPLAY_PROGRESS_H__
