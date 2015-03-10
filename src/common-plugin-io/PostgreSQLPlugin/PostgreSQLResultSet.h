/**
 * @file PostgreSQLResultSet.h
 * @brief interfaces of PostgreSQLResultSet class
 *
 * @author N.Tashiro
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_DB_POSTGRESQL_RESULT_SET_H__
#define __KOME_DB_POSTGRESQL_RESULT_SET_H__


//struct sqlite3_stmt;


namespace kome {
	namespace db {
		namespace postgresql {

			class PostgreSQL;
			class PostgreSQLStatement;

			/**
			 * @class PostgreSQLResultSet
			 * @brief database statement class
			 */
			class PostgreSQLResultSet : public kome::db::ResultSet {
			public:
				/**
				 * @fn PostgreSQLResultSet( PostgreSQLStatement& stmt )
				 * @brief constructor
				 * @param stmt statement
				 */
				PostgreSQLResultSet( PostgreSQLStatement& stmt );

				/**
				 * @fn virtual ~PostgreSQLResultSet()
				 * @brief destructor
				 */
				virtual ~PostgreSQLResultSet();

			protected:
				/** statement */
				PostgreSQLStatement& m_postgresqlStmt;

				/** tuples */
				int m_nTuples;

			public:
				/**
				 * @fn PostgreSQLStatement& getPostgreSQLStatement()
				 * @brief gets PostgreSQLStatement 
				 * @return statement
				 */
				PostgreSQLStatement& getPostgreSQLStatement();

			protected:
				/**
				 * @fn virtual bool onNext()
				 * @brief This method is called by next method. (override method)
				 * @return If true, the current item is valid.
				 */
				virtual bool onNext();

				/**
				 * @fn std::string virtual onGetString( const int col, const char* df )
				 * @brief This method is called by getString method. (override method)
				 * @param col column index
				 * @param df default value
				 * @return string value
				 */
				virtual std::string onGetString( const int col, const char* df );

				/**
				 * @fn int virtual onGetInt( const int col, const int df )
				 * @brief This method is called by getInt method. (override method)
				 * @param col column index
				 * @param df default value
				 * @return integer value
				 */
				virtual int onGetInt( const int col, const int df );

				/**
				 * @fn virtual double onGetReal( const int col, const double df )
				 * @brief gets This method is called by getReal method. (override method)
				 * @param col column index
				 * @param df default value
				 * @return real number value
				 */
				virtual double onGetReal( const int col, const double df );

				/**
				 * @fn virtual void* onGetData( const int col )
				 * @brief This method is called by getData method. (override method)
				 * @param col column index
				 * @return data
				 */
				virtual void* onGetData( const int col );

				/**
				 * @fn virtual int onGetDataSize( const int col )
				 * @brief This method is called by getDataSize method. (override method)
				 * @param col column index
				 * @return data size
				 */
				virtual int onGetDataSize( const int col );
			};
		}
	}
}


#endif	// __KOME_DB_POSTGRESQL_RESULT_SET_H__
