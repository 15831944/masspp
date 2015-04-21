/**
 * @file Matrix.h
 * @brief interfaces of Matrix class
 *
 * @author S.Tanaka
 * @date 2006.09.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CORE_MATRIX_H__
#define __KOME_CORE_MATRIX_H__


#include "Vector.h"


namespace kome {
	namespace core {

		/**
		 * @class Matrix
		 * @brief matrix class
		 */
		class CORE_TOOLKITS_CLASS Matrix {
		public:
			/**
			 * @fn Matrix( const unsigned int row, const unsigned int col )
			 * @brief constructor
			 * @param[in] row length of row
			 * @param[in] col length of col
			 */
			Matrix( const unsigned int row, const unsigned int col );

			/**
			 * @fn Matrix( const Matrix& mat )
			 * @brief copy constructor
			 * @param[in] mat Matrix object to be copied
			 */
			Matrix( const Matrix& mat );

			/**
			 * @fn virtual ~Matrix()
			 * @brief destructor
			 */
			virtual ~Matrix();

		public:
			/** length of row */
			const unsigned int m_row;
			/** length of col */
			const unsigned int m_col;
			/** value array */
			double* const m_array;

		public:
			/**
			 * @fn unsigned int getRowSize()
			 * @brief gets row size of matrix
			 * @return row size of matrix
			 */
			unsigned int getRowSize();

			/**
			 * @fn unsigned int getColSize()
			 * @brief gets col size of matrix
			 * @return col size of matrix 
			 */
			unsigned int getColSize();

		public:
			/**
			 * @fn double det()
			 * @brief calculates determinant. (This matrix must be a square matrix.)
			 * @return determinant
			 */
			double det();

			/**
			 * @fn double cofactor( const unsigned int row, const unsigned int col )
			 * @brief calculates cofactor (This matrix must be a square matrix.)
			 * @param[in] row row
			 * @param[in] col col
			 * @return cofactor
			 */
			double cofactor( const unsigned int row, const unsigned int col );

			/**
			 * @fn Matrix invert()
			 * @brief gets inverse matrix (This matrix must be a square matrix.)
			 * @return inverse matrix
			 */
			Matrix invert();

			/**
			 * @fn Vector multiply( Vector& v )
			 * @brief multiplies vector
			 * @return multiplied vector
			 */
			Vector multiply( Vector& v );

		public:
			/**
			 * @fn Matrix operator=( const Matrix& m )
			 * @brief copy matrix
			 * @param[in] m source matrix to be copied
			 * @return copied matrix
			 */
			Matrix operator=( const Matrix& m );

			/**
			 * @fn double& operator()( const unsigned int row, const unsigned int col )
			 * @brief gets reference of value of specified element
			 * @param[in] row row
			 * @param[in] col col
			 * @return reference of value of specified element
			 */
			double& operator()( const unsigned int row, const unsigned int col );

			/**
			 * @fn Matrix& operator+=( const Matrix& m )
			 * @brief matrix addition
			 * @param[in] m matrix to add
			 * @return addition matrix
			 */
			Matrix& operator+=( const Matrix& m );

			/**
			 * @fn Matrix& operator-=( const Matrix& m )
			 * @brief matrix subtraction
			 * @param[in] m matrix to subtract
			 * @return subtraction matrix
			 */
			Matrix& operator-=( const Matrix& m );

			/**
			 * @fn Matrix& operator*=( const double k )
			 * @brief scalar multiplication
			 * @param[in] k real number to multiple
			 * @return multiplication matrix
			 */
			Matrix& operator*=( const double k );

			/**
			 * @fn Matrix& operator/=( const double k )
			 * @brief scalar division
			 * @param[in] k real number to divide
			 * @return division matrix
			 */
			Matrix& operator/=( const double k );

			/**
			 * @fn Matrix operator+( const Matrix& m );
			 * @brief matrix addition
			 * @param[in] m matrix to add
			 * @return addition matrix
			 */
			Matrix operator+( const Matrix& m );

			/**
			 * @fn Matrix operator-( const Matrix& m )
			 * @brief matrix subtraction
			 * @param[in] m matrix to subtract
			 * @return subtraction matrix
			 */
			Matrix operator-( const Matrix& m );

			/**
			 * @fn Matrix operator*( const Matrix& m )
			 * @brief matrix multiplication
			 * @param[in] m matrix to multiple
			 * @return multiplication matrix
			 */
			Matrix operator*( const Matrix& m );

			/**
			 * @fn Matrix operator*( const double k )
			 * @brief scalar multiplication
			 * @param[in] k real number to multiple
			 * @return multiplication matrix
			 */
			Matrix operator*( const double k );

			/**
			 * @fn Matrix operator/( const double k )
			 * @brief scalar division
			 * @param[in] k real number to divide
			 * @return division matrix
			 */
			Matrix operator/( const double k );

		
		};
	}
}

#endif		// __KOME_CORE_MATRIX_H__
