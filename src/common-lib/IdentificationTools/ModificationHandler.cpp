/**
 * @file ModificationHandler.cpp
 * @brief implements of ModificationHandler class
 *
 * @author S.Tanaka
 * @date 2012.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ModificationHandler.h"

#include "Substance.h"
#include "Hit.h"
#include "IdentificationManager.h"
#include "Modification.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// define
#define TAG_MOD                "mod"
#define TAG_RESIDUE            "specificity"
#define TAG_MASS               "delta"

#define ATTR_TITLE_MOD         "title"
#define ATTR_HIDDEN_RESIDUE    "hidden"
#define ATTR_SITE_RESIDUE      "site"
#define ATTR_MONOMASS_MASS     "mono_mass"


// constructor
ModificationHandler::ModificationHandler( std::vector< Modification* >& mods ) : m_mods( mods ) {
	// initialize
	m_title = "";
	m_residues.clear();
	m_monomass = -1.0;
	m_hidden = false;
}

// destructor
ModificationHandler::~ModificationHandler() {
}

// start document
void ModificationHandler::onStartDocument() {
	// initialize
	m_title = "";
	m_residues.clear();
	m_monomass = -1.0;
	m_hidden = false;
}

// end document
void ModificationHandler::onEndDocument() {
	m_title = "";
	m_residues.clear();
	m_monomass = 0.0;
	m_hidden = false;
}

// start element
void ModificationHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tags
	if( strcmp( name, TAG_MOD ) == 0 ) {    // <mod>
		m_title = attrs.getStringValue( ATTR_TITLE_MOD, "" );
	}
	else if( strcmp( name, TAG_RESIDUE ) == 0 ) {    // <specificity>
		m_hidden = ( attrs.getIntValue( ATTR_HIDDEN_RESIDUE, 0 ) > 1 );
		std::string site = attrs.getStringValue( ATTR_SITE_RESIDUE, "" );

		if( !site.empty() ) {
			// searh
			int idx = -1;
			for( unsigned int i = 0; i < m_residues.size() && idx < 0; i++ ) {
				if( site.compare( m_residues[ i ] ) == 0 ) {
					idx = (int)i;
				}
			}
			if( idx < 0 ) {
				m_residues.push_back( site );
			}
		}
	}
	else if( strcmp( name, TAG_MASS ) == 0 ) {    // <delta>
		m_monomass = attrs.getDoubleValue( ATTR_MONOMASS_MASS, -1.0 );
	}
}

// end element
void ModificationHandler::onEndElement( const char* name, const char* text ) {
	// manager
	IdentificationManager& identMgr = IdentificationManager::getInstance();

	// each tags
	if( strcmp( name, TAG_MOD ) == 0 ) {    // </mod>
		if( !m_title.empty() && m_residues.size() > 0 && fabs( m_monomass ) > 0.0001 ) {
			for( unsigned int i = 0; i < m_residues.size(); i++ ) {
				std::string title = FMT( "%s (%s)", m_title.c_str(), m_residues[ i ].c_str() );
				Modification* m = new Modification( title.c_str(), m_monomass, m_residues[ i ].c_str(), m_hidden );
				m_mods.push_back( m );
			}
		}

		// initialize
		m_title = "";
		m_residues.clear();
		m_monomass = 0.0;
	}
}
