/**
 * @file LeastSquares.cpp
 * @brief implements of LeastSquares class
 *
 * @author S.Tanaka
 * @date 2007.05.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "LeastSquares.h"

#include "MatrixEquationSolution.h"

#include <math.h>


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// get polynomia
bool LeastSquares::getPolynomia(
		kome::core::XYData& xyData,
		const unsigned int degree,
		kome::core::Vector& coefficients
) {
	// create weights array
	double* weights = NULL;
	if( xyData.getLength() > 0 ) {
		weights = new double[ xyData.getLength() ];
		for( unsigned int i = 0; i < xyData.getLength(); i++ ) {
			weights[ i ] = 1.0;
		}
	}

	// get polynomia
	bool ret = getPolynomia( xyData, weights, degree, coefficients );

	// delete array
	if( weights != NULL ) {
		delete[] weights;
	}

	return ret;
}

// get polynomia
bool LeastSquares::getPolynomia(
		kome::core::XYData& xyData,
		double* weights,
		const unsigned int degree,
		kome::core::Vector& coefficients
) {
	// check the parameter
	if( coefficients.getDimention() != degree + 1 ) {
		LOG_WARN( FMT( "The vector size is illegal." ) );
		return false;
	}

	// variables
	kome::core::Vector vec( degree + 1 );
	kome::core::Matrix mat( degree + 1, degree + 1 );

	// create vector
	for( unsigned int i = 0; i <= degree; i++ ) {
		double v = 0.0;
		for( unsigned int j = 0; j < xyData.getLength(); j++ ) {
			double x = xyData.getX( j );
			double y = xyData.getY( j );

			v += pow( x, (double)i ) * y * weights[ j ];
		}
		vec( i ) = v;
	}

	// create matrix
	for( unsigned int i = 0; i <= degree; i++ ) {
		for( unsigned int j = 0; j <= degree; j++ ) {
			double m = 0.0;
			for( unsigned int k = 0; k < xyData.getLength(); k++ ) {
				double x = xyData.getX( k );

				m += pow( x, (double)( i + j ) ) * weights[ k ];
			}
			mat( i, j ) = m;
		}
	}

	// solve matrix equation
	return MatrixEquationSolution::solveByGaussianElimination( mat, vec, coefficients );
}
