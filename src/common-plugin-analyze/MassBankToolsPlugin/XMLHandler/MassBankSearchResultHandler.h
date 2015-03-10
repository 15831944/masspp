/**
 * @file MassBankSearchResultHandler.h
 * @brief interfaces of MassBankSearchResultHandler class
 *
 * @author S.Tanaka
 * @date 2009.09.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MASBANK_SEARCH_RESULT_HANDLER_H__
#define __KOME_MASBANK_SEARCH_RESULT_HANDLER_H__


#include "../MassBankManager.h"

namespace kome {
	namespace massbank {

		/**
		 * @class MassBankSearchResultHandler
		 * @brief MassBank result handler class
		 */
		class MassBankSearchResultHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn MassBankSearchResultHandler( std::vector< MassBankManager::SearchResult >& results )
			 * @brief constructor
			 * @param results the array to store search results
			 */
			MassBankSearchResultHandler(std::vector< MassBankManager::SearchResult >& results);

			/**
			 * @fn virtual ~MassBankSearchResultHandler()
			 * @brief destructor
			 */
			virtual ~MassBankSearchResultHandler();

			bool isOK() const {return m_error.empty();}

		protected:
			/** results array */
			std::vector< MassBankManager::SearchResult >& m_results;

			/** current result */
			MassBankManager::SearchResult m_currResult;

			/**  result map */
			std::map< std::string, int > m_resultMap;

			/** query name */
			std::string m_query;
			std::string m_error;

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
			 * @param name tag name
			 * @param attrs attributes
			 */
			virtual void onStartElement( const char* name, kome::core::Properties& attrs );

			/**
			 * @fn virtual void onEndElement( const char* name, const char* text )
			 * @brief This method is called by end element method. (override method)
			 * @param name tag name
			 * @param text body text
			 */
			virtual void onEndElement( const char* name, const char* text );
		};
	}
}


#endif	// __KOME_MASBANK_SEARCH_RESULT_HANDLER_H__
