/**
 * @file Sha1.cpp
 * @brief implements of Sha1 class
 *
 * @author S.Tanaka
 * @date 2007.03.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Sha1.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// logical function
unsigned long Sha1::f( const long t, const unsigned long b, const unsigned long c, const unsigned long d ) {
	if( t < 20 ) {
		return ( ( b & c ) | ( ~b & d ) );
	}
	if( t < 40 ) {
		return ( b ^ c ^ d );
	}
	if( t < 60 ) {
		return ( ( b & c ) | ( b & d ) | ( c & d ) );
	}
	if( t < 80 ) {
		return ( b ^ c ^ d );
	}
	return 0;
}

// constant word
unsigned long Sha1::K( const long t ) {
	if( t < 20 ) {
		return 0x5A827999;
	}
	if( t < 40 ) {
		return 0x6ED9EBA1;
	}
	if( t < 60 ) {
		return 0x8F1BBCDC;
	}
	if( t < 80 ) {
		return 0xCA62C1D6;
	}
	return 0;
}

// transform data
std::string Sha1::transformData( void* src, const unsigned int length ) {
	// cast
	unsigned char* uc = (unsigned char*)src;

	// context
	Sha1Context context;
	context.appendMessage( uc, length );
	context.finish();

	// get string
	std::string s;
	for( unsigned int i = 0; i < 5; i++ ) {
		s.append( FMT( "%08x", context.m_h[ i ] ) );
	}

	return s;
}

// transform file
std::string Sha1::transformFile( const char* path ) {
	// string
	std::string s;

	// open the file
	FILE* fp = fileopen( path, "rb" );
	if( fp == NULL ) {
		LOG_ERROR( FMT( "Failed to open the file. [%s]", path ) );
		return s;
	}

	// context
	Sha1Context context;

	// read the file
	unsigned char buff[ 0x1000 ];
	int readSize = 0;
	while( ( readSize = fread( buff, 1, 0x1000, fp ) ) > 0 ) {
		context.appendMessage( buff, readSize );
	}
	context.finish();

	// close file
	fclose( fp );

	// get string
	for( unsigned int i = 0; i < 5; i++ ) {
		s.append( FMT( "%08x", context.m_h[ i ] ) );
	}

	return s;
}



//
// Sha1Context
//

// constructor
Sha1::Sha1Context::Sha1Context() {
	reset();
}

// destructor
Sha1::Sha1Context::~Sha1Context() {
}

// append message
void Sha1::Sha1Context::appendMessage( unsigned char* msg, const unsigned long length ) {
	// check the parameter
	if( length == 0 ) {
		return;
	}

	// set to the block
	for( unsigned int i = 0; i < length; i++ ) {
		m_block[ m_pos++ ] = msg[ i ];
		// set to block
		m_length++;

		// update hash
		if( m_pos == 64 ) {
			updateHash();
			m_pos = 0;
			fillzero( m_block, sizeof( m_block ) );
		}
	}
}

// finish
void Sha1::Sha1Context::finish() {
	// padding
	padBlock();

	// update hash
	updateHash();
}

// update hash
void Sha1::Sha1Context::updateHash() {
	// initialize word
	unsigned long w[ 80 ];
	for( int t = 0; t < 16; t++ ) {
		unsigned long wt = 0;
		for( int i = 0; i < 4; i++ ) {
			wt = wt << 8;
			wt |= m_block[ t * 4 + i ];
		}
		w[ t ] = wt;
	}
	for( int t = 16; t < 80; t++ ) {
		unsigned long wt = w[ t - 3 ] ^ w[ t - 8 ] ^ w[ t - 14 ] ^ w[ t - 16 ];
		w[ t ] = CIRCULAR_LEFT_SHIFT( wt, 1 );
	}

	// calculate
	unsigned long a = m_h[ 0 ];
	unsigned long b = m_h[ 1 ];
	unsigned long c = m_h[ 2 ];
	unsigned long d = m_h[ 3 ];
	unsigned long e = m_h[ 4 ];

	for( int t = 0; t < 80; t++ ) {
		unsigned long tmp = CIRCULAR_LEFT_SHIFT( a, 5 ) + Sha1::f( t, b, c, d ) + e + w[ t ] + Sha1::K( t );

		e = d;
		d = c;
		c = CIRCULAR_LEFT_SHIFT( b, 30 );
		b = a;
		a = tmp;
	}

	// update hash
	m_h[ 0 ] += a;
	m_h[ 1 ] += b;
	m_h[ 2 ] += c;
	m_h[ 3 ] += d;
	m_h[ 4 ] += e;
}

// pads block
void Sha1::Sha1Context::padBlock() {
	m_block[ m_pos ] = 0x80;
	if( m_pos > 55 ) {
		updateHash();
		fillzero( m_block, sizeof( m_block ) );
	}

	// get bit length
	unsigned long highLength = m_length >> 29;
	unsigned long lowLength = m_length << 3;

	m_block[ 56 ] = (unsigned char)( ( highLength >> 24 ) & 0xff );
	m_block[ 57 ] = (unsigned char)( ( highLength >> 16 ) & 0xff );
	m_block[ 58 ] = (unsigned char)( ( highLength >> 8 ) & 0xff );
	m_block[ 59 ] = (unsigned char)( ( highLength ) & 0xff );
	m_block[ 60 ] = (unsigned char)( ( lowLength >> 24 ) & 0xff );
	m_block[ 61 ] = (unsigned char)( ( lowLength >> 16 ) & 0xff );
	m_block[ 62 ] = (unsigned char)( ( lowLength >> 8 ) & 0xff );
	m_block[ 63 ] = (unsigned char)( ( lowLength ) & 0xff );
}

// reset member variables
void Sha1::Sha1Context::reset() {
	// Hash
	m_h[ 0 ] = 0x67452301;
	m_h[ 1 ] = 0xEFCDAB89;
	m_h[ 2 ] = 0x98BADCFE;
	m_h[ 3 ] = 0x10325476;
	m_h[ 4 ] = 0xC3D2E1F0;

	// block
	fillzero( m_block, sizeof( m_block ) );

	// length
	m_length = 0;

	// position
	m_pos = 0;
}
