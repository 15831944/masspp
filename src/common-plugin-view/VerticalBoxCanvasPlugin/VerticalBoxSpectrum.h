/**
 * @file VerticalBoxSpectrum.h
 * @brief VerticalBoxSpectrum.cpp—pƒwƒbƒ_
 * @author OKADA, H.
 * @date 2010.11
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 * 
 */


#ifndef _VERTICALBOXSPECTRUM_H_
#define _VERTICALBOXSPECTRUM_H_

namespace kome {
	namespace objects {

		/**
		 * @class VerticalBoxSpectrum
		 * @brief dummy spectrum class for VerticalBox draw
		 */
		class VerticalBoxSpectrum : public kome::objects::Spectrum {
		public:
			/**
			 * @fn VerticalBoxSpectrum()
			 * @brief constructor
			 */
			VerticalBoxSpectrum();

			/**
			 * @fn virtual ~VerticalBoxSpectrum()
			 * @brief destructor
			 */
			virtual ~VerticalBoxSpectrum();

		protected:
			/** spectra */
			std::vector< Spectrum* > m_spectra;

		public:
			/**
			 * @fn void addSpectrum( Spectrum* spec )
			 * @brief adds spectrum
			 * @param spec spectrum object to be added
			 */
			void addSpectrum( Spectrum* spec );

			/**
			 * @fn void clearSpectra()
			 * @brief clears spectra
			 */
			void clearSpectra();

		   /**			 
			 * @fn void removeSpectrum( Spectrum* spec )
			 * @brief removes spectrum
			 * @param spec spectrum object to be removed
			 */
			void removeSpectrum( Spectrum* spec );

		protected:
			/**
			 * @fn int searchSpectrum( Spectrum* spec )
			 * @brief searches spectrum
			 * @param spec spectrum object to be searched
			 * @return spectrum index (If negative value, specified spectrum is not found.)
			 */
			int searchSpectrum( Spectrum* spec );

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX )
			 * @brief This method is called by getXYData method. (override method)
			 * @param xyData the object to store data points
			 * @param minX minimum x value. (If minX is negative number, minimum x value is not unbounded.)
			 * @param maxX maximum x value. (If maxX is negative number, maximum x value is not unbounded.)
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX );

			/**
			 * @fn virtual void onGetXRange( double* minX, double* maxX )
			 * @brief This method is called by getMinX or getMaxX method. (override method)
			 * @param minX the pointer to store minimum x value
			 * @param maxX the pointer to store maximum x value
			 */
			virtual void onGetXRange( double* minX, double* maxX );

			/**
			 * @fn virtual double onGetTotalIntensity( const double minX, const double maxX )
			 * @brief This method is called by getTotalIntensity method. (override method)
			 * @param minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return total intensity
			 */
			virtual double onGetTotalIntensity( const double minX, const double maxX );

			/**
			 * @fn virtual double onGetMaxIntensity( const double minX, const double maxX )
			 * @brief This method is called by getMaxIntensity method. (override method)
			 * @param minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return max intensity
			 */
			virtual double onGetMaxIntensity( const double minX, const double maxX );

			// >>>>>> @Date:2013/09/05	<Add>	A.Ozaki
			//
			/**
			 * @fn virtual  bool onSetRequestLoadData( void )
			 * @brief This method is called by setRequestLoadData method. (abstract method)
			 */
			virtual void	onSetRequestLoadData( void );
			/**
			 * @fn virtual  bool onResetRequestLoadData( void )
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
			 * @fn virtual  bool onSetFirstAccess( void )
			 * @brief This method is called by setFirstAccess method. (abstract method)
			 */
			virtual void	onSetFirstAccess( void );

			/**
			 * @fn virtual  bool onResetFirstAccess( void )
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
			// <<<<<< @Date:2013/09/05	<Add>	A.Ozaki
		};
	}
}

#endif
