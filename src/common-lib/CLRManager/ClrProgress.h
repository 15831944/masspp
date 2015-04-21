/**
 * @file ClrProgress.h
 * @brief interfaces of ClrProgress class
 *
 * @author S.Tanaka
 * @date 2006.11.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_PROGRESS_H__
#define __KOME_CLR_PROGRESS_H__

namespace kome {
	namespace clr {

		ref class ClrProgressBase;
		
		/**
		 * @class ClrProgress
		 * @brief progress object that has managed class object
		 */
		class ClrProgress : public kome::core::Progress {
		public:
			/**
			 * @fn ClrProgress()
			 * @brief constructor
			 */
			ClrProgress();

			/**
			 * @fn virtual ~ClrProgress()
			 * @brief destructor
			 */
			virtual ~ClrProgress();

		protected:
			/** base progress object */
			gcroot< ClrProgressBase^ > m_baseProgress;

		public:
			/**
			 * @fn void setBaseProgress( ClrProgressBase^ baseProgress )
			 * @brief sets base progress object
			 * @param[in] baseProgress base progress object
			 */
			void setBaseProgress( ClrProgressBase^ baseProgress );

			/**
			 * @fn ClrProgressBase^ getBaseProgress()
			 * @brief gets base progress object
			 * @return base progress object
			 */
			ClrProgressBase^ getBaseProgress();

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
			 * @fn virtual void onSetStatus( const char* status, const bool bForced )
			 * @brief This method is called by setStatus method. (override method)
			 * @param[in] status status
			 * @param[in] bForced flag for drawing forced
			 */
			virtual void onSetStatus( const char* status, const bool bForced );		// @Date:2013/11/25	<Add>	A.Ozaki

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

#endif	// __KOME_CLR_PROGRESS_H__
