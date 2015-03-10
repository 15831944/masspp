/**
 * @file PostgreSQLStatement.h
 * @brief interfaces of PostgreSQLStatement class
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_DB_POSTGRESQL_STATEMENT_H__
#define __KOME_DB_POSTGRESQL_STATEMENT_H__


//struct sqlite3_stmt;


namespace kome {
	namespace db {
		namespace postgresql {

			class PostgreSQL;

			/**
			 * @class PostgreSQLStatement
			 * @brief PostgreSQL statement class
			 */
			class PostgreSQLStatement : public kome::db::Statement {
			public:
				/**
				 * @fn PostgreSQLStatement( PostgreSQL& db, const char* stmt, const char* sql, int params )
				 * @brief constructor
				 * @param db PostgreSQL database
				 * @param stmt statement
				 * @param sql SQL
				 * @param params number of parameters in SQL statement
				 */
				PostgreSQLStatement( PostgreSQL& db, const char* stmt, const char* sql, int params );

				/**
				 * @fn virtual ~PostgreSQLStatement()
				 * @brief destructor
				 */
				virtual ~PostgreSQLStatement();

			public:
				/** result of SQL statement */
				PGresult *m_result;

			protected:
				/** database */
				PostgreSQL& m_PostgreSQL;

				/** statement */
				std::string m_stmt;

				/** dummy flag */
				bool m_dummy;

				/** number of parameters in the SQL statement */
				int m_nParams;

				/** parameters of SQL statement */
				char** m_ppnParamValues;

				/** length of parameters in the SQL statement */
				int* m_pnParamLength;

				/** format of parameters in the SQL statement */
				int* m_pnParamFormats;

			public:
				/**
				 * @fn PostgreSQL& getPostgreSQL()
				 * @brief gets PostgreSQL database object
				 * @return PostgreSQL database object
				 */
				PostgreSQL& getPostgreSQL();

				/**
				 * @fn sqlite3_stmt* getStatement()
				 * @brief gets statement
				 * @return statement
				 */
				const char* getStatement();

				/**
				 * @fn bool executePrepared()
				 * @brief execute prepared SQL
				 * @return If true, it suceeded to execute SQL
				 */
				ExecStatusType executePrepared();

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

#endif	// __KOME_DB_POSTGRESQL_STATEMENT_H__
