/**
 * @file MascotSearchEngineBase.h
 * @brief interfaces of MascotSearchEngineBase class
 *
 * @author S.Tanaka
 * @date 2012.08.09
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_MASCOT_SEARCH_ENGINE_BASE_H__
#define __KOME_MASCOT_SEARCH_ENGINE_BASE_H__


namespace kome {
	namespace mascot {

		/**
		 * @fn MascotSearchEngineBase
		 * @brief mascot search engine base class
		 */
		class MASCOT_LIB_CLASS MascotSearchEngineBase : public kome::ident::SearchEngine {
		public:
			/**
			 * @fn MascotSearchEngineBase( const char* name, const char* search, const bool multipleSpec )
			 * @brief constructor
			 * @pram[in] name search engine name
			 * @param[in] search search type (e.g., MIS / PMF)
			 * @param[in] multipleSpec multiple spectra or not
			 */
			MascotSearchEngineBase( const char* name, const char* search, const bool multipleSpec );

			/**
			 * @fn virtual ~MascotSearchEngineBase()
			 * @brief destructor
			 */
			virtual ~MascotSearchEngineBase();

		protected:
			/** databases */
			std::vector< std::string > m_dbs;

			/** taxonomies */
			std::vector< std::string > m_taxonomies;

			/** search string */
			std::string m_searchStr;

			/** result directory */
			std::string m_resultDir;

			/** search ID */
			std::string m_searchId;

		public:
			/**
			 * @fn void clearDatabases()
			 * @brief clears databases
			 */
			void clearDatabases();

			/**
			 * @fn unsigned int getNumberOfDatabases()
			 * @brief gets the number of databases
			 * @return the number of databases
			 */
			unsigned int getNumberOfDatabases();

			/**
			 * @fn const char* getDatabase( const unsigned int idx )
			 * @brief gets database
			 * @param[in] idx database index
			 * @return database name
			 */
			const char* getDatabase( const unsigned int idx );

			/**
			 * @fn unsigned int getNumberOfTaxonomies()
			 * @brief gets the number of taxonomies
			 * @return the number of taxonomies
			 */
			unsigned int getNumberOfTaxonomies();

			/**
			 * @fn const char* getTaxonomy( const unsigned int idx )
			 * @brief gets the taxonomy
			 * @param[in] idx taxonomy index
			 * @return taxonomy
			 */
			const char* getTaxonomy( const unsigned int idx );

		protected:
			/**
			 * @fn bool onImportResults(
						const std::vector<std::string>& fileList,
						kome::ident::SearchResult& result,
						kome::ident::SpecInfo** spectra,
						const unsigned int specNum,
						kome::core::Progress& prog,
						const bool usePeptideSummary,
						const double minProb
					)
			 * @brief import the result files into a SearchResult object
			 * @param[in] fileList array of result file paths
			 * @param[out] result object to store the search result
			 * @param[in] spectra the array of spectrum
			 * @param[in] specNum the size of spectrum array
			 * @param[out] prog progress display object
			 * @param[in] useMgf If true, use the peptide summary
			 * @param[in] minProb minimum probability
			 * @return If true, it succeeded to import the results.
			 */
			bool onImportResults(
					const std::vector<std::string>& fileList,
					kome::ident::SearchResult& result,
					kome::ident::SpecInfo** spectra,
					const unsigned int specNum,
					kome::core::Progress& prog,
					const bool usePeptideSummary,
					const double minProb
			);

		protected:
			/**
			 * @fn kome::ident::Enzyme* getEnzyme( kome::objects::SettingParameterValues* settings )
			 * @brief gets enzyme object from parameters
			 * @param[in] settings parameters
			 * @return Enzyme object
			 */
			kome::ident::Enzyme* getEnzyme( kome::objects::SettingParameterValues* settings );

			
			/**
			 * @fn std::string getResultFilePath( int index )
			 * @brief gets the result file path
			 * @param[in] index
			 * @return result file path
			 */
			std::string getResultFilePath( int index );

			/**
			 * @fn std::string getResultDir( const char* searchId, const char* baseDir )
			 * @brief gets the result directory
			 * @param[in] searchId search ID
			 * @param[in] baseDir base directory
			 * @return result directory
			 */
			std::string getResultDir( const char* searchId, const char* baseDir = NULL );

			/**
			 * @fn std::string performSearch(
						std::vector< std::string >& peakFiles,
						const char* title,
						kome::objects::SettingParameterValues* settings,
						kome::core::Progress& progress
					)
			 * @brief performs search
			 * @param[in] peakFiles peak list files
			 * @param[in] title search title
			 * @param[in] title search title
			 */
			std::string performSearch(
					std::vector< std::string >& peakFiles,
					const char* title,
					kome::objects::SettingParameterValues* settings,
					kome::core::Progress& progress
			);

		protected:
			/**
			 * @fn virtual void onHit( kome::ident::Hit* hit )
			 * @brief on hit
			 * @param[in] hit hit
			 */
			virtual void onHit( kome::ident::Hit* hit );

		public:
			/**
			 * @fn virtual std::string validateSettings( kome::objects::SettingParameterValues* paramSettings )
			 * @brief validate the parameter settings (abstract method)
			 * @param[in] paramSettings identification parameters
			 * @return If an empty string, no validation error occurred. Otherwise, it contains a description of the error.
			 */
			virtual std::string validateSettings( kome::objects::SettingParameterValues* paramSettings );

		protected:
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
			 * @fn virtual kome::ident::SearchEngine::SearchStatus onGetSearchStatus( const char* id )
			 * @brief This method is called by getSearchStatus method. (override method)
			 * @param[in] search ID
			 * @return search status
			 */
			virtual kome::ident::SearchEngine::SearchStatus onGetSearchStatus( const char* id );

			/**
			 * @fn virtual void onGetResult( kome::ident::SearchResult& result, const char* id, kome::core::Progress& progress )
			 * @brief This method is called by getResult method. (override method)
			 * @parma[out] result the object to store the search results
			 * @param[in] id search ID
			 * @param[in] spectra the array of spectra
			 * @param[in] specNum the size of spectrum array
			 * @param[in,out] progress the object to show the progress.
			 */
			virtual void onGetResult(
					kome::ident::SearchResult& result,
					const char* id,
					kome::ident::SpecInfo** spectra,
					const unsigned int specNum,
					kome::core::Progress& progress
			);

			/**
			 * @fn virtual void onGetSearchInfo( SearchInfo& searchInfo, kome::objects::SettingParameterValues* settings )
			 * @brief gets the search information (abstract method)
			 * @param[out] searchInfo the object to store the search information
			 */
			virtual void onGetSearchInfo( kome::ident::SearchInfo& searchInfo, kome::objects::SettingParameterValues* settings );

		public:
			/**
			 * @fn static std::string checkMods( kome::objects::SettingParameterValues* settings )
			 * @brief check modifications
			 * @param[in] settings search settings
			 * @return error message
			 */
			static std::string checkMods( kome::objects::SettingParameterValues* settings );

		};
	}
}


#endif    // __KOME_MASCOT_SEARCH_ENGINE_BASE_H__
