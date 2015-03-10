/**
 * @file DataMapFilterPluginManager.h
 * @brief interfaces of Data Map Filter plug-in functions
 *
 * @author M.Izumi
 * @date 2012.09.06
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_VIEW_DATAMAP_FILTER_MANAGER_H__
#define __KOME_VIEW_DATAMAP_FILTER_MANAGER_H__

#include <wx/wx.h>

namespace kome {
	namespace view {

		class DataMapFilterManager {
		protected:
			/**
			 * @fn DataMapFilterManager()
			 * @brief constructor
			 */
			DataMapFilterManager();

			/**
			 * @fn ~DataMapFilterManager()
			 * @brief destructor
			 */
			virtual ~DataMapFilterManager();

		protected:
			/** filter name */
			std::string m_name;

		public:
			/**
			 * @fn const char* getFilterName()
			 * @brief get Data Map filter name
			 * @return filter name
			 */
			const char* getFilterName();

			/**
			 * @fn void setFilterName( const char* name )
			 * @brief set Data Map filter name
			 * @param[in] name filter name
			 */
			void setFilterName( const char* name );

			/**
			 * @fn void updateDataMap()
			 * @brief Reflect the changed value to DataMap
			 */
			void updateDataMap();

		public:
			/**
			 * @fn static DataMapFilterManager& getInstance()
			 * @brief gets DataMap Filter management object.
			 * @return DataMap filter management object. (This is the only object.)
			 */
			static DataMapFilterManager& getInstance();
		};

	}
}

#endif	// __KOME_VIEW_DATAMAP_FILTER_MANAGER_H__
