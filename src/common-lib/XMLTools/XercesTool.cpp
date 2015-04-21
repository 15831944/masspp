/**
 * @file XercesTool.cpp
 * @brief implements of XercesTool class
 *
 * @author S.Tanaka
 * @date 2006.08.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "XercesTool.h"


using namespace kome::xml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// XMLCh* -> std::string
std::string XercesTool::transcode( const XMLCh* const xmlch ) {
	// check parameter
	if( xmlch == NULL ) {
		return std::string( "" );
	}

	// XMLCh* -> char*
	char* buff = xercesc::XMLString::transcode( xmlch );

	// char* -> std::string
	std::string s( NVL( buff, "" ) );

	// release
	if( buff != NULL ) {
		xercesc::XMLString::release( &buff );
	}

	return s;
}

// judge whether the attributes object has specified key
bool XercesTool::hasAttribute(
		const xercesc::Attributes& attrs,
		const char* const name
) {
	// check parameter
	if( name == NULL ) {
		return false;
	}

	// transcode name
	XMLCh* key = xercesc::XMLString::transcode( name );

	// judges
	bool ret = ( attrs.getValue( key ) != NULL );

	// release
	if( key != NULL ) {
		xercesc::XMLString::release( &key );
	}

	return ret;
}

// get attribute
std::string XercesTool::getAttribute(
		const xercesc::Attributes& attrs,
		const char* const name
) {
	// check parameter
	if( name == NULL ) {
		return std::string( "" );
	}

	// transcode name
	XMLCh* key = xercesc::XMLString::transcode( name );

	// get attribute value
	std::string value = transcode( attrs.getValue( key ) );

	// release
	if( key != NULL ) {
		xercesc::XMLString::release( &key );
	}

	return value;
}

// initialize xercesc
bool XercesTool::initXml() {
	try{
		xercesc::XMLPlatformUtils::Initialize();
	}
	catch( xercesc::XMLException& e ) {
		std::string msg = transcode( e.getMessage() );
		LOG_FATAL(
			FMT(
				"xercesc::XMLPlatformUtils::Initialize() threw an exception. (%s)",
				msg.c_str()
			)
		);
	
		return false;
	}

	return true;
}

// terminate xercesc
void XercesTool::terminateXml() {
	// clear parser
	ParserManager::getInstance().clearParser();

	// terminate XML
	try{
		xercesc::XMLPlatformUtils::Terminate();
	}
	catch( xercesc::XMLException& e ) {
		std::string msg = transcode( e.getMessage() );
		LOG_ERROR(
			FMT(
				"Failed to terminate XML. (%s)",
				msg.c_str()
			)
		);
	}
}

// get parser
xercesc::SAX2XMLReader* XercesTool::getParser( const char* xsdPath ) {
	return ParserManager::getInstance().getParser( xsdPath );
}


/////////////////// Parser Manager ///////////////////////

// constructor
XercesTool::ParserManager::ParserManager() {
}

// destructor
XercesTool::ParserManager::~ParserManager() {
}

// get parser
xercesc::SAX2XMLReader* XercesTool::ParserManager::getParser( const char* xsdPath ) {
	// check path
	std::string path;
	if( xsdPath != NULL ) {
		if( checkfile( xsdPath ) ) {
			path = absolutepath( xsdPath );
		}
		else {
			LOG_ERROR( FMT( "Illegal schema file path [%s]", NVL( xsdPath, "" ) ) );
		}
	}

	// check the map
	if( m_parserMap.find( path ) != m_parserMap.end() ) {
		return m_parserMap[ path ];
	}

	// create the parser
	xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
	m_parserMap[ path ] = parser;

	if( !path.empty() ) {   // load schema file
		LOG_DEBUG( FMT( "Loading settings schema... [%s]", NVL( xsdPath, "" ) ) );

		try{
			parser->loadGrammar( path.c_str(), xercesc::Grammar::SchemaGrammarType, true );

			parser->setFeature( xercesc::XMLUni::fgXercesUseCachedGrammarInParse, true );
			parser->setFeature( xercesc::XMLUni::fgXercesSchema, true );
			parser->setFeature( xercesc::XMLUni::fgXercesSchemaFullChecking, true );
			parser->setFeature( xercesc::XMLUni::fgSAX2CoreValidation, true );
			parser->setFeature( xercesc::XMLUni::fgXercesDynamic, true );
		}
		catch( xercesc::XMLException& e ) {
			parser->setFeature( xercesc::XMLUni::fgSAX2CoreValidation, false );
			std::string msg = kome::xml::XercesTool::transcode( e.getMessage() );
			LOG_ERROR(
				FMT(
					"XML schema [%s] is illegal. (%s)",
					path.c_str(),
					msg.c_str()
				)
			);
		}
	}

	return parser;
}

// clear parser
void XercesTool::ParserManager::clearParser() {
	// delete parser
	std::map< std::string, xercesc::SAX2XMLReader* >::iterator it;
	for( it = m_parserMap.begin(); it!= m_parserMap.end(); it++ ) {
		xercesc::SAX2XMLReader* parser = (*it).second;

		if( parser != NULL ) {
			delete parser;
		}
	}

	// clear map
	m_parserMap.clear();
}

// get instance
XercesTool::ParserManager& XercesTool::ParserManager::getInstance() {
	// create object (This is the only object.)
	static ParserManager mgr;


	return mgr;
}
