/**
 * @file MascotPmfForLCMSSearchEngine.h
 * @brief interfaces of MascotPmfForLCMSSearchEngine class
 *
 * @author S.Tanaka
 * @date 2012.08.09
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_MASCOT_PMF_FOR_LCMS_SEARCH_ENGINE_H__
#define __KOME_MASCOT_PMF_FOR_LCMS_SEARCH_ENGINE_H__


#include "MascotSearchEngineBase.h"


namespace kome {
	namespace mascot {

		/**
		 * @fn MascotPmfForLCMSSearchEngine
		 * @brief mascot PMF search engine class
		 */
		class MascotPmfForLCMSSearchEngine : public MascotSearchEngineBase {
		public:
			/**
			 * @fn MascotPmfForLCMSSearchEngine()
			 * @brief constructor
			 */
			MascotPmfForLCMSSearchEngine();

			/**
			 * @fn virtual ~MascotPmfForLCMSSearchEngine()
			 * @brief destructor
			 */
			virtual ~MascotPmfForLCMSSearchEngine();

		protected:
			/**
			 * @struct PeakInfo
			 * @brief peak information
			 */
			struct PeakInfo {
				double x;
				double y;
				double maxY;
				kome::ident::SpecInfo* spec;
			};

			/**
			 * @typedef Tolerance Unit
			 * @brief tolerance unit
			 */
			typedef enum {
				TOL_DA,
				TOL_PPM
			} ToleranceUnit;

		protected:
			/** peaks folder */
			std::string m_peaksDir;

			/** peak tolerance */
			double m_peakTol;

			/** tolerance unit */
			ToleranceUnit m_peakTolUnit;

			/** peaks */
			std::vector< PeakInfo > m_pmfPeaks;

			/** number of peaks */
			int m_peakNum;

			/** spectra */
			std::vector< kome::ident::SpecInfo* > m_spectra;

			/** protein */
			kome::ident::Substance* m_protein;

		protected:
			/**
			 * @fn virtual void onHit( kome::ident::Hit* hit )
			 * @brief on hit
			 * @param[in] hit hit (override method)
			 */
			virtual void onHit( kome::ident::Hit* hit );

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

		protected:
			/**
			 * @fn static bool lessPeakX( PeakInfo& peak0, PeakInfo& peak1 )
			 * @brief compares peaks by x coordinate
			 * @param[in] peak0 peak to compare
			 * @param[in] peak1 peak to be compared
			 * @return If true, peak0 sorts before peak1.
			 */
			static bool lessPeakX( PeakInfo& peak0, PeakInfo& peak1 );

			/**
			 * @fn static bool lessPeakY( PeakInfo& peak0, PeakInfo& peak1 )
			 * @brief compares peaks by y coordinate
			 * @param[in] peak0 peak to compare
			 * @param[in] peak1 peak to be compared
			 * @return If true, peak0 sorts before peak1.
			 */
			static bool lessPeakY( PeakInfo& peak0, PeakInfo& peak1 );
		};
	}
}


#endif    // __KOME_MASCOT_PMF_FOR_LCMS_SEARCH_ENGINE_H__
