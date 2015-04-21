/**
 * @file Progress.h
 * @brief interfaces of Progress class
 *
 * @author S.Tanaka
 * @date 2006.10.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_PROGRESS_H__
#define __KOME_CORE_PROGRESS_H__


#include "Timer.h"

#include <vector>
#include <map>
#include <utility>
#include <string>


namespace kome {
	namespace core {

		/**
		 * @class Progress
		 * @brief progress display abstract class
		 */
		class CORE_TOOLKITS_CLASS Progress {
		public:
			/**
			 * @fn Progress( const char* title = NULL )
			 * @brief constructor
			 * @param[in] title progress title
			 */
			Progress( const char* title = NULL );

			/**
			 * @fn virtual ~Progress()
			 * @brief destructor
			 */
			virtual ~Progress();

		protected:
			/** sub progresses */
			std::vector< Progress* > m_subProgresses;

			/** start position */
			int m_start;
			/** end position */
			int m_end;
			/** current position */
			int m_pos;
			/** previous position */
			int m_prevPos;
			/** relative position */
			int m_relPos;

			/** title */
			std::string m_title;

			/** main timer */
			Timer m_timer;

			/** timer array */
			std::vector< std::pair< std::string, Timer* > >* m_timers;

		public:
			/**
			 * @fn void setRange( const int start, const int end )
			 * @brief sets progress range
			 * @param[in] start start position
			 * @param[in] end end position
			 */
			void setRange( const int start, const int end );

			/**
			 * @fn void setPosition( const int pos, const bool bForced = false )
			 * @brief sets progress position
			 * @param[in] pos progress position
			 * @param[in] bForced flag of drawing forced
			 */
			void setPosition( const int pos, const bool bForced = false );		// @Date:2013/11/18	<Add>	A.Ozaki

			/**
			 * @fn void setStatus( const char* status, const bool bForced = false )
			 * @brief sets status
			 * @param[in] status status
			 * @param[in] bForced flag of drawing forced
			 */
			void setStatus( const char* status, const bool bForced = false );	// @Date:2013/11/18	<Add>	A.Ozaki

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

// >>>>>>	@Date:2013/11/18	<Add>	A.Ozaki
//
		protected:
			/** previous update time */
			long long	m_llPrevTime;

			/** status message */
			std::string	m_strStatus;
//
// <<<<<<	@Date:2013/11/18	<Add>	A.Ozaki

		public:
			/**
			 * @fn int getRelativePosition()
			 * @brief gets relative position
			 * @return relative position
			 */
			int getRelativePosition();

		public:
			/**
			 * @fn Timer* createTimer( const char* name )
			 * @brief creates timer
			 * @param[in] name timer name
			 * @return created timer
			 */
			Timer* createTimer( const char* name );

			/**
			 * @fn unsigned int getNumberOfTimers()
			 * @brief gets the number of timers
			 * @return the number of timers
			 */
			unsigned int getNumberOfTimers();

			/**
			 * @fn const char* getTimerName( const unsigned int index )
			 * @brief gets timer name
			 * @param[in] index timer index
			 * @return timer name (If NULL, the index is illegal.)
			 */
			const char* getTimerName( const unsigned int index );

			/**
			 * @fn Timer* getTimer( const unsigned int index )
			 * @brief gets timer
			 * @param[in] index timer index
			 * @return timer object
			 */
			Timer* getTimer( const unsigned int index );

		public:
			/**
			 * @fn void createSubProgresses( const unsigned int num )
			 * @brief creates sub progresses
			 * @param[in] num the number of subprogresses
			 */
			void createSubProgresses( const unsigned int num );

			/**
			 * @fn unsigned int getNumberOfSubProgresses()
			 * @brief gets the number of sub progresses
			 * @return the number of sub progresses
			 */
			unsigned int getNumberOfSubProgresses();

			/**
			 * @fn Progress* getSubProgress( const unsigned int idx )
			 * @brief gets subprogress
			 * @param[in] idx sub progress index
			 * @return sub progress object (If NULL, the index is illegal.)
			 */
			Progress* getSubProgress( const unsigned int idx );

		public:
			/**
			 * @fn static Progress& getIgnoringProgress()
			 * @brief gets progress object. But this object does nothing even if a method is called.
			 * @return progress object
			 */
			static Progress& getIgnoringProgress();

		protected:
			/**
			 * @fn virtual void onSetRange( const int start, const int end ) = 0
			 * @brief This method is called by setRange method. (abstract method)
			 * @param[in] start start position
			 * @param[in] end end position
			 */
			virtual void onSetRange( const int start, const int end ) = 0;

			/**
			 * @fn virtual void onSetPosition( const int pos, const int prevPos ) = 0
			 * @brief This method is called by setPosition method. (abstract method)
			 * @param[in] pos progress position
			 * @param[in] prevPos previous position
			 */
			virtual void onSetPosition( const int pos, const int prevPos ) = 0;

			/**
			 * @fn virtual void onSetStatus( const char* status, const bool bForced ) = 0
			 * @brief This method is called by setStatus method. (abstract method)
			 * @param[in] status status
			 * @param[in] bForced flag for drawing forced
			 */
			virtual void onSetStatus( const char* status, const bool bForced ) = 0;		// @Date:2013/11/25	<Add>	A.Ozaki

			/**
			 * @fn virtual bool onIsStopped() = 0
			 * @brief This method is called by isStopped method. (abstract method)
			 * @return If it has to finish, this method returns true
			 */
			virtual bool onIsStopped() = 0;		   

			// >>>>>>	@Date:2013/11/25	<Add>	A.Ozaki
			//
			/**
			 * @fn virtual bool onFill( void ) = 0
			 * @brief This method is called by fill method. (abstract method)
			 */
			virtual void onFill( void ) = 0;
			//
			// <<<<<<	@Date:2013/11/25	<Add>	A.Ozaki
		};

		/**
		 * @class IgnoringProgress
		 * @brief Progress object.
				  Even if a method is called, it disgards it. 
		 */
		class IgnoringProgress : public Progress {
		public:
			/**
			 * @fn IgnoringProgress()
			 * @brief constructor
			 */
			IgnoringProgress();

			/**
			 * @fn virtual ~IgnoringProgress()
			 * @brief destructor
			 */
			virtual ~IgnoringProgress();

		protected:
			/**
			 * @fn virtual void onSetRange( const int, const int )
			 * @brief This method is called by setRange method. (override method)
			 */
			virtual void onSetRange( const int, const int );


			/**
			 * @fn virtual void onSetPosition( const int pos, const int prevPos )
			 * @brief This method is called by setPosition method. (override method)
			 */
			virtual void onSetPosition( const int , const int );

			/**
			 * @fn virtual void onSetStatus( const char*, const bool bForced = false )
			 * @brief This method is called by setStatus method. (override method)
			 * @param[in] bForced flag for drawing forced
			 */
			virtual void onSetStatus( const char*, const bool bForced = false );	// @Date:2013/11/25	<Add>	A.Ozaki

			/**
			 * @fn virtual bool onIsStopped()
			 * @brief This method is called by isStopped method. (override method)
			 * @return If it has to finish, this method returns true
			 */
			virtual bool onIsStopped();

			// >>>>>>	@Date:2013/11/25	<Add>	A.Ozaki
			//
			/**
			 * @fn virtual bool onFill( void )
			 * @brief This method is called by fill method. (override method)
			 */
			virtual void onFill( void );
			//
			// <<<<<<	@Date:2013/11/25	<Add>	A.Ozaki
		};

		/**
		 * @class SubProgress
		 * @brief sub progress object.
		 */
		class SubProgress : public Progress {
		public:
			/**
			 * @fn SubProgress( Progress& parent )
			 * @brief constructor
			 * @param parent parent progress
			 */
			SubProgress( Progress& parent );

			/**
			 * @fn virtual ~SubProgress()
			 * @brief destructor
			 */
			virtual ~SubProgress();

		protected:
			/** parent progress */
			Progress* m_parent;

		protected:
			/**
			 * @fn virtual void onSetRange( const int start, const int end )
			 * @brief This method is called by setRange method. (override method)
			 * @param start start position
			 * @param end end position
			 */
			virtual void onSetRange( const int start, const int end );


			/**
			 * @fn virtual void onSetPosition( const int pos, const int prevPos )
			 * @brief This method is called by setPosition method. (override method)
			 * @param pos progress position
			 * @param prevPos previous position
			 */
			virtual void onSetPosition( const int pos, const int prevPos );

			/**
			 * @fn virtual void onSetStatus( const char* status, const bool bForced = false )
			 * @brief This method is called by setStatus method. (override method)
			 * @param status status
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

#endif	//  __KOME_CORE_PROGRESS_H__
