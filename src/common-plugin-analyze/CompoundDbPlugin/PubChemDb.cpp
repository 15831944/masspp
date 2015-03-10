/**
 * @file PubChemDb.cpp
 * @brief implements of PubChemDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "PubChemDb.h"         

#include <string>


using namespace kome::external_db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PubChemDb::PubChemDb() : kome::ident::ExternalDB(
		"PubChem",
		"http://www.ncbi.nlm.nih.gov/pccompound/?db=pccompound&term=%s&Search="
) {
}

// destructor
PubChemDb::~PubChemDb() {
}

// parse
void PubChemDb::parse( FILE* fp, std::vector< SearchResult >& result ) {
	// buffer
	char buff[ 10000 ];
	int num = 0;

	while( fgets( buff, 10000, fp ) != NULL && num < 20 ) {
		std::string key = "imgsrv.fcgi?cid=";
		std::string acc;
		std::string name;

		char* c = strstr( buff, key.c_str() );
		if( c != NULL ) {
			char* end = strstr( c, "\"" );
			if( end != NULL ) {
				*end = '\0';
				acc = trimstring( c + key.length() );
				c = end + 1;
			}
		}

		if( !acc.empty() ) {
			key = "<a href";
			c = strstr( c, key.c_str() );
			if( c != NULL ) {
				c = strstr( c, ">" );
			}

			if( c != NULL ) {
				std::string tmp;
				char* end = strstr( c, "</a>" );
				if( end != NULL ) {
					*end = '\0';
					tmp = trimstring( c + 1 );
					c = end + 1;
				}

				char s[ 1024 ];
				fillzero( s, 1024 );
				bool inFlg = false;
				int cnt = 0;

				for( unsigned int i = 0; i < tmp.length(); i++ ) {
					char c0 = tmp[ i ];
					if( c0 == '<' ) {
						inFlg = true;
					}

					if( !inFlg ) {
						s[ cnt ] = c0;
						cnt++;
					}

					if( c0 == '>' ) {
						inFlg = false;
					}
				}

				c = strstr( s, ";" );
				if( c != NULL ) {
					*c = '\0';
				}

				name = trimstring( s );
			}
		}

		// add
		if( !acc.empty() && !name.empty() && result.size() < 20 ) {
			result.resize( result.size() + 1 );
			result.back().name = name;
			result.back().accession = acc;
			num++;
		}
	}
}
