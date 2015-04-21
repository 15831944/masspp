/**
 * @file Base64.cpp
 * @brief implements of Base64 class
 *
 * @author S.Tanaka
 * @date 2007.03.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Base64.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Base64::Base64() {
	createMap();
}

// destructor
Base64::~Base64() {
}

// create encode map
void Base64::createMap() {
	// create encode map
	m_encodeMap[ 0x00 ] = 'A'; m_encodeMap[ 0x01 ] = 'B'; m_encodeMap[ 0x02 ] = 'C'; m_encodeMap[ 0x03 ] = 'D';
	m_encodeMap[ 0x04 ] = 'E'; m_encodeMap[ 0x05 ] = 'F'; m_encodeMap[ 0x06 ] = 'G'; m_encodeMap[ 0x07 ] = 'H';
	m_encodeMap[ 0x08 ] = 'I'; m_encodeMap[ 0x09 ] = 'J'; m_encodeMap[ 0x0A ] = 'K'; m_encodeMap[ 0x0B ] = 'L';
	m_encodeMap[ 0x0C ] = 'M'; m_encodeMap[ 0x0D ] = 'N'; m_encodeMap[ 0x0E ] = 'O'; m_encodeMap[ 0x0F ] = 'P';
	m_encodeMap[ 0x10 ] = 'Q'; m_encodeMap[ 0x11 ] = 'R'; m_encodeMap[ 0x12 ] = 'S'; m_encodeMap[ 0x13 ] = 'T';
	m_encodeMap[ 0x14 ] = 'U'; m_encodeMap[ 0x15 ] = 'V'; m_encodeMap[ 0x16 ] = 'W'; m_encodeMap[ 0x17 ] = 'X';
	m_encodeMap[ 0x18 ] = 'Y'; m_encodeMap[ 0x19 ] = 'Z'; m_encodeMap[ 0x1A ] = 'a'; m_encodeMap[ 0x1B ] = 'b';
	m_encodeMap[ 0x1C ] = 'c'; m_encodeMap[ 0x1D ] = 'd'; m_encodeMap[ 0x1E ] = 'e'; m_encodeMap[ 0x1F ] = 'f';
	m_encodeMap[ 0x20 ] = 'g'; m_encodeMap[ 0x21 ] = 'h'; m_encodeMap[ 0x22 ] = 'i'; m_encodeMap[ 0x23 ] = 'j';
	m_encodeMap[ 0x24 ] = 'k'; m_encodeMap[ 0x25 ] = 'l'; m_encodeMap[ 0x26 ] = 'm'; m_encodeMap[ 0x27 ] = 'n';
	m_encodeMap[ 0x28 ] = 'o'; m_encodeMap[ 0x29 ] = 'p'; m_encodeMap[ 0x2A ] = 'q'; m_encodeMap[ 0x2B ] = 'r';
	m_encodeMap[ 0x2C ] = 's'; m_encodeMap[ 0x2D ] = 't'; m_encodeMap[ 0x2E ] = 'u'; m_encodeMap[ 0x2F ] = 'v';
	m_encodeMap[ 0x30 ] = 'w'; m_encodeMap[ 0x31 ] = 'x'; m_encodeMap[ 0x32 ] = 'y'; m_encodeMap[ 0x33 ] = 'z';
	m_encodeMap[ 0x34 ] = '0'; m_encodeMap[ 0x35 ] = '1'; m_encodeMap[ 0x36 ] = '2'; m_encodeMap[ 0x37 ] = '3';
	m_encodeMap[ 0x38 ] = '4'; m_encodeMap[ 0x39 ] = '5'; m_encodeMap[ 0x3A ] = '6'; m_encodeMap[ 0x3B ] = '7';
	m_encodeMap[ 0x3C ] = '8'; m_encodeMap[ 0x3D ] = '9'; m_encodeMap[ 0x3E ] = '+'; m_encodeMap[ 0x3F ] = '/';

	// create decode map
	for( unsigned int i = 0; i < 256; i++ ) {
		m_decodeMap[ i ] = -1;
	}
	for( unsigned int i = 0; i < 64; i++ ) {
		m_decodeMap[ m_encodeMap[ i ] ] = i;
	}
}

// encode
int Base64::encode(
		void* src,
		const unsigned int srcLength,
		char* dest,
		const unsigned int destLength
) {
	// get instance
	Base64& base64 = getInstance();

	// cast
	char* cSrc = (char*)src;

	// buffers
	unsigned char dataBuff[ 3 ];
	unsigned char base64Buff[ 4 ];

	// read
	unsigned int srcPos = 0;
	unsigned int destPos = 0;
	unsigned int encodeSize = 0;
	while( srcPos < srcLength ) {
		// fill zero
		fillzero( dataBuff, sizeof( dataBuff ) );

		// get 3 bytes
		unsigned int count = 0;
		for( unsigned int i = 0; i < 3; i++ ) {
			if( srcPos < srcLength ) {
				dataBuff[ i ] = cSrc[ srcPos++ ];
				count++;
			}
		}

		// get encode data
		base64Buff[ 0 ] = base64.m_encodeMap[ ( dataBuff[ 0 ] >> 2 ) & 0x3f ];
		base64Buff[ 1 ] = base64.m_encodeMap[ ( ( dataBuff[ 0 ] << 4 ) | ( dataBuff[ 1 ] >> 4 ) ) & 0x3f ];
		base64Buff[ 2 ] = base64.m_encodeMap[ ( ( dataBuff[ 1 ] << 2 ) | ( dataBuff[ 2 ] >> 6 ) ) & 0x3f ];
		base64Buff[ 3 ] = base64.m_encodeMap[ ( dataBuff[ 2 ] ) & 0x3f ];

		// store
		for( unsigned int i = 0; i < 4; i++ ) {
			// check the buffer size
			if( destPos >= destLength ) {
				return -1;
			}

			// character
			if( i > count ) {
				dest[ destPos++ ] = '=';
			}
			else {
				dest[ destPos++ ] = base64Buff[ i ];
			}

			encodeSize++;
		}
	}

	return (int)destPos;
}

// decode
int Base64::decode(
		char* src,
		const unsigned int srcLength,
		void* dest,
		const unsigned int destLength
) {
	// get instance
	Base64& base64 = getInstance();

	// cast 
	char* cDest = (char*)dest;

	// buffers
	char dataBuff[ 3 ];
	unsigned char base64Buff[ 4 ];

	// read
	unsigned int srcPos = 0;
	unsigned int destPos = 0;
	unsigned int length = 0;

	while( srcPos < srcLength ) {
		// get base64 data
		unsigned char c = (unsigned char)src[ srcPos++ ];
		if( base64.m_decodeMap[ c ] >= 0 ) {
			base64Buff[ length++ ] = base64.m_decodeMap[ c ];
		}

		// decode
		if( length == 4 || c == '=' ) {
			switch( length ) {
			case 4:
				dataBuff[ 2 ] = ( base64Buff[ 2 ] << 6 ) | ( base64Buff[ 3 ] );
			case 3:
				dataBuff[ 1 ] = ( base64Buff[ 1 ] << 4 ) | ( base64Buff[ 2 ] >> 2 );
			case 2:
				dataBuff[ 0 ] = ( base64Buff[ 0 ] << 2 ) | ( base64Buff[ 1 ] >> 4 );
			default:
				break;				
			}			

			// store
			if( length > 1 ) {
				length--;
				for( unsigned int i = 0; i < length; i++ ) {
					if( destPos >= destLength ) {
						return -1;
					}
					cDest[ destPos++ ] = dataBuff[ i ];
				}
			}

			length = 0;

			if( c == '=' ) {
				srcPos = srcLength;
			}
		}
	}

	return destPos;
}

// get instance
Base64& Base64::getInstance() {
	// create object (This is the only object)
	static Base64 base64;

	return base64;
}
