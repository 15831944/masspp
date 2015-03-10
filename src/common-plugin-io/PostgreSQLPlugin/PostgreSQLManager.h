/**
 * @file PostgreSQLManager.h
 * @brief interfaces of PostgreSQLManager class
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_DB_POSTGRESQL_MANAGER_H__
#define __KOME_DB_POSTGRESQL_MANAGER_H__


#include <string>
#include <map>


namespace kome {
	namespace db {
		namespace postgresql {

			class PostgreSQL;

			/**
			 * @class PostgreSQLManager
			 * @brief This class has some functions that uses PostgreSQL library.
			 */
			class PostgreSQLManager {
			protected:
				/**
				 * @fn PostgreSQLManager()
				 * @brief constructor
				 */
				PostgreSQLManager();

				/**
				 * @fn virtual ~PostgreSQLManager()
				 * @brief destructor
				 */
				virtual ~PostgreSQLManager();

			protected:
				/** DB map */
				std::map< std::string, PostgreSQL* > m_dbMap;

				/** The DB name which must not be connected */
				std::string	m_strCheckDbName;

			public:
				/**
				 * @fn PostgreSQL* getDb( const char* address, int port, const char* name,
					const char* user, const char* password )
				 * @brief gets DB
				 * @param address address of DB
				 * @param port port of PostgreSQL
				 * @param name DB name
				 * @param user User of PostgreSQL
				 * @param password Password of select user
				 * @return DB (If failed to open the DB, return NULL.)
				 */
				PostgreSQL* getDb( const char* address, int port, const char* name,
					const char* user, const char* password );

			public:
				/**
				 * @fn void deleteDb( PostgreSQL* db )
				 * @brief deletes database
				 * @param db database
				 */
				void deleteDb( PostgreSQL* db );
				
				/**
				 * @fn bool checkDbName(const char* name)
				 * @brief check Db Name.
				 * @return if name is "postgres", return false.
				 */
				bool checkDbName(const char* name);

			public:
				/**
				 * @fn static PostgreSQLManager& getInstance();
				 * @brief gets instance
				 * @return PostgreSQL manager object (This is the only object.)
				 */
				static PostgreSQLManager& getInstance();
			};
		}
	}
}

#endif	// __KOME_DB_POSTGRESQL_MANAGER_H__
