/**
 * @file AveragedSpectrum.h
 * @brief interfaces of AveragedSpectrum class
 *
 * @author S.Tanaka
 * @date 2006.10.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_AVERAGED_SPECTRUM_H__
#define __KOME_OBJECTS_AVERAGED_SPECTRUM_H__

#include "Spectrum.h"

#include <vector>


namespace kome {
	namespace objects {

		class DataGroupNode;

		/**
		 * @class AveragedSpectrum
		 * @brief averaged spectrum class
		 */
		class DATA_OBJECTS_CLASS AveragedSpectrum : public Spectrum {
		public:
			/**
			 * @fn AveragedSpectrum( DataGroupNode* group )
			 * @brief constructor
			 * @param[in] group spectrum group object that has this spectrum
			 */
			AveragedSpectrum( DataGroupNode* group );

			/**
			 * @fn virtual ~AveragedSpectrum()
			 * @brief destructor
			 */
			virtual ~AveragedSpectrum();

		protected:
			/** the array of spectra */
			std::vector< Spectrum* > m_spectra;

			/** merge distance */
			double m_d;

		public:
			/**
			 * @fn void addSpectrum( Spectrum* spectrum )
			 * @brief add spectrum to array
			 * @param[in] spectrum spectrum object to be added
			 */
			void addSpectrum( Spectrum* spectrum );

			/**
			 * @fn unsigned int getNumberOfSpectra()
			 * @brief gets the number of spectra
			 * @return the number of spectra
			 */
			unsigned int getNumberOfSpectra();

			/**
			 * @fn Spectrum* getSpectrum( const unsigned int idx )
			 * @brief gets spectrum
			 * @param[in] idx spectrum index
			 * @return spectrum (If NULL, the index is illegal.)
			 */
			Spectrum* getSpectrum( const unsigned int idx );

		public:
			/**
			 * @fn void setMergeDistance( const double d )
			 * @brief sets the merge disntace
			 * @param[in] d merge disntance
			 */
			void setMergeDistance( const double d );

			/**
			 * @fn double getMergeDistance()
			 * @brief gets the merge distance
			 * @return merge distance
			 */ 
			double getMergeDistance();

		protected:
			/**
			 * @fn void updateTitle()
			 * @brief update title
			 */
			void updateTitle();

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

		public:
			/**
			 * @fn static void getAveragedDataPoints( kome::objects::Spectrum** spectra, const unsigned int num, kome::core::XYData& xyData )
			 * @brief gets the averaged data points
			 * @param[in] spectra the array of spectra
			 * @param[in] num the spectrum array size
			 * @param[out] xyData the object to store data points.
			 */
			static void getAveragedDataPoints( kome::objects::Spectrum** spectra, const unsigned int num, kome::core::XYData& xyData );

			/**
			 * @fn static void mergeDataPoints( kome::core::XYData& src0, kome::core::XYData& src1, kome::core::XYData& dst )
			 * @brief merges data points
			 * @param[in] src0 source data points
			 * @param[in] src1 source data points
			 * @param[out] dst the object to store merged data points
			 */
			static void mergeDataPoints( kome::core::XYData& src0, kome::core::XYData& src1, kome::core::XYData& dst );
		};
	}
}

#endif	//  __KOME_OBJECTS_AVERAGED_SPECTRUM_H__
