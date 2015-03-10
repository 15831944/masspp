/**
 * @file AIASpectrum.h
 * @brief interface of AIASpectrum class
 *
 * @author Y.Fujita
 * @date 2012.07.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IO_AIA_SPECTRUM_H__
#define __KOME_IO_AIA_SPECTRUM_H__


namespace kome {
	namespace io {
		namespace aia {

			class AIASample;

			/**
			 * @class AIASpectrum
			 * @brief AIA sample spectrum class
			 */
			class AIASpectrum : public kome::objects::Spectrum {
			public:
				/**
				 * @fn AIASpectrum( AIASample* sample, int scanNumber )
				 * @brief constructor
				 * @param sample AIA sample object
				 */
				AIASpectrum( AIASample* sample, int scanNumber );

				/**
				 * @fn virtual ~AIASpectrum()
				 * @brief destructor
				 */
				virtual ~AIASpectrum();

			protected:
				/** sample */
				AIASample* m_aiaSample;

				/** index */
				long m_index;

				/** number of points */
				long m_num;

			public:
				/**
				 * @fn void setXYInfo( long index, long num )
				 * @brief sets xy data information
				 * @param index point index
				 * @param num point number
				 */
				void setXYInfo( long index, long num );

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
}

#endif		// __KOME_IO_AIA_SPECTRUM_H__
