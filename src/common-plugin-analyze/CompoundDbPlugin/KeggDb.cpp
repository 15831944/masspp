/**
 * @file KeggDb.cpp
 * @brief implements of KeggDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "KeggDb.h"         

#include <string>


using namespace kome::external_db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
KeggDb::KeggDb() : kome::ident::ExternalDB(
		"KEGG",
		"http://www.genome.jp/dbget-bin/www_bfind_sub?mode=bfind&max_hit=100&dbkey=compound&keywords=%s"
) {
}

// destructor
KeggDb::~KeggDb() {
}

// parse
void KeggDb::parse( FILE* fp, std::vector< SearchResult >& result ) {
	// buffer
	char buff[ 20000 ];
	bool flg = false;

	std::string key = "?cpd:";
	int num = 0;
	int pos = 0;

	while( fgets( buff, 20000, fp ) != NULL && num < 20 ) {
		buff[ 19999 ] = '\0';
		char* c = strstr( buff, key.c_str() );
		while( c != NULL ) {
			std::string name;
			std::string acc;

			char* end = strstr( c, "\"" );
			if( end != NULL ) {
				*end = '\0';
				acc = c + key.length();
				acc = trimstring( acc.c_str() );
				c = end + 1;
			}

			c = strstr( c, "<div" );
			if( c != NULL ) {
				c = strstr( c, ">" );
			}
			if( c != NULL ) {
				end = strstr( c, "</div>" );
				if( end != NULL ) {
					*end = '\0';
					name = c + 1;
					name = trimstring( name.c_str() );
					c = end + 1;
				}
			}

			if( !name.empty() && !acc.empty() && result.size() < 20 ) {
				result.resize( result.size() + 1 );
				result.back().name = name;
				result.back().accession = acc;
				num++;
			}

			if( c != NULL ) {
				c = strstr( c, key.c_str() );
			}
		}
	}
}
