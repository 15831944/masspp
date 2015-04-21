/**
 * @file Vector.cpp
 * @brief implements of Vector class
 *
 * @author S.Tanaka
 * @date 2007.02.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Vector.h"

#include <math.h>


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Vector::Vector( const unsigned int dim )
		: m_dim( dim ), m_array( ( dim == 0 ) ? NULL : new double[ dim ] ) {
	if( m_array != NULL ) {
		fillzero( m_array, sizeof( double ) * m_dim );
	}
}

// copy constructor
Vector::Vector( const Vector& v ) 
		: m_dim( v.m_dim ), m_array( ( v.m_array == NULL ) ? NULL : new double[ v.m_dim ] ) {
	if( m_array != NULL ) {
		memcpy( m_array, v.m_array, sizeof( double ) * v.m_dim );
	}
}

// destructor
Vector::~Vector() {
	if( m_array != NULL ) {
		delete[] m_array;
	}
}

// get the dimention size
unsigned int Vector::getDimention() {
	return m_dim;
}

// get norm
double Vector::norm() {
	double n = 0.0;
	for( unsigned int i = 0; i < m_dim; i++ ) {
		n += ( m_array[ i ] * m_array[ i ] );
	}

	n = sqrt( n );

	return n;
}

// dot production
double Vector::dot( const Vector& v ) {
	if( v.m_dim != m_dim ) {
		throw FMT( "Vector size is illegal." );
	}

	double dp = 0.0;
	for( unsigned int i = 0; i < m_dim; i++ ) {
		dp += m_array[ i ] * v.m_array[ i ];
	}

	return dp;
}

// cross production
Vector Vector::cross( const Vector& v ) {
	if( m_dim != 3 || v.m_dim != 3 ) {
		throw FMT( "Vector size is illegal." );
	}

	Vector tmp( 3 );
	tmp( 0 ) = m_array[ 1 ] * v.m_array[ 2 ] - m_array[ 2 ] * v.m_array[ 1 ];
	tmp( 1 ) = m_array[ 2 ] * v.m_array[ 0 ] - m_array[ 0 ] * v.m_array[ 2 ];
	tmp( 2 ) = m_array[ 0 ] * v.m_array[ 1 ] - m_array[ 1 ] * v.m_array[ 0 ];

	return tmp;
}

// copy
Vector Vector::operator=( const Vector& v ) {
	if( v.m_dim != m_dim ) {
		throw FMT( "Vector size is illegal." );
	}

	if( m_array != NULL ) {
		memcpy( m_array, v.m_array, sizeof( double ) * m_dim );
	}

	return *this;
}

// get reference of value
double& Vector::operator()( const unsigned int index ) {
	// check parameter
	if( index >= m_dim ) {
		throw FMT( "The specified element does not exist. [dimension=%d, index=%d]", m_dim, index );
	}

	return m_array[ index ];
}

// vector addition
Vector& Vector::operator+=( const Vector& v ) {
	// check size
	if( v.m_dim != m_dim ) {
		throw FMT( "Vector size is illegal." );
	}

	// addition
	for( unsigned int i = 0; i < m_dim; i++ ) {
		m_array[ i ] += v.m_array[ i ];
	}

	return *this;
}

// vector subtraction
Vector& Vector::operator-=( const Vector& v ) {
	// check size
	if( v.m_dim != m_dim ) {
		throw FMT( "Vector size is illegal." );
	}

	// addition
	for( unsigned int i = 0; i < m_dim; i++ ) {
		m_array[ i ] -= v.m_array[ i ];
	}

	return *this;
}

// scalar multiplication
Vector& Vector::operator*=( const double k ) {
	// multiplication
	for( unsigned int i = 0; i < m_dim; i++ ) {
		m_array[ i ] *= k;
	}

	return *this;
}

// scalar division
Vector& Vector::operator/=( const double k ) {
	// division
	for( unsigned int i = 0; i < m_dim; i++ ) {
		m_array[ i ] /= k;
	}

	return *this;
}

// vector addition
Vector Vector::operator+( const Vector& v ) {
	Vector tmp = *this;
	tmp += v;

	return tmp;
}

// vector subtraction
Vector Vector::operator-( const Vector& v ) {
	Vector tmp = *this;
	tmp -= v;

	return tmp;
}

// scalar multiplication
Vector Vector::operator*( const double k ) {
	Vector tmp = *this;
	tmp *= k;

	return tmp;
}

// scalar division
Vector Vector::operator/( const double k ) {
	Vector tmp = *this;
	tmp /= k;

	return tmp;
}
