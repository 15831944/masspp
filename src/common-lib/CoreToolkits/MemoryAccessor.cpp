/**
 * @file MemoryAccessor.cpp
 * @brief implements of MemoryAccessor class
 *
 * @author S.Tanaka
 * @date 2008.08.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MemoryAccessor.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MemoryAccessor::MemoryAccessor( void* addr, const unsigned int size ) {
	m_addr = (unsigned char*)addr;
	m_size = size;

	m_pos = 0;
}

// destructor
MemoryAccessor::~MemoryAccessor() {
}

// get data
void* MemoryAccessor::getData() {
	return m_addr;
}

// get data size
unsigned int MemoryAccessor::getSize() {
	return m_size;
}

// set reading position
void MemoryAccessor::setPosition( const unsigned int pos ) {
	m_pos = std::min( pos, m_size );
}

// write data
int MemoryAccessor::write( void* addr, int size ) {
	// check parameters
	if( addr == NULL || size <= 0 ) {
		return 0;
	}

	// actual size
	int actualSize = std::min( size, (int)m_size - (int)m_pos );
	if( actualSize <= 0 ) {
		return 0;
	}

	// write
	memcpy( m_addr + m_pos, addr, actualSize );
	m_pos += actualSize;

	return actualSize;
}

// read data
int MemoryAccessor::read( void* addr, int size ) {
	// check parameters
	if( addr == NULL || size <= 0 ) {
		return 0;
	}

	// actual size
	int actualSize = std::min( size, (int)m_size - (int)m_pos );
	if( actualSize <= 0 ) {
		return 0;
	}

	// read
	memcpy( addr, m_addr + m_pos, actualSize );
	m_pos += actualSize;

	return actualSize;
}
