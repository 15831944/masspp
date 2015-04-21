/**
 * @file Buffer.cpp
 * @brief implements of Buffer class
 *
 * @author S.Tanaka
 * @date 2008.08.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Buffer.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Buffer::Buffer() {
	m_pos = 0;
}

// destructor
Buffer::~Buffer() {
}

// clear buffer
void Buffer::clear() {
	m_buffer.clear();
	m_pos = 0;
}

// get buffer
void* Buffer::getBuffer() {
	if( m_buffer.size() > 0 ) {
		return &( m_buffer[ 0 ] );
	}
	return NULL;
}

// get buffer length
unsigned int Buffer::getLength() {
	return m_buffer.size();
}

// set position
void Buffer::setPosition( const unsigned int pos ) {
  m_pos = std::min( pos, (unsigned int)m_buffer.size() );
}

// get position
unsigned int Buffer::getPosition() {
	return m_pos;
}

// rewind position
void Buffer::rewind() {
	setPosition( 0 );
}

// write size
int Buffer::write( void* addr, int size ) {
	// check the parameters
	if( size <= 0 || addr == NULL ) {
		return 0;
	}

	// new size
	unsigned long pos = m_buffer.size();
	unsigned long newSize = pos + size;
	if( newSize > m_buffer.max_size() ) {
		newSize = m_buffer.max_size();
	}
	size = newSize - pos;
	if( size <= 0 ) {
		return 0;
	}

	// resize buffer
	m_buffer.resize( newSize );

	// copy data
	memcpy( &( m_buffer[ pos ] ), addr, size );

	return size;
}

// read
int Buffer::read( void* addr, int size ) {
	// check the parameters
	if( size <= 0 || addr == NULL ) {
		return 0;
	}

	// size
	int restSize = (int)m_buffer.size() - m_pos;
	if( restSize < size ) {
		size = restSize;
	}
	if( size <= 0 ) {
		return 0;
	}

	// read
	memcpy( addr, &( m_buffer[ m_pos ] ), size );
	m_pos = m_pos + size;

	return size;
}
