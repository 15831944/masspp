/**
 * @file SearchEngineWrapper.h
 * @brief interfaces of SearchEngineWrapper class
 *
 * @author S.Tanaka
 * @date 2012.11.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_CLR_SEARCH_ENGINE_WRAPPER_H__
#define __KOME_CLR_SEARCH_ENGINE_WRAPPER_H__


#include "HitWrapper.h"


namespace kome {
	namespace clr {

		ref class SearchResultWrapper;
        ref class ModificationWrapper;
        ref class EnzymeWrapper;

		/**
		 * @class SearchEngineWrapper
		 * @brief search engine wrapper class to use on CLR environment
		 */
		public ref class SearchEngineWrapper {
		public:
			/**
			 * @fn SearchEngineWrapper( kome::ident::SearchEngine& engine )
			 * @briefructor
			 * @param[in] engine search engine object
			 */
			SearchEngineWrapper( kome::ident::SearchEngine& engine );

			/**
			 * @fn virtual ~SearchEngineWrapper()
			 * @brief destructor
			 */
			virtual ~SearchEngineWrapper();

		protected:
			/** search engine object */
			kome::ident::SearchEngine& m_engine;

		public:
			/** search status */
			ENUM_CLASS SearchStatus {
				UNKNOWN = 0,
				WAITING = 1,
				RUNNING = 2,
				COMPLETED = 3,
				ERROR = 4
			};

			/** 
			 * @struct SearchInfo
			 * @brief search information
			 */
			ref struct SearchInfo {
				System::String^    name;
				System::String^    comment;
				System::String^    date;
				int                engineId;
				System::String^    db;
				System::String^    result;
                int                precMassType;
                int                fragMassType;
                EnzymeWrapper^     enzyme;
                System::String^    baseName;
                cli::array< ModificationWrapper^ >^ fixedMods;
				cli::array< ModificationWrapper^ >^ varMods;
				PropertiesWrapper^ searchProps;
				System::String^    searchId;
				cli::array< HitWrapper::SpecInfo^ >^ spectra;
			};

		public:
			/**
			 * @fn kome::ident::SearchEngine& getSearchEngine()
			 * @brief gets search engine object
			 * @return search engine object
			 */
			kome::ident::SearchEngine& getSearchEngine();

		public:
			/**
			 * @fn System::String^ getName()
			 * @brief get the search engine name
			 * @return name
			 */
			System::String^ getName();

			/**
			 * @fn System::String^ getVersion()
			 * @brief get the search engine version
			 * @return version
			 */
			System::String^ getVersion();

			/**
			 * @fn bool isMultipleSpectrum()
			 * @brief judges the target data is multiple or not.
			 * @return If true, this search engine can search from multiple spectra.
			 */
			bool isMultipleSpectrum();

		public:
			/**
			 * @fn int search(
					    System::String^ title,
						System::String^ comment,
						cli::array< HitWrapper::SpecInfo^ >^ spectra,
						const int dataIdx,
						const unsigned int dataNum,
						SettingParameterValuesWrapper^ settings,
						ProgressWrapper^ progress
					)
			 * @brief execute identification
			 * @param[in] title title
			 * @param[in] comment comment
			 * @param[in] spectra the array of spectra
			 * @param[in] settings settings of search
			 * @param[in,out] progress the object to show the progress
			 * @return search ID
			 */
			int search(
				System::String^ title,
				System::String^ comment,
				cli::array< HitWrapper::SpecInfo^ >^ spectra,
				SettingParameterValuesWrapper^ settings,
				ProgressWrapper^ progress
			);

			/**
			 * @fn SearchStatus getSearchStatus( System::String^ id )
			 * @brief gets the search status
			 * @param[in] search ID
			 * @return search status
			 */
			SearchStatus getSearchStatus( System::String^ id );

			/**
			 * @fn void getResult( SearchResultWrapper^ result, System::Strinig^ id, ProgressWrapper^ progress );
			 * @brief gets the search result
			 * @parma[out] result the object to store the search results
			 * @param[in] id search ID
			 * @param[in,out] progress the object to show the progress
			 */
			void getResult( SearchResultWrapper^ result, System::String^ id, ProgressWrapper^ progress );

			/**
			 * @fn bool openResult( System::String^ resultLocation )
			 * @brief display the search result using the specified result location
			 * @param[in] resultLocation string that specifies where the result can be obtained
			 * @return If true, it succeeded to display the result.
			 */
			bool openResult( System::String^ resultLocation );

			/**
			 * @fn bool prepareSearch( ProgressWrapper^ progress )
			 * @brief prepares for search
			 * @param[out] progress the object to show the progres
			 * @return If true, the preparation is finished successfully.
			 */
			bool prepareSearch( ProgressWrapper^ progress );

		public:
			/**
			 * @fn virtual bool isValidSpectrum( SpectrumWrapper^ spec )
			 * @brief judges the specified spectrum is valid to this search engine. (abstract method)
			 * @param[in] spec spectrum to be judged
			 * @return If true, the specified spectrum is valid to this search engine.
			 */
			virtual bool isValidSpectrum( SpectrumWrapper^ spec );

			/**
			 * @fn virtual int preparePeakDetection(
						SettingParameterValuesWrapper^ searchSettings,
						System::String^ peakDetector,
						SettingParameterValuesWrapper^ peakSettings,
						System::String^ chargeDetector,
						SettingParameterValuesWrapper^ chargeSettings,
						System::String^ peakFilter
					)
			 * @brief prepares peak detection. 
			 * @param[in] searchSettings search settings
			 * @param[in] peakDetector the name of peak detector
			 * @param[in] peakSettings peak settings
			 * @param[in] chargeDetector the name of charge detector
			 * @param[in] chargeSettings charge setting
			 * @param[in] peakFilter peak filter
			 * @return peak detection ID
			 */
			virtual int preparePeakDetection(
					SettingParameterValuesWrapper^ searchSettings,
					System::String^ peakDetector,
					SettingParameterValuesWrapper^ peakSettings,
					System::String^ chargeDetector,
					SettingParameterValuesWrapper^ chargeSettings,
					System::String^ peakFilter
			 );
			
			/**
			 * @fn virtual void onSpectrumPeaks(
						SpectrumWrapper^ spec,
						HitWrapper::SpecInfo^ specInfo,
						PeaksWrapper^ peaks,
						cli::array< int >^ peakIds
					)
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
					cli::array< int >^ peakIds
			 );

			/**
			 * @fn virtual bool onFinishPeakDetection()
			 * @brief on finished peak detection (abstract method)
			 */
			virtual bool onFinishPeakDetection();

			/**
			 * @fn virtual System::String^ validateSettings( SettingParameterValuesWrapper^ paramSettings )
			 * @brief validate the parameter settings (abstract method)
			 * @param[in] paramSettings identification parameters
			 * @return If an empty string, no validation error occurred. Otherwise, it contains a description of the error.
			 */
			virtual System::String^ validateSettings( SettingParameterValuesWrapper^ paramSettings );

		public:
			/**
			 * @fn static SearchEngineWrapper^ getEngineFromVariant( ClrVariant^ var )
			 * @brief gets search engine object from variant
			 * @param[in] var variant
			 * @return search engine object
			 */
			static SearchEngineWrapper^ getEngineFromVariant( ClrVariant^ var );

			/**
			 * @fn static SearchInfo^ copySearchInfo( kome::ident::SearchInfo* search )
			 * @brief copies search information
			 * @param[in] search search information
			 * @return copied search information
			 */
			static SearchInfo^ copySearchInfo( kome::ident::SearchInfo* search );

			/**
			 * @fn static void copySearchInfo(  SearchInfo^ search, kome::ident::SearchInfo& dst )
			 * @brief copy search info
			 * @param[in] search
			 * @param[out] dst
			 */
			static void copySearchInfo( SearchInfo^ search, kome::ident::SearchInfo& dst );

		};
	}
}


#endif		// __KOME_CLR_SEARCH_ENGINE_WRAPPER_H__

