/**
 * @file SpectraSearchManager.h
 * @brief interfaces of SpectraSearchManager class
 *
 * @author S.Tanaka
 * @date 2007.01.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SEARCH_SPECTRA_SEARCH_MANAGER_H__
#define __KOME_SEARCH_SPECTRA_SEARCH_MANAGER_H__


namespace kome {
	namespace search {

		class SpectraSearchFilter;
		class SpectraSearchDialog;
		
		/**
		 * @class SpectraSearchManager
		 * @brief spectra search management class
		 */
		class SpectraSearchManager {
		protected:
			/**
			 * @fn SpectraSearchManager()
			 * @brief constructor
			 */
			SpectraSearchManager();

			/**
			 * @fn virtual ~SpectraSearchManager()
			 * @brief destructor
			 */
			virtual ~SpectraSearchManager();

		public:
			/**
			 * @typedef SearchType
			 * @brief search type
			 */
			typedef enum {
				SEARCH_NEW,
				SEARCH_APPEND,
				SEARCH_NARROW
			} SearchType;
		protected:
			/** SearchType naem */
			std::string m_strTypeName;

			/** SearchType */
			SpectraSearchManager::SearchType m_type;

			/** DataSet object */
			kome::objects::DataSet* m_dataSet;

			kome::objects::DataSet* m_filterSpectra;
		public:
			/**
			 * @fn void setDataSet( kome::objects::DataSet* dataset )
			 * @brief set DataSet 
			 * @param[in] dataset DataSet object
			 */
			void setDataSet( kome::objects::DataSet* dataset );

			/**
			 * @fn kome::objects::DataSet* getDataSet()
			 * @brief get DataSet 
			 * @return DataSet object
			 */
			kome::objects::DataSet* getDataSet();

			void setFilterSpectra( kome::objects::DataSet* dataset );
			kome::objects::DataSet* getFilterSpectra();

			/**
			 * @fn void openDialog(  kome::objects::DataGroupNode* group )
			 * @brief opens dialog
			 * @param[in] group group to be filtered
			 */
			void openDialog( kome::objects::DataGroupNode* group );

			/**
			 * @fn bool openDialog( kome::objects::DataSet* dataSet )
			 * @brief open dialog
			 * @param [in] dataSet data set object
			 * @return dialog closing condition(false=cancel action)
			 */
			bool openDialog( kome::objects::DataSet* dataSet, kome::objects::DataSet& ds );
			
			/**
			 * @fn void searchSpectra( kome::objects::DataSet* dataSet, SearchType type, SpectraSearchFilter& filter )
			 * @brief
			 * @param dataSet data set object
			 * @param type search type
			 * @param filter search filter
			 */
			void searchSpectra( kome::objects::DataSet* dataSet, kome::objects::DataSet& ds, SearchType type, SpectraSearchFilter& filter );
			
			/**
			 * @fn void searchSpectra( kome::objects::Sample* sample, SearchType type, SpectraSearchFilter& filter )
			 * @brief searches spectra
			 * @param[in] sample sample
			 * @param type search type
			 * @param filter search filter
			 */
			void searchSpectra( kome::objects::Sample* sample, SearchType type, SpectraSearchFilter& filter );

			/**
			 * @fn void executeFilter(
					SpectraSearchFilter& filter,
					kome::objects::DataSet& dataSet,
					std::set< kome::objects::Spectrum* >& specSet
				)
			 * @brief executes filter
			 * @param filter search filter
			 * @param spectra source spectra
			 * @param specSet the set to store filtered spectra
			 */
			void executeFilter(
				SpectraSearchFilter& filter,
				kome::objects::DataSet& dataSet,
				std::set< kome::objects::Spectrum* >& specSet
			);

			/**
			 * @fn void updateSpectra(
					kome::objects::DataSet& dataSet,
					std::set< kome::objects::Spectrum* >& specSet
				)
			 * @brief updates spectra
			 * @param spectra spectra object to be updated
			 * @param specSet allowed spectra set
			 */
			void updateSpectra(
				kome::objects::DataSet& dataSet,
				std::set< kome::objects::Spectrum* >& specSet
			);

			/**
			 * @fn const char* getSearchTypeName( SpectraSearchManager::SearchType type )
			 * @brief get search type name
			 * @param [in]type SearchType
			 * @return name search type name
			 */		
			const char* getSearchTypeName( SpectraSearchManager::SearchType type );

			/**
			 * @fn SpectraSearchManager::SearchType getSearchType( std::string typeName )
			 * @brief get search type 
			 * @param[in] typeName search type name
			 * @return type search type
			 */
			SpectraSearchManager::SearchType getSearchType( std::string typeName );
		public:
			/**
			 * @fn static SpectraSearchManager& getInstance()
			 * @brief gets spectra search management object.
			 * @return spectra search management object. (This is the only object.)
			 */
			static SpectraSearchManager& getInstance();
		};
	}
}

#endif		//  __KOME_SEARCH_SPECTRA_SEARCH_MANAGER_H__
