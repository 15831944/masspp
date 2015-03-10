/**
 * @file SQLiteStatement.h
 * @brief interfaces of SQLiteStatement class
 *
 * @author S.Tanaka
 * @date 2008.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_SQLITE_STATEMENT_H__
#define __KOME_DB_SQLITE_STATEMENT_H__


struct sqlite3_stmt;


namespace kome {
	namespace db {
		namespace sqlite {

			class SQLiteDb;

			/**
			 * @class SQLiteStatement
			 * @brief SQLite statement class
			 */
			class SQLiteStatement : public kome::db::Statement {
			public:
				/**
				 * @fn SQLiteStatement( SQLiteDb& db, sqlite3_stmt* stmt, const char* sql )
				 * @brief constructor
				 * @param db SQLite database
				 * @param stmt statement
				 * @param sql SQL
				 */
				SQLiteStatement( SQLiteDb& db, sqlite3_stmt* stmt, const char* sql );

				/**
				 * @fn virtual ~SQLiteStatement()
				 * @brief destructor
				 */
				virtual ~SQLiteStatement();

			protected:
				/** database */
				SQLiteDb& m_sqliteDb;

				/** statement */
				sqlite3_stmt* m_stmt;

				/** dummy flag */
				bool m_dummy;

				/** query flag */
				bool m_queryFlg;

			public:
				/**
				 * @fn SQLiteDb& getSQLiteDb()
				 * @brief gets SQLite database object
				 * @return SQLite database object
				 */
				SQLiteDb& getSQLiteDb();

				/**
				 * @fn sqlite3_stmt* getStatement()
				 * @brief gets statement
				 * @return statement
				 */
				sqlite3_stmt* getStatement();

			public:
				/**
				 * @fn void setDummy( const bool dummy )
				 * @brief sets dummy flag value
				 * @return dummy flag value
				 */
				void setDummy( const bool dummy );

				/**
				 * @fn bool isDummy()
				 * @brief gets dummy flag value
				 * @return dummy flag value
				 */
				bool isDummy();

			protected:
				/**
				 * @fn virtual bool onResetBind()
				 * @brief This method is called by resetBind method. (override method)
				 * @return If true, it succeeded to reset bind
				 */
				virtual bool onResetBind();

				/**
				 * @fn virtual bool onBindString( const int pos, const char* s )
				 * @brief This method is called by bindString method. (override method)
				 * @param pos bind position
				 * @param s character string
				 * @return If true, it succeeded to bind string
				 */
				virtual bool onBindString( const int pos, const char* s );

				/**
				 * @fn virtual bool onBindInteger( const int pos, const int val )
				 * @brief This method is called by bindInteger method. (override method)
				 * @param pos bind position
				 * @param val integer value
				 * @return If true, it succeeded to bind integer
				 */
				virtual bool onBindInteger( const int pos, const int val ); 

				/**
				 * @fn virtual bool onBindReal( const int pos, const double val )
				 * @brief This method is called by bindReal method (override method)
				 * @param pos bind position
				 * @param val real number value
				 * @return If true, it succeeded to bind real number
				 */
				virtual bool onBindReal( const int pos, const double val );

				/**
				 * @fn virtual bool onBindData( const int pos, void* data, const unsigned int size )
				 * @brief This method is called by bindData method (override method)
				 * @param pos bind position
				 * @param data data position
				 * @param size data size
				 * @return If true, it succeeded to bind data
				 */
				virtual bool onBindData( const int pos, void* data, const unsigned int size );

				/**
				 * @fn virtual bool onBindNull( const int pos )
				 * @brief This method is called by bindNull method (override method)
				 * @param pos bind position
				 * @return If true, it succeeded to bind null
				 */
				virtual bool onBindNull( const int pos );

				/**
				 * @fn virtual bool onExecute()
				 * @brief This method is called by execute method (override method)
				 * @return If true, it succeeded to execute SQL
				 */
				virtual bool onExecute();

				/**
				 * @fn virtual kome::db::ResultSet* onExecuteQuery()
				 * @brief This method is called by executeQuery method (override method)
				 * @return result set object (If NULL, it failed to execute query)
				 */
				virtual kome::db::ResultSet* onExecuteQuery();
			};
		}
	}
}

#endif	// __KOME_DB_SQLITE_STATEMENT_H__
