/**
 * @file EnzymeHandler.cpp
 * @brief implements of EnzymeHandler class
 *
 * @author S.Tanaka
 * @date 2012.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "EnzymeHandler.h"

#include "Substance.h"
#include "Hit.h"
#include "IdentificationManager.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// define
#define TAG_ENZYME            "enz"
#define TAG_NAME              "name"
#define TAG_CLEAVE            "cleave"
#define TAG_NON_CLEAVE        "non-cleave"

#define ATTR_TERM_ENZYME      "term"


// constructor
EnzymeHandler::EnzymeHandler( std::vector< Enzyme* >& enzymes ) : m_enzymes( enzymes ) {
	// initialize
	m_name = "";
	m_term = Enzyme::UNKNOWN;
	m_cleave = "";
	m_noncleave = "";
}

// destructor
EnzymeHandler::~EnzymeHandler() {
}

// start document
void EnzymeHandler::onStartDocument() {
	// clear
	m_enzymes.clear();

	// initialize
	m_name = "";
	m_term = Enzyme::UNKNOWN;
	m_cleave = "";
	m_noncleave = "";
}

// end document
void EnzymeHandler::onEndDocument() {
	// initialize
	m_name = "";
	m_term = Enzyme::UNKNOWN;
	m_cleave = "";
	m_noncleave = "";
}

// start element
void EnzymeHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tags
	if( strcmp( name, TAG_ENZYME ) == 0 ) {    // <enz>
		std::string term = attrs.getStringValue( ATTR_TERM_ENZYME, "" );
		if( term.compare( "N" ) == 0 ) {
			m_term = Enzyme::N_TERM;
		}
		else if( term.compare( "C" ) == 0 ) {
			m_term = Enzyme::C_TERM;
		}
		else {
			m_term = Enzyme::UNKNOWN;
		}
	}
}

// end element
void EnzymeHandler::onEndElement( const char* name, const char* text ) {
	// manager
	IdentificationManager& identMgr = IdentificationManager::getInstance();

	// each tags
	if( strcmp( name, TAG_ENZYME ) == 0 ) {    // </mod>
		if( !m_name.empty() ) {
			Enzyme* enzyme = new Enzyme( m_name.c_str() );
			enzyme->addCutter( m_term, m_cleave.c_str(), m_noncleave.c_str() );
			m_enzymes.push_back( enzyme );
		}

		// initialize
		m_name = "";
		m_term = Enzyme::UNKNOWN;
		m_cleave = "";
		m_noncleave = "";
	}
	else if( strcmp( name, TAG_NAME ) == 0 ) {    // </name>
		m_name = text;
	}
	else if( strcmp( name, TAG_CLEAVE ) == 0 ) {    // </cleave>
		m_cleave = text;
	}
	else if( strcmp( name, TAG_NON_CLEAVE ) == 0 ) {    // </non-cleave>
		m_noncleave = text;
	}
}
