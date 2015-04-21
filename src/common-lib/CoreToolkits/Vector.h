/**
 * @file Vector.h
 * @brief interfaces of Vector class
 *
 * @author S.Tanaka
 * @date 2007.02.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CORE_VECTOR_H__
#define __KOME_CORE_VECTOR_H__


namespace kome {
	namespace core {

		/**
		 * @class Vector
		 * @brief vector class
		 */
		class CORE_TOOLKITS_CLASS Vector {
		public:
			/**
			 * @fn Vector( const unsigned int dim )
			 * @brief constructor
			 * @param[in] dim dimention size
			 */
			Vector( const unsigned int dim );

			/**
			 * @fn Vector( const Vector& v )
			 * @brief copy constructor
			 * @param[in] v Vector object to be copied
			 */
			Vector( const Vector& v );

			/**
			 * @fn virtual ~Vector()
			 * @brief destructor
			 */
			virtual ~Vector();

		protected:
			/** dimention size */
			const unsigned int m_dim;
			/** value array */
			double* const m_array;

		public:
			/**
			 * @fn unsigned int getDimention()
			 * @brief gets the dimention size of vector
			 * @return the dimention size of vector
			 */
			unsigned int getDimention();

		public:
			/**
			 * @fn double norm()
			 * @brief gets the norm of the vector
			 * @return norm
			 */
			double norm();

			/**
			 * @fn double dot( const Vector& v )
			 * @brief gets dot production
			 * @param[in] v vector to product
			 * @return dot production
			 */
			double dot( const Vector& v );

			/**
			 * @fn Vector cross( const Vector& v )
			 * @brief get cross production
			 * @param[in] v vector to product
			 * @return cross production vector
			 */
			Vector cross( const Vector& v );

		public:
			/**
			 * @fn Vector operator=( const Vector& v )
			 * @brief copy vector
			 * @param[in] v source vector to be copied
			 * @return copied vector
			 */
			Vector operator=( const Vector& v );

			/**
			 * @fn double& operator()( const unsigned int index )
			 * @brief gets reference of value of specified element
			 * @param[in] index index
			 * @return reference of value of specified element
			 */
			double& operator()( const unsigned int index );

			/**
			 * @fn Vector& operator+=( const Vector& v )
			 * @brief vector addition
			 * @param[in] v vector to add
			 * @return addition vector
			 */
			Vector& operator+=( const Vector& v );

			/**
			 * @fn Vector& operator-=( const Vector& v )
			 * @brief vector subtraction
			 * @param[in] v vector to subtract
			 * @return subtraction vector
			 */
			Vector& operator-=( const Vector& v );

			/**
			 * @fn Vector& operator*=( const double k )
			 * @brief scalar multiplication
			 * @param[in] k real number to multiple
			 * @return multiplication vector
			 */
			Vector& operator*=( const double k );

			/**
			 * @fn Vector& operator/=( const double k )
			 * @brief scalar division
			 * @return k real number to divide
			 * @return division vector
			 */
			Vector& operator/=( const double k );

			/**
			 * @fn Vector operator+( const Vector& v )
			 * @brief vector addition
			 * @param[in] v vector to add
			 * @return addition vector
			 */
			Vector operator+( const Vector& v );

			/**
			 * @fn Vector operator-( const Vector& v )
			 * @brief vector subtraction
			 * @param[in] v vector to subtract
			 * @return subtraction vector
			 */
			Vector operator-( const Vector& v );

			/**
			 * @fn Vector operator*( const double k )
			 * @brief scalar multiplication
			 * @param[in] k real number to multiple
			 * @return multiplication vector
			 */
			Vector operator*( const double k );

			/**
			 * @fn Vector operator/( const double k )
			 * @brief scalar division
			 * @param[in] k real number to divide
			 * @return division vector
			 */
			Vector operator/( const double k );

		
		};
	}
}


#endif		//  __KOME_CORE_VECTOR_H__
