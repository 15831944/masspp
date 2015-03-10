/**
 * @file CasDb.cpp
 * @brief implements of CasDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "CasDb.h"         

#include <string>


using namespace kome::external_db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
CasDb::CasDb() : kome::ident::ExternalDB(
		"CAS",
		"http://webbook.nist.gov/cgi/cbook.cgi?Name=*%s*&Units=SI"
) {
}

// destructor
CasDb::~CasDb() {
}

// parse
void CasDb::parse( FILE* fp, std::vector< SearchResult >& result ) {
	// buffer
	char buff[ 4096 ];
	bool flg = false;
	int num = 0;

	while( fgets( buff, 4096, fp ) != NULL && num < 20 ) {
		std::string line = trimstring( buff );

		if( line.find( "<ol>" ) != line.npos ) {
			flg = true;
		}

		if( line.find( "</ol>" ) != line.npos ) {
			flg = false;
		}

		if( flg ) {
			std::string accession;
			std::string name;

			std::string key = "cgi/cbook.cgi?ID=C";
			std::string::size_type pos = line.find( key );
			if( pos != line.npos ) {
				accession = line.substr( pos + key.length() );
				pos = accession.find( "&" );
				if( pos != accession.npos ) {
					accession = accession.substr( 0, pos );
				}
			}

			char buff[ 1024 ];
			fillzero( buff, sizeof( char ) * 1024 );
			int cnt = 0;
			bool inFlg = false;

			for( unsigned int i = 0; i < line.length(); i++ ) {
				char c = line[ i ];

				if( c == '<' ) {
					inFlg = true;
				}

				if( !inFlg ) {
					buff[ cnt ] = c;
					cnt++;
				}

				if( c == '>' ) {
					inFlg = false;
				}
			}

			name = trimstring( buff );

			if( !accession.empty() && !name.empty() && result.size() < 20 ) {
				result.resize( result.size() + 1 );
				result.back().accession = accession;
				result.back().name = name;
				num++;
			}
		}
	}
}
