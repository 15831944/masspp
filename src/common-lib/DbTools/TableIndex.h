/**
 * @file TableIndex.h
 * @brief interfaces of TableIndex class
 *
 * @author S.Tanaka
 * @date 2008.09.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_TABLE_INDEX_H__
#define __KOME_DB_TABLE_INDEX_H__


#include <string>
#include <vector>


namespace kome {
	namespace db {

		class Table;
		class TableColumn;

		/**
		 * @class TableIndex
		 * @brief table index management class
		 */
		class DB_TOOLS_CLASS TableIndex {
		public:
			/**
			 * @fn TableIndex( Table* table )
			 * @brief constructor
			 * @param[in] table table
			 */
			TableIndex( Table* table );

			/**
			 * @fn virtual ~TableIndex()
			 * @brief destructor
			 */
			virtual ~TableIndex();

		protected:
			/** table */
			Table* m_table;

			/** Index name */
			std::string m_name;

		protected:
			/** columns */
			std::vector< TableColumn* > m_columns;

		public:
			/**
			 * @fn Table* getTable()
			 * @brief gets table
			 * @return table
			 */
			Table* getTable();

			/**
			 * @fn void setName( const char* name )
			 * @brief sets Index name
			 * @param[in] name Index name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets Index name
			 * @return Index name
			 */
			const char* getName();

		public:
			/**
			 * @fn void addColumn( TableColumn* col )
			 * @brief adds table columns
			 * @param[in] col table column to be added
			 */
			void addColumn( TableColumn* col );

			/**
			 * @fn unsigned int getNumberOfColumns()
			 * @brief gets the number of table columns
			 * @return the number of table columns
			 */
			unsigned int getNumberOfColumns();

			/**
			 * @fn TableColumn* getColumn( const unsigned int index )
			 * @brief gets table column
			 * @param[in] index column index
			 * @return table column (If NULL, the index is illegal.)
			 */
			TableColumn* getColumn( const unsigned int index );
		};
	}
}


#endif	// __KOME_DB_TABLE_INDEX_H__
