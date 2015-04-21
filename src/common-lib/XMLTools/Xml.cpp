/**
 * @file Xml.cpp
 * @brief implements of Xml class
 *
 * @author S.Tanaka
 * @date 2007.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Xml.h"


using namespace kome::xml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// convert string
std::string Xml::text2xml( const char* s ) {
	// struct
	struct ReplaceInfo {
		std::string src;
		std::string dst;
	};

	ReplaceInfo replaces[] = {
		{ "&" , "&amp;"  },
		{ "<" , "&lt;"   },
		{ ">" , "&gt;"   },
		{ "\"", "&quot;" }
	};

	unsigned num = sizeof( replaces ) / sizeof( ReplaceInfo );

	// convert
	std::string str = NVL( s, "" );
	for( unsigned int i = 0; i < num; i++ ) {
		str = replacestring( str.c_str(), replaces[ i ].src.c_str(), replaces[ i ].dst.c_str() );
	}

	return str;
}
