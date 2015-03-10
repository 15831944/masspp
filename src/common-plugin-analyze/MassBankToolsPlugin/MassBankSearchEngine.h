/**
 * @file MassBankSearchEngine.h
 * @brief implements of MassBankSearchEngine class
 *
 * @author M.Fukuda
 * @date 2009.06.24
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank{
		class MassBankSearchEngine : public kome::ident::SearchEngine {

		public:
			MassBankSearchEngine();
			~MassBankSearchEngine();

		private:
			// menber
			std::vector<std::string> queryStringsTags_;

		public:

			/**
			 * @brief judges the specified spectrum is valid to this search engine. (abstract method)
			 * @param[in] spec spectrum to be judged
			 * @return If true, the specified spectrum is valid to this search engine.
			 */
			virtual bool isValidSpectrum(kome::objects::Spectrum* spec);
			
			/**
			 * @brief This method is called when peaks of a spectrum are detected. (abstract method)
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
			 * @brief on finished peak detection (abstract method)
			 */
			virtual bool onFinishPeakDetection();

			/**
			 * @brief validate the parameter settings (abstract method)
			 * @param[in] paramSettings identification parameters
			 * @return If an empty string, no validation error occurred. Otherwise, it contains a description of the error.
			 */
			virtual std::string validateSettings(
				kome::objects::SettingParameterValues* paramSettings);

		protected:

			/**
			 * @brief This method is called from search method. (abstract method)
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
			 * @brief This method is called by preparePeakDetection method. (abstract method)
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
			 * @brief This method is called by getSearchStatus method. (abstract method)
			 * @param[in] search ID
			 * @return search status
			 */
			virtual SearchStatus onGetSearchStatus(const char* id);

			/**
			 * @brief This method is called by getResult method. (override method)
			 * @parma[out] result the object to store the search results
			 * @param[in] id search ID
			 * @param[in,out] progress the object to show the progress
			 */
			virtual void onGetResult(
				kome::ident::SearchResult& result, 
				const char* id, 
				kome::ident::SpecInfo** spectra,
				const unsigned int specNum,
				kome::core::Progress& progress);

			/**
			 * @brief display the search result using the specified result location (abstract method)
			 * @param[in] resultLocation string that specifies where the result can be obtained
			 * @return If true, it succeeded to display the result.
			 */
			virtual bool onOpenResult(const char* resultLocation);

			/**
			 * @brief This method is called from the prepareSearch method. (abstract method)
			 * @param[out] progress the object to show the progres
			 * @return If true, the preparation is finished successfully.
			 */
			virtual bool onPrepareSearch(kome::core::Progress& progress);

			/**
			 * @brief This method is called when identification is finished. (abstract method)
			 */
			virtual void onEndSearch();

			/**
			 * @brief gets the search information (abstract method)
			 * @param[out] searchInfo the object to store the search information
			 * @param[in] settings search setttings
			 */
			virtual void onGetSearchInfo(
				kome::ident::SearchInfo& searchInfo, 
				kome::objects::SettingParameterValues* settings);

			void onSetSubTracts(
				const unsigned int id,
				kome::ident::SearchResult& result, 
				kome::ident::SpecInfo** spectra,
				const unsigned int specNum
				);
		};
	}
}
