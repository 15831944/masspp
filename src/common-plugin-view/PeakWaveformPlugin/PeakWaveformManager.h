/**
 * @file PeakWaveformManager.h
 * @brief interfaces of PeakWaveform plug-in manager
 *
 * @author M.Izumi
 * @data 2013.02.25
 *
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_VIEW_PEAK_WAVEFORM_MANAGER_H__
#define __KOME_VIEW_PEAK_WAVEFORM_MANAGER_H__

namespace kome {
	namespace view {
	
		class PeakWaveformSpectrum;
		class PeakWaveformChromatogram;

		/**
		 * @class PeakWaveformManager
		 * @brief peak waveform manager
		 */
		class PeakWaveformManager {
		public:
			/**
			 * @fn PeakWaveformManager()
			 * @brief constructor
			 */
			PeakWaveformManager();

			/**
			 * @fn virtual ~PeakWaveformManager()
			 * @brief destructor
			 */
			virtual ~PeakWaveformManager();

		protected:
			/** PeakWaveform spectra */
			std::vector< PeakWaveformSpectrum* > m_spectra;
			/** PeakWaveform chromatograms */
			std::vector< PeakWaveformChromatogram* > m_chromatograms;

		public:
			/**
			 * @fn bool openPeakWaveformSpectrum( kome::objects::Spectrum* spec )
			 * @brief open peak waveform spectrum
			 * @param spec spectrum object
			 * @return successfully opened
			 */
			bool openPeakWaveformSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn bool openPeakWaveformChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief open peak waveform chromatogram
			 * @param chrom chromatogram object
			 * @return successfully opend
			 */
			bool openPeakWaveformChromatogram( kome::objects::Chromatogram* chrom );
	
		public:
			/**
			 * @fn static PeakWaveformManager& getInstance()
			 * @brief gets instance. (This is the only object.)
			 * @return PeakWaveformManager object
			 */
			static PeakWaveformManager& getInstance();
		};

	}
}


#endif // __KOME_VIEW_RAW_WAVEFORM_MANAGER_H__
