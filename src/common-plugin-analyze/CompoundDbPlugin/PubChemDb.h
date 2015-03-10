/**
 * @file PubChemDb.h
 * @brief interfaces of PubChemDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_EXTERNAL_PUBCHEM_DB_H__
#define __KOME_EXTERNAL_PUBCHEM_DB_H__


namespace kome {
	namespace external_db {

		/**
		 * @fn PubChemDb
		 * @brief CAS database
		 */
		class PubChemDb : public kome::ident::ExternalDB {
		public:
			/**
			 * @fn PubChemDb()
			 * @brief CAS database
			 */
			PubChemDb();

			/**
			 * @fn virtual ~PubChemDb()
			 * @brief destructor
			 */
			virtual ~PubChemDb();

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


#endif    // __KOME_EXTERNAL_PUBCHEM_DB_H__
