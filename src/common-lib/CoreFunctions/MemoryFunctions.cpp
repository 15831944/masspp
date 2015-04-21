/**
 * @file MemoryFunctions.cpp
 * @brief implements of function which deals with memory
 *
 * @author S.Tanaka
 * @date 2006.07.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MemoryFunctions.h"

#include <stdio.h>
#include <stdlib.h>


// fill array
void fillarray( void* p, void* value, size_t size, size_t n ) {
	// void* -> char* (because void* doesn't have the size of element)
	char* cp = (char*)p;
	char* cv = (char*)value;

	// fill
	for( unsigned int i = 0; i < n; i++ ) {
		char* p_i = cp + ( size * i );
		for( unsigned int j = 0; j < size; j++ ) {
			p_i[ j ] = cv[ j ];
		}
	}
}

// fill 0
void fillzero( void* p, size_t n ) {
	unsigned char zero = 0;

	fillarray( p, &zero, 1, n );
}

// is big endian
bool isbigendian() {
	// create 2 bytes data
	short num = 1;	// (Big: 00 01,  Little: 01 00)

	// get first byte
	char* c = (char*)&num;

	return ( *c == 0 );
}

// is little endian
bool islittleendian() {
	return !isbigendian();
}

// memory reverse
void memreverse( void* p, size_t n ) {
	// check parameters
	if( p == NULL || n == 0 ) {
		return;
	}

	// create buffer
	char* buffer = new char[ n ];

	// copy to buffer
	memcpy( buffer, p, n );

	// reverse
	char* cp = (char*)p;
	for( unsigned int i = 0; i < n; i++ ) {
		cp[ i ] = buffer[ n - 1 - i ];
	}

	// delete
	delete[] buffer;
}

// memory swap
void memswap( void* p0, void* p1, size_t n ) {
	// check parameters
	if( p0 == NULL || p1 == NULL || n == 0 ) {
		return;
	}

	// create buffer
	void* tmp = malloc( n );

	// copy
	memcpy( tmp, p0, n );
	memcpy( p0, p1, n );
	memcpy( p1, tmp, n );

	// free
	free( tmp );
}
