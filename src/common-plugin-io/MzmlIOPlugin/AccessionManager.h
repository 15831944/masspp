/*
 * @file AccessionManager.h
 * @brief interfaces of AccessionManager class
 *
 * @author S.Tanaka
 * @date 2009.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_ACCESSION_MANAGER_H__
#define __KOME_IO_ACCESSION_MANAGER_H__


namespace kome {
	namespace io {
		namespace mzml {

			class Accession;

			/**
			 * @class AccessionManager
			 * @brief accession object management class
			 */
			class AccessionManager {
			protected:
				/**
				 * @fn AccessionManager()
				 * @brief constructor
				 */
				AccessionManager();

				/**
				 * @fn virtual ~AccessionManager()
				 * @brief destructor
				 */
				virtual ~AccessionManager();

			protected:
				/** accessions */
				std::vector< Accession* > m_accessions;

			protected:
				/**
				 * @fn void clearAccessions()
				 * @brief clears accessions
				 */
				void clearAccessions();

			public:
				/**
				 * @fn unsigned int getNumberOfAccessions()
				 * @brief gets the number of accessions
				 * @return the number of accessions
				 */
				unsigned int getNumberOfAccessions();

				/**
				 * @fn Accession* getAccession( const unsigned int idx )
				 * @brief gets accession
				 * @param idx accession index
				 * @return accession (If NULL, the index is illegal.)
				 */
				Accession* getAccession( const unsigned int idx );

				/**
				 * @fn Accession* getAccession( const char* id )
				 * @brief gets accession
				 * @param id accession ID
				 * @return accession (If NULL, the ID is illegal.)
				 */
				Accession* getAccession( const char* id );

			protected:
				/**
				 * @fn void readOboFile( const char* path )
				 * @brief reads obo file
				 * @param path obo file path
				 */
				void readOboFile( const char* path );

			public:
				/**
				 * @fn static AccessionManager& getInstance()
				 * @brief gets accession manager object (This is the only object.)
				 * @return accession manager object
				 */
				static AccessionManager& getInstance();
			};
		}
	}
}


#endif	// __KOME_IO_ACCESSION_MANAGER_H__
