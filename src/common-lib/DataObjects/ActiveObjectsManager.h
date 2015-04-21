/**
 * @file ActiveObjectsManager.h
 * @brief interfaces of ActiveObjectsManager class
 *
 * @author S.Tanaka
 * @date 2007.12.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_ACTIVE_OBJECT_MANAGER_H__
#define __KOME_OBJECTS_ACTIVE_OBJECT_MANAGER_H__


#include "DefaultDataManager.h"

#include "DataSet.h"

#include <map>


namespace kome {
	namespace objects {

		class SampleSet;
		class Sample;
		class Spectrum;
		class DataGroupNode;
		class Chromatogram;
		class DataMapInfo;

		/**
		 * @class ActiveObjectsManager
		 * @brief active object management class
		 */
		class DATA_OBJECTS_CLASS ActiveObjectsManager : public DefaultDataManager {
		protected:
			/**
			 * @fn ActiveObjectsManager()
			 * @brief constructor
			 */
			ActiveObjectsManager();

			/**
			 * @fn virtual ~ActiveObjectsManager()
			 * @brief destructor
			 */
			virtual ~ActiveObjectsManager();

		protected:
			/** filtered spectra map */
			// @date 2012.04.23 <Mod> M.Izumi
			// SPEC No.85266 Filter Spectra should search ALL groups in the active sample, not just 1
			std::map< Sample*, DataSet* > m_filteredDataSetMap;

			/** data set */
			DataSet m_dataSet;

			/** opened samples */
			std::vector< Sample* > m_samples;

			/** file history */
			std::vector< std::string > m_fileHistory;

			/** active sample */
			Sample* m_acSample;
	
			/** the array for closing samples */
			std::vector< Sample* > m_tmpSamples;

			/** open canvas spectrum (single mode) */
			std::vector< Spectrum* > m_openSpectra;	

			/** tagname map */
			std::map< Sample*, std::string > m_tagNameMap;

		public:
			/**
			 * @fn unsigned int getNumberOfOpenedSamples()
			 * @brief gets the number of opened samples
			 * @return the number of opened samples
			 */
			unsigned int getNumberOfOpenedSamples();

			/**
			 * @fn Sample* getOpenedSample( const unsigned int idx )
			 * @brief gets opened sample
			 * @param[in] idx opened sample index
			 * @return opened sample (If NULL, the index is illegal.)
			 */
			Sample* getOpenedSample( const unsigned int idx );

			/**
			 * @fn std::vector< Spectrum* > getOpenSpectra()
			 * @brief 
			 * @return opened Spectrra ( single mode )
			 */
			std::vector< Spectrum* > getOpenSpectra(); 
			
			/**
			 * @fn void addOpenSpectra( Spectrum* spec )
			 * @brief Get Open spectra
			 * @param spec spectrum object
			 */
			void addOpenSpectra( Spectrum* spec );
			
			/**
			 * @fn void removeOpenSpectra( Spectrum* spec )
			 * @brief on closed spectrum canvas ( remove spectrum )
			 * @param spec spectrum object
			 */
			void removeOpenSpectra( Spectrum* spec );
				
		public:
			/**
			 * @fn void setActiveSample( Sample* sample )
			 * @brief sets the active sample
			 * @param[in] sample active sample
			 */
			void setActiveSample( Sample* sample );

			/**
			 * @fn Sample* getActiveSample()
			 * @brief gets the active sample
			 * @return active sample
			 */
			Sample* getActiveSample();

		public:
			/**
			 * @fn DataSet* getFilteredDataSet( Sample* sample )
			 * @brief gets filtered data set
			 * @param[in] sample sample
			 * @return filtered data set
			 */
			DataSet* getFilteredDataSet( Sample* sample );

		protected:
			/**
			 * @fn int searchSample( Sample* sample )
			 * @brief searchs sample from array
			 * @param[in] sample sample object to be searched
			 * @return sample index (If negative value, specified sample was not found.)
			 */
			int searchSample( Sample* sample );

		public:
			/**
			 * @fn void clearFileHistory()
			 * @brief clears file history
			 */
			void clearFileHistory();

			/**
			 * @fn void addHistoryFile( const char* path )
			 * @brief adds history file
			 * @param[in] path file path
			 */
			void addHistoryFile( const char* path );

			/**
			 * @fn void removeHistoryFile( const char* path )
			 * @brief removes history file
			 * @param[in] path file path to be removed
			 */
			void removeHistoryFile( const char* path );

			/**
			 * @fn unsigned int getNumberOfHistoryFiles()
			 * @brief gets the number of history files
			 * @return the number of history files
			 */
			unsigned int getNumberOfHistoryFiles();

			/**
			 * @fn const char* getHistoryFile( const unsigned int idx )
			 * @brief gets history file
			 * @param[in] idx history file index
			 * @return history file path
			 */
			const char* getHistoryFile( const unsigned int idx );

		protected:
			/**
			 * @fn void readHistoryFromIni()
			 * @brief reads history files from ini file
			 */
			void readHistoryFromIni();

		protected:
			/**
			 * @fn virtual void onOpenSample( Sample* sample )
			 * @brief This method is called when a sample is opened. (override method)
			 * @param[in] sample sample object to be opened
			 */
 			virtual void onOpenSample( Sample* sample );

			/**
			 * @fn virtual void onCloseSample( Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( Sample* sample, const bool deleting );

	
		public:
			/**
			 * @fn void addTabSample( Sample* sample )
			 * @brief adds opened sample
			 * @param[in] sample sample object to be closed
			 */
			void addTabSample( Sample* sample );
			
			/**
			 * @fn void delTabSample( Sample* sample )
			 * @brief deletes opened sample
			 * @param[in] sample sample object to be closed
			 */
			void delTabSample( Sample* sample );

			/**
			 * @fn std::vector< Sample* > getTabSamples()
			 * @brief gets the array of opened samples
			 * @return m_tmpSamples
			 */
			std::vector< Sample* > getTabSamples();

		public:
			/**
			 * @fn void setSampleTagName( Sample* sample, const char* name )
			 * @brief sets the sample tag name
			 * @param[in] sample sample
			 * @param[in] name tag name
			 */
			void setSampleTagName( Sample* sample, const char* name );

			/**
			 * @fn const char* getSampleTagName( Sample* sample )
			 * @brief gets the sample tag name
			 * @param[in] sample sample
			 * @return tag name
			 */
			const char* getSampleTagName( Sample* sample );

		protected:
			/**
			 * @fn void setTabSample()
			 * @brief initializes the array of opened samples
			 */
			void setTabSample();

		public:
			/**
			 * @fn static ActiveObjectsManager& getInstance()
			 * @brief get active object manager object (This is the only object.)
			 * @return active manager object
			 */
			static ActiveObjectsManager& getInstance();
		};
	}
}


#endif		// __KOME_OBJECTS_ACTIVE_OBJECT_MANAGER_H__
