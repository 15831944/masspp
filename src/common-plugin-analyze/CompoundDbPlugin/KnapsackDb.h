/**
 * @file KnapsackDb.h
 * @brief interfaces of KnapsackDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_EXTERNAL_KNAPSACK_DB_H__
#define __KOME_EXTERNAL_KNAPSACK_DB_H__


namespace kome {
	namespace external_db {

		/**
		 * @fn KnapsackDb
		 * @brief CAS database
		 */
		class KnapsackDb : public kome::ident::ExternalDB {
		public:
			/**
			 * @fn KnapsackDb()
			 * @brief CAS database
			 */
			KnapsackDb();

			/**
			 * @fn virtual ~KnapsackDb()
			 * @brief destructor
			 */
			virtual ~KnapsackDb();

		protected:
			/**
			 * @fn virtual void parse( FILE* fp, std::vector< SearchResult >& result )
			 * @brief parses the search result (override method)
			 * @param[in] fp file descriptor
			 * @param[ut] the array to store the search result
			 */
			virtual void parse( FILE* fp, std::vector< SearchResult >& result );
		};
	}
}


#endif    // __KOME_EXTERNAL_KNAPSACK_DB_H__
