/**
 * @file RawWaveformChromatogram.h
 * @brief interfaces of RawWaveformChromatogram class
 * 
 * @author M.Izumi
 * @date 2013.02.26
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved. 
 */

#ifndef __KOME_VIEW_RAW_WAVEFORM_CHROMATOGRAM_H__
#define __KOME_VIEW_RAW_WAVEFORM_CHROMATOGRAM_H__

namespace kome {
	namespace view {
		/**
		 * @class RawWaveformChromatogram
		 * @brief raw waveform chromatogram class
		 */
		class RawWaveformChromatogram : public kome::objects::Chromatogram {
		public:
			/**
			 * @fn RawWaveformChromatogram( kome::objects::Chromatogram& chrom )
			 * @brief constructor
			 * @param chrom
			 */
			RawWaveformChromatogram( kome::objects::Chromatogram& chrom );
			
			/**
			 * @fn virtual ~RawWaveformChromatogram()
			 * @brief destructor
			 */

			virtual ~RawWaveformChromatogram();

		protected:
			/** chromatogram */
			kome::objects::Chromatogram& m_orgChrom;
		public:
		
		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData )
			 * @brief This method is called by getXYData method (override method)
			 * @param xyData object to store data points
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData );

			/**
			 * @fn virtual void onGetSpectra(
				kome::objects::DataSet& dataSet,
				const double startRt,
				const double endRt,
				const kome::objects::SearchType startSearch,
				const kome::objects::SearchType endSearc
			 )
			 * @brief This method is called by searchSpectrum or getSpectra method (overriede method)
			 * @param spectra spectra object to store spectra
			 * @param startRt start RT
			 * @param endRt end RT
			 * @param startSearch start search type
			 * @param endSearch end search type
			 */
			virtual void onGetSpectra(
				kome::objects::DataSet& dataSet,
				const double startRt,
				const double endRt,
				const kome::objects::SearchType startSearch,
				const kome::objects::SearchType endSearch
			);

			/**
			 * @fn virtual double onGetMass( const unsigned int index )
			 * @brief This method is called by getMass method (override method)
			 * @param index data index
			 * @return mass
			 */
			virtual double onGetMass( const unsigned int index );

			/**
			 * @fn virtual int onGetMsStage( const unsigned int index )
			 * @brief This method is called by getMsStage method (override method)
			 * @param index data index
			 * @return MS stage.
			 */
			virtual int onGetMsStage( const unsigned int index );

			/**
			 * @fn virtual double onGetPrecursor( const unsigned int index )
			 * @brief This method is called by getPrecursor method (override method)
			 * @param index data index
			 * @return precursor mass
			 */
			virtual double onGetPrecursor( const unsigned int index );
		};
	}
}

#endif	// __KOME_VIEW_RAW_WAVEFORM_CHROMATOGRAM_H__
