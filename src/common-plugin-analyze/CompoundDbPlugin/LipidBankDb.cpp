/**
 * @file LipidBankDb.cpp
 * @brief implements of LipidBankDb class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "LipidBankDb.h"         

#include <string>


using namespace kome::external_db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
LipidBankDb::LipidBankDb() : kome::ident::ExternalDB(
		"Lipid Bank",
		"http://lipidbank.jp/cgi-bin/ALL.cgi?LipidBankID=&LipidName=%s&Source=&ChemSynthesys=&BioActivity=&Metabolism=&GenInfo=&Formula=&AMolLow=&AMolLowSign=le&AMolUpSign=le&AMolUp=&category=ALL"
) {
}

// destructor
LipidBankDb::~LipidBankDb() {
}

// parse
void LipidBankDb::parse( FILE* fp, std::vector< SearchResult >& result ) {
	// buffer
	char buff[ 10000 ];
	int num = 0;

	while( fgets( buff, 10000, fp ) != NULL && num < 20 ) {
		std::string key = "/cgi-bin/detail.cgi?id=";
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
			key = "</b>/";
			c = strstr( c, key.c_str() );
			if( c != NULL ) {
				char* end = strstr( c, "<br>" );
				if( end != NULL ) {
					*end = '\0';
				}

				std::string tmp = trimstring( c + key.length() );
				char s[ 1024 ];
				fillzero( s, 1024 );
				int cnt = 0;
				bool inFlg = false;
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
				name = trimstring( s );
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
