/**
 * @file MatrixTool.cpp
 * @brief implements of MatrixTool class
 *
 * @author S.Tanaka
 * @date 2007.02.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "MatrixTool.h"


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// get identity matrix
kome::core::Matrix MatrixTool::getIdentityMatrix( const unsigned int dim ) {
	// create matrix
	kome::core::Matrix m( dim, dim );

	// set elements
	for( unsigned int i = 0; i < m.getRowSize(); i++ ) {
		for( unsigned int j = 0; j < m.getColSize(); j++ ) {
			m( i, j ) = ( ( i == j ) ? 1.0 : 0.0 );
		}
	}

	return m;
}

// get zero matrix
kome::core::Matrix MatrixTool::getZeroMatrix( const unsigned int dim ) {
	// create matrix
	kome::core::Matrix m( dim, dim );

	// set elements
	for( unsigned int i = 0; i < m.getRowSize(); i++ ) {
		for( unsigned int j = 0; j < m.getColSize(); j++ ) {
			m( i, j ) = 0.0;
		}
	}

	return m;
}
