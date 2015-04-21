/**
 * @file Matrix.cpp
 * @brief implements of Matrix class
 *
 * @author S.Tanaka
 * @date 2006.09.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "Matrix.h"

#include <gtest/gtest.h>


using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Matrix::Matrix( const unsigned int row, const unsigned int col )
		: m_row( row ), m_col( col ),
			m_array( ( row == 0 || col == 0 ) ? NULL : new double[ row * col ] ) {
	if( m_array != NULL ) {
		fillzero( m_array, sizeof( double ) * m_row * m_col );
	}
}

// copy constructor
Matrix::Matrix( const Matrix& mat )
		: m_row( mat.m_row ), m_col( mat.m_col ),
			m_array( mat.m_array == NULL ? NULL : new double[ mat.m_row * mat.m_col ] ) {
	if( m_array != NULL ) {
		memcpy( m_array, mat.m_array, sizeof( double ) * mat.m_row * mat.m_col );
	}
}

// destructor
Matrix::~Matrix() {
	if( m_array != NULL ) {
		delete[] m_array;
	}
}

// get row size
unsigned int Matrix::getRowSize() {
	return m_row;
}

// get col size
unsigned int Matrix::getColSize() {
	return m_col;
}

// calculate determinant
double Matrix::det() {
	// check size
	if( m_row != m_col ) {
		throw FMT( "This is not a square matrix." );
	}
	if( m_row == 0 ) {
		throw FMT( "Matrix size is illegal." );
	}
	if( m_row == 1 ) {
		return m_array[ 0 ];
	}

	// calculate
	double d = 0.0;
	for( unsigned int i = 0; i < m_col; i++ ) {
		d += operator()( 0, i ) * cofactor( 0, i );
	}

	return d;
}

// calculate cofactor
double Matrix::cofactor( const unsigned int row, const unsigned int col ) {
	// check size
	if( m_row != m_col ) {
		throw FMT( "This is not a square matrix." );
	}
	if( m_row <= 1 ) {
		throw FMT( "Matrix size is illegal." );
	}

	// check parameter
	if( row >= m_row || col >= m_col ) {
		throw FMT( "The specified element does not exist. [row=%d, col=%d]", row, col );
	}

	// create matrix to calculate determinant
	Matrix m( m_row - 1 , m_col - 1 );
	for( unsigned int i = 0; i < m_row - 1; i++ ) {
		int r = ( i < row ) ? i : i + 1;
		for( unsigned int j = 0; j < m_col - 1; j++ ) {
			int c = ( j < col ) ? j : j + 1;

			m( i, j ) = operator()( r, c );
		}
	}

	// calculate cofactor
	double d = m.det();
	if( ( row + col ) % 2 != 0 ) {
		d = - d;
	}

	return d;
}

// get inverse matrix
Matrix Matrix::invert() {
	// check size
	if( m_row != m_col ) {
		throw FMT( "This is not a square matrix." );
	}
	if( m_row == 0 ) {
		throw FMT( "Matrix size is illegal." );
	}

	// calculate determinant
	double d = det();
	if( d == 0.0 ) {
		throw FMT( "There isn't an inverse matrix." );
	}

	// create inverse matrix
	Matrix m( m_row, m_col );
	if( m_row == 1 ) {
		m( 0, 0 ) = 1.0 / d;
	}
	else {
		for( unsigned int i = 0; i < m_row; i++ ) {
			for( unsigned int j = 0; j < m_col; j++ ) {
				m( i, j ) = cofactor( j, i ) / d;
			}
		}
	}

	return m;
}

// multiply
Vector Matrix::multiply( Vector& v ) {
	// check size
	if( m_col != v.getDimention() ) {
		throw FMT( "The size is illegal. [col=%d, dim=%d]", m_row, v.getDimention() );
	}

	// multiply
	Vector tmp( m_row );
	for( unsigned int i = 0; i < m_row; i++ ) {
		double elm = 0.0;
		for( unsigned int j = 0; j < m_col; j++ ) {
			elm += (*this)( i, j ) * v( j );
		}

		tmp( i ) = elm;
	}

	return tmp;
}

// copy
Matrix Matrix::operator=( const Matrix& m ) {
	if( m.m_row != m_row || m.m_col != m_col ) {
		throw FMT( "Matrix size is illegal." );
	}

	if( m_array != NULL ) {
		memcpy( m_array, m.m_array, sizeof( double ) * m_row * m_col );
	}

	return *this;
}

// get reference of value
double& Matrix::operator()( const unsigned int row, const unsigned int col ) {
	// check parameter
	if( row >= m_row || col >= m_col ) {
		throw FMT( "The specified element does not exist. [row=%d, col=%d]", row, col );
	}

	return m_array[ row + col * m_row ];
}

// matrix addition
Matrix& Matrix::operator+=( const Matrix& m ) {
	// check size
	if( m.m_row != m_row || m.m_col != m_col ) {
		throw FMT( "Matrix size is illegal." );
	}

	// size
	unsigned int size = m_row * m_col;

	// addition
	for( unsigned int i = 0; i < size; i++ ) {
		m_array[ i ] += m.m_array[ i ];
	}

	return *this;
}

// matrix subtraction
Matrix& Matrix::operator-=( const Matrix& m ) {
	// check size
	if( m.m_row != m_row || m.m_col != m_col ) {
		throw FMT( "Matrix size is illegal." );
	}

	// size
	unsigned int size = m_row * m_col;

	// addition
	for( unsigned int i = 0; i < size; i++ ) {
		m_array[ i ] -= m.m_array[ i ];
	}

	return *this;
}

// scalar multiplication
Matrix& Matrix::operator*=( const double k ) {
	// get the size of array
	unsigned int size = m_row * m_col;

	// multiplication
	for( unsigned int i = 0; i < size; i++ ) {
		m_array[ i ] *= k;
	}

	return *this;
}

// scalar division
Matrix& Matrix::operator/=( const double k ) {
	// get the size of array
	unsigned int size = m_row * m_col;

	// dividion
	for( unsigned int i = 0; i < size; i++ ) {
		m_array[ i ] /= k;
	}

	return *this;
}

// matrix addition
Matrix Matrix::operator+( const Matrix& m ) {
	Matrix tmp = *this;
	tmp += m;

	return tmp;
}

// matrix subtraction
Matrix Matrix::operator-( const Matrix& m ) {
	Matrix tmp = *this;
	tmp -= m;

	return tmp;
}

// matrix multiplication
Matrix Matrix::operator*( const Matrix& m ) {
	// check size
	if( this->m_col != m.m_row ) {
		throw FMT( "Matrix size is illegal." );
	}

	// copy
	Matrix tmp( this->m_row, m.m_col );

	// change size
	unsigned int num = this->m_col;

	// multiplication
	for( unsigned int i = 0; i < this->m_row; i++ ) {
		for( unsigned int j = 0; j < m.m_col; j++ ) {
			double val = 0.0;
			for( unsigned int k = 0; k < num; k++ ) {
				val += this->m_array[ i + k * this->m_row ] * m.m_array[ k + j * m.m_row ];
			}
			tmp( i, j ) = val;
		}
	}

	return tmp;
}

// scalar multiplication
Matrix Matrix::operator*( const double k ) {
	Matrix tmp = *this;
	tmp *= k;

	return tmp;
}

// scalar division
Matrix Matrix::operator/( const double k ) {
	Matrix tmp = *this;
	tmp /= k;

	return tmp;
}

// Fixture class
class MatrixTest : public testing::Test {
protected:
	// matrix A
    Matrix m_A;

	// matrix B
	Matrix m_B;

	// matrix C
	Matrix m_C;

	// matrix D
	Matrix m_D;

public:
	// constructor
	MatrixTest() : m_A( 2, 2 ), m_B( 2, 2 ), m_C( 3, 2 ), m_D( 2, 3 ) {
	}

	// setup the test fixture class (override method)
	virtual void SetUp() {
		// matrix A
		m_A( 0, 0 ) = 1.0;
		m_A( 0, 1 ) = 2.0;
		m_A( 1, 0 ) = 3.0;
		m_A( 1, 1 ) = 4.0;

		// matrix B
		m_B( 0, 0 ) = 5.0;
		m_B( 0, 1 ) = 6.0;
		m_B( 1, 0 ) = 7.0;
		m_B( 1, 1 ) = 8.0;

		// matrix C
		m_C( 0, 0 ) = 1.0;
		m_C( 0, 1 ) = 2.0;
		m_C( 1, 0 ) = 2.0;
		m_C( 1, 1 ) = 3.0;
		m_C( 2, 0 ) = 3.0;
		m_C( 2, 1 ) = 4.0;

		// matrix D
		m_D( 0, 0 ) = 1.0;
		m_D( 0, 1 ) = 2.0;
		m_D( 0, 2 ) = 2.0;
		m_D( 1, 0 ) = 3.0;
		m_D( 1, 1 ) = 3.0;
		m_D( 1, 2 ) = 4.0;
	}
};

// row size
TEST( Matrix, rowSize ) {
	Matrix A( 3, 4 );

	ASSERT_EQ( A.getRowSize(), 3 );
}

// column size
TEST( Matrix, colSize ) {
	Matrix A( 4, 5 );

	ASSERT_EQ( A.getColSize(), 5 );
}

// addition test
TEST_F( MatrixTest, addition ) {
	// A + B
	Matrix C = m_A + m_B;

	ASSERT_DOUBLE_EQ( 6.0, C( 0, 0 ) );
	ASSERT_DOUBLE_EQ( 8.0, C( 0, 1 ) );
	ASSERT_DOUBLE_EQ( 10.0, C( 1, 0 ) );
	ASSERT_DOUBLE_EQ( 12.0, C( 1, 1 ) );

	// A + C
	ASSERT_ANY_THROW( m_A + m_C );
	
}
