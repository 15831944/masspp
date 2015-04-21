/**
 * @file DbManager.h
 * @brief interfaces of DbManager class
 *
 * @author S.Tanaka
 * @date 2008.12.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_MANAGER_H__
#define __KOME_DB_MANAGER_H__


namespace kome {
	namespace db {

		class Database;

		/**
		 * @class DbManager
		 * @brief database manager class
		 */
		class DB_TOOLS_CLASS DbManager {
		protected:
			/**
			 * @fn DbManager()
			 * @brief constructor
			 */
			DbManager();

			/**
			 * @fn virtual ~DbManager()
			 * @brief destructor
			 */
			virtual ~DbManager();

			/** database */
			Database* m_db;

		protected:
			/** database set */
			std::set< Database* > m_dbSet;

		public:
			/**
			 * @fn void setDb( Database* db )
			 * @brief sets database object
			 * @param[in] db database
			 */
			void setDb( Database* db );

			/**
			 * @fn Database* getDb()
			 * @brief gets database object
			 * @return database
			 */
			Database* getDb();

		public:
			/**
			 * @fn void addDb( Database* db )
			 * @brief adds dtabase
			 * @param[in] db database to be add
			 */
			void addDb( Database* db );

			/**
			 * @fn void removeDb( Database* db )
			 * @brief removes database
			 * @param[in] db database to be removed
			 */
			void removeDb( Database* db );

			/**
			 * @fn void closeAllDb()
			 * @brief closes all database
			 */
			void closeAllDb();

		public:
			/**
			 * @fn static DbManager& getInstance()
			 * @brief get DbManager instance (This is the only object.)
			 * @return DbManager object
			 */
			static DbManager& getInstance();
		};
	}
}


#endif	//  __KOME_DB_MANAGER_H__
