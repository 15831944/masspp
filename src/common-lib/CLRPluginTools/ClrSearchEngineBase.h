/**
 * @file ClrSearchEngineBase.h
 * @brief interfaces of ClrSearchEngineBase class
 *
 * @author S.Tanaka
 * @date 2012.11.07
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_CLR_SEARCH_ENGINET_BASE_H__
#define __KOME_CLR_SEARCH_ENGINET_BASE_H__


#include "SearchEngineWrapper.h"
#include "HitWrapper.h"

#include "IdentificationManagerWrapper.h"


namespace kome {
	namespace clr {

		ref class SearchResultWrapper;

		/**
		 * @class ClrSearchEngineBase
		 * @brief abstraction class of search engine on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrSearchEngineBase : public SearchEngineWrapper {
#else
		public ref class ClrSearchEngineBase abstract : public SearchEngineWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrSearchEngineBase( System::String^ name, System::String^ version, bool multipleSpec )
			 * @brief constructor
			 * @param[in] name search engine name
			 * @param[in] version search engine version (can be "")
			 * @param[in] multipleSpec If true, this search engine can search from multiple spectra.
			 * @return search engine object
			 */
			ClrSearchEngineBase( System::String^ name, System::String^ version, bool multipleSpec );

			/**
			 * @fn virtual ~ClrSearchEngineBase()
			 * @brief destructor
			 */
			virtual ~ClrSearchEngineBase();

		public:
			/**
			 * @fn virtual bool isValidSpectrum( SpectrumWrapper^ spec ) override = 0
			 * @brief judges the specified spectrum is valid to this search engine. (abstract method)
			 * @param[in] spec spectrum to be judged
			 * @return If true, the specified spectrum is valid to this search engine.
			 */
			virtual bool isValidSpectrum( SpectrumWrapper^ spec ) override = 0;
			
			/**
			 * @fn virtual void onSpectrumPeaks(
						SpectrumWrapper^ spec,
						HitWrapper::SpecInfo^ specInfo,
						PeaksWrapper^ peaks,
						cli::array< int >^ peakids
					) override = 0
			 * @brief This method is called when peaks of a spectrum are detected. (abstract method)
			 * @param[in] spec spectrum 
			 * @param[in] specInfo spectrum information
			 * @param[in] peaks detected peaks
			 * @param[in] peakIds the array of peak IDs of peak matrix.
			 */
			virtual void onSpectrumPeaks(
					SpectrumWrapper^ spec,
					HitWrapper::SpecInfo^ specInfo,
					PeaksWrapper^ peaks,
					cli::array< int >^ peakids
			 ) override = 0;

			/**
			 * @fn virtual bool onFinishPeakDetection() override = 0
			 * @brief on finished peak detection (abstract method)
			 */
			virtual bool onFinishPeakDetection() override = 0;

		public:
			/**
			 * @fn virtual System::String^ onSearch(
						System::String^ title,
						System::String^ comment,
						cli::array< HitWrapper::SpecInfo^ >^ spectra,
						SettingParameterValuesWrapper^ settings,
						ProgressWrapper^ progress
					) = 0
			 * @brief This method is called from search method. (abstract method)
			 * @param[in] title title
			 * @param[in] comment commetn
			 * @param[in] spectra the array of spectra
			 * @param[in] settings settings of search
			 * @param[in,out] progress the object to show the progress
			 * @return the identifier of search.
			 */
			virtual System::String^ onSearch(
				System::String^ title,
				System::String^ comment,
				cli::array< HitWrapper::SpecInfo^ >^ spectra,
				SettingParameterValuesWrapper^ settings,
				ProgressWrapper^ progress
			) = 0;

			/**
			 * @fn virtual bool onPreparePeakDetection(
						SettingParameterValuesWrapper^ searchSettings,
						System::String^ peakDetector,
						SettingParameterValuesWrapper^ peakSettings,
						System::String^ chargeDetector,
						SettingParameterValuesWrapper^ chargeSettings,
						System::String^ peakFilter
					)
			 * @brief This method is called by preparePeakDetection method. (abstract method)
			 * @param[in] searchSettings search parameters
			 * @param[in] peakDetector peak detector algorithm
			 * @param[in] peakSettings peak detection settings
			 * @param[in] chargeDetector charge calculation algorithm
			 * @param[in] chargeSettings charge calculation settings
			 * @param[in] peakFilter peak filter
			 * @return If true, preparation of peak detection is finished successfully.
			 */
			virtual bool onPreparePeakDetection(
				SettingParameterValuesWrapper^ searchSettings,
				System::String^ peakDetector,
				SettingParameterValuesWrapper^ peakSettings,
				System::String^ chargeDetector,
				SettingParameterValuesWrapper^ chargeSettings,
				System::String^ peakFilter
			) = 0;


			/**
			 * @fn virtual SearchStatus onGetSearchStatus( System::String^ id ) = 0
			 * @brief This method is called by getSearchStatus method. (abstract method)
			 * @param[in] id search ID
			 * @return search status
			 */
			virtual SearchStatus onGetSearchStatus( System::String^ id ) = 0;

			/**
			 * @fn virtual void onGetResult(
						SearchResultWrapper^ result,
						System::String^ id,
						cli::array< HitWrapper::SpecInfo^ >^ spectra,
						ProgressWrapper^ progress
					) = 0
			 * @brief This method is called by getResult method. (override method)
			 * @parma[out] result the object to store the search results
			 * @param[in] id search ID
			 * @param[in] spectra the array of spectrum
			 * @param[in,out] progress the object to show the progress.
			 */
			virtual void onGetResult(
					SearchResultWrapper^ result,
					System::String^ id,
					cli::array< HitWrapper::SpecInfo^ >^ spectra,
					ProgressWrapper^ progress
			) = 0;

			/**
			 * @fn virtual bool onOpenResult( System::String^ resultLocation ) = 0
			 * @brief display the search result using the specified result location (abstract method)
			 * @param[in] resultLocation string that specifies where the result can be obtained
			 * @return If true, it succeeded to display the result.
			 */
			virtual bool onOpenResult( System::String^ resultLocation ) = 0;

			/**
			 * @fn virtual bool onPrepareSearch( ProgressWrapper^ progress ) = 0
			 * @brief This method is called from the prepareSearch method. (abstract method)
			 * @param[out] progress the object to show the progres
			 * @return If true, the preparation is finished successfully.
			 */
			virtual bool onPrepareSearch( ProgressWrapper^ progress ) = 0;

			/**
			 * @fn virtual void onEndSearch() = 0
			 * @brief This method is called when identification is finished. (abstract method)
			 */
			virtual void onEndSearch() = 0;

			/**
			 * @fn virtual void onGetSearchInfo( SearchEngineWrapper::SearchInfo^ searchInfo, SettingParameterValuesWrapper^ settings ) = 0
			 * @brief gets the search information (abstract method)
			 * @param[out] searchInfo the object to store the search information
			 * @param[in] settings search settings
			 */
			virtual void onGetSearchInfo( SearchEngineWrapper::SearchInfo^ searchInfo, SettingParameterValuesWrapper^ settings ) = 0;
		};
	}
}

#endif	// __KOME_CLR_SEARCH_ENGINET_BASE_H__
