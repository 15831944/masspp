/**
 * @file LipidMapsDb.cpp
 * @brief implements of LipidMapsDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "LipidMapsDb.h"         

#include <string>


using namespace kome::external_db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
LipidMapsDb::LipidMapsDb() : kome::ident::ExternalDB(
		"LIPID MAPS",
		"http://www.lipidmaps.org/data/structure/LMSDFuzzySearch.php?&Name=%s&Mode=ProcessTextOntologySearch"
) {
}

// destructor
LipidMapsDb::~LipidMapsDb() {
}

// parse
void LipidMapsDb::parse( FILE* fp, std::vector< SearchResult >& result ) {
	// buffer
	char buff[ 10000 ];
	int num = 0;

	while( fgets( buff, 10000, fp ) != NULL && num < 20 ) {
		std::string key = "/data/LMSDRecord.php?LMID=";
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
			key = "<td>";
			c = strstr( c, key.c_str() );
			if( c != NULL ) {
				char* end = strstr( c, "</td>" );
				if( end != NULL ) {
					*end = '\0';
					std::string s = trimstring( c + key.length() );
					char tmp[ 1024 ];
					fillzero( tmp, 1024 );
					int cnt = 0;
					bool inFlg = false;
					for( unsigned int i = 0; i < s.length(); i++ ) {
						char c0 = s[ i ];
						if( c0 == '<' ) {
							inFlg = true;
						}
						if( !inFlg ) {
							tmp[ cnt ] = c0;
							cnt++;
						}
						if( c0 == '>' ) {
							inFlg = false;
						}
					}
					name = trimstring( tmp );
					c = end + 1;
				}
			}

			c = strstr( c + 1, "<td>" );
			if( c != NULL ) {
				c = strstr( c + 1, "<td>" );
			}
			if( c != NULL ) {
				char* end = strstr( c, "</td>" );
				if( end != NULL ) {
					*end = '\0';
				}

				std::string s = trimstring( c );
				char tmp[ 1024 ];
				fillzero( tmp, 1024 );
				int cnt = 0;
				bool inFlg = false;
				for( unsigned int i = 0; i < s.length(); i++ ) {
					char c0 = s[ i ];
					if( c0 == '<' ) {
						inFlg = true;
					}

					if( !inFlg ) {
						tmp[ cnt ] = c0;
						cnt++;
					}

					if( c0 == '>' ) {
						inFlg = false;
					}
				}

				std::string tmp2 = trimstring( tmp );
				if( !tmp2.empty() ) {
					name.append( FMT( " (%s)", tmp2.c_str() ) );
				}
			}
		}

		if( !acc.empty() && !name.empty() && result.size() < 20 ) {
			result.resize( result.size() + 1 );
			result.back().name = name;
			result.back().accession = acc;
			num++;
		}
	}
}
