/**
 * @file Rect.h
 * @brief definition of Rect class
 *
 * @author S.Tanaka
 * @date 2006.12.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CORE_RECT_H__
#define __KOME_CORE_RECT_H__

namespace kome {
	namespace core {
		/**
		 * @class Rect
		 * @brief rectangle management class
		 */
		template <typename T> class Rect {
		public:
			/**
			 * @fn Rect()
			 * @brief constructor
			 */
			Rect() : left(), top(), right(), bottom() {
			}

			/**
			 * @fn Rect( T& l, T& t, T& r, T& b )
			 * @brief constructor
			 * @param[in] l left position
			 * @param[in] t top position
			 * @param[in] r right position
			 * @param[in] b bottom position
			 */
			Rect( T& l, T& t, T& r, T& b ) : left( l ), top( t ), right( r ), bottom( b ) {
			}

			/**
			 * @fn virtual ~Rect()
			 * @brief destructor
			 */
			virtual ~Rect() {
			}

		public:
			/** position of left */
			T left;
			/** position of top */
			T top;
			/** position of right */
			T right;
			/** position of bottom */
			T bottom;

		public:
			/**
			 * @fn T getWidth()
			 * @brief gets width of rectangle
			 * @return width
			 */
			T getWidth() {
				return RANGE( left, right );
			}

			/**
			 * @fn T getHeight()
			 * @brief gets height of rectangle
			 * @return height
			 */
			T getHeight() {
				return RANGE( top, bottom );
			}
		};
	}
}

#endif		// __KOME_CORE_RECT_H__
