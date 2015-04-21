/**
 * @file XmlHandler.cpp
 * @brief implements of XMLHandler class
 *
 * @author S.Tanaka
 * @date 2007.04.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "XmlHandler.h"

#include "XercesTool.h"
#include <xercesc/framework/MemBufInputSource.hpp>


using namespace kome::xml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// cosntructor
XmlHandler::XmlHandler() {
	m_error = false;
	m_parser = NULL;
}

// destructor
XmlHandler::~XmlHandler() {
}

// parse file
void XmlHandler::parse( const char* xmlFile ) {
	// create a parser
	xercesc::SAX2XMLReader* parser = kome::xml::XercesTool::getParser( NULL );
	parser->setFeature( xercesc::XMLUni::fgXercesSchema, false );
	parser->setFeature( xercesc::XMLUni::fgXercesSchemaFullChecking, false );
	parser->setFeature( xercesc::XMLUni::fgXercesLoadSchema, false );
	parser->setFeature( xercesc::XMLUni::fgXercesCalculateSrcOfs, true );

	parser->setContentHandler( this );
	parser->setErrorHandler( this );

	m_parser = parser;

	// parse
	try{
		parser->parse( xmlFile );
		setError( false );
	}
	catch( const xercesc::XMLException& e ) {
		LOG_ERROR(
			FMT(
				"XML Exception: %s",
				kome::xml::XercesTool::transcode( e.getMessage() ).c_str()
			)
		);
		setError( true );
	}
	catch( const xercesc::SAXParseException& e ) {
		LOG_ERROR(
			FMT(
				"SAX Parse Exception: %s",
				kome::xml::XercesTool::transcode( e.getMessage() ).c_str()
			)
		);
		setError( true );
	}
/*
	catch( ... ) {
		LOG_ERROR(
			FMT(
				"Unexpected Exception"
			)
		);
		setError( true );
	}
*/
}

// parse XML in the memory
void XmlHandler::parse( void* addr, const unsigned int size ) {
	// create a parser
	xercesc::SAX2XMLReader* parser = kome::xml::XercesTool::getParser( NULL );
	parser->setFeature( xercesc::XMLUni::fgXercesSchema, false );
	parser->setFeature( xercesc::XMLUni::fgXercesSchemaFullChecking, false );
	parser->setFeature( xercesc::XMLUni::fgXercesLoadSchema, false );
	parser->setFeature( xercesc::XMLUni::fgXercesCalculateSrcOfs, true );

	parser->setContentHandler( this );
	parser->setErrorHandler( this );

	// parse
	xercesc::MemBufInputSource source( (const XMLByte*)addr, size, "memory_buffer", false );
	parser->parse( source );

	try{
		parser->parse( source );
		setError( false );
	}
	catch( const xercesc::XMLException& e ) {
		LOG_ERROR(
			FMT(
				"XML Exception: %s",
				kome::xml::XercesTool::transcode( e.getMessage() ).c_str()
			)
		);
		setError( true );
	}
	catch( const xercesc::SAXParseException& e ) {
		LOG_ERROR(
			FMT(
				"SAX Parse Exception: %s",
				kome::xml::XercesTool::transcode( e.getMessage() ).c_str()
			)
		);
		setError( true );
	}
	catch( ... ) {
		LOG_ERROR(
			FMT(
				"Unexpected Exception"
			)
		);
		setError( true );
	}
}

// judges whether received notification of a error
bool XmlHandler::isError() {
	return m_error;
}

// set error flag
void XmlHandler::setError( const bool error ) {
	m_error = error;
}

// end document
void XmlHandler::endDocument() { 
	onEndDocument();
}

// end element
void XmlHandler::endElement(
		const XMLCh* const uri,
		const XMLCh* const localname,
		const XMLCh* const qname
) {
	// get name
	std::string name = XercesTool::transcode( localname );

	// trim text
	m_text = trimstring( m_text.c_str() );

	// end element
	onEndElement( name.c_str(), m_text.c_str() );

	// clear text
	m_text.clear();
}

// characters
void XmlHandler::characters(
		const XMLCh* const chars,
		const XMLSize_t length
) {
	m_text.append( XercesTool::transcode( chars ) );
}

// start document
void XmlHandler::startDocument() {
	m_error = false;
	onStartDocument();
}

// start element
void XmlHandler::startElement(
		const XMLCh* const uri,
		const XMLCh* const localname,
		const XMLCh* const qname,
		const xercesc::Attributes& attrs
) {
	// transcode
	std::string name = XercesTool::transcode( localname );
	
	// create attributes
	kome::core::Properties attributes;
	for( unsigned int i = 0; i < attrs.getLength(); i++ ) {
		std::string key = XercesTool::transcode( attrs.getLocalName( i ) );
		std::string value = XercesTool::transcode( attrs.getValue( i ) );

		if( !attributes.hasKey( key.c_str() ) ) {
			attributes.setValue( key.c_str(), value.c_str() );
		}
	}

	// start element
	onStartElement( name.c_str(), attributes );

	// clear text
	m_text.clear();
}

// warning
void XmlHandler::warning( const xercesc::SAXParseException& e ) {
	std::string msg = XercesTool::transcode( e.getMessage() );
	LOG_WARN( FMT( "SAX Parser: %s", msg.c_str() ) );
}

// error
void XmlHandler::error( const xercesc::SAXParseException& e ) {
	std::string msg = XercesTool::transcode( e.getMessage() );
	LOG_ERROR( FMT( "SAX Prser: %s", msg.c_str() ) );
	m_error = true;
}

// fatal error
void XmlHandler::fatalErrror( const xercesc::SAXParseException& e ) {
	error( e );
}

// get position
long long XmlHandler::getPosition() {
	// check the member
	if( m_parser == NULL ) {
		return -1;
	}

	// get the position 
	XMLFilePos pos = m_parser->getSrcOffset();


	return (long long)pos;
}
