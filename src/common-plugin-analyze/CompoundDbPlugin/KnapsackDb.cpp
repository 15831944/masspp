/**
 * @file KnapsackDb.cpp
 * @brief implements of KnapsackDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "KnapsackDb.h"         

#include <string>


using namespace kome::external_db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
KnapsackDb::KnapsackDb() : kome::ident::ExternalDB(
		"KNApSAcK",
		"http://kanaya.naist.jp/knapsack_jsp/result.jsp?sname=all&word=%s"
) {
}

// destructor
KnapsackDb::~KnapsackDb() {
}

// parse
void KnapsackDb::parse( FILE* fp, std::vector< SearchResult >& result ) {
	// buffer
	char buff[ 2049 ];

	std::string name;
	std::string acc;
	int cnt = 0;
	int num = 0;

	while( fgets( buff, 2049, fp ) != NULL && num < 20 ) {
		std::string line = trimstring( buff );

		if( line.find( "<tr>" ) != line.npos || line.find( "</tr>" ) != line.npos ) {
			if( !name.empty() && !acc.empty() && result.size() < 20 ) {
				result.resize( result.size() + 1 );
				result.back().name = name;
				result.back().accession = acc;

				name.clear();
				acc.clear();
				cnt = 0;
				num++;
			}
		}

		if( line.find( "<td" ) != line.npos ) {
			cnt++;

			if( cnt == 1 || cnt == 3 || cnt == 4 ) {
				char tmp[ 1024 ];
				fillzero( tmp, 1024 );

				bool inFlg = false;
				int tmpCnt = 0;
				for( unsigned int i = 0; i < line.size(); i++ ) {
					char c = line[ i ];
					if( c == '<' ) {
						inFlg = true;
					}

					if( !inFlg ) {
						tmp[ tmpCnt ] = c;
						tmpCnt++;
					}

					if( c == '>' ) {
						inFlg = false;
					}
				}

				std::string s = trimstring( tmp );

				if( cnt == 1 ) {
					acc = s;
				}
				else if( cnt == 3 ) {
					name = s;
				}
				else if( cnt == 3 ) {
					name.append( " " );
					name.append( FMT( "(%s)", s.c_str() ) );
				}
			}
		}
	}
}
