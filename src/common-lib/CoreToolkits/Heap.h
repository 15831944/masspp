/**
 * @file  Heap.h
 * @brief definition of Heap class
 *
 * @author S.Tanaka
 * @date 2007.12.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_HEAP_H__
#define __KOME_CORE_HEAP_H__


#include <vector>
#include <boost/function.hpp>


namespace kome {
	namespace core {

		/**
		 * @class Heap
		 * @brief heap data management class
		 */
		template < typename T > class Heap {		
		public:
			/**
			 * @fn Heap()
			 * @brief constructor
			 */
			Heap() {
				m_arr = NULL;
				m_size = 0;
				m_arrSize = 0;
				m_less = std::less< T >();
			}

			/**			
			 * @fn Heap( boost::function< bool( T&, T& ) > lessFun )
			 * @brief constructor
			 * @param[in] lessFun compare function
			 */
			Heap( boost::function< bool( T&, T& ) > lessFun ) {
				m_arr = NULL;
				m_size = 0;
				m_arrSize = 0;
				m_less = lessFun;
			}

			/**
			 * @fn ~Heap
			 * @brief destructor
			 */
			virtual ~Heap() {
				clear();
			}

		protected:
			/** array */
			T* m_arr;

			/** heap size */
			unsigned int m_size;

			/** array size */
			unsigned int m_arrSize;

			/** compare function */
			boost::function< bool( T&, T& ) > m_less;
			

		public:
			/**
			 * @fn void clear()
			 * @brief clears heap
			 */
			void clear() {
				// delete
				if( m_arr != NULL ) {
					delete[] m_arr;
				}

				// initialize
				m_arr = NULL;
				m_size = 0;
				m_arrSize = 0;
			}

			/**
			 * @fn unsigned size()
			 * @brief gets heap size
			 * @return heap size
			 */
			unsigned size() {
				return m_size;
			}

			/**
			 * @fn T at( const unsigned int index )
			 * @brief gets heap element
			 * @param[in] index index
			 * @return heap element
			 */
			T at( const unsigned int index ) {
				return m_arr[ index ];
			}

			/**
			 * @fn void add( T elm )
			 * @brief adds element
			 * @param[in] elm element to be added
			 */
			void add( T elm ) {
				// check array
				if( m_size + 1 >= m_arrSize ) {
					// new array
					m_arrSize *= 2;
					if( m_arrSize < 10 ) {
						m_arrSize = 10;
					}
					T* arr = new T[ m_arrSize ];

					// copy
					memcpy( arr, m_arr, sizeof( T ) * m_size );

					// swap
					if( m_arr != NULL ) {
						delete[] m_arr;
					}
					m_arr = arr;
				}

				// add
				m_arr[ m_size ] = elm;
				int idx = (int)m_size;
				m_size++;

				// swap
				while( idx > 0 ) {
					int hlfIdx = ( idx - 1 ) / 2;
					if( m_less( m_arr[ idx ], m_arr[ hlfIdx ] ) ) {
						T tmp = m_arr[ idx ];
						m_arr[ idx ] = m_arr[ hlfIdx ];
						m_arr[ hlfIdx ] = tmp;
						idx = hlfIdx;
					}
					else {
						idx = -1;
					}
				}
			}

			/**
			 * @fn void remove( const unsigned int index )
			 * @brief removes element
			 * @param[in] index element index
			 */
			void remove( const unsigned int index ) {
				// check the index
				if( index >= m_size ) {
					return;
				}

				// array[ N ] -> array[ index ]
				m_arr[ index ] = m_arr[ m_size - 1 ];
				m_size--;

				// swap
				int idx = index;
				if( idx > 0 && m_less( m_arr[ idx ], m_arr[ idx / 2 ] ) ) {  // parent
					while( idx > 0 ) {
						int hlfIdx = ( idx - 1 ) / 2;
						if( m_less( m_arr[ idx ], m_arr[ hlfIdx ] ) ) {
							T tmp = m_arr[ idx ];
							m_arr[ idx ] = m_arr[ hlfIdx ];
							m_arr[ hlfIdx ] = tmp;
							idx = hlfIdx;
						}
						else {
							idx = -1;
						}
					}
				}
				else {		// children
					while( idx < (int)m_size ) {
						// index
						int lIdx = idx * 2 + 1;
						int rIdx = idx * 2 + 2;

						// check left
						bool lswap = false;
						if( lIdx < (int)m_size ) {
							if( m_less( m_arr[ lIdx ], m_arr[ idx ] ) ) {
								lswap = true;
							}
						}

						// check right
						bool rswap = false;
						if( rIdx < (int)m_size ) {
							if( m_less( m_arr[ rIdx ], m_arr[ idx ] ) ) {
								rswap = true;
							}
						}

						// compare
						if( lswap && rswap ) {
							if( m_less( m_arr[ rIdx ],  m_arr[ lIdx ] ) ) {
								lswap = false;
							}
							else {
								rswap = false;
							}
						}

						// swap
						int swapIdx = (int)m_size;
						if( lswap ) {
							swapIdx = lIdx;
						}
						else if( rswap ) {
							swapIdx = rIdx;
						}

						if( swapIdx < (int)m_size ) {
							T tmp = m_arr[ idx ];
							m_arr[ idx ] = m_arr[ swapIdx ];
							m_arr[ swapIdx ] = tmp;
						}
						idx = swapIdx;
					}
				}
			}
		};
	}
}

#endif		// __KOME_CORE_HEAP_H__
