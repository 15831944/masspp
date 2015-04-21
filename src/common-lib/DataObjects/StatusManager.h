/**
 * @file StatusManager.h
 * @brief interfaces of StatusManager class
 *
 * @author S.Tanaka
 * @date 2007.01.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_OBJECTS_STATUS_MANAGER_H__
#define __KOME_OBJECTS_STATUS_MANAGER_H__

#include <string>
#include <vector>
#include <map>

namespace kome {
	namespace objects {
		/**
		 * @class StatusManager
		 * @brief status variables management class
		 */
		class DATA_OBJECTS_CLASS StatusManager {
		protected:
			/**
			 * @fn StatusManager()
			 * @brief constructor
			 */
			StatusManager();

			/**
			 * @fn virtual ~StatusManager()
			 * @brief destructor
			 */
			virtual ~StatusManager();

		protected:
			/** status values map */
			std::map< std::string, std::string > m_statusMap;

			/** the array of status name */
			std::vector< std::string > m_statuses;

		public:
			/**
			 * @fn void setValue( const char* name, const char* value )
			 * @brief sets status value
			 * @param[in] name status name
			 * @param[in] value status value
			 */
			void setValue( const char* name, const char* value );

			/**
			 * @fn const char* getValue( const char* name )
			 * @brief gets status value
			 * @param[in] name status name
			 * @return status value
			 */
			const char* getValue( const char* name );

			/**
			 * @fn bool hasStatus( const char* name )
			 * @brief returns true if there is status variable specified name
			 * @param[in] name status name
			 * @return true if there is status variable specified name
			 */
			bool hasStatus( const char* name );

			/**
			 * @fn unsigned int getNumberOfStatuses()
			 * @brief gets the number of status variables
			 * @return the number of status variables
			 */
			unsigned int getNumberOfStatuses();

			/**
			 * @fn const char* getStatusName( const unsigned int index )
			 * @brief gets status variable name
			 * @param[in] index status variable index
			 * @return status variable name (If the index is illegal, this method returns NULL.)
			 */
			const char* getStatusName( const unsigned int index );

			/**
			 * @fn const char* getStatusValue( const unsigned int index )
			 * @brief gets status value
			 * @param[in] index status variable index
			 * @return status value (If the index is illegal, this method returns NULL.)
			 */
			const char* getStatusValue( const unsigned int index );

			/**
			 * @fn void setToIniFile( kome::core::IniFile& ini, const char* section )
			 * @brief sets status values to ini file information object
			 * @param[out] ini ini file information object to be stored values
			 * @param[in] section section name
			 */
			void setToIniFile( kome::core::IniFile& ini, const char* section );

			/**
			 * @fn void getFromIniFile( kome::core::IniFile& ini, const char* section )
			 * @brief gets status values from ini file information object
			 * @param[in] ini ini file information object
			 * @param[in] section section name
			 */
			void getFromIniFile( kome::core::IniFile& ini, const char* section );

		public:
			/**
			 * @fn static StatusManager& getInstance()
			 * @brief gets status manager object
			 * @return status manager object (This is the only object.)
			 */
			static StatusManager& getInstance();
		};
	}
}

#endif		// __KOME_OBJECTS_STATUS_MANAGER_H__
