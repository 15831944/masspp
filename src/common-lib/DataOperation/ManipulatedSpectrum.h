/**
 * @file ManipulatedSpectrum.h
 * @brief Manipulated Spectrum class
 *
 * @author M.Izumi
 * @date 2012.11.12
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME__OPERATION_MANIPULATED_SPECTRUM_H__
#define __KOME__OPERATION_MANIPULATED_SPECTRUM_H__

namespace kome {
	namespace operation {

		/**
		* @class ManipulatedSpectrum
		* @brief ManipulatedSpectrum class
		*/
		class DATA_OPERATION_CLASS ManipulatedSpectrum : public kome::objects::Spectrum {
		public:
			/**
			 * @fn ManipulatedSpectrum( ManipulatedSample* sample, kome::objects::Spectrum& org, const bool op = false )
			 * @brief constructor
			 * @param sample
			 * @param org
			 * @param op
			 */
			ManipulatedSpectrum( ManipulatedSample* sample, kome::objects::Spectrum& org, const bool op = false );

			/**
			 * @fn virtual ~ManipulatedSpectrum()
			 * @brief destructor
			 */
			virtual ~ManipulatedSpectrum();

		protected:
			/** original spectrum */
			kome::objects::Spectrum& m_org;

		protected:
			/** sample object */
			ManipulatedSample* m_sample;

		public:
			/**
			 * @fn void setSample( ManipulatedSample* sample )
			 * @brief set sample object
			 * @param sample
			 */
			void setSample( ManipulatedSample* sample ){ m_sample = sample; }

			/**
			 * @fn ManipulatedSample* getSample()
			 * @brief get sample object
			 * @return sample
			 */
			ManipulatedSample* getSample(){ return m_sample; }
		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX )
			 * @brief This method is called by getXYData method. (override method)
			 * @param[out] xyData the object to store data points
			 * @param[in] minX minimum x value. (If minX is negative number, minimum x value is not unbounded.)
			 * @param[in] maxX maximum x value. (If maxX is negative number, maximum x value is not unbounded.)
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX );
			
			/**
			 * @fn virtual void onGetXRange( double* minX, double* maxX )
			 * @brief This method is called by getMinX or getMaxX method. (override method)
			 * @param[out] minX the pointer to store minimum x value
			 * @param[out] maxX the pointer to store maximum x value
			 */
			virtual void onGetXRange( double* minX, double* maxX );

			/**
			 * @fn virtual double onGetTotalIntensity( const double minX, const double maxX )
			 * @brief This method is called by getTotalIntensity method. (override method)
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return total intensity
			 */
			virtual double onGetTotalIntensity( const double minX, const double maxX );

			/**
			 * @fn virtual double onGetMaxIntensity( const double minX, const double maxX )
			 * @brief This method is called by getMaxIntensity method. (override method)
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return max intensity
			 */
			virtual double onGetMaxIntensity( const double minX, const double maxX );

			// >>>>>> @Date:2013/09/02	<Add>	A.Ozaki
			//
			/**
			 * @fn virtual  void onSetRequestLoadData( void )
			 * @brief This method is called by setRequestLoadData method. (abstract method)
			 */
			virtual void	onSetRequestLoadData( void );
			/**
			 * @fn virtual  void onResetRequestLoadData( void )
			 * @brief This method is called by resetRequestLoadData method. (abstract method)
			 */
			virtual void	onResetRequestLoadData( void );
			
			/**
			 * @fn virtual  bool onIsRequestLoadData( void )
			 * @brief This method is called by isRequestLoadData method. (abstract method)
			 * @return If true, file read request is valid.
			 */
			virtual bool	onIsRequestLoadData( void );

			/**
			 * @fn virtual  void onSetFirstAccess( void )
			 * @brief This method is called by setFirstAccess method. (abstract method)
			 */
			virtual void	onSetFirstAccess( void );

			/**
			 * @fn virtual  void onResetFirstAccess( void )
			 * @brief This method is called by resetFirstAccess method. (abstract method)
			 */
			virtual void	onResetFirstAccess( void );
			
			/**
			 * @fn virtual  bool onIsFirstAccess( void )
			 * @brief This method is called by isFirstAccess method. (abstract method)
			 * @return If true, the first accessing.
			 */
			virtual bool	onIsFirstAccess( void );
			
			/**
			 * @fn virtual  bool onLoadData( void )
			 * @brief This method is called by loadData method. (abstract method)
			 * @return If true, file reading success.
			 */
			virtual bool	onLoadData( void );
			//
			// <<<<<< @Date:2013/09/02	<Add>	A.Ozaki

		};
	}
}

#endif // __KOME__OPERATION_MANIPULATED_SPECTRUM_H__
