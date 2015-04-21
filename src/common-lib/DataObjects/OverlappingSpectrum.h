/**
 * @file OverlappingSpectrum.h
 * @brief interfaces of OverlappingSpectrum class
 *
 * @author S.Tanaka
 * @date 2008.11.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_OVERLAPPING_SPECTRUM_H__
#define __KOME_OBJECTS_OVERLAPPING_SPECTRUM_H__


#include "Spectrum.h"


namespace kome {
	namespace objects {

		/**
		 * @class OverlappingSpectrum
		 * @brief dummy spectrum class for overlapping draw
		 */
		class DATA_OBJECTS_CLASS OverlappingSpectrum : public Spectrum {
		public:
			/**
			 * @fn OverlappingSpectrum()
			 * @brief constructor
			 */
			OverlappingSpectrum();

			/**
			 * @fn virtual ~OverlappingSpectrum()
			 * @brief destructor
			 */
			virtual ~OverlappingSpectrum();

		protected:
			/** spectra */
			std::vector< Spectrum* > m_spectra;

			/** minimum value of  X */
			std::vector< double >	m_dMinX;
			/** maximum value of Y */
			std::vector< double >	m_dMaxX;
			/** minimum value of Y */
			std::vector< double >	m_dMinY;
			/** maximum value of Y */
			std::vector< double >	m_dMaxY;

			bool	*m_bFlags;
			double	*m_dInts;
			int		m_nAllocLen;
			
		public:
			/**
			 * @fn void addSpectrum( Spectrum* spec )
			 * @brief adds spectrum
			 * @param[in] spec spectrum object to be added
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
			 * @param[in] spec spectrum object to be removed
			 */
			void removeSpectrum( Spectrum* spec );

		protected:
			/**
			 * @fn int searchSpectrum( Spectrum* spec )
			 * @brief searches spectrum
			 * @param[in] spec spectrum object to be searched
			 * @return spectrum index (If negative value, specified spectrum is not found.)
			 */
			int searchSpectrum( Spectrum* spec );

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
		};
	}
}

#endif	// __KOME_OBJECTS_OVERLAPPING_SPECTRUM_H__
