/**
 * @file ClrSpectrum.h
 * @brief interfaces of ClrSpectrum class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SPECTRUM_H__
#define __KOME_CLR_SPECTRUM_H__


#include "ClrSpectrumBase.h"


namespace kome {
	namespace clr {

		/**
		 * @class ClrSpectrum
		 * @brief Spectrum class that has managed class object
		 */
		class ClrSpectrum : public kome::objects::Spectrum {
		public:
			/**
			 * @fn ClrSpectrum( SampleWrapper^ sample, System::String^ name )
			 * @brief constructor
			 * @param[in] sample sample object that has this spectrum
			 * @param[in] name spectrum name
			 */
			ClrSpectrum( SampleWrapper^ sample, System::String^ name );

			/**
			 * @fn virtual ~ClrSpectrum()
			 * @brief destructor
			 */
			virtual ~ClrSpectrum();

		protected:
			/** base spectrum object */
			gcroot< ClrSpectrumBase^ > m_baseSpectrum;

		public:
			/**
			 * @fn void setBaseSpectrum( ClrSpectrumBase^ baseSpectrum )
			 * @brief sets base spectrum object
			 * @param[in] baseSpectrum base spectrum object
			 */
			void setBaseSpectrum( ClrSpectrumBase^ baseSpectrum );

			/**
			 * @fn ClrSpectrumBase^ getBaseSpectrum()
			 * @brief gets base spectrum object
			 * @return base spectrum object
			 */
			ClrSpectrumBase^ getBaseSpectrum();

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData, double const minX, double const maxX )
			 * @brief This method is called by getXYData method. (override method)
			 * @param[in,out] xyData the object to store data points
			 * @param[in] minX minimum x value. (If minX is negative number, minimum x value is not unbounded.)
			 * @param[in] maxX maximum x value. (If maxX is negative number, maximum x value is not unbounded.)
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData, double const minX, double const maxX );

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

// >>>>>>	@Date:2013/07/18	<Add>	A.Ozaki
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
// <<<<<<	@Date:2013/07/18	<Add>	A.Ozaki
		};
	}
}

#endif	// __KOME_CLR_SPECTRUM_H__
