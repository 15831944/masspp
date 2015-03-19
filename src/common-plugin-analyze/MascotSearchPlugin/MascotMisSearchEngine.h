/**
 * @file MascotMisSearchEngine.h
 * @brief interfaces of MascotMisSearchEngine class
 *
 * @author S.Tanaka
 * @date 2012.08.09
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_MASCOT_MIS_SEARCH_ENGINE_H__
#define __KOME_MASCOT_MIS_SEARCH_ENGINE_H__


#include "MascotSearchEngineBase.h"


namespace kome {
	namespace mascot {

		/**
		 * @fn MascotMisSearchEngine
		 * @brief mascot MIS search engine class
		 */
		class MascotMisSearchEngine : public MascotSearchEngineBase {
		public:
			/**
			 * @fn MascotMisSearchEngine()
			 * @brief constructor
			 */
			MascotMisSearchEngine();

			/**
			 * @fn virtual ~MascotMisSearchEngine()
			 * @brief destructor
			 */
			virtual ~MascotMisSearchEngine();

		protected:
			/** peaks folder */
			std::string m_peaksDir;

			/** peak files */
			std::vector< std::string > m_peakFiles;

			/** file descriptor */
			FILE* m_fp;

			/** peaks count */
			int m_peaksCnt;

		public:
			/**
			 * @fn virtual bool isValidSpectrum( kome::objects::Spectrum* spec )			 
			 * @brief judges the specified spectrum is valid to this search engine. (override method)
			 * @param[in] spec spectrum to be judged
			 * @return If true, the specified spectrum is valid to this search engine.
			 */
			virtual bool isValidSpectrum( kome::objects::Spectrum* spec );
			
			/**
			 * @fn virtual void onSpectrumPeaks(
						kome::objects::Spectrum* spec,
						kome::ident::SpecInfo* specInfo,
						kome::objects::Peaks* peaks,
						int* peakIds,
						const unsigned int peakIdsSize
					)
			 * @brief This method is called when peaks of a spectrum are detected. (override method)
			 * @param[in] spec spectrum 
			 * @param[in] specInfo spectrum information
			 * @param[in] peaks detected peaks
			 * @param[in] peakIds the array of peak IDs of peak matrix.
			 * @param[in] peakIdsSize the array size of peakIds
			 */
			virtual void onSpectrumPeaks(
					kome::objects::Spectrum* spec,
					kome::ident::SpecInfo* specInfo,
					kome::objects::Peaks* peaks,
					int* peakIds,
					const unsigned int peakIdsSize
			 );

			/**
			 * @fn virtual bool onFinishPeakDetection()
			 * @brief on finished peak detection (override method)
			 */
			virtual bool onFinishPeakDetection();

		protected:
			/**
			 * @fn virtual std::string onSearch(
						const char* title,
						const char* comment,
						kome::ident::SpecInfo** spectra,
						const unsigned int specNum,
						kome::objects::SettingParameterValues* settings,
						kome::core::Progress& progress
					)
			 * @brief This method is called from search method. (override method)
			 * @param[in] title title
			 * @param[in] comment commetn
			 * @param[in] spectra the array of spectra
			 * @param[in] specNum the array size of spectra
			 * @param[in] settings settings of search
			 * @param[in,out] progress the object to show the progress
			 * @return identifier of search
			 */
			virtual std::string onSearch(
				const char* title,
				const char* comment,
				kome::ident::SpecInfo** spectra,
				const unsigned int specNum,
				kome::objects::SettingParameterValues* settings,
				kome::core::Progress& progress
			);

			/**
			 * @fn virtual bool onPreparePeakDetection(
						kome::objects::SettingParameterValues* searchSettings,
						const char* peakDetector,
						kome::objects::SettingParameterValues* peakSettings,
						const char* chargeDetector,
						kome::objects::SettingParameterValues* chargeSettings,
						const char* peakFilter
					)
			 * @brief This method is called by preparePeakDetection method. (override method)
			 * @param[in] settings search parameters
			 * @param[in] settings search parameters
			 * @param[in] peakDetector peak detector algorithm
			 * @param[in] peakSettings peak detection settings
			 * @param[in] chargeDetector charge calculation algorithm
			 * @param[in] chargeSettings charge calculation settings
			 * @param[in] peakFilter peak filter
			 * @return If true, preparation of peak detection is finished successfully.
			 */
			virtual bool onPreparePeakDetection(
				kome::objects::SettingParameterValues* searchSettings,
				const char* peakDetector,
				kome::objects::SettingParameterValues* peakSettings,
				const char* chargeDetector,
				kome::objects::SettingParameterValues* chargeSettings,
				const char* peakFilter
			);

			/**
			 * @fn virtual void onEndSearch()
			 * @brief This method is called when identification is finished. (override method)
			 */
			virtual void onEndSearch();
		};
	}
}


#endif    // __KOME_MASCOT_MIS_SEARCH_ENGINE_H__
