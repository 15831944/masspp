/**
 * @file CompoundDbManager.h
 * @brief interfaces of CompoundDbManager class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_COMPOUND_DB_MANAGER_H__
#define __KOME_COMPOUND_DB_MANAGER_H__



namespace kome {
	namespace external_db {

		/**
		 * @class CompoundDbManager
		 * @brief mascot search management class
		 */
		class CompoundDbManager {
		protected:
			/**
			 * @fn CompoundDbManager()
			 * @brief constructor
			 */
			CompoundDbManager();

			/**
			 * @fn virtual ~CompoundDbManager()
			 * @brief destructor
			 */
			virtual ~CompoundDbManager();

		protected:
			/** databases */
			std::vector< kome::ident::ExternalDB* > m_databases;

		public:
			/**
			 * @fn kome::ident::ExternalDB* getDb( const char* db )
			 * @brief gets the external database
			 * @param[in] db database name
			 * @return database
			 */
			kome::ident::ExternalDB* getDb( const char* db );

		public:
			/**
			 * @fn static CompoundDbManager& getInstance()
			 * @brief gets object
			 * @return CompoundDbManager object. (This is the only object.)
			 */
			static CompoundDbManager& getInstance();
		};
	}
}

#endif	// __KOME_COMPOUND_DB_MANAGER_H__
