/**
 * @file Statement.h
 * @brief interfaces of Statement class
 *
 * @author S.Tanaka
 * @date 2008.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_STATEMENT_H__
#define __KOME_DB_STATEMENT_H__


namespace kome {
	namespace db {

		class Database;
		class ResultSet;

		/**
		 * @class Statement
		 * @brief database statement class
		 */
		class DB_TOOLS_CLASS Statement {
		public:
			/**
			 * @fn Statement( Database& db, const char* sql )
			 * @brief constructor
			 * @param[in] db database
			 * @param[in] sql SQL
			 */
			Statement( Database& db, const char* sql );

			/**
			 * @fn virtual ~Statement()
			 * @brief destructor
			 */
			virtual ~Statement();

		protected:
			/** database */
			Database& m_db;

			/** SQL */
			std::string m_sql;

		public:
			/**
			 * @fn Database& getDb()
			 * @brief gets database
			 * @return database
			 */
			Database& getDb();

		public:
			/**
			 * @fn void close()
			 * @brief closes statement
			 */
			void close();

		public:
			/**
			 * @fn bool resetBind()
			 * @brief resets binded parameters
			 * @return If true, it succeeded to reset bind
			 */ 
			bool resetBind();

			/**
			 * @fn bool bindString( const int pos, const char* s )
			 * @brief binds string value
			 * @param[in] pos bind position
			 * @param[in] s character string
			 * @return If true, it succeeded to bind string
			 */
			bool bindString( const int pos, const char* s );

			/**
			 * @fn bool bindInteger( const int pos, const int val )
			 * @brief binds integer value
			 * @param[in] pos bind position
			 * @param[in] val integer value
			 * @return If true, it succeeded to bind integer
			 */
			bool bindInteger( const int pos, const int val );

			/**
			 * @fn bool bindReal( const int pos, const double val )
			 * @brief binds real number value
			 * @param[in] pos bind position
			 * @param[in] val real number value
			 * @return If true, it succeeded to bind real number
			 */
			bool bindReal( const int pos, const double val );

			/**
			 * @fn bool bindData( const int pos, void* data, const unsigned int size )
			 * @brief binds data
			 * @param[in] pos bind position
			 * @param[in] data data position
			 * @param[in] size data size
			 * @return If true, it succeeded to bind data
			 */
			bool bindData( const int pos, void* data, const unsigned int size );

			/**
			 * @fn bool bindNull( const int pos )
			 * @brief binds null
			 * @param[in] pos bind position
			 * @return If true, it succeeded to bind null
			 */
			bool bindNull( const int pos );

			/**
			 * @fn bool execute()
			 * @brief executes statement
			 * @return If true, it succeeded to execute SQL
			 */
			bool execute();

			/**
			 * @fn ResultSet* executeQuery()
			 * @brief executes query
			 * @return result set object (If NULL, it failed to execute query)
			 */
			ResultSet* executeQuery();

		protected:
			/**
			 * @fn virtual bool onResetBind() = 0
			 * @brief This method is called by resetBind method. (abstract method)
			 * @return If true, it succeeded to reset bind
			 */
			 virtual bool onResetBind() = 0;

			/**
			 * @fn virtual bool onBindString( const int pos, const char* s ) = 0
			 * @brief This method is called by bindString method. (abstract method)
			 * @param[in] pos bind position
			 * @param[in] s character string
			 * @return If true, it succeeded to bind string
			 */
			virtual bool onBindString( const int pos, const char* s ) = 0;

			/**
			 * @fn virtual bool onBindInteger( const int pos, const int val ) = 0
			 * @brief This method is called by bindInteger method. (abstract method)
			 * @param[in] pos bind position
			 * @param[in] val integer value
			 * @return If true, it succeeded to bind integer
			 */
			virtual bool onBindInteger( const int pos, const int val ) = 0; 

			/**
			 * @fn virtual bool onBindReal( const int pos, const double val ) = 0
			 * @brief This method is called by bindReal method (abstract method)
			 * @param[in] pos bind position
			 * @param[in] val real number value
			 * @return If true, it succeeded to bind real number
			 */
			virtual bool onBindReal( const int pos, const double val ) = 0;

			/**
			 * @fn virtual bool onBindData( const int pos, void* data, const unsigned int size ) = 0
			 * @brief This method is called by bindData method (abstract method)
			 * @param[in] pos bind position
			 * @param[in] data data position
			 * @param[in] size data size
			 * @return If true, it succeeded to bind data
			 */
			virtual bool onBindData( const int pos, void* data, const unsigned int size ) = 0;

			/**
			 * @fn virtual bool onBindNull( const int pos ) = 0
			 * @brief This method is called by bindNull method (abstract method)
			 * @param[in] pos bind position
			 * @return If true, it succeeded to bind null
			 */
			virtual bool onBindNull( const int pos ) = 0;

			/**
			 * @fn virtual bool onExecute() = 0
			 * @brief This method is called by execute method (abstract method)
			 * @return If true, it succeeded to execute SQL
			 */
			virtual bool onExecute() = 0;

			/**
			 * @fn virtual ResultSet* onExecuteQuery() = 0
			 * @brief This method is called by executeQuery method (abstract method)
			 * @return result set object (If NULL, it failed to execute query)
			 */
			virtual ResultSet* onExecuteQuery() = 0;
		};
	}
}


#endif	// __KOME_DB_STATEMENT_H__
