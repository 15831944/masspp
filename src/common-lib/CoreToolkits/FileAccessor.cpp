/**
 * @file FileAccessor.cpp
 * @brief implements of FileAccessor class
 *
 * @author S.Tanaka
 * @date 2008.08.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "FileAccessor.h"


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
FileAccessor::FileAccessor( FILE* fp ) {
	m_fp = fp;
}

// destructor
FileAccessor::~FileAccessor() {
}

// get file descriptor
FILE* FileAccessor::getFile() {
	return m_fp;
}

// write data
int FileAccessor::write( void* addr, int size ) {
	// check parameters
	if( addr == NULL || size == 0 || m_fp == NULL ) {
		return 0;
	}

	// write data
	int ret = fwrite( addr, 1, size, m_fp );

	return ret;
}

// read data
int FileAccessor::read( void* addr, int size ) {
	// check parameters
	if( addr == NULL || size == 0 || m_fp == NULL ) {
		return 0;
	}

	// read data
	int ret = fread( addr, 1, size, m_fp );

	return ret;
}
