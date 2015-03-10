/**
 * @file PostgreSQL.h
 * @brief interfaces of PostgreSQL class
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_DB_POSTGRESQL_DB_H__
#define __KOME_DB_POSTGRESQL_DB_H__


namespace kome {
	namespace db {
		namespace postgresql {

			/**
			 * @class PostgreSQL
			 * @brief PostgreSQL database management class
			 */
			class PostgreSQL : public kome::db::Database {
			public:
				/**
				 * @fn PostgreSQL( const char* address, int port, const char* name,
					const char* user, const char* password, PGconn* db )
				 * @brief constructor
				 * @param address address of DB
				 * @param port port of PostgreSQL
				 * @param name DB name
				 * @param user user of PostgreSQL
				 * @param password password of select user
				 * @param db connection data for DB
				 */
				PostgreSQL( const char* address, int port, const char* name,
					const char* user, const char* password, PGconn* db );

				/**
				 * @fn virtual ~PostgreSQL()
				 * @brief destructor
				 */
				virtual ~PostgreSQL();

			protected:
				/** file path */
				std::string m_path;

				/** server address */
				std::string m_address;

				/** server port */
				std::string m_port;

				/** db name */
				std::string m_name;

				/** user */
				std::string m_user;

				/** password */
				std::string m_password;

				/** postgresql connect struct */
				PGconn *m_db;

				/** statement id */
				long m_nStatementId;

			public:
				/**
				 * @fn PGconn* getDb()
				 * @brief gets PGconn struct
				 * @return PGconn struct
				 */
				PGconn* getDb();

			protected:
				/**
				 * @fn virtual std::string getColumnString( TableColumn& col )
				 * @brief gets column string (override method)
				 * @param col column information
				 * @return column string
				 */
				virtual std::string getColumnString( TableColumn& col );

			protected:
				/**
				 * @fn virtual bool onClose()
				 * @brief This method is called by close method (override method)
				 * @return If true, it succeeded to close database
				 */
				virtual bool onClose();

				/**
				 * @fn virtual bool onCommit()
				 * @brief This method is called by commit method (override method)
				 * @return If true, it succeeded to commit database
				 */
				virtual bool onCommit();

				/**
				 * @fn virtual bool onRollback()
				 * @brief This method is called by rollback method (override method)
				 * @return If true, it succeeded to rollback database
				 */
				virtual bool onRollback();

				/**
				 * @fn virtual bool onExecute( const char* sql )
				 * @brief This method is called by execute method
				 * @param sql SQL
				 * @return If true, it succeeded to execute SQL
				 */
				virtual bool onExecute( const char* sql );

				/**
				 * @fn virtual kome::db::ResultSet* onExecuteQuery( const char* sql )
				 * @brief This method is called by executeQuery method (override method)
				 * @param sql SLQ
				 * @return result set object (If NULL, executing query is failed.)
				 */
				virtual kome::db::ResultSet* onExecuteQuery( const char* sql );

				/**
				 * @fn virtual kome::db::Statement* onPrepare( const char* sql )
				 * @brief This methos is called by prepare method (override method)
				 * @param sql prepared SQL
				 * @return statement object (If NULL, prepareing is failed.)
				 */
				virtual kome::db::Statement* onPrepare( const char* sql );

				/**
				 * @fn virtual bool onVacuum()
				 * @brief This method is called by vacuume method (override method)
				 * @return If true, it succeeded to vacuume
				 */
				virtual bool onVacuum();

				/**
				 * @fn virtual bool onInit()
				 * @brief This method is called by initDb method (override method)
				 * @return If true, it succeeded to initialize database
				 */
				virtual bool onInit();

				/**
				 * @fn virtual bool onHasTable( const char* tableName )
				 * @brief This method is called by hasTable method. (override method)
				 * @param[in] tableName table name
				 * @return If true, the specified table exists.
				 */
				virtual bool onHasTable( const char* tableName );
			};
		}
	}
}


#endif	// __KOME_DB_POSTGRESQL_DB_H__
