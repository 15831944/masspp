/**
 * @file KeggDb.h
 * @brief interfaces of KeggDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_EXTERNAL_KEGG_DB_H__
#define __KOME_EXTERNAL_KEGG_DB_H__


namespace kome {
	namespace external_db {

		/**
		 * @fn KeggDb
		 * @brief CAS database
		 */
		class KeggDb : public kome::ident::ExternalDB {
		public:
			/**
			 * @fn KeggDb()
			 * @brief CAS database
			 */
			KeggDb();

			/**
			 * @fn virtual ~KeggDb()
			 * @brief destructor
			 */
			virtual ~KeggDb();

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


#endif    // __KOME_EXTERNAL_KEGG_DB_H__
