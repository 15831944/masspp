/**
 * @file CasDb.h
 * @brief interfaces of CasDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_EXTERNAL_CAS_DB_H__
#define __KOME_EXTERNAL_CAS_DB_H__


namespace kome {
	namespace external_db {

		/**
		 * @fn CasDb
		 * @brief CAS database
		 */
		class CasDb : public kome::ident::ExternalDB {
		public:
			/**
			 * @fn CasDb()
			 * @brief CAS database
			 */
			CasDb();

			/**
			 * @fn virtual ~CasDb()
			 * @brief destructor
			 */
			virtual ~CasDb();

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


#endif    // __KOME_EXTERNAL_CAS_DB_H__
