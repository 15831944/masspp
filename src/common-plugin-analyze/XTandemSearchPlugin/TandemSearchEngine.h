/**
 * @file TandemSearchEngine.h
 * @brief interfaces of TandemSearchEngine class
 *
 * @author S.Tanaka
 * @date 2012.08.10
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __TANDEM_SEARCH_ENGINE_H__
#define __TANDEM_SEARCH_ENGINE_H__


namespace kome {
	namespace xtandem {

		/**
		 * @class TandemSearchEngine
		 * @brief X! Tandem search engine class
		 */
		class TandemSearchEngine : kome::ident::SearchEngine {
		public:
			/**
			 * @fn TandemSearchEngine()
			 * @brief cosntructor
			 */
			TandemSearchEngine();

			/**
			 * @fn virtual ~TandemSearchEngine()
			 * @brief destructor
			 */
			virtual ~TandemSearchEngine();

		protected:
			/** file descriptor */
			FILE* m_fp;

			/** working directory */
			std::string m_workingDir;

			/** result file */
			std::string m_resultFile;

			/** peak file path */
			std::string m_peaksPath;

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

			/**
			 * @fn virtual std::string validateSettings( kome::objects::SettingParameterValues* paramSettings )
			 * @brief validate the parameter settings (override method)
			 * @param[in] paramSettings identification parameters
			 * @return If an empty string, no validation error occurred. Otherwise, it contains a description of the error.
			 */
			virtual std::string validateSettings( kome::objects::SettingParameterValues* paramSettings );

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
			 * @fn virtual kome::ident::SearchEngine::SearchStatus onGetSearchStatus( const char* id )
			 * @brief This method is called by getSearchStatus method. (override method)
			 * @param[in] search ID
			 * @return search status
			 */
			virtual kome::ident::SearchEngine::SearchStatus onGetSearchStatus( const char* id );

			/**
			 * @fn virtual void onGetResult(
						kome::ident::SearchResult& result,
						const char* id,
						kome::ident::SpecInfo** spectra,
						const unsigned int specNum,
						kome::core::Progress& progress
					)
			 * @brief This method is called by getResult method. (override method)
			 * @parma[out] result the object to store the search results
			 * @param[in] id search ID
			 * @param[in] spectra the array of spectrum
			 * @parma[in] specNum the size of spectrum array
			 * @param[in,out] progress the object to show the progress
			 */
			virtual void onGetResult(
					kome::ident::SearchResult& result,
					const char* id,
					kome::ident::SpecInfo** spectra,
					const unsigned int specNum,
					kome::core::Progress& progress
			);

			/**
			 * @fn virtual bool onOpenResult( const char* resultLocation )
			 * @brief display the search result using the specified result location (override method)
			 * @param[in] resultLocation string that specifies where the result can be obtained
			 * @return If true, it succeeded to display the result.
			 */
			virtual bool onOpenResult( const char* resultLocation );

			/**
			 * @fn virtual bool onPrepareSearch( kome::core::Progress& progress )
			 * @brief This method is called from the prepareSearch method. (override method)
			 * @param[out] progress the object to show the progres
			 * @return If true, the preparation is finished successfully.
			 */
			virtual bool onPrepareSearch( kome::core::Progress& progress );

			/**
			 * @fn virtual void onEndSearch()
			 * @brief This method is called when identification is finished. (override method)
			 */
			virtual void onEndSearch();

			/**
			 * @fn virtual void onGetSearchInfo( kome::ident::SearchInfo& searchInfo, kome::objects::SettingParameterValues* settings )
			 * @brief gets the search information (override method)
			 * @param[out] searchInfo the object to store the search information
			 * @param[in] settings search setttings
			 */
			virtual void onGetSearchInfo( kome::ident::SearchInfo& searchInfo, kome::objects::SettingParameterValues* settings );

		public:
			/**
			 * @fn static std::string checkParams( kome::objects::SettingParameterValues* settings )
			 * @brief check parameters
			 * @param[in] settings search parameters
			 * @return error message
			 */
			static std::string checkParams( kome::objects::SettingParameterValues* settings );
		};
	}
}

#endif    // __TANDEM_SEARCH_ENGINE_H__
