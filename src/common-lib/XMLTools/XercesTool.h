/**
 * @file XercesTool.h
 * @brief interfaces of XercesTool class
 *
 * @author S.Tanaka
 * @date 2006.08.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_XML_XERCES_TOOL_H__
#define __KOME_XML_XERCES_TOOL_H__


#include <string>
#include <map>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/validators/common/Grammar.hpp>

namespace kome {
	namespace xml {

		/**
		 * @class XercesTool
		 * @brief This class has some functions that uses xercesc library.
		 */
		class XML_TOOLS_CLASS XercesTool {
		public:
			/**
			 * @fn static std::string transcode( const XMLCh* const xmlch )
			 * @brief transcode XMLCh* to std::string.
			 * @param[in] xmlch character string XMLCh*
			 * @return std::string object
			 */
			static std::string transcode( const XMLCh* const xmlch );

			/**
			 * @fn static bool hasAttribute(
					const xercesc::Attributes& attrs,
					const char* const name
				)
			 * @brief judges whether the attributes object has specified key
			 * @return If true, the attributes object has specified key
			 */
			static bool hasAttribute(
					const xercesc::Attributes& attrs,
					const char* const name
			);

			/**
			 * @fn static std::string getAttribute(
						const xercesc::Attributes& attrs,
						const char* const name
				)
			 * @brief get attribute
			 * @param[in] attrs attribute information object
			 * @param[in] name attribute name
			 * @return attribute value
			 */
			static std::string getAttribute(
					const xercesc::Attributes& attrs,
					const char* const name
			);

			/**
			 * @fn static bool initXml()
			 * @brief prepare to use xercesc library
			 * @return If it succeeded to initialize xercesc, this method returns true.
			 */
			static bool initXml();

			/**
			 * @fn static void terminateXml()
			 * @brief terminate xercesc
			 */
			static void terminateXml();

			/**
			 * @fn static xercesc::SAX2XMLReader* getParser( const char* xsdPath )
			 * @brief gets XML parser
			 * @param[in] xsdPath XML schema file
			 */
			static xercesc::SAX2XMLReader* getParser( const char* xsdPath );

		protected:
			/**
			 * @class ParserManager
			 * @brief XML parser management class
			 */
			class ParserManager {
			protected:
				/**
				 * @fn ParserManager()
				 * @brief constructor
				 */
				ParserManager();

				/**
				 * @fn virtual ~ParserManager()
				 * @brief destructor
				 */
				virtual ~ParserManager();

			public:
				/**
				 * @fn xercesc::SAX2XMLReader* getParser( const char* xsdPath )
				 * @brief gets XML parser
				 * @param[in] xsdPath XML schema file path
				 * @return XML parser
				 */
				xercesc::SAX2XMLReader* getParser( const char* xsdPath );

				/**
				 * @fn void clearParser()
				 * @brief clears parser information
				 */
				void clearParser();

			protected:
				/** parser map */
				std::map< std::string, xercesc::SAX2XMLReader* > m_parserMap;

			public:
				/**
				 * @fn static ParserManager& getInstance()
				 * @brief gets parser manager object
				 * @return parser manager object (This is the only object.)
				 */
				static ParserManager& getInstance();
			};
		};
	}
}

#endif	// __KOME_XML_XERCES_TOOL_H__
