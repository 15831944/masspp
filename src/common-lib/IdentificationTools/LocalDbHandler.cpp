/**
 * @file LocalDbHandler.cpp
 * @brief implements of LocalDbHandler class
 *
 * @author S.Tanaka
 * @date 2012.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LocalDbHandler.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// define
#define TAG_NAME                  "name"
#define TAG_WEB                   "web"
#define TAG_FORMAT                "format"
#define TAG_URL                   "url"
#define TAG_FILE_NAME             "file-name"
#define TAG_PATH                  "path"
#define TAG_DB_FILE               "db-file"
#define TAG_REF                   "ref"
#define TAG_DB                    "db"
#define TAG_ACC_REGX              "acc-regx"
#define TAG_DESCRIPTION           "description"
#define TAG_RESIDUES              "residues"

#define ATTR_TYPE_PATH            "type"


// constructor
LocalDbHandler::LocalDbHandler( std::vector< DbInfo >& localDbs ) : m_localDbs( localDbs ) {
	onStartDocument();
}

// destructor
LocalDbHandler::~LocalDbHandler() {
}

// start document
void LocalDbHandler::onStartDocument() {
	// initialize
	m_currentDbInfo = NULL;
	m_currentFile = NULL;
	m_type = TYPE_OTHER;
	m_formatName = "";
}

// end document
void LocalDbHandler::onEndDocument() {
}

// start element
void LocalDbHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tags
	if( strcmp( name, TAG_PATH ) == 0 ) {    // <path>
		std::string type = attrs.getStringValue( ATTR_TYPE_PATH, "" );
		type = touppercase( type.c_str() );

		if( type.compare( "FASTA" ) == 0 ) {
			m_type = TYPE_FASTA;
		}
		else if( type.compare( "FASTA_PRO" ) == 0
				|| type.compare( "FASTA-PRO" ) == 0 || type.compare( "FASTAPRO" ) == 0 ) {
			m_type = TYPE_FASTA_PRO;
		}
		else if( type.compare( "BLAST" ) == 0 ) {
			m_type = TYPE_BLAST;
		}
		else {
			m_type = TYPE_OTHER;
		}
	}
}

// end element
void LocalDbHandler::onEndElement( const char* name, const char* text ) {
	// each tags
	if( strcmp( name, TAG_NAME ) == 0 ) {    // </name>
		std::string n = NVL( text, "" );
		if( n.empty() ) {
			m_currentDbInfo = NULL;
		}
		else {
			m_localDbs.resize( m_localDbs.size() + 1 );
			m_currentDbInfo = &( m_localDbs.back() );
			m_currentDbInfo->name = n;
		}
	}
	else if( strcmp( name, TAG_WEB ) == 0 ) {    // </web>
		std::string n = NVL( text, "" );
		if( !n.empty() && m_currentDbInfo != NULL ) {
			m_currentDbInfo->url = n;
		}
	}
	else if( strcmp( name, TAG_FORMAT ) == 0 ) {    // </format>
		m_formatName = NVL( text, "" );
	}
	else if( strcmp( name, TAG_URL ) == 0 ) {    // </url>
		std::string n = NVL( text, "" );
		if( !n.empty() && !m_formatName.empty() && m_currentDbInfo != NULL ) {
			m_currentDbInfo->formats.resize( m_currentDbInfo->formats.size() + 1 );
			m_currentDbInfo->formats.back().name = m_formatName;
			m_currentDbInfo->formats.back().url = n;
		}

		m_formatName = "";
	}
	else if( strcmp( name, TAG_FILE_NAME ) == 0 ) {    // </file-name>
		std::string n = NVL( text, "" );

		if( !n.empty() && m_currentDbInfo != NULL ) {
			m_currentDbInfo->localDbs.resize( m_currentDbInfo->localDbs.size() + 1 );
			m_currentFile = &( m_currentDbInfo->localDbs.back() );
			m_currentFile->name = n;
		}
		else {
			m_currentFile = NULL;
		}
	}
	else if( strcmp( name, TAG_PATH ) == 0 ) {    // </path>
		std::string n = NVL( text, "" );
		if( !n.empty() && m_currentFile != NULL ) {
			std::string::size_type pos0 = n.find( "$(" );
			if( pos0 != n.npos ) {
				std::string::size_type pos1 = n.find( ")", pos0 + 2 );
				if( pos1 != n.npos ) {
					std::string envariable = n.substr( pos0 + 2, pos1 - ( pos0 + 2 ) );
					std::string val = getenvariable( envariable.c_str() );

					n = n.substr( 0, pos0 ) + val + n.substr( pos1 + 1 );
				}
			}

			m_currentFile->localFiles.push_back( std::make_pair( m_type, n ) );
		}
		m_type = TYPE_OTHER;
	}
	else if( strcmp( name, TAG_DB_FILE ) == 0 ) {    // </db-file>
		m_currentFile = NULL;
	}
	else if( strcmp( name, TAG_REF ) == 0 ) {    // </ref>
		m_formatName = "";
	}
	else if( strcmp( name, TAG_DB ) == 0 ) {
		m_currentDbInfo = NULL;
		m_currentFile = NULL;
		m_type = TYPE_OTHER;
		m_formatName = "";
	}
	else if( strcmp( name, TAG_ACC_REGX ) == 0 ) {    // </acc-regx>
		std::string n = NVL( text, "" );
		if( !n.empty() && m_currentDbInfo != NULL ) {
			m_currentDbInfo->regx = n;
		}
	}
	else if( strcmp( name, TAG_RESIDUES ) == 0 ) {    // </residues>
		std::string n = NVL( text, "" );
		if( !n.empty() && m_currentDbInfo != NULL ) {
			std::string::size_type pos0 = n.find( "$(" );
			if( pos0 != n.npos ) {
				std::string::size_type pos1 = n.find( ")", pos0 + 2 );
				if( pos1 != n.npos ) {
					std::string envariable = n.substr( pos0 + 2, pos1 - ( pos0 + 2 ) );
					std::string val = getenvariable( envariable.c_str() );

					n = n.substr( 0, pos0 ) + val + n.substr( pos1 + 1 );
				}
			}

			m_currentDbInfo->residues = n;
		}		
	}
	else if( strcmp( name, TAG_DESCRIPTION ) == 0 ) {
		std::string n = NVL( text, "" );
		if( !n.empty() && m_currentDbInfo != NULL ) {
			m_currentDbInfo->description = n;
		}
	}
}
