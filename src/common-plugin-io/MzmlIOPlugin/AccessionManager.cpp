/*
 * @file AccessionManager.cpp
 * @brief implements of AccessionManager class
 *
 * @author S.Tanaka
 * @date 2009.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "AccessionManager.h"
#include "Accession.h"

#include <fstream>

using namespace kome::io::mzml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
AccessionManager::AccessionManager() {
	// obo file path
	kome::core::ResourceBundle& rb = kome::core::ResourceBundle::getInstance();
	std::string path = rb.searchResource( "psi-ms", NULL, "obo" );
	
	readOboFile( path.c_str() );
}

// destructor
AccessionManager::~AccessionManager() {
	clearAccessions();
}

// clear accessions
void AccessionManager::clearAccessions() {
	for( unsigned int i = 0; i < m_accessions.size(); i++ ) {
		delete m_accessions[ i ];
	}

	m_accessions.clear();
}

// get the number of accessions
unsigned int AccessionManager::getNumberOfAccessions() {
	return m_accessions.size();
}

// get accession
Accession* AccessionManager::getAccession( const unsigned int idx ) {
	if( idx >= m_accessions.size() ) {
		return NULL;
	}
	return m_accessions[ idx ];
}

// get accession
Accession* AccessionManager::getAccession( const char* id ) {
	// string object
	std::string idStr = NVL( id, "" );

	// search
	Accession* acc = NULL;
	for( unsigned int i = 0; i < m_accessions.size() && acc == NULL; i++ ) {
		Accession* tmp = m_accessions[ i ];
		if( idStr.compare( tmp->getId() ) == 0 ) {
			acc = tmp;
		}
	}

	return acc;
}

// read obo file
void AccessionManager::readOboFile( const char* path ) {
	// open
	std::fstream fs;
	fs.open( path, std::ios::in );
	if( fs.fail() ) {
		return;
	}

	// read
	char line[ 0x1000 ];
	Accession* acc = NULL;
	std::map< std::string, Accession* > accMap;
	std::map< Accession*, std::string > parentMap;

	while( !fs.eof() ) {
		fs.getline( line, 0x1000 );
		line[ 0x1000 - 1 ] = '\0';

		char* colon = strstr( line, ":" );
		if( colon != NULL ) {
			// title
			std::string title( line, colon );
			title = trimstring( title.c_str() );

			// value
			char* v = colon + 1;
			while( *v == ' ' || *v == '\n' || *v == '\t' || *v == '\r' ) {
				v = v + 1;
			}
			std::string value = trimstring( v );

			// set accession
			if( title.compare( "id" ) == 0 ) {	// ID
				acc = new Accession( value.c_str() );
				m_accessions.push_back( acc );
				accMap[ value ] = acc;
			}
			else if( title.compare( "name" ) == 0 ) {	// name
				if( acc != NULL ) {
					acc->setName( value.c_str() );
				}
			}
			else if( title.compare( "is_a" ) == 0 ) {	// parent
				unsigned int pos = value.find( "!" );
				if( pos != (unsigned int)value.npos ) {
					value = value.substr( 0, pos );
					value = trimstring( value.c_str() );

					parentMap[ acc ] = value;
				}
			}
		}
	}

	// close
	fs.close();

	// set parents
	for( unsigned int i = 0; i < m_accessions.size(); i++ ) {
		acc = m_accessions[ i ];
		if( parentMap.find( acc ) != parentMap.end() ) {
			std::string parentName = parentMap[ acc ];
			if( accMap.find( parentName ) != accMap.end() ) {
				Accession* parent = accMap[ parentName ];
				acc->setParent( parent );
			}
		}
	}
}

// get accession manager object
AccessionManager& AccessionManager::getInstance() {
	static AccessionManager mgr;
	return mgr;
}
