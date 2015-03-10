/**
 * @file ChebiDb.cpp
 * @brief implements of ChebiDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "ChebiDb.h"         

#include <string>


using namespace kome::external_db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ChebiDb::ChebiDb() : kome::ident::ExternalDB(
		"ChEBI",
		"http://www.ebi.ac.uk/ebisearch/search.ebi?db=chebi&t=%s"
) {
}

// destructor
ChebiDb::~ChebiDb() {
}

// parse
void ChebiDb::parse( FILE* fp, std::vector< SearchResult >& result ) {
	// buffer
	char buff[ 4096 ];
	bool flg = false;

	std::string acc;
	std::string name;

	int num = 0;

	while( fgets( buff, 4096, fp ) != NULL && num < 20 ) {
		std::string line = trimstring( buff );

		if( line.find( "<div" ) != line.npos && line.find( "result-detail" ) != line.npos ) {
			acc.clear();
			name.clear();
		}

		// name
		if( line.find( "<a href" ) != line.npos ) {
			std::string::size_type pos = line.find( ">" );
			if( pos != line.npos ) {
				name = line.substr( pos + 1 );
				pos = name.find( "<" );
				if( pos != name.npos ) {
					name = name.substr( 0, pos );
				}
				name = trimstring( name.c_str() );
			}
		}
		
		// accession
		if( line.find( "<img" ) != line.npos ) {
			std::string key = "chebiId=";
			std::string::size_type pos = line.find( key );
			if( pos != line.npos ) {
				acc = line.substr( pos + key.length() );
				pos = acc.find( "\"" );
				if( pos != acc.npos ) {
					acc = acc.substr( 0, pos );
				}
				acc = trimstring( acc.c_str() );
			}
		}

		if( result.size() < 20 && !acc.empty() && !name.empty() ) {
			result.resize( result.size() + 1 );
			result.back().name = name;
			result.back().accession = acc;
			name.clear();
			acc.clear();
			num++;
		}
	}
}
