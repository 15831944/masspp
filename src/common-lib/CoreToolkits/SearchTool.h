/**
 * @file SearchTool.h
 * @brief interfaces of SearchTool class
 *
 * @author S.Tanaka
 * @date 2006.07.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_SEARCH_TOOL_H__
#define __KOME_CORE_SEARCH_TOOL_H__


#include <boost/function.hpp>


namespace kome {
	namespace core {

		/**
		 * @class SearchTool
		 * @brief This class has some searching algorithm method.
		 */
		class CORE_TOOLKITS_CLASS SearchTool {
		public:
			/**
			 * @fn static int binarySearch(
						S value,
						boost::function< T ( int ) > getElementFun,
						boost::function< int ( T, S ) > compareFun,
						int size
					)
			 * @brief binary search
			 * @param[in] value specified value
			 * @param[in] getElementFun function which get element from array.
			 * @param[in] compareFun comparison function.
			   return value 
			   - more than zero : element's value is greater than specified value.
			   - zero: element's value is equal to specified value.
			   - less than zero: element's value is less than specified value.
			 * @param[in] size size of array
			 * @return If it contains a element which has specified value, this function returns the index. [0 or positive]
			   Otherwise it returns (- insertion point - 1). [negative]
			 */
			template< typename S, typename T >
			static int binarySearch(
					S value,
					boost::function< T ( int ) > getElementFun,
					boost::function< int ( T, S ) > compareFun,
					int size
			) {
				// range of search
				int top = 0;
				int bottom = size - 1;
				int idx = -1;

				// search
				while( top <= bottom && idx < 0 ) {
					// get element located middle of searching range
					int mid = ( top + bottom ) / 2;
					T elm0 = getElementFun( mid );

					// compare
					int cmp = compareFun( elm0, value );
					if( cmp == 0 ) {	// particular element is found.
						idx = mid;

						// return the first of particular elements.
						mid--;
						while( mid >= top ) {
							T elm1 = getElementFun( mid );
							if( compareFun( elm1, value ) == 0 ) {	// particular element
								idx = mid;
								mid--;
							}
							else {
								mid = top - 1;
							}
						}
					}
					else if( cmp > 0 ) {	// Element's value is greater than searching value.
						bottom = mid - 1;
					}
					else {	// Element's value is less than searching value.
						top = mid + 1;
					}
				}

				if( idx < 0 ) {		// not found
					idx = - top - 1;
				}

				return idx;
			}
		};
	}
}

#endif	// __KOME_CORE_SEARCH_TOOL_H__
