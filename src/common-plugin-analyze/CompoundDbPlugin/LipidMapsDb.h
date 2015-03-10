/**
 * @file LipidMapsDb.h
 * @brief interfaces of LipidMapsDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_EXTERNAL_LIPID_MAPS_DB_H__
#define __KOME_EXTERNAL_LIPID_MAPS_DB_H__


namespace kome {
	namespace external_db {

		/**
		 * @fn LipidMapsDb
		 * @brief CAS database
		 */
		class LipidMapsDb : public kome::ident::ExternalDB {
		public:
			/**
			 * @fn LipidMapsDb()
			 * @brief CAS database
			 */
			LipidMapsDb();

			/**
			 * @fn virtual ~LipidMapsDb()
			 * @brief destructor
			 */
			virtual ~LipidMapsDb();

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


#endif    // __KOME_EXTERNAL_LIPID_MAPS_DB_H__
