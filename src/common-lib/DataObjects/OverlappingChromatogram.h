/**
 * @file OverlappingChromatogram.h
 * @brief interfaces of Overlapping Chromatogram class
 *
 * @author S.Tanaka
 * @date 2008.11.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_OVERLAPPING_CHROMATOGRAM_H__
#define __KOME_OBJECTS_OVERLAPPING_CHROMATOGRAM_H__


#include "Chromatogram.h"

#include <vector>


namespace kome {
	namespace objects {

		/**
		 * @class OverlappingChromatogram
		 * @brief dummy chromatogram class for overlapping draw
		 */
		class DATA_OBJECTS_CLASS OverlappingChromatogram : public Chromatogram {
		public:
			/**
			 * @fn OverlappingChromatogram()
			 * @brief constructor
			 */
			OverlappingChromatogram();

			/**
			 * @fn virtual ~OverlappingChromatogram()
			 * @brief destructor
			 */
			virtual ~OverlappingChromatogram();

		protected:
			/** chromatograms */
			std::vector< Chromatogram* > m_chroms;

			/** minimum value of  X */
			std::vector< double > m_dMinX;
			/** maximum value of Y */
			std::vector< double > m_dMaxX;
			/** minimum value of Y */
			std::vector< double > m_dMinY;
			/** maximum value of Y */
			std::vector< double > m_dMaxY;

			bool	*m_bFlags;
			double	*m_dInts;
			int		m_nAllocLen;

		public:
			/**
			 * @fn void clearChromatograms()
			 * @brief clears chromatograms
			 */
			void clearChromatograms();

			/**
			 * @fn void addChromatogram( Chromatogram* chrom )
			 * @brief adds chromatogram
			 * @param[in] chrom chromatogram object to be added
			 */
			void addChromatogram( Chromatogram* chrom );

			/**
			 * @fn void removeChromatogram( Chromatogram* chrom )
			 * @brief removes chromatogram
			 * @param[in] chrom chromatogram object to be removed
			 */
			void removeChromatogram( Chromatogram* chrom );

		public:
			/**
			 * @fn void update()
			 * @brief updates canvas
			 */
			void update();

		protected:
			/**
			 * @fn int searchChromatogram( Chromatogram* chrom )
			 * @brief searches chromatogram
			 * @param[in] chrom chromatogram object to be searched
			 * @return chromatogram index (If negative value, specified chromatogram is not found.)
			 */
			int searchChromatogram( Chromatogram* chrom );

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData )
			 * @brief gets chromatogram data points (override method)
			 * @param[out] xyData object to store data points
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData );

			/**
			 * @fn virtual void onGetSpectra(
					DataSet& dataSet,
					const double startRt,
					const double endRt,
					const SearchType startSearch,
					const SearchType endSearch
				)
			 * @brief This method is called by searchSpectrum or getSpectra method (override method)
			 * @param[out] dataSet data set object to store spectra
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @param[in] startSearch start search type
			 * @param[in] endSearch end search type
			 */
			virtual void onGetSpectra(
				DataSet& dataSet,
				const double startRt,
				const double endRt,
				const SearchType startSearch,
				const SearchType endSearch
			);

			/**
			 * @fn virtual double onGetMass( const unsigned int index )
			 * @brief This method is called by getMass method (override method)
			 * @param[in] index data index
			 * @return mass
			 */
			virtual double onGetMass( const unsigned int index );

			/**
			 * @fn virtual int onGetMsStage( const unsigned int index )
			 * @brief This method is called by getMsStage method (override method)
			 * @param[in] index data index
			 * @return MS stage.
			 */
			virtual int onGetMsStage( const unsigned int index );

			/**
			 * @fn virtual double onGetPrecursor( const unsigned int index )
			 * @brief This method is called by getPrecursor method (override method)
			 * @param[in] index data index
			 * @return precursor mass
			 */
			virtual double onGetPrecursor( const unsigned int index );
		};
	}
}

#endif	// __KOME_OBJECTS_OVERLAPPING_CHROMATOGRAM_H__
