/**
 * @file MsbSpectrum.h
 * @brief interfaces of MsbSpectrum class
 *
 * @author S.Tanaka
 * @date 2006.10.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MSB_SPECTRUM_H__
#define __KOME_IO_MSB_SPECTRUM_H__


#include "MsbManager.h"
#include "SpectrumPartArray.h"

#include <vector>


namespace kome {
	namespace io {
		namespace msb {

			class MsbSample;

			/**
			 * @class MsbSpectrum
			 * @brief msb sample spectrum class
			 */
			class MsbSpectrum : public kome::objects::Spectrum {
			public:
				/**
				 * @fn MsbSpectrum( MsbSample* sample, const char* name, FILE* fp )
				 * @brief constructor
				 * @param sample sample object
				 * @param name spectrum name
				 * @param fp file descriptor
				 */
				MsbSpectrum( MsbSample* sample, const char* name, FILE* fp );

				/**
				 * @fn virtual ~MsbSpectrum()
				 * @brief destructor
				 */
				virtual ~MsbSpectrum();

			protected:
				/** file descriptor */
				FILE* m_fp;

				/** spectrum parts */
				SpectrumPartArray m_parts;

			public:
				/**
				 * @ SectrumPartArray& getParts()
				 * @brief gets the spectrum part array
				 * @return spectrum part array
				 */
				SpectrumPartArray& getParts();

			public:
				/**
				 * @fn void addPart(
						const unsigned int length,
						const float start,
						const float end,
						const float totalIntensity,
						const float minIntensity,
						const float maxIntensity,
						const unsigned int msAlign,
						const unsigned int intAlign
					)
				 * @brief add spectrum part information
				 * @param length number of data points int the part
				 * @param start minimum m/z value in the part
				 * @param end maximum m/z value in the part
				 * @param totalIntensity total intensity value in the part
				 * @param minIntensity min intensity value in the part
				 * @param maxIntensity max intensity value in the part
				 * @param msAlign m/z data align
				 * @param intAlign intensity data align
				 */
				void addPart(
					const unsigned int length,
					const float start,
					const float end,
					const float totalIntensity,
					const float minIntensity,
					const float maxIntensity,
					const unsigned int msAlign,
					const unsigned int intAlign
				);

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

#endif		// __KOME_IO_MSB_SPECTRUM_H__
