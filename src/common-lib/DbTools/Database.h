/**
 * @file Database.h
 * @brief interfaces of Database class
 *
 * @author S.Tanaka
 * @date 2008.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_DATABASE_H__
#define __KOME_DB_DATABASE_H__


#include <set>


namespace kome {
	namespace db {

		class Table;
		class TableColumn;
		class TableIndex;
		class Statement;
		class ResultSet;

		/**
		 * @class Database
		 * @brief database management class
		 */
		class DB_TOOLS_CLASS Database {
		public:
			/**
			 * @fn Database()
			 * @brief constructor
			 */
			Database();

			/**
			 * @fn virtual ~Database()
			 * @brief destructor
			 */
			virtual ~Database();

		protected:
			/** closed flag */
			bool m_closed;

			/** initialized flag */
			bool m_initialized;

			/** auto commint flag */
			bool m_autoCommit;

		protected:
			/** the set of result set */
			std::set< ResultSet* > m_resultSets;

			/**  the set of statement */
			std::set< Statement* > m_statements;

		public:
			/**
			 * @fn void clearSqlData()
			 * @brief clears result sets and statements
			 */
			void clearSqlData();

			/**
			 * @fn void addResultSet( ResultSet* resultSet )
			 * @brief adds result set
			 * @param[in] resultSet result set object to be added
			 */
			void addResultSet( ResultSet* resultSet );

			/**
			 * @fn void removeResultSet( ResultSet* resultSet )
			 * @brief removes result set
			 * @param[in] resultSet result set object to be removed
			 */
			void removeResultSet( ResultSet* resultSet );

			/**
			 * @fn unsigned int getNumberOfResultSets()
			 * @brief gets the number of result sets
			 * @return the number of result sets
			 */
			unsigned int getNumberOfResultSets();

			/**
			 * @fn void addStatement( Statement* statement )
			 * @brief adds statement
			 * @param[in] statement statement object to be added
			 */
			void addStatement( Statement* statement );

			/**
			 * @fn void removeStatement( Statement* statement )
			 * @brief removes statemetn
			 * @param[in] statement statement object to be removed
			 */
			void removeStatement( Statement* statement );

			/**
			 * @fn unsigned int getNumberOfStatements()
			 * @brief gets the number of statements
			 * @return the number of statements
			 */
			unsigned int getNumberOfStatements();

		public:
			/**
			 * @fn bool isClosed()
			 * @brief judges whether this database is closed or not
			 * @return If true, this database is closed
			 */
			bool isClosed();

			/**
			 * @fn bool isInitialized()
			 * @brief judges this database is initialized or not
			 * @return If true, this database is initialized
			 */
			bool isInitialized();

			/**
			 * @fn void setAutoCommit( const bool autoCommit )
			 * @brief set auto commit mode
			 * @param[in] autoCommit auto commit mode or not
			 */
			void setAutoCommit( const bool autoCommit );

			/**
			 * @fn bool isAutoCommit()
			 * @brief judges whether this database is auto commit mode or not
			 * @return If true, this database is auto commit mode
			 */
			bool isAutoCommit();

		public:
			/**
			 * @fn bool close()
			 * @brief closes database
			 * @return If true, it succeeded to close database
			 */
			bool close();

			/**
			 * @fn bool commit()
			 * @brief commits database
			 * @return If true, it succeeded to commit database
			 */
			bool commit();

			/**
			 * @fn bool rollback()
			 * @brief rollbacks database
			 * @return If true, it succeeded to rollback database
			 */
			bool rollback();

			/** 
			 * @fn bool execute( const char* sql )
			 * @brief executes SQL
			 * @param[in] sql SQL
			 * @return If true, it succeeded to execute SQL
			 */
			bool execute( const char* sql );

			/**
			 * @fn ResultSet* executeQuery( const char* sql )
			 * @brief executes querry
			 * @param[in] sql SQL
			 * @return result set object (If false, executing query is failed.)
			 */
			ResultSet* executeQuery( const char* sql );

			/**
			 * @fn Statement* prepare( const char* sql )
			 * @brief prepares SQL
			 * @param[in] sql prepared SQL
			 * @return statement (If NULL, prepareing is failed)
			 */
			Statement* prepare( const char* sql );

			/**
			 * @fn bool vacuum()
			 * @brief vacuums database
			 * @return If true, it succeeded to vacuum.
			 */
			bool vacuum();

			/**
			 * @fn bool initDb()
			 * @brief initializes database
			 * @return If true, it succeeded to initialize database.
			 */
			bool initDb();

		public:
			/**
			 * @fn bool createTable( Table& table )
			 * @brief creates table
			 * @param[in] table databae table information
			 * @return If true, it succeeded to create table
			 */
			bool createTable( Table& table );

			/**
			 * @fn bool dropTable( Table& table )
			 * @brief drops table
			 * @param[in] table databae table information
			 * @return If true, it succeeded to drop table
			 */
			bool dropTable( Table& table );

			/**
			 * @fn bool dropTable( const char* tableName )
			 * @brief drops table
			 * @param[in] tableName table name
			 * @return If true, it succeeded to drop table
			 */
			bool dropTable( const char* tableName );

			/**
			 * @fn bool initTable( Table& table )
			 * @brief initialized table
			 * @param[in] table table
			 * @return If true, it succeeded to initialize table
			 */
			bool initTable( Table& table );

			/**
			 * @fn bool hasTable( const char* tableName )
			 * @brief judges whether the specified table exists or not.
			 * @param[in] tableName table name
			 * @return If true, the specified table exists.
			 */
			bool hasTable( const char* tableName );

			/**
			 * @fn bool createTmpTable( Table& table )
			 * @brief creates temporary table
			 * @param[in] table table information
			 * @return If true, creating temporaty table is finished successfully.
			 */
			bool createTmpTable( Table& table );

		protected:
			/**
			 * @fn bool createIndex( TableIndex& index )
			 * @brief creates index
			 * @param[in] index index
			 * @return If true, it succeeded to create index
			 */
			bool createIndex( TableIndex& index );

			/**
			 * @fn bool dropIndex( TableIndex& index )
			 * @brief drops index
			 * @param[in] index index
			 * @return If true, it succeeded to drop index
			 */
			bool dropIndex( TableIndex& index );

		public:
			/**
			 * @fn int getMaxValue( const char* table, const char* col )
			 * @brief gets the max value
			 * @param[in] table table name
			 * @param[in] col column name
			 * @return max value
			 */
			int getMaxValue( const char* table, const char* col );

			/**
			 * @fn int getRecordCount( const char* table )
			 * @brief gets the record count
			 * @param[in] table table name
			 * @return record count
			 */
			 int getRecordCount( const char* table );

		public:
			/**
			 * @fn virtual std::string getColumnString( TableColumn& col )
			 * @brief gets column string
			 * @param[in] col column information
			 * @return column string
			 */
			virtual std::string getColumnString( TableColumn& col );

		protected:
			/**
			 * @fn virtual std::string getCreateTableString( Table& table )
			 * @brief gets the sql for creating table
			 * @param[in] table table information
			 * @return SQL for creating table
			 */
			virtual std::string  getCreateTableString( Table& table );

			/**
			 * @fn virtual bool onCreateTmpTable( Table& table )
			 * @brief This method is called by createTmpTable method. (abstract method)
			 * @param[in] table table information
			 * @return If true, creating temporary table is finished successfully.
			 */
			virtual bool onCreateTmpTable( Table& table );


		protected:
			/**
			 * @fn virtual bool onClose() = 0
			 * @brief This method is called by close method (abstract method)
			 * @return If true, it succeeded to close database
			 */
			virtual bool onClose() = 0;

			/**
			 * @fn virtual bool onCommit() = 0
			 * @brief This method is called by commit method (abstract method)
			 * @return If true, it succeeded to commit database
			 */
			virtual bool onCommit() = 0;

			/**
			 * @fn virtual bool onRollback() = 0
			 * @brief This method is called by rollback method (abstract method)
			 * @return If true, it succeeded to rollback database
			 */
			virtual bool onRollback() = 0;

			/**
			 * @fn virtual bool onExecute( const char* sql ) = 0
			 * @brief This method is called by execute method
			 * @param[in] sql SQL
			 * @return If true, it succeeded to execute SQL
			 */
			virtual bool onExecute( const char* sql ) = 0;

			/**
			 * @fn virtual ResultSet* onExecuteQuery( const char* sql ) = 0
			 * @brief This method is called by executeQuery method (abstract method)
			 * @param[in] sql SLQ
			 * @return result set object (If NULL, executing query is failed.)
			 */
			 virtual ResultSet* onExecuteQuery( const char* sql ) = 0;

			 /**
			  * @fn virtual Statement* onPrepare( const char* sql ) = 0
			  * @brief This methos is called by prepare method (abstract method)
			  * @param[in] sql prepared SQL
			  * @return statement object (If NULL, prepareing is failed.)
			  */
			 virtual Statement* onPrepare( const char* sql ) = 0;

			 /**
			  * @fn virtual bool onVacuum() = 0
			  * @brief This method is called by vacuum method (abstract method)
			  * @return If true, it succeeded to vacuum
			  */
			 virtual bool onVacuum() = 0;

			 /**
			  * @fn virtual bool onInit() = 0
			  * @brief This method is called by initDb method (abstract method)
			  * @return If true, it succeeded to initialize database
			  */
			 virtual bool onInit() = 0;

			/**
			 * @fn virtual bool onHasTable( const char* tableName ) = 0
			 * @brief This method is called by hasTable method. (abstract method)
			 * @param[in] tableName table name
			 * @return If true, the specified table exists.
			 */
			virtual bool onHasTable( const char* tableName ) = 0;
		};
	}
}


#endif	// __KOME_DB_DATABASE_H__
