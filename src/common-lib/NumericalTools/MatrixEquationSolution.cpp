/**
 * @file MatrixEquationSolution.cpp
 * @brief implements of MatrixEquationSolution class
 *
 * @author S.Tanaka
 * @date 2007.05.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MatrixEquationSolution.h"

#include <math.h>


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// solve matrix equation by Gaussian elimination
bool MatrixEquationSolution::solveByGaussianElimination(
		kome::core::Matrix& mat,
		kome::core::Vector& vec,
		kome::core::Vector& ans
) {
	// check parameters
	if( vec.getDimention() == 0
			|| vec.getDimention() != mat.getRowSize()
			|| vec.getDimention() != mat.getColSize() ) {
		LOG_WARN( FMT( "The matrix or vector size is illegal." ) );
		return false;
	}

	// create working matrix
	kome::core::Matrix m( mat.getRowSize(), mat.getColSize() + 1 );
	for( unsigned int i = 0; i < mat.getRowSize(); i++ ) {
		for( unsigned int j = 0; j < mat.getColSize(); j++ ) {
			m( i, j ) = mat( i, j );
		}
		m( i, mat.getColSize() ) = vec( i );
	}

	// forward elimination
	for( unsigned int i = 0; i < m.getRowSize(); i++ ) {
		// pivot
		double pvt = 0.0;
		unsigned int pvtRow = i;

		for( unsigned int j = i; j < m.getRowSize(); j++ ) {
			double tmpPvt = m( j, i );
			if( fabs( tmpPvt ) > fabs( pvt ) ) {
				pvt = tmpPvt;
				pvtRow = j;
			}
		}
		if( pvt == 0.0 ) {
			LOG_WARN( FMT( "The matrix equation is illegal." ) );
			return false;
		}

		// swap
		if( pvtRow != i ) {
			kome::core::Vector tmpVec( m.getColSize() );
			for( unsigned int j = i; j < m.getColSize(); j++ ) {
				tmpVec( j ) = m( i, j );
				m( i, j ) = m( pvtRow, j );
			}
			for( unsigned int j = i; j < m.getColSize(); j++ ) {
				m( pvtRow, j ) = tmpVec( j );
			}
		}

		// normalize
		for( unsigned int j = i; j < m.getColSize(); j++ ) {
			m( i, j ) /= pvt;
		}

		// elimination
		for( unsigned int j = i + 1; j < m.getRowSize(); j++ ) {
			double rate = m( j, i );
			m( j, i ) = 0.0;
			for( unsigned int k = i + 1; k < m.getColSize(); k++ ) {
				m( j, k ) -= rate * m( i, k );
			}
		}
	}

	// backward substitution
	for( int i = (int)m.getRowSize() - 1; i >= 0; i-- ) {
		double a = m( i, m.getColSize() - 1 );
		for( unsigned int j = i + 1; j < m.getRowSize(); j++ ) {
			a -= m( i, j ) * ans( j );
		}

		ans( i ) = a;
	}

	return true;
}
