/**
 * @file XmlHandler.h
 * @brief interfaces of XMLHandler class
 *
 * @author S.Tanaka
 * @date 2007.04.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_XML_HANDLER_H__
#define __KOME_XML_HANDLER_H__


#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>


namespace kome {
	namespace xml {

		/**
		 * @class XmlHandler
		 * @brief XML Handler class
		 */
		class XML_TOOLS_CLASS XmlHandler : public xercesc::DefaultHandler {
		public:
			/**
			 * @fn XmlHandler()
			 * @brief constructor
			 */
			XmlHandler();

			/**
			 * @fn virtual ~XmlHandler()
			 * @brief destructor
			 */
			virtual ~XmlHandler();

		private:
			/** whether this handler received notification of a error */
			bool m_error;

			/** xml characters */
			std::string m_text;

			/** parser */
			xercesc::SAX2XMLReader* m_parser;

		public:
			/**
			 * @fn void parse( const char* xmlFile )
			 * @brief parses XML file
			 * @param[in] xmlFile XML file path
			 */
			void parse( const char* xmlFile );

			/**
			 * @fn void parse( void* addr, const unsigned int size )
			 * @brief parses XML in the memory
			 * @param[in] addr address
			 * @param[in] size XML size
			 */
			void parse( void* addr, const unsigned int size );

		public:
			/**
			 * @fn bool isError()
			 * @brief judges whether this handler received notification of a error
			 * @return If this handler received notification of a error, this method returns true
			 */
			bool isError();

			/**
			 * @fn void setError( const bool error )
			 * @brief sets error flag
			 * @param[in] error error flag value
			 */
			void setError( const bool error );

		public:
			/** 
			 * @fn virtual void endDocument()
			 * @brief This method is called at the end of the document.
			 */
			virtual void endDocument();

			/**
			 * @fn virtual void endElement(
						const XMLCh* const uri,
						const XMLCh* const localname,
						const XMLCh* const qname
				)
			 * @brief This method is called at the end of the element.
			 * @param[in] uri the URI of the asscioated namespace for this element
			 * @param[in] localname the local part of the element name
			 * @param[in] qname the QName of this element
			 */
			virtual void endElement(
					const XMLCh* const uri,
					const XMLCh* const localname,
					const XMLCh* const qname
			);

			/**
			 * @fn virtual void characters( const XMLCh* const chars, const XMLSize_t length )
			 * @brief This method is called at character data.
			 * @param[in] chars the characters from the XML document
			 * @param[in] length the number of characters to read form the array
			 */
			virtual void characters( const XMLCh* const chars, const XMLSize_t length );

			/**
			 * @fn virtual void startDocument()
			 * @brief This method is called at the start of the document.
			 */
			virtual void startDocument();

			/**
			 * @fn virtual void startElement(
					const XMLCh* const uri,
					const XMLCh* const localname,
					const XMLCh* const qname,
					const xercesc::Attributes& attrs
				)
			 * @brief This method is called at the start of the element.
			 * @param[in] uri the URI of the asscioated namespace for this element
			 * @param[in] localname the local part of the element name
			 * @param[in] qname the QName of this element
			 * @param[in] attrs attrs of the element
			 */
			virtual void startElement(
				const XMLCh* const uri,
				const XMLCh* const localname,
				const XMLCh* const qname,
				const xercesc::Attributes& attrs
			);

			/**
			 * @fn virtual void warning( const xercesc::SAXParseException& e )
			 * @brief This method is called when this handler receives notification of a warning. 
			 * @param[in] e exception object
			 */
			virtual void warning( const xercesc::SAXParseException& e );

			/**
			 * @fn virtual void error( const xercesc::SAXParseException& e )
			 * @brief This method is called when this hadler receives notification of a recoverable error. 
			 * @param[in] e exception object
			 */
			virtual void error( const xercesc::SAXParseException& e );

			/**
			 * @fn virtual void fatalErrror( const xercesc::SAXParseException& e )
			 * @brief This method is called when this hadler receives notification of a non-recoverable error. 
			 * @param[in] e exception object
			 */
			virtual void fatalErrror( const xercesc::SAXParseException& e );

		protected:
			/**
			 * @fn long long getPosition()
			 * @brief gets the position
			 * @return position
			 */
			long long getPosition();

		protected:
			/**
			 * @fn virtual void onStartDocument() = 0
			 * @brief This method is called by startDocument method. (abstract method)
			 */
			virtual void onStartDocument() = 0;

			/**
			 * @fn virtual void onEndDocument() = 0
			 * @brief This method is called by endDocument method. (abstract method)
			 */
			virtual void onEndDocument() = 0;

			/**
			 * @fn virtual void onStartElement( const char* name, kome::core::Properties& attrs ) = 0
			 * @brief This method is called by startElement method. (abstract method)
			 * @param[in] name tag name
			 * @param[in] attrs attributes
			 */
			virtual void onStartElement( const char* name, kome::core::Properties& attrs ) = 0;

			/**
			 * @fn virtual void onEndElement( const char* name, const char* text ) = 0
			 * @brief This method is called by end element method. (abstract method)
			 * @param[in] name tag name
			 * @param[in] text body text
			 */
			virtual void onEndElement( const char* name, const char* text ) = 0;
		};
	}
}

#endif		// __KOME_XML_HANDLER_H__
