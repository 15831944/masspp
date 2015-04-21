/**
 * @file Table.h
 * @brief interfaces of table of database class
 *
 * @author S.Tanaka
 * @date 2008.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_TABLE_H__
#define __KOME_DB_TABLE_H__


#include <vector>
#include <string>


namespace kome {
	namespace db {

		class TableColumn;
		class TableIndex;

		/**
		 * @class Table
		 * @brief table of database management class
		 */
		class DB_TOOLS_CLASS Table {
		public:
			/**
			 * @fn Table()
			 * @brief constructor
			 */
			Table();

			/**
			 * @fn Table( const char* name )
			 * @brief constructor
			 * @param[in] name table name
			 */
			Table( const char* name );

			/**
			 * @fn virtual ~Table()
			 * @brief destructor
			 */
			virtual ~Table();

		protected:
			/** table name */
			std::string m_name;

			/** columns */
			std::vector< TableColumn* > m_columns;

			/** indices */
			std::vector< TableIndex* > m_indices;

		public:
			/**
			 * @fn void setName( const char* name )
			 * @brief sets table name
			 * @param[in] name table name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets table name
			 * @return table name
			 */
			const char* getName();

		public:
			/**
			 * @fn unsigned int getNumberOfColumns()
			 * @brief gets the number ofl columns
			 * @return the number of columns
			 */
			unsigned int getNumberOfColumns();

			/**
			* @fn TableColumn* addColumn( const char* name )
			* @brief adds column
			* @param[in] name column name
			* @return new table column object
			*/
			TableColumn* addColumn( const char* name );

			/**
			 * @fn TableColumn* getColumn( const unsigned int col )
			 * @brief gets column
			 * @param[in] col column number
			 * @return table column object (If NULL, the column number is ilelegal.)
			 */
			TableColumn* getColumn( const unsigned int col );

		public:
			/**
			 * @fn unsigned int getNumberOfIndices()
			 * @brief gets the number of indices
			 * @return the number of indices
			 */
			unsigned int getNumberOfIndices();

			/**
			 * @fn TableIndex* addIndex( const char* name )
			 * @brief adds table index
			 * @param[in] name index name
			 */
			TableIndex* addIndex( const char* name );

			/**
			 * @fn TableIndex* getIndex( const unsigned int index )
			 * @brief gets table index number
			 * @return table index (If NULL, the index is illegal.)
			 */
			TableIndex* getIndex( const unsigned int index );
		};
	}
}


#endif	// __KOME_DB_TABLE_H__
