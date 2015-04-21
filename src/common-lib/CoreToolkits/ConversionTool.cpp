/**
 * @file ConversionTool.cpp
 * @brief implements of IDTool class
 *
 * @author S.Tanaka
 * @date 2006.09.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ConversionTool.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// get name from array
std::string ConversionTool::getIdName(
		const int id,
		IDInfo* idInfoArray,
		const unsigned int arraySize
) {
	// string object
	std::string name;

	// check parameters
	if( idInfoArray == NULL || arraySize == 0 ) {
		return name;
	}

	// find ID	
	for( unsigned int i = 0; i < arraySize && name.empty(); i++ ) {
		if( idInfoArray[ i ].id == id ) {	// matched
			name = std::string( NVL( idInfoArray[ i ].name, "" ) );
		}
	}

	return name;
}
