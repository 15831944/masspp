/**
 * @file RawWaveformManager.h
 * @brief interfaces of RawWaveform plug-in manager
 *
 * @author M.Izumi
 * @data 2013.02.25
 *
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_VIEW_RAW_WAVEFORM_MANAGER_H__
#define __KOME_VIEW_RAW_WAVEFORM_MANAGER_H__

namespace kome {
	namespace view {
	
		class RawWaveformSpectrum;
		class RawWaveformChromatogram;

		/**
		 * @class RawWaveformManager
		 * @brief raw waveform manager
		 */
		class RawWaveformManager {
		public:
			/**
			 * @fn RawWaveformManager()
			 * @brief constructor
			 */
			RawWaveformManager();

			/**
			 * @fn virtual ~RawWaveformManager()
			 * @brief destructor
			 */
			virtual ~RawWaveformManager();

		protected:
			/** spectra */
			std::vector< RawWaveformSpectrum* > m_spectra;
			/** chromatogram */
			std::vector< RawWaveformChromatogram* > m_chromatograms;
		public:
			/**
			 * @fn bool openRawWaveformSpectrum( kome::objects::Spectrum* spec )
			 * @brief open raw waveform spectrum
			 * @param spec spectrum objects
			 * @return successfully opend
			 */
			bool openRawWaveformSpectrum( kome::objects::Spectrum* spec );
			
			/**
			 * @fn bool openRawWaveformChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief open raw waveform chromatogram
			 * @param chrom chromatogram object
			 * @return successfully opend
			 */
			bool openRawWaveformChromatogram( kome::objects::Chromatogram* chrom );	
		public:
			/**
			 * @fn static RawWaveformManager& getInstance()
			 * @brief gets instance. (This is the only object.)
			 * @return RawWaveformManager object
			 */
			static RawWaveformManager& getInstance();
		};

	}
}


#endif // __KOME_VIEW_RAW_WAVEFORM_MANAGER_H__
