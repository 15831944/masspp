/**
 * @file Accession.h
 * @brief interfaces of Accession class
 *
 * @author S.Tanaka
 * @date 2009.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_ACCESSION_H__
#define __KOME_IO_ACCESSION_H__


namespace kome {
	namespace io {
		namespace mzml {

			/**
			 * @class Accession
			 * @brief accession information class
			 */
			class Accession {
			public:
				/**
				 * @fn Accession( const char* id, const char* name )
				 * @brief constructor
				 * @param id accession ID
				 * @param name accession name
				 */
				Accession( const char* id, const char* name );

				/**
				 * @fn Accession( const char* id )
				 * @brief constructor
				 * @param id accession ID
				 */
				Accession( const char* id );

				/**
				 * @fn virtual ~Accession()
				 * @brief destructor
				 */
				virtual ~Accession();

			protected:
				/** ID */
				std::string m_id;

				/** name */
				std::string m_name;

				/** parent */
				Accession* m_parent;

				/** origin */
				Accession* m_origin;

			public:
				/**
				 * @fn const char* getId()
				 * @brief gets accession ID
				 * @return accession ID
				 */
				const char* getId();

				/**
				 * @fn void setName( const char* name )
				 * @breif sets accession name
				 * @param name accession name
				 */
				void setName( const char* name );

				/**
				 * @fn const char* getName()
				 * @brief gets accession name
				 * @return accession name
				 */
				const char* getName();

				/**
				 * @fn void setParent( Accession* parent )
				 * @brief sets parent accession
				 * @param parent parent accession
				 */
				void setParent( Accession* parent );

				/**
				 * @fn Accession* getParent()
				 * @brief gets parent accession
				 * @return parent accession
				 */
				Accession* getParent();

				/**
				 * @fn Accession* getOrigin()
				 * @brief gets origin accesion
				 * @return origin accession
				 */
				Accession* getOrigin();
			};
		}
	}
}


#endif	// __KOME_IO_ACCESSION_H__
