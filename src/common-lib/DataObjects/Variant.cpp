/**
 * @file Variant.cpp
 * @brief implements of Variant class
 *
 * @author S.Tanaka
 * @date 2006.09.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Variant.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Variant::Variant() {
	type = UNKNOWN;
	prim.pt = NULL;
	prim.dblVal = 0.0;

	arr = NULL;	
}

// copy constructor
Variant::Variant( const Variant& var ) {
	// copy member
	prim = var.prim;
	type = var.type;
	arr = NULL;

	if( var.arr != NULL && var.prim.intVal > 0 ) {
		int size = var.prim.intVal;
		if( var.type == ARRAY ) {
			arr = new Variant[ size ];
			for( int i = 0; i < size; i++ ) {
				( (Variant*)arr )[ i ] = ( (Variant*)var.arr )[ i ];
			}
		}
		else if( var.type == STRING ) {
			arr = new char[ size ];
			memcpy( arr, var.arr, size );
		}

		prim.intVal = size;
	}
}

// destructor
Variant::~Variant() {
	deleteArray();
}

// substitution operator
Variant& Variant::operator=( const Variant& other ) {
	// copy member
	prim = other.prim;
	type = other.type;
	arr = NULL;

	if( other.arr != NULL && other.prim.intVal > 0 ) {
		int size = other.prim.intVal;
		if( other.type == ARRAY ) {
			arr = new Variant[ size ];
			for( int i = 0; i < size; i++ ) {
				( (Variant*)arr )[ i ] = ( (Variant*)other.arr )[ i ];
			}
		}
		else if( other.type == STRING ) {
			arr = new char[ size ];
			memcpy( arr, other.arr, size );
		}

		prim.intVal = size;
	}

	return *this;
}

// set string
void Variant::setString( const char* s ) {
	// delete old array
	deleteArray();

	// check parameter
	if( s == NULL ) {
		type = UNKNOWN;
		return;
	}

	// set type
	type = STRING;

	// create array
	int size = strlen( s ) + 1;
	arr = new char[ size ];
	prim.intVal = size;

	// copy
	memcpy( arr, s, size );
}

// get string
char* Variant::getString() {
	return (char*)arr;
}

// create array
Variant* Variant::createArray( const unsigned int size ) {
	// delete old array
	deleteArray();

	// check parameter
	if( size == 0 ) {
		type = UNKNOWN;
		return NULL;
	}

	// type
	type = ARRAY;

	// set size
	prim.intVal = size;

	// create array
	arr = new Variant[ size ];

	return getArray();
}

// get array
Variant* Variant::getArray() {
	return (Variant*)arr;
}

// get element of the array
Variant* Variant::getElement( const unsigned int index ) {
	// check parameter
	if( index >= (unsigned int)prim.intVal ) {
		return NULL;
	}

	return ( getArray() + index );
}

// get array size
unsigned int Variant::getArraySize() {
	// check the member
	if( arr == NULL ) {
		return 0;
	}

	return (unsigned int)prim.intVal;
}

// delete array
void Variant::deleteArray() {
	// check array
	if( arr == NULL ) {
		return;
	}

	// delete
	delete[] arr;
	arr = NULL;
}
