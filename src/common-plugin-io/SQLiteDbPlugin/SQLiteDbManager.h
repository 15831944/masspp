/**
 * @file SQLiteDbManager.h
 * @brief interfaces of SQLiteDbManager class
 *
 * @author S.Tanaka
 * @date 2008.08.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_SQLITE_MANAGER_H__
#define __KOME_DB_SQLITE_MANAGER_H__


#include <string>
#include <map>


namespace kome {
	namespace db {
		namespace sqlite {

			class SQLiteDb;

			/**
			 * @class SQLiteDbManager
			 * @brief This class has some functions that uses SQLite library.
			 */
			class SQLiteDbManager {
			protected:
				/**
				 * @fn SQLiteDbManager()
				 * @brief constructor
				 */
				SQLiteDbManager();

				/**
				 * @fn virtual ~SQLiteDbManager()
				 * @brief destructor
				 */
				virtual ~SQLiteDbManager();

			protected:
				/** DB map */
				std::map< std::string, SQLiteDb* > m_dbMap;

			public:
				/**
				 * @fn SQLiteDb* getDb( const char* dbPath )
				 * @brief gets DB
				 * @param dbPath database file path
				 * @return DB (If dbPath is null, returns current DB)
				 */
				SQLiteDb* getDb( const char* dbPath );

			public:
				/**
				 * @fn void deleteDb( SQLiteDb* db )
				 * @brief deletes database
				 * @param db database
				 */
				void deleteDb( SQLiteDb* db );

			public:
				/**
				 * @fn static SQLiteDbManager& getInstance();
				 * @brief gets instance
				 * @return SQLite manager object (This is the only object.)
				 */
				static SQLiteDbManager& getInstance();
			};
		}
	}
}

#endif	// __KOME_DB_SQLITE_MANAGER_H__
