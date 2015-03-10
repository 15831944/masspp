/**
 * @file SQLiteDb.h
 * @brief interfaces of SQLiteDb class
 *
 * @author S.Tanaka
 * @date 2008.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_SQLITE_DB_H__
#define __KOME_DB_SQLITE_DB_H__



struct sqlite3;


namespace kome {
	namespace db {
		namespace sqlite {

			/**
			 * @class SQLiteDb
			 * @brief SQLite database management class
			 */
			class SQLiteDb : public kome::db::Database {
			public:
				/**
				 * @fn SQLiteDb( const char* path, sqlite3* db )
				 * @brief constructor
				 * @param path DB file path
				 * @param db sqlite3 struct
				 */
				SQLiteDb( const char* path, sqlite3* db );

				/**
				 * @fn virtual ~SQLiteDb()
				 * @brief destructor
				 */
				virtual ~SQLiteDb();

			protected:
				/** file path */
				std::string m_path;

				/** sqlite3 struct */
				sqlite3* m_db;

			public:
				/**
				 * @fn sqlite3* getDb()
				 * @brief gets sqlite3 struct
				 * @return sqlite3 struct
				 */
				sqlite3* getDb();

				/**
				 * @fn void setDb( sqlite3* db )
				 * @brief sets the sqlite3 database
				 */
				void setDb( sqlite3* db );

			public:
				/**
				 * @fn const char* getPath()
				 * @brief get path
				 * @return get file path
				 */
				const char* getPath();

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


#endif	// __KOME_DB_SQLITE_DB_H__
