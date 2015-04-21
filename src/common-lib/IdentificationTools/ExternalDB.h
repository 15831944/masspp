/**
 * @file ExternalDB.h
 * @brief interfaces of ExternalDB class
 *
 * @author S.Tanaka
 * @date 2013.07.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IDENT_EXTERNAL_DB_H__
#define __KOME_IDENT_EXTERNAL_DB_H__


#include <string>
#include <vector>


namespace kome {
	namespace ident {

		/**
		 * @class ExternalDB
		 * @brief search engine operation management abstract class
		 */
		class IDENT_TOOLS_CLASS ExternalDB {
		public:
			/**
			 * @fn ExternalDB( const char* dbName, const char* searchFormat )
			 * @brief constructor
			 * @param[in] dbName database name
			 * @param[in] searchFormat format
			 */
			ExternalDB( const char* dbName, const char* searchFormat );

			/**
			 * @fn virtual ~ExternalDB()
			 * @brief destructor
			 */
			virtual ~ExternalDB();

		protected:
			/** DB name */
			std::string m_name;

			/** search format */
			std::string m_searchFormat;

			/** temporary folder */
			std::string m_tmpFolder;

		public:
			/**
			 * @struct SearchResult
			 * @brief search result
			 */
			struct SearchResult {
				std::string name;
				std::string accession;
			};

		public:
			/**
			 * @fn const char* getName()
			 * @brief gets the DB name
			 */
			const char* getName();

		public:
			/**
			 * @fn virtual std::string getSearchUrl( const char* keyword )
			 * @brief gets the search URL
			 * @param[in] keyword search keyword
			 * @return search URL
			 */
			virtual std::string getSearchUrl( const char* keyword );

			/**
			 * @fn void search( std::vector< SearchResult >& result, const char* key )
			 * @brief search the DB
			 * @param[out] result the array to store the search result
			 * @param[in] key keyword
			 */
			void search( std::vector< SearchResult >& result, const char* key );

		protected:
			/**
			 * @fn virtual void parse( FILE* fp, std::vector< SearchResult >& result ) = 0
			 * @brief parses the search result
			 * @param[in] fp file descriptor
			 * @param[out] result the array to store the search result
			 */
			virtual void parse( FILE* fp, std::vector< SearchResult >& result ) = 0;

		};

		/**
		 * @class ExternalDBManager
		 * @brief external database manager
		 */
		class ExternalDBManager {
		protected:
			/**
			 * @fn ExternalDBManager()
			 * @brief constructor
			 */
			ExternalDBManager();
			
			/**
			 * @fn virtual ~ExternalDBManager()
			 * @brief destructor
			 */
			virtual ~ExternalDBManager();

		protected:
			/** external database set */
			std::set< ExternalDB* > m_dbSet;

		public:
			/**
			 * @fn void addDb( ExternalDB* db )
			 * @brief adds database
			 * @param[in] db added database
			 */
			void addDb( ExternalDB* db );

			/**
			 * @fn void removeDb( ExternalDB* db )
			 * @brief removes database
			 * @param[in] db removed database
			 */
			void removeDb( ExternalDB* db );

		public:
			/**
			 * @fn static ExternalDBManager& getInstance()
			 * @brief gets external dababase manager. (This is the only object.)
			 * @return external dabatase manager
			 */
			static ExternalDBManager& getInstance();
		};
	}
}


#endif    // __KOME_IDENT_EXTERNAL_DB_H__
