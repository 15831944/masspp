/**
 * @file SQLiteResultSet.h
 * @brief interfaces of SQLiteResultSet class
 *
 * @author S.Tanaka
 * @date 2008.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_SQLITE_RESULT_SET_H__
#define __KOME_DB_SQLITE_RESULT_SET_H__


struct sqlite3_stmt;


namespace kome {
	namespace db {
		namespace sqlite {

			class SQLiteDb;
			class SQLiteStatement;

			/**
			 * @class SQLiteResultSet
			 * @brief database statement class
			 */
			class SQLiteResultSet : public kome::db::ResultSet {
			public:
				/**
				 * @fn SQLiteResultSet( SQLiteStatement& stmt )
				 * @brief constructor
				 * @param stmt statement
				 */
				SQLiteResultSet( SQLiteStatement& stmt );

				/**
				 * @fn virtual ~SQLiteResultSet()
				 * @brief destructor
				 */
				virtual ~SQLiteResultSet();

			protected:
				/** statement */
				SQLiteStatement& m_sqliteStmt;

			public:
				/**
				 * @fn SQLiteStatement& getSQLiteStatement()
				 * @brief gets SQLiteStatement 
				 * @return statement
				 */
				SQLiteStatement& getSQLiteStatement();

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


#endif	// __KOME_DB_SQLITE_RESULT_SET_H__
