/**
 * @file ClrXYDataBase.cpp
 * @brief implements of ClrXYDataBase class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrXYDataBase.h"

#include "ClrXYData.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrXYDataBase::ClrXYDataBase() : XYDataWrapper( ClrObjectPool::getInstance().createXYData() ) {
	( (ClrXYData&)m_xyData ).setBaseData( this );
}

// destructor
ClrXYDataBase::~ClrXYDataBase() {
	ClrObjectPool::getInstance().destroyXYData( (ClrXYData&)m_xyData );
}

// on load data
bool ClrXYDataBase::onLoadData( IoFun^ readFun ) {
	// check the parameter
	if( readFun == nullptr ) {
		return false;
	}

	// length
	unsigned long len = 0;
	readFun( &len, sizeof( len ) );

	// read data
	if( len > 0 ) {
		double* arr = new double[ len * 2 ];
		readFun( arr, sizeof( double ) * len * 2 );

		onReserve( len );
		for( unsigned int i = 0; i < len; i++ ) {
			onAddPoint( arr[ i * 2 ], arr[ i * 2 + 1 ] );
		}

		delete[] arr;
	}

	return true;
}

// on save data
bool ClrXYDataBase::onSaveData( IoFun^ writeFun ) {
	// check the parameter
	if( writeFun == nullptr ) {
		return false;
	}

	// length
	unsigned long len = onGetLength();
	writeFun( &len, sizeof( len ) );

	// write data
	if( len > 0 ) {
		double* arr = new double[ len * 2 ];
		for( unsigned int i = 0; i < len; i++ ) {
			arr[ i * 2 ] = onGetX( i );
			arr[ i * 2 + 1 ] = onGetY( i );
		}

		writeFun( arr, sizeof( double ) * len * 2 );

		delete[] arr;
	}

	return true;
}
