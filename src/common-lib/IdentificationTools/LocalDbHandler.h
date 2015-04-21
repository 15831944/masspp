/**
 * @file LocalDbHandler.h
 * @brief interfaces of LocalDbHandler class
 *
 * @author S.Tanaka
 * @date 2012.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IDENT_LOCAL_DB_HANDLER_H__
#define __KOME_IDENT_LOCAL_DB_HANDLER_H__


#include "Substance.h"
#include "Hit.h"
#include "IdentificationManager.h"


namespace kome {
	namespace ident {


		/**
		 * @class LocalDbHandler
		 * @brief XML handler class to read plug-in file
		 */
		class LocalDbHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn LocalDbHandler( std::vector< DbInfo >& localDbs )
			 * @brief constructor
			 * @param[in] localDbs the array of local database information
			 */
			LocalDbHandler( std::vector< DbInfo >& localDbs );

			/**
			 * @fn virtual ~LocalDbHandler()
			 * @brief destructor
			 */
			virtual ~LocalDbHandler();

		protected:
			/** enzymes */
			std::vector< DbInfo >& m_localDbs;

        protected:
			/** current DB info */
			DbInfo* m_currentDbInfo;

			/** current local DB file information */
			LocalDbFileInfo* m_currentFile;

			/** file type */
			DbFileType m_type;

			/** format name */
			std::string m_formatName;

		protected:
			/**
			 * @fn virtual void onStartDocument()
			 * @brief This method is called by startDocument method. (override method)
			 */
			virtual void onStartDocument();

			/**
			 * @fn virtual void onEndDocument()
			 * @brief This method is called by endDocument method. (override method)
			 */
			virtual void onEndDocument();

			/**
			 * @fn virtual void onStartElement( const char* name, kome::core::Properties& attrs )
			 * @brief This method is called by startElement method. (override method)
			 * @param[in] name tag name
			 * @param[in] attrs attributes
			 */
			virtual void onStartElement( const char* name, kome::core::Properties& attrs );

			/**
			 * @fn virtual void onEndElement( const char* name, const char* text )
			 * @brief This method is called by end element method. (override method)
			 * @param[in] name tag name
			 * @param[in] text body text
			 */
			virtual void onEndElement( const char* name, const char* text );			
		};
	}
}

#endif	// __KOME_IDENT_LOCAL_DB_HANDLER_H__
