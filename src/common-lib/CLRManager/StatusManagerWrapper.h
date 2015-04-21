/**
 * @file StatusManagerWrapper.h
 * @brief interfaces of StatusManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2007.01.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_STATUS_MANAGER_WRAPPER_H__
#define __KOME_CLR_STATUS_MANAGER_WRAPPER_H__

namespace kome {
	namespace clr { 

		ref class ClrIniFile;

		/**
		 * @class StatusManagerWrapper
		 * @brief status manager wrapper class to use on CLR environment
		 */
		public ref class StatusManagerWrapper {
		protected:
			/**
			 * @fn StatusManagerWrapper( kome::objects::StatusManager& statusMgr )
			 * @brief constructor
			 * @param[in] statusMgr status manager object
			 */
			StatusManagerWrapper( kome::objects::StatusManager& statusMgr );

			/**
			 * @fn virtual ~StatusManagerWrapper()
			 * @brief destructor
			 */
			virtual ~StatusManagerWrapper();

		protected:
			/** status manager object */
			kome::objects::StatusManager& m_statusMgr;

			/** status manager wrapper object (This is the only object.) */
			static StatusManagerWrapper^ m_mgr;

		public:
			/**
			 * @fn kome::objects::StatusManager& getStatusManager()
			 * @brief gets status manager object
			 * @return status manager object
			 */
			kome::objects::StatusManager& getStatusManager();

		public:
			/**
			 * @fn void setValue( System::String^ name, System::String^ value )
			 * @brief sets status value
			 * @param[in] name status name
			 * @param[in] value status value
			 */
			void setValue( System::String^ name, System::String^ value );

			/**
			 * @fn System::String^ getValue( System::String^ name )
			 * @brief gets status value
			 * @param[in] name status name
			 * @return status value
			 */
			System::String^ getValue( System::String^ name );

			/**
			 * @fn bool hasStatus( System::String^ name )
			 * @brief returns true if there is status variable specified name
			 * @param[in] name status name
			 * @return true if there is status variable specified name
			 */
			bool hasStatus( System::String^ name );

			/**
			 * @fn unsigned int getNumberOfStatuses()
			 * @brief gets the number of status variables
			 * @return the number of status variables
			 */
			unsigned int getNumberOfStatuses();

			/**
			 * @fn System::String^ getStatusName( unsigned int index )
			 * @brief gets status variable name
			 * @param[in] index status variable index
			 * @return status variable name (If the index is illegal, this method returns NULL.)
			 */
			System::String^ getStatusName( unsigned int index );

			/**
			 * @fn System::String^ getStatusValue( unsigned int index )
			 * @brief gets status value
			 * @param[in] index status variable index
			 * @return status value (If the index is illegal, this method returns NULL.)
			 */
			System::String^ getStatusValue( unsigned int index );

			/**
			 * @fn void setToIniFile( ClrIniFile^ ini, System::String^ section )
			 * @brief sets status values to ini file information object
			 * @param[out] ini ini file information object to be stored values
			 * @param[in] section section name
			 */
			void setToIniFile( ClrIniFile^ ini, System::String^ section );

			/**
			 * @fn void getFromIniFile( ClrIniFile^ ini, System::String^ section )
			 * @brief gets status values from ini file information object
			 * @param[in] ini ini file information object
			 * @param[in] section section name
			 */
			void getFromIniFile( ClrIniFile^ ini, System::String^ section );

		public:
			/**
			 * @fn static StatusManagerWrapper^ getInstance()
			 * @brief gets status manager object
			 * @return status manager object (This is the only object.)
			 */
			static StatusManagerWrapper^ getInstance();
		};
	}
}

#endif		// __KOME_CLR_STATUS_MANAGER_WRAPPER_H__

