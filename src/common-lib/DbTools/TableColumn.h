/**
 * @file TableColumn.h
 * @brief interfaces of TableColumn class
 *
 * @author S.Tanaka
 * @date 2008.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DB_TABLE_COLUMN_H__
#define __KOME_DB_TABLE_COLUMN_H__


#include <string>


namespace kome {
	namespace db {

		class Table;

		/**
		 * @class TableColumn
		 * @brief table column management class
		 */
		class DB_TOOLS_CLASS TableColumn {
		public:
			/**
			 * @fn TableColumn( Table* table )
			 * @brief constructor
			 * @param[in] table table
			 */
			TableColumn( Table* table );

			/**
			 * @fn virtual ~TableColumn()
			 * @brief destructor
			 */
			virtual ~TableColumn();

		public:
			/**
			 * @typedef DataType
			 * @brief column data type
			 */
			typedef enum {
				VARCHAR = 0,
				INTEGER = 1,
				REAL	= 2,
				BINARY  = 3
			} DataType;

		protected:
			/** table */
			Table* m_table;

			/** column name */
			std::string m_name;

			/** data type */
			DataType m_type;

			/** data length */
			int m_length;

			/** not null flag */
			bool m_notNull;

			/** auto increment flag */
			bool m_autoIncrement;

		public:
			/**
			 * @fn Table* getTable()
			 * @brief gets table
			 * @return table
			 */
			Table* getTable();

			/**
			 * @fn void setName( const char* name )
			 * @brief sets column name
			 * @param[in] name column name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName()
			 * @brief gets column name
			 * @return column name
			 */
			const char* getName();

			/**
			 * @fn void setDataType( const DataType type )
			 * @brief sets data type
			 * @param[in] type data type
			 */
			void setDataType( const DataType type );

			/**
			 * @fn DataType getDataType()
			 * @brief gets data tyep
			 * @return data type
			 */
			DataType getDataType();

			/**
			 * @fn void setDataLength( const int length )
			 * @brief sets data length
			 * @param[in] length data length
			 */
			void setDataLength( const int length );

			/**
			 * @fn int getDataLength()
			 * @brief gets data length
			 * @return data length
			 */
			int getDataLength();

			/**
			 * @fn void setNotNull( const bool notNull )
			 * @brief sets not null
			 * @param[in] notNull not null flag value
			 */
			void setNotNull( const bool notNull );

			/**
			 * @fn bool isNotNull()
			 * @brief ges not null flag value
			 * @return not null flag value
			 */
			bool isNotNull();

			/**
			 * @fn void setAutoIncrement( const bool autoIncrement )
			 * @brief sets auto increment flag value
			 * @param[in] autoIncrement auto increment flag value
			 */
			void setAutoIncrement( const bool autoIncrement );

			/**
			 * @fn bool isAutoIncrement()
			 * @brief gets auto increment flag value
			 * @return auto increment flag value
			 */
			bool isAutoIncrement();
		};
	}
}


#endif	// __KOME_DB_TABLE_COLUMN_H__
