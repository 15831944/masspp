/**
 * @file ActiveObjectsManagerWrapper.h
 * @brief interfaces of ActiveObjectsManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_ACTIVE_OBJECTS_MANAGER_WRAPPER_H__
#define __KOME_CLR_ACTIVE_OBJECTS_MANAGER_WRAPPER_H__


#include "DataManagerWrapper.h"


namespace kome { 
	namespace clr {

		ref class SampleSetWrapper;
		ref class SampleWrapper;
		ref class DataGroupNodeWrapper;
		ref class DataSetWrapper;
		ref class SpectrumWrapper;
		ref class ChromatogramWrapper;
		ref class DataMapInfoWrapper;

		/**
		 * @class ActiveObjectsManagerWrapper
		 * @brief active objects manager wrapper class to use on CLR environment
		 */
		public ref class ActiveObjectsManagerWrapper : public DataManagerWrapper {
		protected:
			/**
			 * @fn ActiveObjectsManagerWrapper( kome::objects::ActiveObjectsManager& mgr )
			 * @brief constructor
			 * @param[in] mgr data manager object
			 */
			ActiveObjectsManagerWrapper( kome::objects::ActiveObjectsManager& mgr );

			/**
			 * @fn virtual ~ActiveObjectsManagerWrapper()
			 * @brief destructor
			 */
			virtual ~ActiveObjectsManagerWrapper();

		protected:
			/** data manager object */
			kome::objects::ActiveObjectsManager& m_mgr;

		public:
			/**
			 * @fn kome::objects::ActiveObjectsManager& getActiveObjectsManager()
			 * @brief gets data manager object
			 * @return data manager object
			 */
			kome::objects::ActiveObjectsManager& getActiveObjectsManager();

		public:
			/**
			 * @fn unsigned int getNumberOfOpenedSamples()
			 * @brief gets the number of opened samples
			 * @return the number of opened samples
			 */
			unsigned int getNumberOfOpenedSamples();

			/**
			 * @fn SampleWrapper^ getOpenedSample( unsigned int idx )
			 * @brief gets opened sample
			 * @param[in] idx opened sample index
			 * @return opened sample (If NULL, the index is illegal.)
			 */
			SampleWrapper^ getOpenedSample( unsigned int idx );

			/**
			 * @fn void setActiveSample( SampleWrapper^ sample )
			 * @brief sets the active sample
			 * @param[in] sample active sample
			 */
			void setActiveSample( SampleWrapper^ sample );

			/**
			 * @fn SampleWrapper^ getActiveSample()
			 * @brief gets the active sample
			 * @return active sample
			 */
			SampleWrapper^ getActiveSample();

		public:
			/**
			 * @fn DataSetWrapper^ getFilteredDataSet( SampleWrapper^ sample )
			 * @brief gets filtered data set
			 * @param[in] sample sample
			 * @return filtered data set
			 */
			DataSetWrapper^ getFilteredDataSet( SampleWrapper^ sample );

		public:
			/**
			 * @fn void clearFileHistory()
			 * @brief clears file history
			 */
			void clearFileHistory();

			/**
			 * @fn void addHistoryFile( System::String^ path )
			 * @brief adds history file
			 * @param[in] path file path
			 */
			void addHistoryFile( System::String^ path );

			/**
			 * @fn void removeHistoryFile( System::String^ path )
			 * @brief removes history file
			 * @param[in] path file path
			 */
			void removeHistoryFile( System::String^ path );


			/**
			 * @fn unsigned int getNumberOfHistoryFiles()
			 * @brief gets the number of history files
			 * @return the number of history files
			 */
			unsigned int getNumberOfHistoryFiles();

			/**
			 * @fn System::String^ getHistoryFile( unsigned int idx )
			 * @brief gets history file
			 * @param[in] idx history file index
			 * @return history file path
			 */
			System::String^ getHistoryFile( unsigned int idx );

		public:
			/**
			 * @fn void setSampleTagName( SampleWrapper^ sample, System::String^ name )
			 * @brief sets the sample tag name
			 * @param[in] sample sample
			 * @param[in] name tag name
			 */
			void setSampleTagName( SampleWrapper^ sample, System::String^ name );

			/**
			 * @fn System::String^ getSampleTagName( SampleWrapper^ sample )
			 * @brief gets the sample tag name
			 * @param[in] sample sample
			 * @return tag name
			 */
			System::String^ getSampleTagName( SampleWrapper^ sample );

		public:
			/**
			 * @fn static ActiveObjectsManagerWrapper^ getInstance()
			 * @brief get active object manager object (This is the only object.)
			 * @return active manager object
			 */
			static ActiveObjectsManagerWrapper^ getInstance();
		};
	}
}

#endif		// __KOME_CLR_ACTIVE_OBJECTS_MANAGER_WRAPPER_H__
