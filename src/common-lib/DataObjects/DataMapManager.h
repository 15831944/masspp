/**
 * @file DataMapManager.h
 * @brief interfaces of DataMapManager class
 *
 * @author S.Tanaka
 * @date 2007.09.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_DATA_MAP_MANAGER_H__
#define __KOME_VIEW_DATA_MAP_MANAGER_H__


#include <list>
#include <string>

#define MAP_ROW_SIZE	300
#define MAP_COL_SIZE	300

#include "DefaultDataManager.h"
#include "DataGroupNode.h"
#include "DataMapInfo.h"


namespace kome {
	namespace objects {	// @date 2011/06/16 <Add> OKADA

		/**
		 * @class DataMapManager
		 * @brief class to draw profile
		 */
		class DATA_OBJECTS_CLASS DataMapManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn DataMapManager()
			 * @brief constructor
			 */
			DataMapManager();

			/**
			 * @fn virtual ~DataMapManager()
			 * @brief destructor
			 */
			virtual ~DataMapManager();

		protected:
			/** data map */
			std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > m_dataMap;

		public:
			/**
			 * @fn std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > getDataMap()
			 * @brief gets the map of data map.
			 * @return map
			 */
			std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > getDataMap();

			/**
			 *@ fn void setDataMap2( std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > dataMap )
			 * @brief sets the map of data map
			 * @param[in] dataMap data map
			 */
			void setDataMap2( std::map< kome::objects::DataGroupNode*, kome::objects::DataMapInfo* > dataMap );

		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

		public:
			/**
			 * @fn static DataMapManager& getInstance()
			 * @brief gets instance. (This is the only object.)
			 * @return DataMapManager object
			 */
			static DataMapManager& getInstance();
		};
	}
}

#endif		//  __KOME_VIEW_DATA_MAP_MANAGER_H__
