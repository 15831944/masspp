/**
 * @file AlignedChromatogram.h
 * @brief interfaces of AlignedChromatogram class
 *
 * @author S.Tanaka
 * @date 2009.02.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_ALIGNED_CHROMATOGRAM_H__
#define __KOME_OPERATION_ALIGNED_CHROMATOGRAM_H__


#include "Alignment.h"


namespace kome {
	namespace operation {

		/**
		 * @class AlignedChromatogram
		 * @brief aligned chromatogram class
		 */
		class DATA_OPERATION_CLASS AlignedChromatogram : public kome::objects::Chromatogram {
		public:
			/**
			 * @fn AlignedChromatogram( kome::objects::Chromatogram& org, Alignment* align )
			 * @brief constructor
			 * @param[in] org original chromatogram
			 * @param[in] align alignment
			 */
			AlignedChromatogram( kome::objects::Chromatogram& org, Alignment* align );

			/**
			 * @fn virtual ~AlignedChromatogram()
			 * @brief destructor
			 */
			virtual ~AlignedChromatogram();

		protected:
			/** original chromatogram */
			kome::objects::Chromatogram& m_org;

			/** alignment */
			Alignment* m_align;

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData )
			 * @brief This method is called by getXYData method (override method)
			 * @param[out] xyData object to store data points
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData );

			/**
			 * @fn virtual void onGetSpectra(
					kome::objects::DataSet& spectra,
					const double startRt,
					const double endRt,
					const kome::objects::SearchType startSearch,
					const kome::objects::SearchType endSearch
				)
			 * @brief This method is called by searchSpectrum or getDataSet method (overriede method)
			 * @param[out] spectra spectra object to store spectra
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @param[in] startSearch start search type
			 * @param[in] endSearch end search type
			 */
			virtual void onGetSpectra(
				kome::objects::DataSet& spectra,
				const double startRt,
				const double endRt,
				const kome::objects::SearchType startSearch,
				const kome::objects::SearchType endSearch
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


#endif	// __KOME_OPERATION_ALIGNED_CHROMATOGRAM_H__
