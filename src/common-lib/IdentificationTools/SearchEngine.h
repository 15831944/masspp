/**
 * @file SearchEngine.h
 * @brief interfaces of SearchEngine class
 *
 * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#pragma once


namespace kome {
	namespace ident {

		class Enzyme;
		class Modification;

		/**
		 * @struct SearchInfo
		 * @brief db search information
		 */
		struct SearchInfo {
			std::string                    name;
			std::string                    comment;
			std::string                    date;
			int                            engineId;
			std::string                    db;
			std::string                    result;
			int                            precMassType;
            int                            fragMassType;
            Enzyme*                        enzyme;
            std::string                    baseName;
			std::vector< Modification* >   fixedMods;
			std::vector< Modification* >   varMods;
			kome::core::Properties         searchProps;
			std::string                    searchId;
			SpecInfo**                     spectra;
			int                            specNum;
		};

		/**
		 * @class SearchEngine
		 * @brief search engine operation management abstract class
		 */
		class IDENT_TOOLS_CLASS SearchEngine
		{
		public:
			/**
			 * @fn SearchEngine( const char* name, const char* version, cosnt bool multipleSpec )
			 * @brief constructor
			 * @param[in] name search engine name
			 * @param[in] version search engine version (can be "")
			 * @param[in] multipleSpec If true, this search engine can search from multiple spectra.
			 */
			SearchEngine( const char* name, const char* version, const bool multipleSpec );

			/**
			 * @fn virtual ~SearchEngine( void )
			 * @brief destructor
			 */
			virtual ~SearchEngine( void );

		private:
			/** Search engine name */
			std::string m_strName;

			/** Search engine version */
			std::string m_strVersion;

			/** multiple spectrum */
			bool m_multipleSpec;

			/** Search Info */
			SearchInfo m_searchInfo;

		public:
			/**
			 * @typedef SearchStatus
			 * @brief search status
			 */
			typedef enum {
				STATUS_UNKNOWN = 0,
				STATUS_WAITING = 1,
				STATUS_RUNNING = 2,
				STATUS_COMPLETED = 3,
				STATUS_ERROR = 4
			} SearchStatus;

		public:
			/**
			 * @fn std::string getName( void ) const
			 * @brief get the search engine name
			 * @return name
			 */
			std::string getName( void ) const;

			/**
			 * @fn std::string getVersion( void ) const
			 * @brief get the search engine version
			 * @return version
			 */
			std::string getVersion( void ) const;

			/**
			 * @fn bool isMultipleSpectrum()
			 * @brief judges the target data is multiple or not.
			 * @return If true, this search engine can search from multiple spectra.
			 */
			bool isMultipleSpectrum();

		public:
			/**
			 * @fn int search(
					    const char* title,
						const char* comment,
						SpecInfo** spectra,
						const unsigned int specNum,
						kome::objects::SettingParameterValues* setting,
						kome::core::Progress* progress
					)
			 * @brief execute identification
			 * @param[in] title title
			 * @param[in] comment comment
			 * @param[in] spectra the array of spectra information
			 * @param[in] specNum number of spectrum information
			 * @param[in] setting settings of search
			 * @param[in,out] progress the object to show the progress
			 * @return search ID
			 */
			int search(
				const char* title,
				const char* comment,
				SpecInfo** spectra,
				const unsigned int specNum,
				kome::objects::SettingParameterValues* settings,
				kome::core::Progress* progress
			);

			/**
			 * @fn SearchStatus getSearchStatus( const char* id )
			 * @brief gets the search status
			 * @param[in] id search ID
			 * @return search status
			 */
			SearchStatus getSearchStatus( const char* id );

			/**
			 * @fn void getResult( SearchResult& result, const char* id, kome::core::Progress& progress );
			 * @brief gets the search result
			 * @param[out] result the object to store the search results
			 * @param[in] id search ID
			 * @param[in,out] progress the object to show the progress
			 */
			void getResult( SearchResult& result, const char* id, kome::core::Progress& progress );

			/**
			 * @fn bool openResult( const char* resultLocation )
			 * @brief display the search result using the specified result location
			 * @param[in] resultLocation string that specifies where the result can be obtained
			 * @return If true, it succeeded to display the result.
			 */
			bool openResult( const char* resultLocation );

			/**
			 * @fn bool prepareSearch( kome::core::Progress& progress )
			 * @brief prepares for search
			 * @param[in,out] progress the object to show the progres
			 * @return If true, the preparation is finished successfully.
			 */
			bool prepareSearch( kome::core::Progress& progress );

			/**
			 * @fn int preparePeakDetection(
						kome::objects::SettingParameterValues* searchSettings,
						const char* peakDetector,
						kome::objects::SettingParameterValues* peakSettings,
						const char* chargeDetector,
						kome::objects::SettingParameterValues* chargeSettings,
						const char* peakFilter
					)
			 * @brief prepares peak detection. 
			 * @param[in] searchSettings settings search parameters
			 * @param[in] peakDetector peak detector algorithm
			 * @param[in] peakSettings peak detection settings
			 * @param[in] chargeDetector charge calculation algorithm
			 * @param[in] chargeSettings charge calculation settings
			 * @param[in] peakFilter peak filter
			 * @return peak detection ID
			 */
			int preparePeakDetection(
				kome::objects::SettingParameterValues* searchSettings,
				const char* peakDetector,
				kome::objects::SettingParameterValues* peakSettings,
				const char* chargeDetector,
				kome::objects::SettingParameterValues* chargeSettings,
				const char* peakFilter
			);

		public:
			/**
			 * @fn virtual bool isValidSpectrum( kome::objects::Spectrum* spec ) = 0			 
			 * @brief judges the specified spectrum is valid to this search engine. (abstract method)
			 * @param[in] spec spectrum to be judged
			 * @return If true, the specified spectrum is valid to this search engine.
			 */
			virtual bool isValidSpectrum( kome::objects::Spectrum* spec ) = 0;
			
			/**
			 * @fn virtual void onSpectrumPeaks(
						kome::objects::Spectrum* spec,
						SpecInfo* specInfo,
						kome::objects::Peaks* peaks,
						int* peakIds,
						const unsigned int peakIdsSize
					) = 0
			 * @brief This method is called when peaks of a spectrum are detected. (abstract method)
			 * @param[in] spec spectrum 
			 * @param[in] specInfo spectrum information
			 * @param[in] peaks detected peaks
			 * @param[in] peakIds the array of peak IDs of peak matrix.
			 * @param[in] peakIdsSize the array size of peakIds
			 */
			virtual void onSpectrumPeaks(
					kome::objects::Spectrum* spec,
					SpecInfo* specInfo,
					kome::objects::Peaks* peaks,
					int* peakIds,
					const unsigned int peakIdsSize
			 ) = 0;

			/**
			 * @fn virtual bool onFinishPeakDetection() = 0
			 * @brief on finished peak detection (abstract method)
			 */
			virtual bool onFinishPeakDetection() = 0;

			/**
			 * @fn virtual std::string validateSettings( kome::objects::SettingParameterValues* paramSettings ) = 0
			 * @brief validate the parameter settings (abstract method)
			 * @param[in] paramSettings identification parameters
			 * @return If an empty string, no validation error occurred. Otherwise, it contains a description of the error.
			 */
			virtual std::string validateSettings( kome::objects::SettingParameterValues* paramSettings ) = 0;

		protected:
			/**
			 * @fn virtual std::string onSearch(
						const char* title,
						const char* comment,
						SpecInfo** spectra,
						const unsigned int specNum,
						kome::objects::SettingParameterValues* settings,
						kome::core::Progress& progress
					) = 0
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
				SpecInfo** spectra,
				const unsigned int specNum,
				kome::objects::SettingParameterValues* settings,
				kome::core::Progress& progress
			) = 0;

			/**
			 * @fn virtual bool onPreparePeakDetection(
						kome::objects::SettingParameterValues* searchSettings,
						const char* peakDetector,
						kome::objects::SettingParameterValues* peakSettings,
						const char* chargeDetector,
						kome::objects::SettingParameterValues* chargeSettings,
						const char* peakFilter
					)
			 * @brief This method is called by preparePeakDetection method. (abstract method)
			 * @param[in] searchSettings settings search parameters
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
			) = 0;


			/**
			 * @fn virtual SearchStatus onGetSearchStatus( const char* id ) = 0
			 * @brief This method is called by getSearchStatus method. (abstract method)
			 * @param[in] id search ID
			 * @return search status
			 */
			virtual SearchStatus onGetSearchStatus( const char* id ) = 0;

			/**
			 * @fn virtual void onGetResult(
						SearchResult& result,
						const char* id,
						SpecInfo** spectra,
						const unsigned int specNum,
						kome::core::Progress& progress
					) = 0
			 * @brief This method is called by getResult method. (abstract method)
			 * @param[out] result the object to store the search results
			 * @param[in] id search ID
			 * @param[in] spectra the array of spectra
			 * @param[in] specNum the size of spectra array
			 * @param[in,out] progress the object to show the progress
			 */
			virtual void onGetResult(
					SearchResult& result,
					const char* id,
					SpecInfo** spectra,
					const unsigned int specNum,
					kome::core::Progress& progress
			) = 0;

			/**
			 * @fn virtual bool onOpenResult( const char* resultLocation ) = 0
			 * @brief display the search result using the specified result location (abstract method)
			 * @param[in] resultLocation string that specifies where the result can be obtained
			 * @return If true, it succeeded to display the result.
			 */
			virtual bool onOpenResult( const char* resultLocation ) = 0;

			/**
			 * @fn virtual bool onPrepareSearch( kome::core::Progress& progress ) = 0
			 * @brief This method is called from the prepareSearch method. (abstract method)
			 * @param[out] progress the object to show the progres
			 * @return If true, the preparation is finished successfully.
			 */
			virtual bool onPrepareSearch( kome::core::Progress& progress ) = 0;

			/**
			 * @fn virtual void onEndSearch() = 0
			 * @brief This method is called when identification is finished. (abstract method)
			 */
			virtual void onEndSearch() = 0;

			/**
			 * @fn virtual void onGetSearchInfo( SearchInfo& searchInfo, kome::objects::SettingParameterValues* settings ) = 0
			 * @brief gets the search information (abstract method)
			 * @param[out] searchInfo the object to store the search information
			 * @param[in] settings search setttings
			 */
			virtual void onGetSearchInfo( SearchInfo& searchInfo, kome::objects::SettingParameterValues* settings ) = 0;
		};
	}
}
