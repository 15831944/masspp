/**
 * @file Point.h
 * @brief definition of Point class
 *
 * @author S.Tanaka
 * @date 2006.12.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CORE_POINT_H__
#define __KOME_CORE_POINT_H__

namespace kome {
	namespace core {

		/**
		 * @class Point
		 * @brief 2 dimensional point information management class
		 */
		template < typename T > class Point {
		public:
			/**
			 * @fn Point();
			 * @brief constructor
			 */
			Point() : px(), py() {
			}

			/**
			 * @fn Point( T x, T y )
			 * @brief constructor
			 * @param[in] x x coordinate
			 * @param[in] y y coordinate
			 */
			Point( T x, T y ) : px( x ), py( y ) {
			}

			/**
			 * @fn virtual ~Point()
			 * @brief destructor
			 */
			virtual ~Point() {
			}

		public:
			/** x coordinate */
			T px;
			/** y coordinate */
			T py;

		public:
			/**
			 * @fn static bool lessX( const Point< T >& p0, const Point< T >& p1 )
			 * @brief compare to sort
			 * @param[in] p0 point object to be compared
			 * @param[in] p1 point object to compare
			 * @return If x coordinate of p0 is less than it of p1, this method returns true
			 */
			static bool lessX( const Point< T >& p0, const Point< T >& p1 ) {
				return ( p0.px < p1.px );
			}

			/**
			 * @fn static bool greaterX( const Point< T >& p0, const Point< T >& p1 )
			 * @brief compare to sort
			 * @param[in] p0 point object to be compared
			 * @param[in] p1 point object to compare
			 * @return If x coordinate of p0 is greater than it of p1, this method returns true
			 */
			static bool greaterX( const Point< T >& p0, const Point< T >& p1 ) {
				return ( p0.px > p1.px );
			}

			/**
			 * @fn static bool lessY( const Point< T >& p0, const Point< T >& p1 )
			 * @brief compare to sort
			 * @param[in] p0 point object to be compared
			 * @param[in] p1 point object to compare
			 * @return If y coordinate of p0 is less than it of p1, this method returns true
			 */
			static bool lessY( const Point< T >& p0, const Point< T >& p1 ) {
				return ( p0.py < p1.py );
			}

			/**
			 * @fn static bool greaterY( const Point< T >& p0, const Point< T >& p1 )
			 * @brief compare to sort
			 * @param[in] p0 point object to be compared
			 * @param[in] p1 point object to compare
			 * @return If y coordinate of p0 is greater than it of p1, this method returns true
			 */
			static bool greaterY( const Point< T >& p0, const Point< T >& p1 ) {
				return ( p0.py > p1.py );
			}
		};
	}
}

#endif		// __KOME_CORE_POINT_H__
