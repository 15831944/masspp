/**
 * @file ProgressWrapper.h
 * @brief interfaces of ProgressWrapper class
 *
 * @author S.Tanaka
 * @date 2006.11.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_PROGRESS_WRAPPER_H__
#define __KOME_CLR_PROGRESS_WRAPPER_H__

namespace kome {
	namespace clr {

		ref class TimerWrapper;

		/**
		 * @class ProgressWrapper
		 * @brief progress wrapper class to use on CLR environment
		 */
		public ref class ProgressWrapper {
		public:
			/**
			 * @fn ProgressWrapper( kome::core::Progress& progress )
			 * @brief constructor
			 * @param[in] progress progress object
			 */
			ProgressWrapper( kome::core::Progress& progress );

			/**
			 * @fn virtual ~ProgressWrapper()
			 * @brief destructor
			 */
			virtual ~ProgressWrapper();

		protected:
			/** progress object */
			kome::core::Progress& m_progress;

		public:
			/**
			 * @fn kome::core::Progress& getProgress()
			 * @brief gets progress object
			 * @return progress object
			 */
			kome::core::Progress& getProgress();

		public:
			/**
			 * @fn void setRange( int start, int end )
			 * @brief sets progress range
			 * @param[in] start start position
			 * @param[in] end end position
			 */
			void setRange( int start, int end );

			/**
			 * @fn void setPosition( int pos )
			 * @brief sets progress position
			 * @param[in] pos progress position
			 */
			void setPosition( int pos );

			/**
			 * @fn void setStatus( System::String^ status )
			 * @brief sets status
			 * @param[in] status status
			 */
			void setStatus( System::String^ status );

			/**
			 * @fn bool isStopped()
			 * @brief judges whether it has to finish
			 * @return If it has to finish, this method returns true
			 */
			bool isStopped();

			/**
			 * @fn void fill()
			 * @brief sets end position
			 */
			void fill();

		public:
			/**
			 * @fn int getRelativePosition()
			 * @brief gets relative position
			 * @return relative position
			 */
			int getRelativePosition();

		public:
			/**
			 * @fn TimerWrapper^ createTimer( System::String^ name )
			 * @brief creates timer
			 * @param[in] name timer name
			 * @return created timer
			 */
			TimerWrapper^ createTimer( System::String^ name );

			/**
			 * @fn unsigned int getNumberOfTimers()
			 * @brief gets the number of timers
			 * @return the number of timers
			 */
			unsigned int getNumberOfTimers();

			/**
			 * @fn System::String^ getTimerName( unsigned int index )
			 * @brief gets timer name
			 * @param[in] index timer index
			 * @return timer name (If NULL, the index is illegal.)
			 */
			System::String^ getTimerName( unsigned int index );

			/**
			 * @fn TimerWrapper^ getTimer( unsigned int index )
			 * @brief gets timer
			 * @param[in] index timer index
			 * @return timer object
			 */
			TimerWrapper^ getTimer( unsigned int index );

		public:
			/**
			 * @fn void createSubProgresses( unsigned int num )
			 * @brief creates sub progresses
			 * @param[in] num the number of subprogresses
			 */
			void createSubProgresses( unsigned int num );

			/**
			 * @fn unsigned int getNumberOfSubProgresses()
			 * @brief gets the number of sub progresses
			 * @return the number of sub progresses
			 */
			unsigned int getNumberOfSubProgresses();

			/**
			 * @fn ProgressWrapper^ getSubProgress( unsigned int idx )
			 * @brief gets subprogress
			 * @param[in] idx sub progress index
			 * @return sub progress object (If NULL, the index is illegal.)
			 */
			ProgressWrapper^ getSubProgress( unsigned int idx );

		public:
			/**
			 * @fn static ProgressWrapper^ getIgnoringProgress()
			 * @brief gets progress object. But this object does nothing even if a method is called.
			 * @return progress object
			 */
			static ProgressWrapper^ getIgnoringProgress();
		};
	}
}

#endif	// __KOME_CLR_PROGRESS_WRAPPER_H__
