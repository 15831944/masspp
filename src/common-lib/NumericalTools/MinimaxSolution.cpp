/**
 * @file MinimaxSolution.cpp
 * @brief implements of MinimaxSolution class
 *
 * @author S.Tanaka
 * @date 2007.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MinimaxSolution.h"

#include <math.h>


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// Downhill Simplex Method
void MinimaxSolution::downhillSimplex(
		double* p,
		double* evals,
		const unsigned int dim,
		boost::function< double ( double* ) > fun,
		const double tol,
		const unsigned int iter
) {
	// variables
	int wrstIdx = -1;
	int wrstIdx2 = -1;
	int bstIdx = -1;

	double* g = new double[ dim ];
	double* tmp = new double[ dim ];
	double* tmp2 = new double[ dim ];

	// evaluation values
	for( unsigned int i = 0; i < dim + 1; i++ ) {
		evals[ i ] = fun( p + (dim * i ) );
	}

	// Downhill Simplex
	unsigned int count = 0;
	while( true ) {
		// initialize
		wrstIdx = -1;
		wrstIdx2 = -1;
		bstIdx = -1;

		// get evaluation values
		for( unsigned int i = 0; i < dim + 1; i++ ) {
			// value
			double eval = evals[ i ];

			// worst, 2nd worst, best
			if( wrstIdx < 0 || eval > evals[ wrstIdx ] ) {  // worst
				wrstIdx2 = wrstIdx;
				wrstIdx = (int)i;
			}
			else if( eval > evals[ wrstIdx2 ] ) {   // 2nd worst
				wrstIdx2 = (int)i;
			}
			if( bstIdx < 0 || eval < evals[ bstIdx ] ) {	// best
				bstIdx = (int)i;
			}
		}

		// ratio
		double ratio = ( fabs( evals[ bstIdx ] - evals[ wrstIdx ] ) / fabs( evals[ bstIdx ] + evals[ wrstIdx ] ) );
		if( count > iter || ratio < tol ) {
			break;
		}

		// get the center of gravity
		for( unsigned int j = 0; j < dim; j++ ) {
			g[ j ] = 0.0;
		}
		for( unsigned int i = 0; i < dim + 1; i++ ) {
			double* v = p + ( dim * i );

			for( unsigned int j = 0; j < dim; j++ ) {
				g[ j ] += v[ j ] / (double)( dim + 1 );
			}
		}

		// refrection
		double* vertex = p + ( dim * wrstIdx );
		for( unsigned j = 0; j < dim; j++ ) {
			tmp[ j ] = 2.0 * g[ j ] - vertex[ j ];
		}
		double eval = fun( tmp );

		if( eval < evals[ bstIdx ] ) {		// expansion
			for( unsigned j = 0; j < dim; j++ ) {
				tmp2[ j ] = 3.0 * g[ j ] - 2.0 * vertex[ j ];
			}
			double eval2 = fun( tmp2 );
			if( eval2 < eval ) {
				eval = eval2;
				memcpy( tmp, tmp2, sizeof( double ) * dim );
			}
		}
		else if( eval >= evals[ wrstIdx2 ] ) {		// contraction
			for( unsigned j = 0; j < dim; j++ ) {
				tmp2[ j ] = ( g[ j ] + vertex[ j ] ) / 2.0;
			}
			double eval2 = fun( tmp2 );
			if( eval2 < eval ) {
				eval = eval2;
				memcpy( tmp, tmp2, sizeof( double ) * dim );
			}
		}

		// update
		if( eval < evals[ wrstIdx2 ] ) {	// swap
			evals[ wrstIdx ] = eval;
			memcpy( vertex, tmp, sizeof( double ) * dim );
		}
		else {		// multiple contraction
			double* bstVertex = p + ( dim * bstIdx );

			for( unsigned int i = 0; i < dim + 1; i++ ) {
				if( i != bstIdx ) {
					vertex = p + ( dim * i );

					for( unsigned int j = 0; j < dim; j++ ) {
						vertex[ j ] = ( bstVertex[ j ] + vertex[ j ] ) / 2.0;
					}

					evals[ i ] = fun( vertex );
				}
			}
		}

		// part of the way
		count++;
	}

	// swap to locate best point to top
	if( bstIdx > 0 ) {
		memswap( evals, evals + bstIdx, sizeof( double ) );
		memswap( p, p + ( bstIdx * dim ), sizeof( double ) * dim );
	}

	// delete
	delete[] g;
	delete[] tmp;
	delete[] tmp2;
}
