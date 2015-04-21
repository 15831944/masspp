/**
 * @file Transformation3D.cpp
 * @brief implements of Transformation3D class
 *
 * @author S.Tanaka
 * @date 2007.02.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "Transformation3D.h"

#include "Math.h"
#include "MatrixTool.h"

#include <math.h>


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// get 3x3 identity matrix
kome::core::Matrix Transformation3D::getIdentityMatrix3() {
	return MatrixTool::getIdentityMatrix( 3 );
}

// get 4x4 identity matrix
kome::core::Matrix Transformation3D::getIdentityMatrix4() {
	return MatrixTool::getIdentityMatrix( 4 );
}

// get 3x3 rotation matrix
kome::core::Matrix Transformation3D::getRotationMatrix3(
	const double theta,
	kome::core::Vector& axis
) {
	// cos, sin
	double t = theta * Math::PI / 180.0;
	double cosT = cos( t );
	double sinT = sin( t );

	// axis
	kome::core::Vector a = axis / axis.norm();
	double ux = a( 0 );
	double uy = a( 1 );
	double uz = a( 2 );

	// create rotation matrix
	kome::core::Matrix rot( 3, 3 );
	rot( 0, 0 ) = ( 1.0 - cosT ) * ux * ux + cosT;
	rot( 0, 1 ) = ( 1.0 - cosT ) * ux * uy - uz * sinT;
	rot( 0, 2 ) = ( 1.0 - cosT ) * ux * uz + uy * sinT;
	rot( 1, 0 ) = ( 1.0 - cosT ) * uy * ux + uz * sinT;
	rot( 1, 1 ) = ( 1.0 - cosT ) * uy * uy + cosT;
	rot( 1, 2 ) = ( 1.0 - cosT ) * uy * uz - ux * sinT;
	rot( 2, 0 ) = ( 1.0 - cosT ) * uz * ux - uy * sinT;
	rot( 2, 1 ) = ( 1.0 - cosT ) * uz * uy + ux * sinT;
	rot( 2, 2 ) = ( 1.0 - cosT ) * uz * uz + cosT;

	return rot;
}

// create matrix
kome::core::Matrix Transformation3D::getRotationMatrix4(
	const double theta,
	kome::core::Vector& axis
) {
	// create 3x3 matrix
	kome::core::Matrix rot3 = getRotationMatrix3( theta, axis );

	// create 4x4 matrix
	kome::core::Matrix rot = getIdentityMatrix4();

	// set elements
	for( unsigned int i = 0; i < rot3.getRowSize(); i++ ) {
		for( unsigned int j = 0; j < rot3.getColSize(); j++ ) {
			rot( i, j ) = rot3( i, j );
		}
	}

	return rot;
}
